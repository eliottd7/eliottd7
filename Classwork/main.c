#include "parser.h"
#include <fcntl.h>

// Compile time expansion macro instead of function for performance
#define prompt(X) do { if(X) printf("my_shell$"); } while(0)

int main(int argc, char* argv[])
{
    bool display_prompt = (argc < 2);
    string input_buffer = (string)calloc(MAX_INPUT_LENGTH, 1);

    while(true){
        prompt(display_prompt);

        if(fgets(input_buffer, MAX_INPUT_LENGTH, stdin) == NULL) break;
        pipeline* p = parse(input_buffer);
        if(p == NULL) continue;

        // init pipes
        int new_pipe[2] = {-1,-1};
        int old_pipe[2] = {-1,-1};
        int status;

        for(int i = 0; i < p->cmd_count; i++){
            if(i < (p->cmd_count - 1)){
                if(pipe(new_pipe) < 0){
                    perror("Pipe()");
                    exit(EXIT_FAILURE);
                }
            }

            pid_t pid = fork();
            if(pid < 0){
                perror("Fork()");
                exit(EXIT_FAILURE);
            }

            else if(pid == 0){ //Child
                if(i > 0){ //not first command
                    dup2(old_pipe[0], 0);
                    close(old_pipe[0]);
                    close(old_pipe[1]);
                }
                else if(p->commands[0].in_path != NULL){ //redirect in
                    int infd = open(p->commands[0].in_path, O_RDONLY | O_CLOEXEC);
                    if(infd < 0){
                        perror("Redirect IN open()");
                        exit(EXIT_FAILURE);
                    }
                    dup2(infd, STDIN_FILENO);
                }
                if(i < (p->cmd_count - 1)){ //not last command
                    close(new_pipe[0]);
                    dup2(new_pipe[1], 1);
                    close(new_pipe[1]);
                }
                else if(p->commands[i].out_path != NULL){
                    int outfd = open(p->commands[i].out_path, O_WRONLY | O_CREAT | O_CLOEXEC, S_IRWXU);
                    if(outfd < 0){
                        perror("Redirect OUT open()");
                        exit(EXIT_FAILURE);
                    }
                    dup2(outfd, STDOUT_FILENO);
                }
                execvp(p->commands[i].args[0], p->commands[i].args);
                perror("Exec()");
                exit(EXIT_FAILURE);
            }

            else { // Parent
                if(i > 0){
                    close(old_pipe[0]);
                    close(old_pipe[1]);
                }
                if(i < (p->cmd_count - 1)){
                    old_pipe[0] = new_pipe[0];
                    old_pipe[1] = new_pipe[1];
                }
                if(p->background == false) waitpid(pid, &status, 0);
            }
        }

        close(old_pipe[0]);
        close(old_pipe[1]);

        memset(p, 0, sizeof(pipeline)); //ensure data clear if same p reused
        free(p);
        memset(input_buffer, 0, MAX_INPUT_LENGTH);
    }

    printf("\n");
    free(input_buffer);
    return 0;
}

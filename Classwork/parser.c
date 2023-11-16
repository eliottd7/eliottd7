#include "parser.h"

// Helper functions

// Remove any '&' from a string without breaking the string into 2
void removeAmpersand(char *str) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        if (*src != '&') {
            *dst = *src;
            dst++;
        }
    }
    *dst = '\0';
}

// Redirection file paths are mixed into command's arguments
// This function moves them to the i_path or o_path field of command struct
// while "shortening" the args array without causing segfaults
int redirection_check(pipeline* p, int cmdn, int argn){
    if(p == NULL) return -1;
    if(argn > p->commands[cmdn].num_args) return 0;
    if(cmdn > p->cmd_count) return 0;
    string delimiter = "<>";
    string found = strpbrk(p->commands[cmdn].args[argn], delimiter);
    if(found != NULL){
        if(strlen(found) < 2){ // token is just "<" or ">", path in next arg[]
            int tmp = p->commands[cmdn].num_args;
            p->commands[cmdn].num_args -= 2;
            if(found[0] == '<' && cmdn == 0){ //redir in
                p->commands[cmdn].in_path =
                    strdup(p->commands[cmdn].args[argn+1]);
            }
            else if((found[0] == '>') && (cmdn == p->cmd_count-1)){ //redir out
                p->commands[cmdn].out_path = 
                    strdup(p->commands[cmdn].args[argn+1]);
            }
            for(int i = argn; i < p->commands[cmdn].num_args; i++){
                p->commands[cmdn].args[i] = p->commands[cmdn].args[i+2];
            }
            for(int i = p->commands[cmdn].num_args; i < tmp; i++){
                if(p->commands[cmdn].args[i] != NULL){
                    p->commands[cmdn].args[i] = NULL;
                }
            }
        }
        else { //token includes full path name, must copy without <>
            int tmp = p->commands[cmdn].num_args;
            p->commands[cmdn].num_args -= 1;
            if(found[0] == '<' && cmdn == 0){ //redir in
                p->commands[cmdn].in_path = strtok(found, "<");
            }
            else if((found[0] == '>') && (cmdn == p->cmd_count-1)){ //redir out
                p->commands[cmdn].out_path = strtok(found, ">");
            }
            for(int i = argn; i < p->commands[cmdn].num_args; i++){
                p->commands[cmdn].args[i] = p->commands[cmdn].args[i+1];
            }
            for(int i = p->commands[cmdn].num_args; i < tmp; i++){
                if(p->commands[cmdn].args[i] != NULL){
                    p->commands[cmdn].args[i] = NULL;
                }
            }
        }
        //Recursively check same args again
        if(p->commands[cmdn].args[argn] != NULL)
            return redirection_check(p, cmdn, argn);
    }
    return 0;
}


// Parser function

pipeline* parse(const string cmdline){
    if(cmdline[0] < 33){ //checks most whitespace characters
        return NULL;
    }
    
    // To keep cmdline const, duplicate string to "input"
    string input = strcat(strtok(strdup(cmdline), "\n"), "");
    
    //Check for backgrounding
    string delimiter = "&";
    string bgd = strpbrk(input, delimiter);
    bool background = (bgd == NULL)? false : true;
    if(bgd){
        removeAmpersand(input);
    }
    
    // Split input by pipes
    int cmd_count = 0;
    string pipe_clusters[MAX_NUM_TOKENS];
    delimiter = "|";
    string ptc = strtok(input, delimiter);
    while(ptc != NULL){
        pipe_clusters[cmd_count++] = ptc;
        ptc = strtok(NULL, delimiter);
    }
    
    // Set up pipeline struct to return
    pipeline* p = (pipeline*)malloc(sizeof(pipeline) + cmd_count*sizeof(void*));
    p->pipe_count = (cmd_count > 1)? cmd_count - 1 : 0;
    p->cmd_count = cmd_count;
    p->background = background;
    
    // Tokenize by spaces
    for(int i = 0; i < p->cmd_count; i++){
        delimiter = " ";
        int arg_iterator = 0;
        string sp = strtok(pipe_clusters[i], delimiter);
        while(sp != NULL){
            p->commands[i].args[arg_iterator] = sp;
            arg_iterator++;
            sp = strtok(NULL, delimiter);
        }
        p->commands[i].num_args = arg_iterator;
    }
    
    // Remove arguments if they're actually redirection paths
    for(int i = 0; i < p->cmd_count; i++){
        for(int j = 0; j < p->commands[i].num_args; j++){
            if(redirection_check(p, i, j) != 0) exit(EFAULT);
        }
    }
    
    return p;
}

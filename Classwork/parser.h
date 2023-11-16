#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define string char*

#define MAX_INPUT_LENGTH    512
#define MAX_NUM_TOKENS      64  // soft cap of 64 tokens
#define MAX_ARGS_LENGTH     16  // soft cap of 16 arguments per command

typedef struct command {
    string out_path;
    string in_path;
    unsigned int num_args;
    string args[MAX_ARGS_LENGTH];
} command;

typedef struct pipeline {
    unsigned int cmd_count;
    unsigned int pipe_count;
    bool background;
    command commands[MAX_NUM_TOKENS];
} pipeline;

pipeline* parse(const string cmdline);
int pfree(pipeline* p);

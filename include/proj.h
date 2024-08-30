#ifndef PROJ_H
#define PROJ_H

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Function Declarations for builtin shell commands:
int proj_cd(char **args);
int proj_help(char **args);
int proj_exit(char **args);

// Function Declarations for execution:
int execute(char **args);
int launch(char **args);
int proj_num_builtins(void);

// Function Declarations for utilities:
char *read_line(void);
char **split_line(char *line);
void loop(void);

extern char *builtin_str[];
extern int (*builtin_func[])(char **);

#endif

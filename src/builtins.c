#include "proj.h"

int proj_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "proj: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("proj");
        }
    }
    return 1;
}

int proj_help(char **args) {
    int i;
    printf("Custom Shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < proj_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int proj_exit(char **args) {
    return 0;
}

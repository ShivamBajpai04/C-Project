#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "proj.h"

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#define ESCAPE_SEQUENCE 27

char *read_line(void) {
    int bufsize = RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "proj: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        // Detect arrow keys (escape sequences)
        if (c == ESCAPE_SEQUENCE) { // Escape character
            getchar(); // Skip the '['
            switch (getchar()) {
                case 'A': // Up arrow
                    if (history_index > 0) {
                        char *prev_command = get_previous_command();
                        if (prev_command) {
                            // Clear the current line
                            printf("\r%s $ %s", getcwd(NULL, 0), prev_command);
                            strcpy(buffer, prev_command);
                            position = strlen(buffer);
                        }
                    }
                    continue;
                case 'B': // Down arrow
                    if (history_index < history_count) {
                        char *next_command = get_next_command();
                        if (next_command) {
                            // Clear the current line
                            printf("\r%s $ %s", getcwd(NULL, 0), next_command);
                            strcpy(buffer, next_command);
                            position = strlen(buffer);
                        }
                    }
                    continue;
            }
        }

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "proj: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **split_line(char *line) {
    int bufsize = TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "proj: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "proj: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

#define HISTORY_SIZE 100

char *history[HISTORY_SIZE];
int history_count = 0;
int history_index = 0;

void add_to_history(char *command) {
    if (history_count < HISTORY_SIZE) {
        history[history_count] = strdup(command);
        history_count++;
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(command);
    }
    history_index = history_count;
}

char *get_previous_command() {
    if (history_index > 0) {
        history_index--;
        return history[history_index];
    }
    return NULL;
}

char *get_next_command() {
    if (history_index < history_count - 1) {
        history_index++;
        return history[history_index];
    }
    return NULL;
}

void loop(void) {
    char *line;
    char **args;
    int status;
    char cwd[PATH_MAX];

    do {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s $ ", cwd);
        } else {
            perror("proj");
            exit(EXIT_FAILURE);
        }

        line = read_line();

        if (strlen(line) > 0) { // Only save non-empty commands
            add_to_history(line);
        }

        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

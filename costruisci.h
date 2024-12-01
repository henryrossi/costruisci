#ifndef COSTRUISCI_H
#define COSTRUISCI_H

#include <stdlib.h>

struct cos_cmd {
        const char **items;
        size_t size;
        size_t cap;
};

void cos_cmd_append(struct cos_cmd *cmd, ...);
void cos_cmd_run(struct cos_cmd *cmd);

#endif /* COPNSTRUISCI_H */

// #define COSTRUISCI_IMPL
#ifdef COSTRUISCI_IMPL

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <stdarg.h>
#include <unistd.h>
#include <wait.h>

#define ARRAY_INIT_CAP 64

#define cos_array_init(array)                                                  \
        do {                                                                   \
                (array)->cap = ARRAY_INIT_CAP;                                 \
                (array)->size = 0;                                             \
                (array)->items =                                               \
                    calloc((array)->cap, sizeof(*(array)->items));             \
                if ((array)->items == NULL) {                                  \
                        fprintf(stderr, "Failed to initialize array\n");       \
                }                                                              \
        } while (false)

#define cos_array_free(array)                                                  \
        do {                                                                   \
                free((array)->items);                                          \
                (array)->size = 0;                                             \
                (array)->cap = 0;                                              \
        } while (false)

#define cos_array_append(array, item)                                          \
        do {                                                                   \
                if ((array)->size == (array)->cap) {                           \
                        (array)->cap *= 2;                                     \
                        void *data =                                           \
                            calloc((array)->cap, sizeof(*(array)->items));     \
                        if (data == NULL) {                                    \
                                fprintf(stderr,                                \
                                        "Failed to increase size of array\n"); \
                        }                                                      \
                        memcpy(data, (array)->items,                           \
                               (array)->size * sizeof(*(array)->items));       \
                        free((array)->items);                                  \
                        (array)->items = data;                                 \
                }                                                              \
                (array)->items[(array)->size] = item;                          \
                (array)->size += 1;                                            \
        } while (false)

void cos_cmd_append(struct cos_cmd *cmd, ...) {
        if (cmd->items == NULL) {
                cos_array_init(cmd);
        }

        va_list va;
        va_start(va, cmd);

        char *arg = va_arg(va, char *);
        while (arg != 0) {
                cos_array_append(cmd, arg);
                arg = va_arg(va, char *);
        }

        va_end(va);
}

void cos_cmd_run(struct cos_cmd *cmd) {
        // const char *exec_file = cmd->items[0];
        //
        // char **args = calloc(cmd->size, sizeof(cmd->items));
        // if (args == NULL) {
        //         fprintf(stderr,
        //                 "Failed to allocate space for command arguments");
        //         return;
        // }
        // memcpy(args, cmd->items + 1, cmd->size);
        // args[cmd->size - 1] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
                execvp(cmd->items[0], (char * const *) cmd->items);
                return;
        }
        wait(&pid);

        printf("Running command: ");
        for (int i = 0; i < cmd->size; i++) {
                printf("%s ", cmd->items[i]);
        }
        printf("\n");
}

#endif /* CONSTRUISCI_IMPL */

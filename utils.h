#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdio.h>

#define eprintf(args...) fprintf(stderr, args)


#define log_message(level, module, args...) { eprintf("[%s] %s : ", level, module); eprintf(args); }
#define log_DEBUG(module, args...) log_message("DEBUG", module, args)
#define log_INFO(module, args...) log_message("INFO", module, args)
#define log_WARN(module, args...) log_message("WARN", module, args)
#define log_ERROR(module, args...) log_message("ERROR", module, args)

#define log_FATAL(module) { perror(module); exit(1); }

#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3
#define FATAL 4


#define nn_strcmp(a, b) (a != NULL && b != NULL && strcmp(a, b) == 0)


size_t readCommand(char *command, size_t max);


#endif

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define eprintf(args...) fprintf(stderr, args)


#define log_INFO(name, args...) printf("[INFO] (%s)", name); printf(args)
#define log_WARN(name, args...) fprintf(stdout, "[WARN] (%s)", name); fprintf(stdout, args)
#define log_


size_t readCommand(char *command, size_t max);


#endif

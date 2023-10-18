#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"


size_t readCommand(char *command, size_t max) {
    memset(command, 0, max);
    char ch;
    size_t readLen = 0;
    while (read(STDIN_FILENO, &ch, 1) > 0) {
        if (ch == '\n')
            break;
        command[readLen++] = ch;
        if (readLen == max - 1)
            break;
    }
    if (ch != '\n') {
        putc('\n', stdout);
    }
    command[readLen] = '\0';
    return readLen;
}

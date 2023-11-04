#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "command.h"


CmdList *cmdlist_open() {
    CmdList *cmdlist = calloc(sizeof(CmdList), 1);
    if (cmdlist == NULL)
        log_FATAL("cmdlist_open");

    cmdlist->alloc = 16;
    cmdlist->list = calloc(sizeof(Cmd*), cmdlist->alloc);
    if (cmdlist->list == NULL)
        log_FATAL("cmdlist_open");

    cmdlist->count = 0;

    return cmdlist;
}

void cmdlist_close(CmdList *cmdlist) {
    for (size_t i = 0; i < cmdlist->count; i++) {
        cmd_close(cmdlist->list[i]);
    }
    free(cmdlist->list);
    free(cmdlist);
}

Cmd *cmdlist_get(const CmdList *cmdlist, const char *name) {
    for (size_t i = 0; i < cmdlist->count; i++) {
        if (strcmp(cmdlist->list[i]->name, name) == 0)
            return cmdlist->list[i];
    }
    return NULL;
}

void cmdlist_preserve(CmdList *cmdlist) {
    if (cmdlist->count == cmdlist->alloc) {
        cmdlist->alloc *= 2;
        cmdlist->list = realloc(cmdlist->list, sizeof(Cmd*) * cmdlist->alloc);
        if (cmdlist->list == NULL)
            log_FATAL("cmdlist_preserve");
    }
}

void cmdlist_add(CmdList *cmdlist, Cmd *cmd) {
    cmdlist_preserve(cmdlist);
    cmdlist->list[cmdlist->count++] = cmd;
}


Cmd *cmd_open(const char *name, const char *description, int lua_function) {
    Cmd *cmd = calloc(sizeof(Cmd), 1);
    cmd->name = name;
    cmd->description = description;
    cmd->lua_function = lua_function;
    return cmd;
}

void cmd_close(Cmd *cmd) {
    free(cmd);
}

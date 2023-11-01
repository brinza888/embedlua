#ifndef COMMAND_H
#define COMMAND_H


typedef struct Cmd {
    const char *name;
    const char *description;
} Cmd;


typedef struct CmdList {
    Cmd **list;
    size_t count;
    size_t alloc;
} CmdList;


CmdList *cmdlist_open();
void cmdlist_close(CmdList *cmdlist);
Cmd *cmdlist_get(const CmdList *cmdlist, const char *name);
void cmdlist_preserve(CmdList *cmdlist);
void cmdlist_add(CmdList *cmdlist, Cmd *cmd);

Cmd *cmd_open(const char *name, const char *description);
void cmd_close(Cmd *cmd);


#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "plugin.h"
#include "utils.h"


int main(int argc, char *argv[]) {
    PluginList *plist = plist_open();
    plist_loaddir(plist, "plugins");

    bool running = true;

    const size_t maxCommandLen = 1024;
    size_t commandLen = 0;
    char command[maxCommandLen];

    while (running) {
        putc('>', stdout);
        putc(' ', stdout);
        fflush(stdout);
        commandLen = readCommand(command, maxCommandLen);

        if (commandLen == 0 || strcmp(command, "exit") == 0) {
            running = false;
        }
        else if (strcmp(command, "pl_list") == 0) {
            printf("Plugins:\n");
            for (size_t i = 0; i < plist->count; i++) {
                printf("\tPlugin %s (v%s) by %s\n",
                       plist->list[i]->name,
                       plist->list[i]->version,
                       plist->list[i]->author);
            }
        }
        else if (strcmp(command, "pl_cmds") == 0) {
            printf("Commands:\n");
            for (size_t i = 0; i < plist->count; i++) {
                Plugin *pl = plist->list[i];
                for (size_t j = 0; j < pl->cmdlist->count; j++) {
                    printf("%s (from: %s)\n", pl->cmdlist->list[j]->name, pl->name);
                }
            }
        }
        else {
            bool handled = false;
            for (size_t i = 0; i < plist->count; i++) {
                handled = handled || pl_command(plist->list[i], command);
            }
            if (!handled) {
                eprintf("Command not exists: %s\n", command);
            } 
        }
    }
    
    plist_close(plist);
}

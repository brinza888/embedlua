#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <dirent.h>
#include <unistd.h>
#include <linux/limits.h>

#include "plugin.h"
#include "utils.h"


int main(int argc, char *argv[]) {
    Plugin *plugins[PLUGIN_MAX];
    size_t pluginCount = 0;
    
    /* load plugins */
    DIR *pluginDir = opendir("plugins");
    if (pluginDir == NULL) {
        eprintf("[LOADER] No plugin directory\n");
        exit(1);
    }
    struct dirent *element;
    char path[PATH_MAX];
    while ((element = readdir(pluginDir))) {
        if (element->d_name[0] == '.')
            continue;
        strcpy(path, "plugins");
        strcat(path, "/");
        strcat(path, element->d_name);
        Plugin *pl = openPlugin(path);
        if (pl != NULL)
            plugins[pluginCount++] = pl;
    }
    closedir(pluginDir);
    printf("[LOADER] Loaded %zu plugins\n", pluginCount);
    
    /* main code */

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
            break;
        }
        else if (strcmp(command, "list") == 0) {
            printf("Plugins list:\n");
            for (size_t i = 0; i < pluginCount; i++) {
                printf("\tPlugin %s (v%s) by %s\n",
                       plugins[i]->name,
                       plugins[i]->version,
                       plugins[i]->author);
            }
        }
        else {
            eprintf("Command not exists: %s\n", command);
        }
    }
    
    /* unload plugins */
    for (size_t i = 0; i < pluginCount; i++) {
        closePlugin(plugins[i]);
    }
    printf("[LOADER] Unloaded all plugins\n");
}

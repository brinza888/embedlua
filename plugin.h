#ifndef PLUGIN_H
#define PLUGIN_H

#include <dirent.h>
#include <unistd.h>
#include <linux/limits.h>

#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <lua5.4/lauxlib.h>

#define PLUGIN_FILENAME_MAX 256


typedef struct Plugin {
    char filename[PLUGIN_FILENAME_MAX];
    const char *name;
    const char *author;
    const char *version;
    const char *alias;
    lua_State *L;

    char **commands;
} Plugin;


typedef struct PluginList {
    Plugin **list;
    size_t count;
    size_t alloc;
} PluginList;


PluginList *plist_open();
void plist_close(PluginList *plist);
Plugin *plist_get(PluginList *plist, const char *alias, const char *name, const char* filename);
void plist_preserve(PluginList *plist);
void plist_add(PluginList *plist, Plugin *plugin);
void plist_load(PluginList *plist, const char *filename);
void plist_loaddir(PluginList *plist, const char *dir);

Plugin *pl_open(const char *filename);
void pl_close(Plugin *plugin);
void pl_command(Plugin *plugin, const char *command);


#endif

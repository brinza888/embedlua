#ifndef PLUGIN_H
#define PLUGIN_H

#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <lua5.4/lauxlib.h>

#define PLUGIN_FILENAME_MAX 128


typedef struct Plugin {
    char filename[PLUGIN_FILENAME_MAX];
    const char *name;
    const char *author;
    const char *version;
    const char *alias;
    lua_State *L;
} Plugin;


typedef struct PluginList {
    Plugin **list;
    size_t count;
}


PluginList *plist_open();
void plist_close(PluginList *plist);
Plugin *plist_get(const char *alias, const char *name, const char* filename);

Plugin *pl_open(const char *filename);
void pl_close(Plugin *plugin);
void pl_command(Plugin *plugin, char *command);


#endif

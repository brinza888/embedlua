#include <stdlib.h>
#include <string.h>

#include "plugin.h"
#include "utils.h"


PluginList *plist_open() {
    PluginList *plist = calloc(sizeof(PluginList), 1);
    plist->
}

void plist_close(PluginList *plist) {

}

Plugin *plist_get(const char *alias, const char *name, const char* filename) {
    for ()
}


Plugin *pl_open(const char *filename) {
    printf("[pl_open] Loading %s\n", filename);
    Plugin *plugin = (Plugin*) calloc(sizeof(Plugin), 1);
    if (plugin == NULL) {
        eprintf("[pl_open] Not enough memory for Plugin struct\n");
        exit(1);
    }

    strncpy(plugin->filename, filename, PLUGIN_FILENAME_MAX);
    
    plugin->L = luaL_newstate();
    luaL_openlibs(plugin->L);

    if (luaL_dofile(plugin->L, plugin->filename) == LUA_OK) {
        lua_pop(plugin->L, lua_gettop(plugin->L));
    }
    else {
        const char *errorMsg = lua_tostring(plugin->L, -1);
        eprintf("[pl_open] Can't open plugin %s : %s\n", filename, errorMsg);
        closePlugin(plugin);
        return NULL;
    }

    lua_getglobal(plugin->L, "pl_name");
    plugin->name = lua_tostring(plugin->L, -1);

    lua_getglobal(plugin->L, "pl_version");
    plugin->version = lua_tostring(plugin->L, -1);

    lua_getglobal(plugin->L, "pl_author");
    plugin->author = lua_tostring(plugin->L, -1);

    lua_getglobal(plugin->L, "pl_alias");
    plugin->alias = lua_tostring

    return plugin;
}

void pl_close(Plugin *plugin) {
    printf("[LOADER] Unloading %s\n", plugin->filename);
    lua_close(plugin->L);
    free(plugin);
}

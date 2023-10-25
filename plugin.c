#include <stdlib.h>
#include <string.h>

#include "plugin.h"
#include "utils.h"


PluginList *plist_open() {
    PluginList *plist = calloc(sizeof(PluginList), 1);
    if (plist == NULL)
        log_FATAL("plist_open");

    plist->count = 0;
    plist->alloc = 16;

    plist->list = calloc(sizeof(Plugin*), plist->alloc);
    if (plist->list == NULL)
        log_FATAL("plist_open");

    return plist;
}

void plist_close(PluginList *plist) {
    for (size_t i = 0; i < plist->count; i++) {
        pl_close(plist->list[i]);  // close each Plugin
    }
    free(plist->list);             // free array of Plugin*
    free(plist);                   // free struct PluginList
}

Plugin *plist_get(PluginList *plist, const char *alias, const char *name, const char* filename) {
    for (size_t i = 0; i < plist->count; i++) {
        Plugin *pl = plist->list[i];
        if (nn_strcmp(alias, pl->alias) || nn_strcmp(name, pl->name) || nn_strcmp(filename, pl->filename)) {
            return pl;
        }
    }
    return NULL;
}

void plist_preserve(PluginList *plist) {
    if (plist->count == plist->alloc) {
        plist->alloc *= 2;
        plist->list = realloc(plist->list, sizeof(Plugin*) * plist->alloc);
        if (plist->list == NULL)
            log_FATAL("plist_preserve");
    }
}

void plist_add(PluginList *plist, Plugin *plugin) {
    plist_preserve(plist);
    plist->list[plist->count++] = plugin;
}

void plist_load(PluginList *plist, const char *filename) {
    plist_add(plist, pl_open(filename));
}

void plist_loaddir(PluginList *plist, const char *dir) {
    DIR *pluginDir = opendir(dir);
    if (pluginDir == NULL)
        log_FATAL("plist_loaddir");

    struct dirent *element;
    char path[PATH_MAX];
    while ((element = readdir(pluginDir))) {
        if (element->d_name[0] == '.')
            continue;
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, element->d_name);
        plist_load(plist, path);
    }
    closedir(pluginDir);
}


Plugin *pl_open(const char *filename) {
    log_DEBUG("Plugin", "Loading %s\n", filename);
    Plugin *plugin = (Plugin*) calloc(sizeof(Plugin), 1);
    if (plugin == NULL)
        log_FATAL("pl_open");

    strncpy(plugin->filename, filename, PLUGIN_FILENAME_MAX);
    
    plugin->L = luaL_newstate();
    luaL_openlibs(plugin->L);

    if (luaL_dofile(plugin->L, plugin->filename) == LUA_OK) {
        lua_pop(plugin->L, lua_gettop(plugin->L));
    }
    else {
        const char *errorMsg = lua_tostring(plugin->L, -1);
        log_ERROR("Plugin", "Can't open plugin %s : %s\n", filename, errorMsg);
        pl_close(plugin);
        return NULL;
    }

    lua_getglobal(plugin->L, "pl_name");
    plugin->name = lua_tostring(plugin->L, -1);

    lua_getglobal(plugin->L, "pl_version");
    plugin->version = lua_tostring(plugin->L, -1);

    lua_getglobal(plugin->L, "pl_author");
    plugin->author = lua_tostring(plugin->L, -1);

    lua_getglobal(plugin->L, "pl_alias");
    plugin->alias = lua_tostring(plugin->L, -1);

    return plugin;
}

void pl_close(Plugin *plugin) {
    log_DEBUG("Plugin", "Unloading %s\n", plugin->filename);
    lua_close(plugin->L);
    free(plugin);
}

bool pl_command(Plugin *plugin, const char *command) {
    lua_getglobal(plugin->L, "pl_command");
    if (lua_isfunction(plugin->L, -1) == 0) {
        log_WARN("Plugin", "No function pl_command in plugin %s\n", plugin->name);
        return false;
    }
    bool result = false;
    lua_pushlstring(plugin->L, command, strlen(command));
    if (lua_pcall(plugin->L, 1, 1, 0) == LUA_OK) {
        if (lua_isboolean(plugin->L, -1) == 1) {
            result = lua_toboolean(plugin->L, -1);
        }
        lua_pop(plugin->L, lua_gettop(plugin->L));
    }
    return result;
}

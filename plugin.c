#include <stdlib.h>
#include <string.h>

#include "plugin.h"
#include "utils.h"
#include "api.h"


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

void plist_reload(PluginList *plist) {
    log_DEBUG("Plugin", "Reloading all!\n");
    for (size_t i = 0; i < plist->count; i++) {
        plist->list[i] = pl_reload(plist->list[i]);
    }
}

Plugin *pl_open(const char *filename) {
    log_DEBUG("Plugin", "Loading %s\n", filename);
    Plugin *plugin = (Plugin*) calloc(sizeof(Plugin), 1);
    if (plugin == NULL)
        log_FATAL("pl_open");

    strncpy(plugin->filename, filename, PLUGIN_FILENAME_MAX);
    plugin->cmdlist = cmdlist_open();

    plugin->L = luaL_newstate();
    luaopen_base(plugin->L);
    luaopen_string(plugin->L);
    luaopen_table(plugin->L);
    luaopen_math(plugin->L);

    lua_pushliteral(plugin->L, REG_PLUGIN_PTR);
    lua_pushlightuserdata(plugin->L, (void*) plugin);
    lua_settable(plugin->L, LUA_REGISTRYINDEX);

    lua_newtable(plugin->L);
    luaL_setfuncs(plugin->L, pluginAPI, 0);
    lua_setglobal(plugin->L, "api");

    if (luaL_dofile(plugin->L, plugin->filename) == LUA_OK) {
        lua_pop(plugin->L, lua_gettop(plugin->L));
    }
    else {
        const char *errorMsg = lua_tostring(plugin->L, -1);
        log_ERROR("Plugin", "Can't open plugin %s : %s\n", filename, errorMsg);
        pl_close(plugin);
        return NULL;
    }

    return plugin;
}

void pl_close(Plugin *plugin) {
    log_DEBUG("Plugin", "Unloading %s\n", plugin->filename);
    lua_close(plugin->L);
    cmdlist_close(plugin->cmdlist);
    free(plugin);
}

Plugin *pl_reload(Plugin *plugin) {
    Plugin *newplugin = pl_open(plugin->filename);
    pl_close(plugin);
    return newplugin;
}

bool pl_command(Plugin *plugin, const char *command) {
    Cmd *cmd = cmdlist_get(plugin->cmdlist, command);
    if (!cmd)
        return false;

    lua_rawgeti(plugin->L, LUA_REGISTRYINDEX, cmd->lua_function);
    lua_pushlstring(plugin->L, command, strlen(command));
    if (lua_pcall(plugin->L, 1, 1, 0) == LUA_OK) {
        lua_pop(plugin->L, lua_gettop(plugin->L));
    }
    else {
        const char *errorMsg = lua_tostring(plugin->L, -1);
        log_ERROR("Plugin", "Unsuccess command %s: %s\n", command, errorMsg);
    }

    return true;
}

Plugin *pl_fromlua(lua_State *L) {
    lua_pushliteral(L, REG_PLUGIN_PTR);
    lua_gettable(L, LUA_REGISTRYINDEX);
    Plugin *plugin = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return plugin;
}

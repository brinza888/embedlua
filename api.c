#include "api.h"
#include "plugin.h"
#include "command.h"

#include "utils.h"


luaL_Reg pluginAPI[] = {
    {"setname", api_setname},
    {"setversion", api_setversion},
    {"setauthor", api_setauthor},
    {"setalias", api_setalias},
    {"regcmd", api_regcmd},
};


int api_setname(lua_State *L) {
    Plugin *plugin = pl_fromlua(L);
    plugin->name = luaL_checklstring(L, 1, NULL);
    return 0;
}

int api_setversion(lua_State *L) {
    Plugin *plugin = pl_fromlua(L);
    plugin->version = luaL_checklstring(L, 1, NULL);
    return 0;
}

int api_setauthor(lua_State *L) {
    Plugin *plugin = pl_fromlua(L);
    plugin->author = luaL_checklstring(L, 1, NULL);
    return 0;
}

int api_setalias(lua_State *L) {
    Plugin *plugin = pl_fromlua(L);
    plugin->alias = luaL_checklstring(L, 1, NULL);
    return 0;
}

int api_regcmd(lua_State *L) {
    Plugin *plugin = pl_fromlua(L);
    const char *command_name = luaL_checklstring(L, 1, NULL);
    const char *description = luaL_checklstring(L, 2, NULL);
    int func = luaL_ref(L, LUA_REGISTRYINDEX);
    log_DEBUG("PluginAPI", "Registered for %s command %s (ref: %d)\n",
        plugin->name,
        command_name,
        func
    )
    cmdlist_add(plugin->cmdlist, cmd_open(command_name, description, func));
    return 0;
}

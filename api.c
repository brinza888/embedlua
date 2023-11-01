#include "api.h"


int plugin_setname(lua_State *L) {

}

int plugin_setversion(lua_State *L) {

}

int plugin_setauthor(lua_State *L) {

}

int plugin_setalias(lua_State *L) {

}

int plugin_regcmd(lua_State *L) {
    const char *command_name = luaL_checklstring(L, 1, NULL);
    const char *description = luaL_checklstring(L, 2, NULL);
    cmdlist_add(curr_plugin->cmdlist, cmd_open(command_name, description));
    return 0;
}

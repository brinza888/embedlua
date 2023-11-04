#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
#include <lua5.4/lauxlib.h>


int api_setname(lua_State *L);
int api_setversion(lua_State *L);
int api_setauthor(lua_State *L);
int api_setalias(lua_State *L);

int api_regcmd(lua_State *L);


extern luaL_Reg pluginAPI[];

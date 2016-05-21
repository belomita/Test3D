#include "lua_extention.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int stacktrace_error_handler(lua_State* L)
{
	const char* msg = luaL_checkstring(L, 1);
	luaL_traceback(L, L, msg, 1);
	return 1;
}

int lua_pcall_stacktrace(lua_State* L, int nargs, int nret)
{
	/* calculate stack position for message handler */
	int hpos = lua_gettop(L) - nargs;
	int ret = 0;
	/* push custom error message handler */
	lua_pushcfunction(L, stacktrace_error_handler);
	/* move it before function and arguments */
	lua_insert(L, hpos);
	/* call lua_pcall function with custom handler */
	ret = lua_pcall(L, nargs, nret, hpos);
	/* remove custom error message handler from stack */
	lua_remove(L, hpos);
	/* pass return value of lua_pcall */
	return ret;
}


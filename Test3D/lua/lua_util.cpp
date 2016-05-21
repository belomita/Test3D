#include "script_system.h"

#include "util/logger.h"


static int lua_util_log_message(lua_State* L)
{
	const char* msg = luaL_optstring(L, 1, "");
	int level = (int)lua_tointeger(L, lua_upvalueindex(1));

	util_log_message(level, msg);
	return 0;
}

static void lua_util_register_log(lua_State* L, int level, const char* name)
{
	lua_pushinteger(L, level);
	lua_pushcclosure(L, lua_util_log_message, 1);
	lua_setglobal(L, name);
}

void lua_open_util_lib(lua_State* L)
{
	lua_util_register_log(L, LogLevel_Debug, "util_log_debug");
	lua_util_register_log(L, LogLevel_Info,  "util_log_info");
	lua_util_register_log(L, LogLevel_Sys,   "util_log_sys");
	lua_util_register_log(L, LogLevel_Warn,  "util_log_warn");
	lua_util_register_log(L, LogLevel_Error, "util_log_err");

}

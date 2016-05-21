#include "script_system.h"
#include "util/logger.h"

static lua_State*  g_LuaState = nullptr;

static int         g_ScriptSystemFuncMap[SCRIPT_FUNC_MAX] = { LUA_NOREF };
static const char* g_ScriptSystemFuncNames[SCRIPT_FUNC_MAX] = {
	"SCRIPT_FUNC_NONE",
	"SCRIPT_FUNC_START",
	"SCRIPT_FUNC_UPDATE",
	"SCRIPT_FUNC_STOP",
};
int  script_system_export(lua_State* L)
{
	int func_type = (int)luaL_checkinteger(L, 1);
	if(func_type > SCRIPT_FUNC_NONE && func_type < SCRIPT_FUNC_MAX) {
		luaL_checktype(L, 2, LUA_TFUNCTION);

		lua_pushvalue(L, 2);
		int reference = luaL_ref(L, LUA_REGISTRYINDEX);
		g_ScriptSystemFuncMap[func_type] = reference;
	}
	return 0;
}

bool script_system_init()
{
	g_LuaState = luaL_newstate();
	luaL_openlibs(g_LuaState);
	script_system_register_lua(script_system_export);

	script_system_register_libs(g_LuaState);
	
	if(script_system_do_file("pub/scripts/startup.lua"))
		return true;
	return false;
}

void script_system_uninit()
{
	if (g_LuaState) {
		lua_close(g_LuaState);
		g_LuaState = nullptr;
	}
}

lua_State* script_system_get_state()
{
	return g_LuaState;
}

void script_system_register_full(const char* name, lua_CFunction func)
{
	if (!g_LuaState)
		return;

	lua_pushcfunction(g_LuaState, func);
	lua_setglobal(g_LuaState, name);
}

bool script_system_start()
{
	if (!g_LuaState)
		return false;
	if(script_system_do_file("pub/scripts/main.lua"))
		return script_system_invoke(SCRIPT_FUNC_START);
	return false;
}

void script_system_update()
{
	if (!g_LuaState)
		return;
	script_system_invoke(SCRIPT_FUNC_UPDATE);
}

void script_system_stop()
{
	if(!g_LuaState)
		return;
	script_system_invoke(SCRIPT_FUNC_STOP);
}

bool script_system_do_file(const char* fname)
{
	if (!g_LuaState)
		return false;
	lua_State* L = g_LuaState;
	int err = luaL_loadfile(L, fname);
	if (err) {
		util_log_err("script_system_do_file: load file '%s' failed\nreason: %s", fname, lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}
	err = lua_pcall_stacktrace(L, 0, 0);
	if (err) {
		util_log_err(lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}
	return true;
}

bool script_system_invoke(ScriptSystemFunc func_index)
{
	if (!g_LuaState)
		return false;
	if (func_index <= SCRIPT_FUNC_NONE || func_index >= SCRIPT_FUNC_MAX)
		return false;

	lua_State* L = g_LuaState;

	int func_ref = g_ScriptSystemFuncMap[func_index];
	if (func_ref != LUA_NOREF) {
		int type = lua_rawgeti(L, LUA_REGISTRYINDEX, func_ref);
		if (type != LUA_TFUNCTION) {
			util_log_err("script_system_invoke: function %s not exists.", g_ScriptSystemFuncNames[func_index]);
			lua_pop(L, 1);
			return false;
		}
		int err = lua_pcall_stacktrace(L, 0, 0);
		if (err) {
			util_log_err(lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}
	}
	return true;
}


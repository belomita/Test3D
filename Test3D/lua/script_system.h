#pragma once

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lua_extention.h"

enum ScriptSystemFunc
{
	SCRIPT_FUNC_NONE   = 0,
	SCRIPT_FUNC_START  = 1,
	SCRIPT_FUNC_UPDATE = 2,
	SCRIPT_FUNC_STOP   = 3,

	SCRIPT_FUNC_MAX,
};

bool       script_system_init();
void       script_system_uninit();

lua_State* script_system_get_state();
void       script_system_register_libs(lua_State*);

bool       script_system_start();
void       script_system_update();
void       script_system_stop();

bool       script_system_invoke(ScriptSystemFunc func_index);
bool       script_system_do_file(const char* fname);

void       script_system_pcall_stacktrace(lua_State* L, int nargs, int nret);

void       script_system_register_full(const char* name, lua_CFunction func);
#define    script_system_register_lua(func) script_system_register_full(#func, func)

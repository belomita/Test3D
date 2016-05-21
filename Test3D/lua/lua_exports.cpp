#include "script_system.h"
#include "lua_imgui.h"

extern void lua_open_util_lib(lua_State*);

void script_system_register_libs(lua_State* L)
{
	lua_open_util_lib(L);
	lua_open_imgui_lib(L);
}

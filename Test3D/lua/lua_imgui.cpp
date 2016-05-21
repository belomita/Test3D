#include "script_system.h"

#include "util/logger.h"
#include "imgui/imgui.h"
#include "imgui/imgui_dx11.h"
#include <map>
#include <string>

#include "lua_imgui.h"

int imgui_func_table_ref = LUA_NOREF;

struct ImguiLuaFunc
{
	lua_State*  L = nullptr;
	std::string func_name;
	int         func_ref = LUA_NOREF;
	int         func_result = true;
};
static std::map<std::string, ImguiLuaFunc*>  imgui_func_map;

extern void LuaImGuiBegin();
extern const char* LuaImGuiEnd();
void imgui_lua_wrapped(ImGuiIO& io, void* luafunc)
{
	assert(imgui_func_table_ref != LUA_NOREF);

	ImguiLuaFunc* func = (ImguiLuaFunc*)luafunc;
	const char* func_name = func->func_name.c_str();
	int func_ref = func->func_ref;
	int last_result = func->func_result;
	if (func_ref != LUA_NOREF && last_result) {
		lua_State* L = func->L;

		int top = lua_gettop(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, imgui_func_table_ref); //+1
		int abs_index = lua_absindex(L, -1);

		int type = lua_rawgeti(L, abs_index, func_ref); //+1
		if (type != LUA_TFUNCTION) {
			util_log_err("[Lua ImGui]: function %s not exists.", func_name);
			lua_pop(L, 1);
			return;
		}

		LuaImGuiBegin();
		int err = lua_pcall_stacktrace(L, 0, 0); //-1
		const char* imgui_msg = LuaImGuiEnd();
		if (err) {
			func->func_result = false;
			util_log_err("[Lua ImGui]: call function '%s' error: %s", func_name, lua_tostring(L, -1));
			lua_pop(L, 1);
			return;
		}
		else {
			if (imgui_msg) {
				func->func_result = false;
				util_log_warn("[Lua ImGui]: call function '%s' warning:\n %s", func_name, imgui_msg);
			}
		}
		lua_pop(L, 1);
		int top2 = lua_gettop(L);
		assert(top == top2);
		lua_settop(L, top);
	}
}

int imgui_register_func(lua_State* L)
{
	assert(imgui_func_table_ref != LUA_NOREF);

	std::string func_name = luaL_checkstring(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	auto it = imgui_func_map.find(func_name);
	if (it != imgui_func_map.end()) {
		int old_ref = it->second->func_ref;
		it->second->func_result = true;
		luaL_unref(L, imgui_func_table_ref, old_ref);
	}
	else {
		ImguiLuaFunc* func = new ImguiLuaFunc;
		func->L = L;
		func->func_name = func_name;

		auto ret = imgui_func_map.emplace(func_name, func);
		it = ret.first;
		ImGui_RegisterFunc(&imgui_lua_wrapped, it->second);
	}
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, imgui_func_table_ref);
	int abs_index = lua_absindex(L, -1);

	lua_pushvalue(L, 2);
	int func_ref = luaL_ref(L, abs_index);
	it->second->func_ref = func_ref;

	return 0;
}

void lua_imgui_init()
{
	lua_State* L = script_system_get_state();

	lua_newtable(L);
	imgui_func_table_ref = luaL_ref(L, LUA_REGISTRYINDEX);
}

void lua_imgui_uninit()
{
	lua_State* L = script_system_get_state();
	luaL_unref(L, LUA_REGISTRYINDEX, imgui_func_table_ref);
	imgui_func_table_ref = LUA_NOREF;
}


extern void LoadImguiBindings(lua_State* );
void lua_open_imgui_lib(lua_State* L)
{
	LoadImguiBindings(L);
	script_system_register_lua(imgui_register_func);
}

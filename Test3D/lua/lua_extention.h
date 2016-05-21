#pragma once

struct lua_State;
int lua_pcall_stacktrace(lua_State* L, int nargs, int nret);
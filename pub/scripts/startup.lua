
__script_system_share  = {}
__script_system_consts = {}
__script_system_modules = {}

global_exports = setmetatable({}, {
	__newindex = function(t, k, v) rawset(t, k, v); rawset(_ENV, k, v) end
})

system_exports = setmetatable({}, {
	__newindex = function(t, k, v) script_system_export(k, v) end
})

setmetatable(_ENV, { 
	__index = function(t, k)
		return __script_system_consts[k] or __script_system_share[k] or global_exports[k]
	end
})

function pcall(fn, ...)
	local msg_handler = function (msg)
		return debug.traceback(msg, 2)
	end
	return xpcall(fn, msg_handler, ...)
end

function script_system_module(module_name)
	local m = __script_system_modules[module_name]
	if not m then
		m = setmetatable({}, {__index = _ENV })
		__script_system_modules[module_name] = m
	end
	return m
end

function script_system_do_file(fname, env)
	local f, err = io.open(fname, 'rb')
	if not f then error(err) end
	
	local content = f:read("a")
	local func, err = load(content, fname, "bt", env or _ENV)
	if not func then error(err) end
	
	local ok, err = pcall(func)
	if not ok then
		log_warn(err)
	end
end


script_system_do_file("pub/scripts/enums_base.lua", __script_system_consts)
script_system_do_file("pub/scripts/logger.lua", _ENV)





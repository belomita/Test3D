local function __emit_log_fmt_func(fnlog)
	return function (fmt, ...)
		local msg = string.format(fmt, ...)
		fnlog(msg)
	end
end

local function __emit_log_func(fnlog)
	return function (...)
		local args = {...}
		local argstrings = {}
		for _, v in ipairs(args) do 
			argstrings[#argstrings+1] = tostring(v)
		end
		local msg = table.concat(argstrings, '    ')
		fnlog(msg)
	end
end

local _util_log_debug = util_log_debug
local _util_log_info  = util_log_info 
local _util_log_sys   = util_log_sys  
local _util_log_warn  = util_log_warn 
local _util_log_err   = util_log_err  

log_fmt_debug = __emit_log_fmt_func(_util_log_debug)
log_fmt_info  = __emit_log_fmt_func(_util_log_info )
log_fmt_sys   = __emit_log_fmt_func(_util_log_sys  )
log_fmt_warn  = __emit_log_fmt_func(_util_log_warn )
log_fmt_err   = __emit_log_fmt_func(_util_log_err  )

log_debug     = __emit_log_func(_util_log_debug)
log_info      = __emit_log_func(_util_log_info )
log_sys       = __emit_log_func(_util_log_sys  )
log_warn      = __emit_log_func(_util_log_warn )
log_err       = __emit_log_func(_util_log_err  )
print         = log_debug

script_system_do_file("pub/scripts/ui.lua")

local function start()
	global_exports.ui_startup()
	ui_stop()
end

local function update()
	
end


local function stop()
	log_sys("stop!")
end

system_exports[SCRIPT_FUNC_START]  = start
system_exports[SCRIPT_FUNC_UPDATE] = update
system_exports[SCRIPT_FUNC_STOP]   = stop

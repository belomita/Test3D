
local _ENV = script_system_module('ui')

print("ui module now!")



local opened = true
local function test_imgui()
	if not opened then return end
	
	local ok
	ok, opened = imgui.Begin('LuaImGuiWindow', opened)
	if not ok then 
		imgui.End()
		return
	end
	
	imgui.Text('0')
	imgui.End()
end

function ui_startup()
	imgui_register_func('test_imgui', test_imgui)
end

function ui_stop()
	print("ui stop!")
end

function ui_update()
	
end 

global_exports.ui_startup = ui_startup
global_exports.ui_stop = ui_stop




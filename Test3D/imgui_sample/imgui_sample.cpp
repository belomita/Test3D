#include "imgui_sample.h"
#include "imgui/imgui.h"
#include "imgui/imgui_dx11.h"


void ImGuiSimpleTest(ImGuiIO& io, void*)
{
	static bool opened = true;
	if(!ImGui::Begin("SimpleTest", &opened)){
		ImGui::End();
		return;
	}
 
	ImGui::Text("Hello, world!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

void ImGuiSample_Init()
{
	ImGui_RegisterFunc(&ImGuiSimpleTest);
}


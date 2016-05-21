#include "imgui_dx11.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"

#include "dx11/dx11_layer.h"

#include <vector>
	
static void _ImGui_InitFonts()
{
	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("pub/imgui/DroidSans.ttf", 17.0f);
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
}


static bool g_bInited = false;
bool ImGui_Init(HWND hWnd)
{
	ImGui_ImplDX11_Init(hWnd, D3D_GetDevice(), D3D_GetDeviceContext());
	_ImGui_InitFonts();

	g_bInited = true;
	return true;
}

void ImGui_UnInit()
{
	if (!g_bInited) {
		return;
	}
	ImGui_ImplDX11_Shutdown();
	g_bInited = false;
}


extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ImGui_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam))
		return TRUE;

	return FALSE;
}

void ImGui_Render()
{
	// Rendering
	ImGui::Render();
}

struct ImGuiFuncInfo
{
	ImGuiFunc pFunc     = nullptr;
	void*     pUserData = nullptr;
};

static std::vector<ImGuiFuncInfo>  g_FuncList;
void ImGui_RegisterFunc(ImGuiFunc func, void* userdata)
{
	ImGuiFuncInfo info;
	info.pFunc     = func;
	info.pUserData = userdata;
	g_FuncList.push_back(info);
}

void ImGui_Run()
{
	ImGui_ImplDX11_NewFrame();
	for(auto& info: g_FuncList) {
		info.pFunc(ImGui::GetIO(), info.pUserData);
	}
}

void ImGui_BeforeVideoChange()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
}

void ImGui_AfterVideoChange(UINT width, UINT height)
{
	ImGui_ImplDX11_CreateDeviceObjects();
}



#include <tchar.h>

#include "dx11/dx11_layer.h"
#include "imgui/imgui_dx11.h"
#include "imgui_sample/imgui_sample.h"

#include "app.h"
#include "lua/script_system.h"

#include "lua/lua_imgui.h"

static HINSTANCE g_hInstance = nullptr;
static HWND g_hWnd = nullptr;

LRESULT WINAPI _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(ImGui_WndProc(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
		case WM_SIZE:
		{
			ImGui_BeforeVideoChange();
			D3D_BeforeVideoChange();

			UINT width  = (UINT)LOWORD(lParam);
			UINT height = (UINT)HIWORD(lParam);
			D3D_AfterVideoChange(width, height);
			ImGui_AfterVideoChange(width, height);
			return 0;
		}
		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void _App_CreateWnd()
{
	WNDCLASSEX wc = { 
			sizeof(WNDCLASSEX), 
			CS_CLASSDC, 
			_WndProc, 
			0L, 0L, 
			g_hInstance, 
			NULL, 
			LoadCursor(NULL, IDC_ARROW), 
			NULL, 
			NULL, 
			_T("TESTBED"), 
			NULL 
	};

	RegisterClassEx(&wc);
	g_hWnd = CreateWindow(
		_T("TESTBED"), 
		_T("Testbed"), 
		WS_OVERLAPPEDWINDOW, 
		100, 100, 1280, 800, 
		NULL, NULL, 
		wc.hInstance,
		NULL
	);
}

bool App_Init()
{
	if(!script_system_init())
		return false;
		
	g_hInstance = GetModuleHandle(NULL);

	_App_CreateWnd();

	if(D3D_Init(g_hWnd) == S_OK) {
		ImGui_Init(g_hWnd);
		ImGuiSample_Init();

		ShowWindow(g_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(g_hWnd);
	
		lua_imgui_init();
		if (script_system_start())
			return true;
	}

	return false;
}

void App_UnInit()
{
	script_system_stop();

	lua_imgui_uninit();
	script_system_uninit();

	ImGui_UnInit();
	D3D_UnInit();

	UnregisterClass(_T("Testbed"), g_hInstance);
}

void App_Run()
{
	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	LARGE_INTEGER duetime;
	duetime.QuadPart = 0;
	SetWaitableTimer(hTimer, &duetime, 16, NULL, NULL, FALSE);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	for (;;) {
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		
		script_system_update();
		ImGui_Run();

		D3D_ClearScreen();
		ImGui_Render();
		D3D_Present();

		WaitForSingleObject(hTimer, INFINITE);
	}
}

HWND App_GetWnd()
{
	return g_hWnd;
}

HINSTANCE App_GetInstance()
{
	return g_hInstance;
}


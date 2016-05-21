#pragma once

#include <windows.h>

bool App_Init();
void App_UnInit();
void App_Run();

HWND      App_GetWnd();
HINSTANCE App_GetInstance();
#pragma once

#include <windows.h>

struct ImGuiIO;

bool    ImGui_Init(HWND hWnd);
void    ImGui_UnInit();
void    ImGui_Run();
void    ImGui_Render();

typedef void (*ImGuiFunc)(ImGuiIO&, void* userdata);
void    ImGui_RegisterFunc(ImGuiFunc func, void* userdata = nullptr);

LRESULT ImGui_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void    ImGui_BeforeVideoChange();
void    ImGui_AfterVideoChange(UINT width, UINT height);
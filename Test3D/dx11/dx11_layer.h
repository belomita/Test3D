#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

HRESULT              D3D_Init(HWND hWnd);
void                 D3D_UnInit();

ID3D11Device*        D3D_GetDevice();
ID3D11DeviceContext* D3D_GetDeviceContext();
IDXGISwapChain*      D3D_GetSwapChain();

void                 D3D_ClearScreen();
void                 D3D_Present();

void                 D3D_BeforeVideoChange();
void                 D3D_AfterVideoChange(UINT width, UINT height);
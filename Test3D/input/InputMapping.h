#pragma once

#include <map>
#include <vector>
#include <stdint.h>
#include <windows.h>
#include "InputTypes.h"

#include "math/Vector2.h"

class InputMapping
{
public:
	InputMapping(const std::vector<uint32_t>& userkeys, const std::vector<uint32_t> virtualkeys);
	~InputMapping();

	void     Update();
	void     PostUpdate();
	void     UpdateMouseDelta();

	LRESULT  HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	uint32_t GetUserKey(uint32_t virtual_key);
	uint8_t  GetUserKeyState(uint32_t user_key);

	void     AddUserKeyState(uint32_t user_key, uint8_t keystate);
	void     DelUserKeyState(uint32_t user_key, uint8_t keystate);
	void     SetUserKeyState(uint32_t user_key, uint8_t keystate);

	void     SetDragRect(const RECT& rc);

	bool     IsMouseDown(int mouse) { return m_MouseStates[mouse]; }
	Vector2  GetMousePos();
	Vector2  GetMouseDelta();

private:
	std::map<uint32_t, uint32_t>  m_KeyMap;     // virtual_key -> user_key
	std::map<uint32_t, uint8_t>   m_KeyStates;  // user_key    -> states
	int32_t                       m_KeyDownNum = 0;
	bool                          m_MouseStates[MouseButton::Count];
	uint8_t                       m_MouseButtonMask = 0;
	int32_t                       m_MouseWheelDelta = 0;

	RECT                          m_DragRect;
	POINT                         m_LastMousePosition;

	Vector2                       m_MouseDelta;
};
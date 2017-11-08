#include "InputMapping.h"
#include <assert.h>

#define INVALID_USER_KEY  0xFFFFFFFF

inline bool IsKeyDownState(uint8_t keystate)
{
	return ((keystate & KeyMask::IsDown)  == KeyMask::IsDown);
}
inline bool WasKeyDownState(uint8_t keystate)
{
	return ((keystate & KeyMask::WasDown) == KeyMask::WasDown);
}

InputMapping::InputMapping(const std::vector<uint32_t>& userkeys, const std::vector<uint32_t> virtualkeys)
{
	assert(userkeys.size() == virtualkeys.size());
	for (size_t i = 0; i < userkeys.size(); ++i) {
		m_KeyMap.emplace(virtualkeys[i], userkeys[i]);
		m_KeyStates.emplace(userkeys[i], 0);
	}

	for (int i = 0; i < MouseButton::Count; i++) {
		m_MouseStates[i] = false;
	}

	SetRect(&m_DragRect, LONG_MIN, LONG_MIN, LONG_MAX, LONG_MAX );
	m_LastMousePosition.x = m_LastMousePosition.y = 0;
	m_MouseDelta = Vector2::ZERO;
}

InputMapping::~InputMapping()
{

}

LRESULT InputMapping::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( lParam );

	switch( uMsg ) {
		case WM_KEYDOWN:
		{
			uint32_t user_key = GetUserKey((uint32_t)wParam);
			if( user_key != INVALID_USER_KEY ) {
				uint8_t keystate = GetUserKeyState(user_key);
				if(!IsKeyDownState(keystate) ) {
					SetUserKeyState(user_key, KeyMask::IsDown | KeyMask::WasDown);
					++m_KeyDownNum;
				}
			}
			break;
		}
		case WM_KEYUP:
		{
			uint32_t user_key = GetUserKey(( uint32_t )wParam);
			if( user_key != INVALID_USER_KEY) {
				DelUserKeyState(user_key, KeyMask::IsDown);
				--m_KeyDownNum;
			}
			break;
		}
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
			{
				POINT cursor_pos = {
					( short )LOWORD( lParam ), ( short )HIWORD( lParam )
				};
				BOOL in_rect = PtInRect( &m_DragRect, cursor_pos);

				// Update member var state
				if(( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && in_rect) {
					m_MouseStates[MouseButton::Left] = true; 
					m_MouseButtonMask |= MouseMask::Left;
				}
				if(( uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && in_rect) {
					m_MouseStates[MouseButton::Middle] = true;
					m_MouseButtonMask |= MouseMask::Middle;
				}
				if(( uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && in_rect) {
					m_MouseStates[MouseButton::Right] = true;
					m_MouseButtonMask |= MouseMask::Right;
				}

				// Capture the mouse, so if the mouse button is 
				// released outside the window, we'll get the WM_LBUTTONUP message
				SetCapture( hWnd );
				GetCursorPos( &m_LastMousePosition );
				return TRUE;
			}

		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_LBUTTONUP:
			{
				// Update member var state
				if( uMsg == WM_LBUTTONUP ) {
					m_MouseStates[MouseButton::Left] = false; 
					m_MouseButtonMask &= ~MouseMask::Left;
				}
				if( uMsg == WM_MBUTTONUP ) {
					m_MouseStates[MouseButton::Middle] = false; 
					m_MouseButtonMask &= ~MouseMask::Middle;
				}
				if( uMsg == WM_RBUTTONUP ) {
					m_MouseStates[MouseButton::Right] = false; 
					m_MouseButtonMask &= ~MouseMask::Right;
				}

				// Release the capture if no mouse buttons down
				if( !m_MouseStates[MouseButton::Left]   &&
					!m_MouseStates[MouseButton::Middle] &&
					!m_MouseStates[MouseButton::Right] ) {
					ReleaseCapture();
				}
				break;
			}

		case WM_CAPTURECHANGED:
		{
			if((HWND)lParam != hWnd) {
				if( (m_MouseButtonMask & MouseMask::Left  ) ||
					(m_MouseButtonMask & MouseMask::Middle) ||
					(m_MouseButtonMask & MouseMask::Right )) {
					m_MouseStates[MouseButton::Left] = false;
					m_MouseStates[MouseButton::Middle] = false;
					m_MouseStates[MouseButton::Right] = false;
					m_MouseButtonMask &= ~MouseMask::Left;
					m_MouseButtonMask &= ~MouseMask::Middle;
					m_MouseButtonMask &= ~MouseMask::Right;
					ReleaseCapture();
				}
			}
			break;
		}

		case WM_MOUSEWHEEL:
			// Update member var state
			m_MouseWheelDelta += ( short )HIWORD( wParam );
			break;
	}

	return FALSE;
}

void InputMapping::Update()
{
	UpdateMouseDelta();
}

void InputMapping::PostUpdate()
{
	m_MouseWheelDelta = 0;
}

uint32_t InputMapping::GetUserKey(uint32_t virtual_key)
{
	auto it = m_KeyMap.find(virtual_key);
	if (it != m_KeyMap.end()) {
		return it->second;
	}
	return INVALID_USER_KEY;
}

uint8_t InputMapping::GetUserKeyState(uint32_t user_key)
{
	auto it = m_KeyStates.find(user_key);
	if (it != m_KeyStates.end()) {
		return it->second;
	}
	return 0;
}

void InputMapping::AddUserKeyState(uint32_t user_key, uint8_t keystate)
{
	assert(m_KeyStates.find(user_key) != m_KeyStates.end());
	m_KeyStates[user_key] |= keystate;
}

void InputMapping::DelUserKeyState(uint32_t user_key, uint8_t keystate)
{
	assert(m_KeyStates.find(user_key) != m_KeyStates.end());
	m_KeyStates[user_key] &= ~keystate;
}

void InputMapping::SetUserKeyState(uint32_t user_key, uint8_t keystate)
{
	assert(m_KeyStates.find(user_key) != m_KeyStates.end());
	m_KeyStates[user_key] = keystate;
}

void InputMapping::SetDragRect(const RECT& rc)
{
	m_DragRect = rc;
}

Vector2 InputMapping::GetMousePos()
{
	return Vector2((float)m_LastMousePosition.x, (float)m_LastMousePosition.y);
}

Vector2 InputMapping::GetMouseDelta()
{
	return m_MouseDelta;
}

void InputMapping::UpdateMouseDelta()
{
	POINT curMouseDelta;
	POINT curMousePos;

	// Get current position of mouse
	GetCursorPos(&curMousePos);

	// Calc how far it's moved since last frame
	curMouseDelta.x = curMousePos.x - m_LastMousePosition.x;
	curMouseDelta.y = curMousePos.y - m_LastMousePosition.y;

	// Record current position for next time
	m_LastMousePosition = curMousePos;

	// Smooth the relative mouse data over a few frames so it isn't 
	// jerky when moving slowly at low frame rates.
	float fPercentOfNew = 1.0f / 2.f;
	float fPercentOfOld = 1.0f - fPercentOfNew;
	m_MouseDelta.x = m_MouseDelta.x * fPercentOfOld + curMouseDelta.x * fPercentOfNew;
	m_MouseDelta.y = m_MouseDelta.y * fPercentOfOld + curMouseDelta.y * fPercentOfNew;
}

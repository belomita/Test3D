#include "FreeCamera.h"

#include "input/InputMapping.h"

#include <stdint.h>
#include <windows.h>
#include <vector>
#include "math/Quaternion.h"

enum FreeCameraKeys
{
	CAM_STRAFE_LEFT = 0,
	CAM_STRAFE_RIGHT,
	CAM_MOVE_FORWARD,
	CAM_MOVE_BACKWARD,
	CAM_MOVE_UP,
	CAM_MOVE_DOWN,
	CAM_RESET,
	CAM_CONTROLDOWN,
	CAM_MAX_KEYS,
};

static std::vector<uint32_t> user_keys = {
	CAM_STRAFE_LEFT,
	CAM_STRAFE_RIGHT,
	CAM_MOVE_FORWARD,
	CAM_MOVE_BACKWARD,
	CAM_MOVE_UP,
	CAM_MOVE_DOWN,
	CAM_RESET,
	CAM_CONTROLDOWN,
};

static std::vector<uint32_t> vkeys = {
	'A',
	'D',
	'W',
	'S',
	'Q',
	'E',
	'R',
	VK_CONTROL,
};

FreeCamera::FreeCamera()
{
	m_Info  = new CameraInfo;
	m_Input = new InputMapping(user_keys, vkeys);
}

FreeCamera::~FreeCamera()
{
	SAFE_DELETE(m_Info);
	SAFE_DELETE(m_Input);
}

CameraType FreeCamera::Type()
{
	return CameraType::Free;
}

void FreeCamera::Enter()
{
	m_Info->position = Vector3::ZERO;
	m_Info->orient   = Vector3::ZERO;
	m_Info->rotation = Matrix3::IDENTITY;
	m_Info->fov      = Vector2(80.f, 80.f);
}

void FreeCamera::Leave()
{
	
}

void FreeCamera::Update(float dt)
{
	dt = 0.016f;

	if (m_Input->IsMouseDown(MouseButton::Right)) {
		Vector2 delta = m_Input->GetMouseDelta();
		delta *= m_RotSpeed;

		Vector3& orient = m_Info->orient;
		orient.x += delta.x;
		orient.y += delta.y;

		orient.y = Math::Clamp(orient.y, -80.f, 80.f);

		m_Info->rotation = Quaternion(orient.x, orient.y, orient.z).RotationMatrix();
	}
}

const CameraInfo& FreeCamera::GetInfo()
{
	return *m_Info;
}

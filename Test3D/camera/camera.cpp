#include "camera.h"
#include "FreeCamera.h"

void CameraControl::Register(CameraBase* camera)
{
	m_Cameras.emplace(camera->Type(), camera);
}

void CameraControl::Activate(CameraType camera_type)
{
	if (m_Current) {
		m_Current->Leave();
	}
	m_Current = m_Cameras[camera_type];
	if (m_Current) {
		m_Current->Enter();
	}
}

void CameraControl::Update(float dt)
{
	if (m_Current) {
		m_Current->Update(dt);
	}
}

static CameraControl* control = nullptr;
void Camera_Init()
{
	if (!control) {
		control = new CameraControl;
	}

	control->Register(new FreeCamera);
	control->Activate(CameraType::Free);
}

void Camera_UnInit()
{
	SAFE_DELETE(control);
}

CameraControl* Camera_GetControl()
{
	return control;
}

void Camera_Update(float dt)
{
	if (control) {
		control->Update(dt);
	}
}

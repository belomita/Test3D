#pragma once

#include "camera.h"
class InputMapping;

class FreeCamera : public CameraBase
{
public:
	FreeCamera();
	~FreeCamera();

	virtual CameraType Type();
	virtual void Enter();
	virtual void Leave();
	virtual void Update(float dt);
	virtual const CameraInfo& GetInfo();

public:
	CameraInfo*    m_Info  = nullptr;
	InputMapping*  m_Input = nullptr;
	float          m_MoveSpeed = 100.f;
	float          m_RotSpeed  = 100.f;

};
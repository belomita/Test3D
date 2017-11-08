#pragma once

#include "util/util.h"
#include "math/Matrix3.h"
#include "math/Vector2.h"
#include "math/Vector3.h"

#include <map>

enum class CameraType
{
	None,
	Free,
	Follow,
};

struct CameraInfo
{
	Vector3  position;
	Vector3  orient;
	Matrix3  rotation;
	Vector2  fov;
};

class CameraBase
{
public:
	virtual CameraType          Type()  = 0;
	virtual void                Enter() = 0;
	virtual void                Leave() = 0;

	virtual void                Update(float dt) = 0;
	virtual const CameraInfo&   GetInfo() = 0;
};

class CameraControl
{
public:
	CameraControl() {}
	~CameraControl(){}

	void Register(CameraBase* camera);
	void Activate(CameraType camera_type);
	void Update(float dt);

	std::map<CameraType, CameraBase*>  m_Cameras;
	CameraBase*                        m_Current = nullptr;

private:
	DISALLOW_COPY_AND_ASSIGN(CameraControl);
};

void             Camera_Init();
void             Camera_UnInit();
void             Camera_Update(float dt);
CameraControl*   Camera_GetControl();
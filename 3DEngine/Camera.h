#pragma once

#include "Abstractions.h"

// ============================================================================
// Camera class
enum PROJECTION_TYPE {
	PT_PARALLEL	= 0,
	PT_CENTRAL	= 1
};

class clsCamera : public clsObject {
private:
	PROJECTION_TYPE projectionType;

	float			hFOV;		// FOV это угол. в радианах
	float			vFOV;
	float			nearClip;
	float			farClip;

public:
	clsCamera();
	clsCamera(
			PROJECTION_TYPE projType,
			float			horizFov,
			float			vertFov
		);

	PROJECTION_TYPE	getProjectionType();
	// void			getFOV(float* horiz, float* vert);
	float			getHFov();
	float			getVFov();
	// void			getClips(float* near, float* far);
	float			getNearCP();
	float			getFarCP();

	void			setProjectionType(PROJECTION_TYPE projType);
	// void			setFOV(float horiz, float vert);
	void			setHFov(float fieldOfViewHoriz);
	void			setVFov(float fieldOfViewVert);
	// void			setClips(float near, float far);
	void			setNearCP(float nearCP);
	void			setFarCP(float farCP);

	void			GetViewMatrix(LPMATRIX3D mOut);
	void			GetParallelMatrix(LPMATRIX3D mOut);
	void			GetPerspectiveMatrix(LPMATRIX3D mOut);
};
typedef clsCamera CAMERA3D, *LPCAMERA3D;

// ============================================================================
// TargetCamera class
class clsTargCamera : public clsCamera {
	VECTOR3D target;

	void Pitch();
	void Yaw();

	void LookAt(VECTOR3D lookAt);
	void LookAt(const clsObject *objToLookAt);
	void LookAt(float lX, float lY, float lZ);
public:
	clsTargCamera(float lX = 0, float lY = 0, float lZ = 0);
	clsTargCamera(
			PROJECTION_TYPE projType,
			float			horizFov,
			float			vertFov,
			float lX = 0, float lY = 0, float lZ = 0
		);

	VECTOR3D getTargetPoint();
	void setTargetPoint(LPVECTOR3D);
	void setTargetPoint(float tX, float tY, float tZ);
	// Absolute translation
	void Translate(const VECTOR3D tV);
	void Translate(float tX, float tY, float tZ);	

	void FollowLookAxis(float units);
	void StrafeLatitude(float units);
	void StrafeLongitude(float units);

	//Relative translation
	void Follow(float units);	// Along local-x
	void Strafe(float units);	// Along local-y
	void Fly(float units);		// Along local-z

	void TargetFollow(float units);	// Along local-x
	void TargetStrafe(float units);	// Along local-y
	void TargetFly(float units);	// Along local-z
};
typedef clsTargCamera TARGCAMERA3D, *LPTARGCAMERA3D;

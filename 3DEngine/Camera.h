#pragma once

#include "Abstractions.h"

// ============================================================================
// Camera class
// ============================================================================
enum PROJECTION_TYPE {
	PT_PARALLEL	= 0,
	PT_CENTRAL	= 1
};

class clsCamera : public clsHull {
protected:
	PROJECTION_TYPE projectionType;

	float			hFOV;		// FOV это угол. в радианах
	float			vFOV;
	float			fDist;
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
	float			getHFov();
	float			getVFov();
	float			getFDist();
	float			getNearCP();
	float			getFarCP();

	void			Zoom(float aspect);

	void			setProjectionType(PROJECTION_TYPE projType);
	void			setHFov(float fieldOfViewHoriz);
	void			setVFov(float fieldOfViewVert);
	void			setFDist(float dist);
	void			setNearCP(float nearCP);
	void			setFarCP(float farCP);

	void			GetViewMatrix(MATRIX3D &mOut);
	void			GetProjectionMatrix(MATRIX3D &mOutm);
};
typedef clsCamera CAMERA3D, *LPCAMERA3D;

// ============================================================================
// TargetCamera class
class clsTargCamera : public clsCamera {
private:
	HULL3D target;

	void setFDist(float dist);
	void Pitch(float angle);
	void Yaw(float angle);
	void LookAt(VECTOR3D lookAt);
	void LookAt(const clsObject &objToLookAt);
	void LookAt(float lX, float lY, float lZ);

public:
	clsTargCamera(float lX = 0, float lY = 0, float lZ = 0);
	clsTargCamera(
			PROJECTION_TYPE projType,
			float			horizFov,
			float			vertFov,
			float lX = 0, float lY = 0, float lZ = 0
		);

	VECTOR3D getTargetPosition();
	float	getTargetDistance();
	void	TargetTranslate(const VECTOR3D &);
	void	TargetTranslate(float tX, float tY, float tZ);
	// Absolute translation
	void Translate(const VECTOR3D &tV);
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

	// Override useful protected methods
};
typedef clsTargCamera TARGCAMERA3D, *LPTARGCAMERA3D;

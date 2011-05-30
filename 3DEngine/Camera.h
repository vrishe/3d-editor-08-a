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
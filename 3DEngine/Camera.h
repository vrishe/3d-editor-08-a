#pragma once

#include "Abstractions.h"

// ============================================================================
// Camera class
enum PROJECTION_TYPE {
	PARALLEL	= 0,
	CENTRAL		= 1
};

class clsCamera : public clsObject {
private:
	PROJECTION_TYPE projectionType;

	float			screenMult;
	float			FOV;		// FOV это угол. в радианах

public:
	clsCamera();

	PROJECTION_TYPE	getProjectionType();
	float			getScreenMult();
	float			getFOV();

	void			setProjectionType(PROJECTION_TYPE projType);
	void			setScreenMult(float scrMult);
	void			setFOV(float fieldOfView);

	void			GetViewMatrix(LPMATRIX3D mOut);
};
typedef clsCamera CAMERA3D, *LPCAMERA3D;
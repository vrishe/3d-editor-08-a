
#include "Camera.h"

// ============================================================================
// Implementation of clsCamera class:
clsCamera::clsCamera() : clsObject(CLS_CAMERA) 
{ 
	projectionType	= PARALLEL;
	screenMult		= 4.f / 3.f;
	FOV				= (float)M_PI_4;
}

PROJECTION_TYPE clsCamera::getProjectionType() { return projectionType; }

float clsCamera::getScreenMult() { return screenMult; }

float clsCamera::getFOV() { return FOV; }

void clsCamera::setProjectionType(PROJECTION_TYPE projType) 
{ 
	projectionType = projType; 
}

void clsCamera::setScreenMult(float scrMult) { screenMult = scrMult; }

void clsCamera::setFOV(float fieldOfView) 
{ 
	if ( fieldOfView > .0F ) FOV = fieldOfView; 
}

void clsCamera::GetViewMatrix(LPMATRIX3D mOut)
{
	Vector3DNormalize(&fWd, &fWd);

	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);

	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);


	mOut->_11 = rWd.x;
	mOut->_12 = uWd.x;
	mOut->_13 = fWd.x;
	mOut->_14 = .0f;

	mOut->_21 = rWd.y;
	mOut->_22 = uWd.y;
	mOut->_23 = fWd.y;
	mOut->_24 = .0f;

	mOut->_31 = rWd.z;
	mOut->_32 = uWd.z;
	mOut->_33 = fWd.z;
	mOut->_34 = .0f;

	mOut->_41 = -Vector3DMultS(&rWd, &pos);
	mOut->_42 = -Vector3DMultS(&uWd, &pos);
	mOut->_43 = -Vector3DMultS(&fWd, &pos);
	mOut->_44 = 1.0f;
}
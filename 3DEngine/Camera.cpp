
#include "Camera.h"

// ============================================================================
// Implementation of clsCamera class:
clsCamera::clsCamera() : clsObject(CLS_CAMERA) 
{ 
	projectionType	= PT_PARALLEL;
	hFOV			= (float)M_PI_2;
	vFOV			= (float)M_PI_2;
	nearClip		= 350.0F;
	farClip			= 1000.0F;
}

clsCamera::clsCamera(
	PROJECTION_TYPE projType,
	float			horizFov,
	float			vertFov
) {
	projectionType	= PT_PARALLEL;
	hFOV			= (float)M_PI_2;
	vFOV			= (float)M_PI_2;
	nearClip		= 350.0f;
	farClip			= 1000.0F;

	setProjectionType(projType);
	setHFov(horizFov);
	setVFov(vertFov);
}

PROJECTION_TYPE clsCamera::getProjectionType() { return projectionType; }
float clsCamera::getHFov() { return hFOV; }
float clsCamera::getVFov() { return vFOV; }
float clsCamera::getNearCP() { return nearClip; }
float clsCamera::getFarCP() { return farClip; }

void clsCamera::setProjectionType(PROJECTION_TYPE projType) 
{ 
	projectionType = projType; 
}

void clsCamera::setHFov(float fieldOfViewHoriz) 
{ 
	if ( fieldOfViewHoriz > .0F) hFOV = fieldOfViewHoriz; 
}

void clsCamera::setVFov(float fieldOfViewVert) 
{ 
	if ( fieldOfViewVert > .0F) vFOV = fieldOfViewVert; 
}

void clsCamera::setNearCP(float nearCP) 
{ 
	if ( nearCP >= .0F && nearCP < farClip ) nearClip = nearCP; 
}

void clsCamera::setFarCP(float farCP) 
{ 
	if ( farCP > nearClip ) farClip = farCP; 
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

void clsCamera::GetParallelMatrix(LPMATRIX3D mOut)
{
	mOut->SetIdentity();

	mOut->_33 = farClip / (farClip - nearClip);
	mOut->_43 = mOut->_33 * (-nearClip);
}

void clsCamera::GetPerspectiveMatrix(LPMATRIX3D mOut)
{
	float fDist = (farClip - nearClip) / 2;
	mOut->SetIdentity();

	mOut->_11 = fDist / tan(hFOV / 2.0f);
	mOut->_22 = fDist / tan(vFOV / 2.0f);
	mOut->_33 = farClip / (farClip - nearClip);
	mOut->_43 = mOut->_33 * (-nearClip);
}

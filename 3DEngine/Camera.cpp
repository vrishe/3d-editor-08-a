
#include "Camera.h"

// ============================================================================
// Implementation of clsCamera class:
clsCamera::clsCamera() : clsObject(CLS_CAMERA) 
{ 
	projectionType	= PT_PARALLEL;
	hFOV			= (float)M_PI_2;
	vFOV			= (float)M_PI_2;
	nearClip		= .0F;
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
	nearClip		= .0f;
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
	VECTOR3D	vF(1.0f, .0f, .0f),
				vR(.0f, 1.0f, .0f),
				vU(.0f, .0f, 1.0f);
	MATRIX3D	M;
	
	Matrix3DRotateAxis(&vR, -pitch, &M);
	Matrix3DTransformNormal(&M, &vF, &vF);
	Vector3DMultV(&vF, &vR, &vU);

	Matrix3DRotateAxis(&vU, -yaw, &M);
	Matrix3DTransformNormal(&M, &vF, &vF);
	Vector3DMultV(&vU, &vF, &vR);

	Matrix3DRotateAxis(&vF, -roll, &M);
	Matrix3DTransformNormal(&M, &vR, &vR);
	Vector3DMultV(&vF, &vR, &vU);	

	Vector3DNormalize(&vF, &vF);
	Vector3DMultV(&vF, &vR, &vU);
	Vector3DNormalize(&vU, &vU);
	Vector3DMultV(&vU, &vF, &vR);
	Vector3DNormalize(&vR, &vR);


	mOut->_11 = vR.x;
	mOut->_12 = vU.x;
	mOut->_13 = vF.x;
	mOut->_14 = .0f;

	mOut->_21 = vR.y;
	mOut->_22 = vU.y;
	mOut->_23 = vF.y;
	mOut->_24 = .0f;

	mOut->_31 = vR.z;
	mOut->_32 = vU.z;
	mOut->_33 = vF.z;
	mOut->_34 = .0f;

	mOut->_41 = -Vector3DMultS(&vR, &pos);
	mOut->_42 = -Vector3DMultS(&vU, &pos);
	mOut->_43 = -Vector3DMultS(&vF, &pos);
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

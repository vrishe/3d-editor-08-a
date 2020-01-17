
#include "Camera.h"
#include <string>

// ============================================================================
// Implementation of clsCamera class:
// ============================================================================
clsCamera::clsCamera() : clsHull(CLS_CAMERA) 
{ 
  projectionType  = PT_PARALLEL;
  hFOV      = (float)M_PI_2;
  vFOV      = (float)M_PI_2;
  nearClip    = 1.0F;
  farClip     = 1000.0F;
  fDist     = (farClip - nearClip) / 2;
}

clsCamera::clsCamera(
  PROJECTION_TYPE projType,
  float     horizFov,
  float     vertFov
) : clsHull(CLS_CAMERA) {
  projectionType  = projType;
  hFOV      = horizFov;
  vFOV      = vertFov;
  nearClip    = 1.0f;
  farClip     = 1000.0F;
  fDist     = (farClip - nearClip) / 2;

  setProjectionType(projType);
  setHFov(horizFov);
  setVFov(vertFov);
}

PROJECTION_TYPE clsCamera::getProjectionType()  { return projectionType; }
float     clsCamera::getHFov()      { return hFOV; }
float     clsCamera::getVFov()      { return vFOV; }
float     clsCamera::getFDist()     { return fDist; }
float     clsCamera::getNearCP()      { return nearClip; }
float     clsCamera::getFarCP()     { return farClip; }

void clsCamera::Zoom(float aspect)
{
  float rel = vFOV / hFOV;
  setHFov(hFOV + aspect);
  vFOV = hFOV * rel;
}

void clsCamera::setProjectionType(PROJECTION_TYPE projType) 
{ 
  projectionType = projType; 
}

void clsCamera::setHFov(float fieldOfViewHoriz) 
{ 
  if ( 
    fieldOfViewHoriz > EPSILON
    && (FLOAT)M_PI - fieldOfViewHoriz > EPSILON
  ) hFOV = fieldOfViewHoriz;
}

void clsCamera::setVFov(float fieldOfViewVert) 
{ 
  if ( 
    fieldOfViewVert > EPSILON
    && (FLOAT)M_PI - fieldOfViewVert > EPSILON
  ) vFOV = fieldOfViewVert;
}

void clsCamera::setFDist(float dist)
{
  if ( 
    dist - nearClip > EPSILON
    && farClip - dist > EPSILON
  ) fDist = dist;
}

void clsCamera::setNearCP(float nearCP) 
{ 
  if ( 
    nearCP >= EPSILON 
    && farClip - nearClip > EPSILON 
  ) nearClip = nearCP; 
}

void clsCamera::setFarCP(float farCP) 
{ 
  if ( farCP - nearClip > EPSILON ) farClip = farCP; 
}

void clsCamera::GetViewMatrix(MATRIX3D &mOut)
{
  mOut._11 = rWd.x;
  mOut._12 = uWd.x;
  mOut._13 = fWd.x;
  mOut._14 = .0f;

  mOut._21 = rWd.y;
  mOut._22 = uWd.y;
  mOut._23 = fWd.y;
  mOut._24 = .0f;

  mOut._31 = rWd.z;
  mOut._32 = uWd.z;
  mOut._33 = fWd.z;
  mOut._34 = .0f;

  mOut._41 = -Vector3DMultS(rWd, pos);
  mOut._42 = -Vector3DMultS(uWd, pos);
  mOut._43 = -Vector3DMultS(fWd, pos);
  mOut._44 = 1.0f;
}

void clsCamera::GetProjectionMatrix(MATRIX3D &mOut)
{
  mOut.SetIdentity();

  mOut._11 = 1 / tan(hFOV / 2.0f);
  mOut._22 = 1 / tan(vFOV / 2.0f);
  mOut._33 = farClip / (farClip - nearClip);
  mOut._43 = mOut._33 * (-nearClip);
  mOut._34 = fDist;
  if ( projectionType == PT_CENTRAL ) 
  {
    mOut._11 *= mOut._34;
    mOut._22 *= mOut._34;
    mOut._44 = -mOut._44;
  }
}

// ============================================================================
// Implementation of clsTargCamera class:
// ============================================================================
clsTargCamera::clsTargCamera(float lX, float lY, float lZ) : clsCamera() 
{
  TargetFollow(fDist);
  TargetTranslate(lX, lY, lZ);
}
clsTargCamera::clsTargCamera(PROJECTION_TYPE projType, 
               float       horizFov,
               float       vertFov,
               float lX, float lY, float lZ
    ) : clsCamera(projType, horizFov, vertFov) {
  TargetFollow(fDist);
  TargetTranslate(lX, lY, lZ);
}

VECTOR3D clsTargCamera::getTargetPosition() { return target.getPosition(); }
float clsTargCamera::getTargetDistance() { return Distance(target); }


// Camera transformations
void clsTargCamera::Translate(const VECTOR3D &tV) 
{ 
  pos   = tV; 
  clsCamera::LookAt(target.getPosition(), NULL);
}

void clsTargCamera::Translate(float tX, float tY, float tZ) 
{ 
  pos = VECTOR3D(tX, tY, tZ); 
  Translate(pos);
}

void clsTargCamera::Follow(float units) 
{
  VECTOR3D point = fWd * units;
  pos += point; 
  target.Translate(target.getPosition() + point); 
  clsCamera::LookAt(target.getPosition(), NULL); 
}

void clsTargCamera::Strafe(float units) 
{ 
  VECTOR3D point = rWd * units;
  pos += point; 
  target.Translate(target.getPosition() + point); 
  clsCamera::LookAt(target.getPosition(), NULL); 
}

void clsTargCamera::Fly(float units)  
{ 
  VECTOR3D point = uWd * units;
  pos += point; 
  target.Translate(target.getPosition() + point); 
  clsCamera::LookAt(target.getPosition(), NULL); 
}


void clsTargCamera::FollowLookAxis(float units) 
{ 
  pos = pos + fWd * units;  
  clsCamera::LookAt(target.getPosition(), NULL); ; 
}

void clsTargCamera::StrafeLongitude(float units) 
{
  MATRIX3D M(true);
  VECTOR3D t  = target.getPosition();
  float angle = Distance(target);
  angle = units / angle;
  Matrix3DRotateAxis(rWd, angle, M);
  pos -= t;
  Matrix3DTransformCoord(M, pos, pos);
  pos += t;
  clsCamera::LookAt(target.getPosition(), NULL); 
}

void clsTargCamera::StrafeLatitude(float units) 
{ 
  MATRIX3D M(true);
  VECTOR3D t  = target.getPosition();
  float angle = Distance(target);
  angle = units / angle;
  Matrix3DRotateAxis(uWd, angle, M);
  pos -= t;
  Matrix3DTransformCoord(M, pos, pos);
  pos += t;
  clsCamera::LookAt(target.getPosition(), NULL); 
}

// Target transformations
void clsTargCamera::TargetTranslate(const VECTOR3D &point) 
{ 
  target.Translate(point);
  clsCamera::LookAt(target.getPosition(), NULL); 
}
void clsTargCamera::TargetTranslate(float tX, float tY, float tZ) 
{ 
  TargetTranslate(VECTOR3D(tX, tY, tZ));
}

void clsTargCamera::TargetFollow(float units) 
{ 
  VECTOR3D point  = target.getPosition() + fWd * units;

  target.Translate(point); 
  clsCamera::LookAt(target.getPosition(), NULL); 
}

void clsTargCamera::TargetStrafe(float units) 
{ 
  VECTOR3D point  = target.getPosition() + rWd * units;

  target.Translate(point); 
  clsCamera::LookAt(target.getPosition(), NULL);  
}

void clsTargCamera::TargetFly(float units)    
{ 
  VECTOR3D point  = target.getPosition() + uWd * units;

  target.Translate(point); 
  clsCamera::LookAt(target.getPosition(), NULL); 
}

// Deprecated methods
void clsTargCamera::Pitch(float angle)            { /* doing nothing */ }
void clsTargCamera::Yaw(float angle)            { /* doing nothing */ }
void clsTargCamera::LookAt(VECTOR3D lookAt)         { /* doing nothing */ }
void clsTargCamera::LookAt(const clsObject &objToLookAt)  { /* doing nothing */ }
void clsTargCamera::LookAt(float lX, float lY, float lZ)  { /* doing nothing */ }
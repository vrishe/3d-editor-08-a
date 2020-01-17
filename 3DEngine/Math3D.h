#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <math.h>

// ============================================================================
// Base mathematics for transforming in 3D

#ifndef EPSILON
#define EPSILON .0001f
#endif // EPSILON

// ============================================================================
// Представление вектора:
typedef struct tagVector
{
  float x;
  float y;
  float z;
} VECTOR, *LPVECTOR;

typedef struct tagVector3D : public tagVector {
public:
  tagVector3D();
  tagVector3D(float fX, float fY, float fZ);

  tagVector3D   operator+ (const tagVector3D& u) const;
  tagVector3D   operator- (const tagVector3D& u) const;
  tagVector3D   operator* (float k) const;
  tagVector3D   operator/ (float k) const;
  
  tagVector3D&  operator+= (const tagVector3D& u);
  tagVector3D&  operator-= (const tagVector3D& u);
  tagVector3D&  operator*= (float k);
  tagVector3D&  operator/= (float k);

  tagVector3D   operator+ () const;
  tagVector3D   operator- () const;

  bool      operator== (const tagVector3D& u) const;
  bool      operator!= (const tagVector3D& u) const;
} VECTOR3D, *LPVECTOR3D;

float Vector3DLength(const VECTOR3D &src);
void Vector3DNormalize(const VECTOR3D &in, VECTOR3D &out);
void Vector3DMultV(
        const VECTOR3D &in1, 
        const VECTOR3D &in2, 
        VECTOR3D &out
      );
float Vector3DMultS(
        const VECTOR3D &in1, 
        const VECTOR3D &in2
      );

#define _sign(x) ((x >= 0) - (x < 0))

// ============================================================================
// Представление матрицы:
typedef struct tagMatrix {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} MATRIX4, *LPMATRIX4;

typedef struct tagMatrix3D : public tagMatrix {
public:
  tagMatrix3D(bool bSetIdentity = false);
  tagMatrix3D(int filler);
  tagMatrix3D(
            float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44
    );

  void Fill(int filler);
  void SetIdentity();

} MATRIX3D, *LPMATRIX3D;

void Matrix3DRotateAxis(
      const VECTOR3D &axis, 
      float angle, 
      MATRIX3D &rslt
    );
void Matrix3DTransformCoord(
      const MATRIX3D &T, 
      const VECTOR3D &in, 
      VECTOR3D &out
    );
void Matrix3DTransformNormal(
      const MATRIX3D &T, 
      const VECTOR3D &in,
      VECTOR3D &out
    );
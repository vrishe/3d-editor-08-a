
#include "Math3D.h"

// ============================================================================
// Implementation of tagVector3D struct:
tagVector3D::tagVector3D() { }
tagVector3D::tagVector3D(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

tagVector3D tagVector3D::operator+ (const tagVector3D& u) const
{
	return tagVector3D(x+u.x, y+u.y, z+u.z);
}

tagVector3D tagVector3D::operator- (const tagVector3D& u) const
{
	return tagVector3D(x-u.x, y-u.y, z-u.z);
}

tagVector3D tagVector3D::operator* (float k) const
{
	return tagVector3D(x*k, y*k, z*k);
}

tagVector3D tagVector3D::operator/ (float k) const
{
	return tagVector3D(x/k, y/k, z/k);
}

tagVector3D& tagVector3D::operator+= (const tagVector3D& u)
{
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

tagVector3D& tagVector3D::operator-= (const tagVector3D& u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

tagVector3D& tagVector3D::operator*= (float k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

tagVector3D& tagVector3D::operator/= (float k)
{
	x /= k;
	y /= k;
	z /= k;
	return *this;
}

tagVector3D tagVector3D::operator+ () const
{
	return tagVector3D(
		x > 0 ? x : -x,
		y > 0 ? y : -y,
		z > 0 ? z : -z
	);
}

tagVector3D tagVector3D::operator- () const
{
	return tagVector3D(
		x < 0 ? x : -x,
		y < 0 ? y : -y,
		z < 0 ? z : -z
	);
}

bool tagVector3D::operator== (const tagVector3D& u) const
{
	return abs(x - u.x) < EPSILON
		&& abs(y - u.y) < EPSILON
		&& abs(z - u.z) < EPSILON;
}

bool tagVector3D::operator!= (const tagVector3D& u) const {	return !operator==(u); }

void Vector3DNormalize(const LPVECTOR3D src, LPVECTOR3D rslt)
{
	*rslt = (*src) / sqrt(
						src->x*src->x 
						+ src->y*src->y 
						+ src->z*src->z
					);
}

void Vector3DMultV(
	const LPVECTOR3D src1, 
	const LPVECTOR3D src2, 
	tagVector3D* rslt
) {
	rslt->x = src1->y * src2->z - src1->z * src2->y;
	rslt->y = src1->z * src2->x - src1->x * src2->z;
	rslt->z = src1->x * src2->y - src1->y * src2->x;
}

float Vector3DMultS(					
	const LPVECTOR3D src1, 
	const LPVECTOR3D src2
) {
	return src1->x * src2->x 
			+ src1->y * src2->y 
			+ src1->z * src2->z;
}

// ============================================================================
// Implementation of tagMatrix3D struct:
tagMatrix3D::tagMatrix3D(bool bSetIdentity) 
{ 
	bSetIdentity ? SetIdentity() : Fill(0); 
}

tagMatrix3D::tagMatrix3D(float filler) { Fill(filler); }
tagMatrix3D::tagMatrix3D(
	        float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44
) {	CopyMemory(m, &a11, sizeof(m)); }

void tagMatrix3D::Fill(int filler) { FillMemory(m, sizeof(m), filler); }
void tagMatrix3D::SetIdentity() 
{ 
	Fill(0);
	for (
		size_t  i	= 0,
				len = sizeof(m) / sizeof(float),
				inc = ((len & 1) | len>>2) + 1;
		i < len;
		i += inc
	) { *((float*)m + i) = 1.0; }
}

void Matrix3DRotateAxis(const LPVECTOR3D axis, float rads, LPMATRIX3D rslt)
{
	float	sinTheta	= sin(rads),
			cosTheta	= 1 - sinTheta * sinTheta,
			_1mCosTheta	= 1 - cosTheta;

	rslt->_11 = axis->x * axis->x * _1mCosTheta + cosTheta;
	rslt->_12 = axis->x * axis->y * _1mCosTheta + axis->z * sinTheta;
	rslt->_13 = axis->x * axis->z * _1mCosTheta + axis->y * sinTheta;
	rslt->_14 = .0f;

	rslt->_21 = rslt->_12;
	rslt->_22 = axis->y * axis->y * _1mCosTheta + cosTheta;
	rslt->_23 = axis->y * axis->z * _1mCosTheta + axis->x * sinTheta;
	rslt->_24 = .0f;

	rslt->_31 = rslt->_13;
	rslt->_32 = rslt->_23;
	rslt->_33 = axis->z * axis->z * _1mCosTheta + cosTheta;
	rslt->_34 = .0f;
	
	rslt->_41 = .0f;
	rslt->_42 = .0f;
	rslt->_43 = .0f;
	rslt->_44 =	1.0f; 
}

void Matrix3DTransformCoord(
			const LPMATRIX3D T, 
			const LPVECTOR3D in, 
			LPVECTOR3D out
) {
	out->x	= T->_11 * in->x 
			+ T->_21 * in->y 
			+ T->_31 * in->z 
			+ T->_41;

	out->y	= T->_12 * in->x 
			+ T->_22 * in->y 
			+ T->_32 * in->z 
			+ T->_42;

	out->z	= T->_13 * in->x 
			+ T->_23 * in->y 
			+ T->_33 * in->z 
			+ T->_43;
}

void Matrix3DTransformNormal(
			const LPMATRIX3D T, 
			const LPVECTOR3D in, 
			LPVECTOR3D out
) {
	out->x	= T->_11 * in->x 
			+ T->_21 * in->y 
			+ T->_31 * in->z;

	out->y	= T->_12 * in->x 
			+ T->_22 * in->y 
			+ T->_32 * in->z;

	out->z	= T->_13 * in->x 
			+ T->_23 * in->y 
			+ T->_33 * in->z;
}
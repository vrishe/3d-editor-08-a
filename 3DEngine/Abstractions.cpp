
#include "Abstractions.h"

// ============================================================================
// Implementation of tagEdge struct:
tagEdge::tagEdge() 
	: first(-1), second(-1) { }

tagEdge::tagEdge(size_t a, size_t b) 
	: first(a), second(b) { }

tagEdge& tagEdge::operator+= (const UINT& p) {
	first += p;
	second += p;
	return *this;
}

bool tagEdge::operator== (const tagEdge &b) const 
{
	return (first == b.first && second == b.second)
			|| (first == b.second && second == b.first);
}

bool tagEdge::operator!= (const tagEdge &b) const {	return !operator==(b); }

bool tagEdge::isContianingVertex(size_t vi) 
{ 
	return first	== vi 
		|| second	== vi; 
}

// Implementation of tagDirectPolygon struct:
VECTOR3D tagDirectPolygon::Normal(size_t startVert) {
	VECTOR3D v1, v2, ans;
	switch (startVert) {
	case 1:
		v1 = VECTOR3D(second.x - first.x,
					second.y - first.y,
					second.z - first.z);
		v2 = VECTOR3D(third.x - first.x,
					third.y - first.y,
					third.z - first.z);
		break;
	case 2:
		v1 = VECTOR3D(first.x - second.x,
					first.y - second.y,
					first.z - second.z);
		v2 = VECTOR3D(third.x - second.x,
					third.y - second.y,
					third.z - second.z);
		break;
	case 3:
		v1 = VECTOR3D(first.x - third.x,
					first.y - third.y,
					first.z - third.z);
		v2 = VECTOR3D(second.x - third.x,
					second.y - third.y,
					second.z - third.z);
	}
	Vector3DMultV(v1, v2, ans);
	return ans;
}

// Implementation of tagPolygon struct:
tagPolygon::tagPolygon() 
	: first(UINT_MAX), second(UINT_MAX), third(UINT_MAX) { }

tagPolygon::tagPolygon(size_t a, size_t b, size_t c) 
	: first(a), second(b), third(c) { }

VECTOR3D tagPolygon::Normal(const LPVERT_LIST vs, size_t startVert) {
	VECTOR3D v1, v2, ans;
	switch (startVert) {
	case 1:
		v1 = VECTOR3D(vs->at(second).x - vs->at(first).x,
					vs->at(second).y - vs->at(first).y,
					vs->at(second).z - vs->at(first).z);
		v2 = VECTOR3D(vs->at(third).x - vs->at(first).x,
					vs->at(third).y - vs->at(first).y,
					vs->at(third).z - vs->at(first).z);
		break;
	case 2:
		v1 = VECTOR3D(vs->at(first).x - vs->at(second).x,
					vs->at(first).y - vs->at(second).y,
					vs->at(first).z - vs->at(second).z);
		v2 = VECTOR3D(vs->at(third).x - vs->at(second).x,
					vs->at(third).y - vs->at(second).y,
					vs->at(third).z - vs->at(second).z);
		break;
	case 3:
		v1 = VECTOR3D(vs->at(first).x - vs->at(third).x,
					vs->at(first).y - vs->at(third).y,
					vs->at(first).z - vs->at(third).z);
		v2 = VECTOR3D(vs->at(second).x - vs->at(third).x,
					vs->at(second).y - vs->at(third).y,
					vs->at(second).z - vs->at(third).z);
	}
	Vector3DMultV(v1, v2, ans);
	return ans;
}

VECTOR3D tagPolygon::Normal(const LPVECTOR3D vs, size_t startVert) {
	VECTOR3D v1, v2, ans;
	switch (startVert) {
	case 1:
		v1 = VECTOR3D(vs[second].x - vs[first].x,
					vs[second].y - vs[first].y,
					vs[second].z - vs[first].z);
		v2 = VECTOR3D(vs[third].x - vs[first].x,
					vs[third].y - vs[first].y,
					vs[third].z - vs[first].z);
		break;
	case 2:
		v1 = VECTOR3D(vs[first].x - vs[second].x,
					vs[first].y - vs[second].y,
					vs[first].z - vs[second].z);
		v2 = VECTOR3D(vs[third].x - vs[second].x,
					vs[third].y - vs[second].y,
					vs[third].z - vs[second].z);
		break;
	case 3:
		v1 = VECTOR3D(vs[first].x - vs[third].x,
					vs[first].y - vs[third].y,
					vs[first].z - vs[third].z);
		v2 = VECTOR3D(vs[second].x - vs[third].x,
					vs[second].y - vs[third].y,
					vs[second].z - vs[third].z);
	}
	Vector3DMultV(v1, v2, ans);
	return ans;
}

tagPolygon& tagPolygon::operator+= (const UINT& p) {
	first += p;
	second += p;
	third += p;
	return *this;
}

bool tagPolygon::operator== (const tagPolygon &b) const
{
	return first	== b.first 
		&& second	== b.second 
		&& third	== b.third;
}

bool tagPolygon::operator!= (const tagPolygon &b) const { return !operator==(b); }

bool tagPolygon::isContainingVertex(size_t vi) 
{ 
	return first	== vi 
		|| second	== vi 
		|| third	== vi; 
}

bool tagPolygon::isContainingEdge(const EDGE3D &e)
{
	return isContainingVertex(e.first) 
		&& isContainingVertex(e.second);
}

// Implementation of tagColor struct:
tagColor::tagColor(unsigned char r, unsigned char g, unsigned char b) {
	Red = r;
	Green = g;
	Blue = b;
}

bool tagColor::operator==(const float &b) const {
	return ( Red == b && Green == b && Blue == b);
}

bool tagColor::operator!=(const float &b) const {
	return ( Red != b || Green != b || Blue != b);
}

// ===========================================================================
// Implementation of tagCOORD_SPACE3D
// ===========================================================================
tagCOORD_SPACE3D::tagCOORD_SPACE3D() 
{
	X = VECTOR3D(1.0f, .0f, .0f);
	Y = VECTOR3D(.0f, 1.0f, .0f);
	Z = VECTOR3D(.0f, .0f, 1.0f);
}

tagCOORD_SPACE3D::tagCOORD_SPACE3D(
							VECTOR3D aX,
							VECTOR3D aY,
							VECTOR3D aZ
) {
	X = aX;
	Y = aY;
	Z = aZ;
}

// ===========================================================================
// Implementation of clsObject class:
size_t clsObject::Counter = 1;

clsObject::clsObject(CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];
	Name[0]	= '\0';

	//UCS		= NULL;
	pos		= VECTOR3D(.0f, .0f, .0f);

	fWd		= VECTOR3D(1.0f, .0f, .0f);
	rWd		= VECTOR3D(.0f, 1.0f, .0f);
	uWd		= VECTOR3D(.0f, .0f, 1.0f);
	world	= VECTOR3D();

	worldScale	= VECTOR3D(1.0f, 1.0f, 1.0f);
	localScale	= worldScale;
}

clsObject::clsObject(
			const VECTOR3D &pt, 
			float p, 
			float y, 
			float r, 
			CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];
	Name[0]	= '\0';

	//UCS		= NULL;
	pos		= pt;

	fWd		= VECTOR3D(1.0f, .0f, .0f);
	rWd		= VECTOR3D(.0f, 1.0f, .0f);
	uWd		= VECTOR3D(.0f, .0f, 1.0f);
	world	= VECTOR3D();

	worldScale	= VECTOR3D(1.0f, 1.0f, 1.0f);
	localScale	= worldScale;

	Pitch(p);
	Yaw(y);
	Roll(r);
}

clsObject::clsObject(
		float pX, 
		float pY, 
		float pZ, 
		float p,
		float y,
		float r, 
		CLASS_ID clsID)
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];
	Name[0]	= '\0';

	//UCS		= NULL;
	pos		= VECTOR3D(pX, pY, pZ);

	fWd		= VECTOR3D(1.0f, .0f, .0f);
	rWd		= VECTOR3D(.0f, 1.0f, .0f);
	uWd		= VECTOR3D(.0f, .0f, 1.0f);
	world	= VECTOR3D();

	worldScale	= VECTOR3D(1.0f, 1.0f, 1.0f);
	localScale	= worldScale;

	Pitch(p);
	Yaw(y);
	Roll(r);
}

clsObject::~clsObject() 
{
	Counter--;
	if ( Name != NULL ) { delete[] Name; Name = NULL; }
}

CLASS_ID clsObject::clsID() { return ClassID; }
size_t clsObject::objID()	{ return ID; }

VECTOR3D clsObject::getPosition() { return pos; }

void clsObject::Follow(float units)	{ pos += fWd * units; }
void clsObject::Strafe(float units)	{ pos += rWd * units; }
void clsObject::Fly(float units)	{ pos += uWd * units; }
void clsObject::LocalTranslate(const VECTOR3D &tV)
{
	pos += fWd * tV.x;
	pos += rWd * tV.y;
	pos += uWd * tV.z;
}
void clsObject::LocalTranslate(float tX, float tY, float tZ)
{
	pos += fWd * tX;
	pos += rWd * tY;
	pos += uWd * tZ;
}
void clsObject::Translate(const VECTOR3D &tV) { pos = tV; }
void clsObject::Translate(float tX, float tY, float tZ) 
{ 
	pos = VECTOR3D(tX, tY, tZ); 
}

void clsObject::ScaleAlong(float factor)	{ localScale.x *= factor; }
void clsObject::ScaleHAcross(float factor)	{ localScale.y *= factor; }
void clsObject::ScaleVAcross(float factor)	{ localScale.z *= factor; }
void clsObject::LocalScale(float fX, float fY, float fZ)
{
	localScale.x *= fX;
	localScale.y *= fY;
	localScale.z *= fZ;
}
void clsObject::Scale(float fX, float fY, float fZ)
{
	worldScale.x *= fX;
	worldScale.y *= fY;
	worldScale.z *= fZ;
}


void clsObject::Pitch(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(rWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void clsObject::Yaw(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(uWd, angle, M);
	Matrix3DTransformNormal(M, fWd, fWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(uWd, fWd, rWd);
	Vector3DNormalize(rWd, rWd);
}

void clsObject::Roll(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(fWd, angle, M);
	Matrix3DTransformNormal(M, rWd, rWd);

	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void clsObject::LocalRotate(float pitch, float yaw, float roll)
{
	MATRIX3D M;

	Matrix3DRotateAxis(rWd, pitch, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);

	Matrix3DRotateAxis(uWd, yaw, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(uWd, fWd, rWd);
	Vector3DNormalize(rWd, rWd);

	Matrix3DRotateAxis(fWd, roll, M);
	Matrix3DTransformNormal(M, rWd, rWd);
	Vector3DNormalize(fWd, fWd);
	Vector3DMultV(fWd, rWd, uWd);
	Vector3DNormalize(uWd, uWd);
}

void clsObject::Rotate(float y, float z, float x)
{
	MATRIX3D M;

	world.x = x;
	world.y = y;
	world.z = z;

	Matrix3DRotateAxis(VECTOR3D(.0f, 1.0f, .0f), world.y, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Matrix3DRotateAxis(VECTOR3D(.0f, .0f, 1.0f), world.z, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Matrix3DRotateAxis(VECTOR3D(1.0f, .0f, .0f), world.x, M);
	Matrix3DTransformNormal(M, fWd, fWd);
	Matrix3DTransformNormal(M, rWd, rWd);
	Matrix3DTransformNormal(M, uWd, uWd);
	Matrix3DTransformCoord(M, pos, pos);

	Vector3DNormalize(fWd, fWd);
	Vector3DNormalize(rWd, rWd);
	Vector3DNormalize(uWd, uWd);

}

float clsObject::Distance(const VECTOR3D &vDst)
{
	VECTOR3D v = VECTOR3D(
					pos.x - vDst.x,
					pos.y - vDst.y,
					pos.z - vDst.z
				);
	return Vector3DLength(v);
}

float clsObject::Distance(const clsObject &objDst)
{
	return Distance(objDst.pos);
}

void clsObject::LookAt(const VECTOR3D &lookAt, const LPVECTOR3D upOrient)
{
	VECTOR3D lookDir	= lookAt - pos;
	VECTOR3D vUp		= abs(uWd.z) < EPSILON ? uWd : VECTOR3D(0, 0, 1) * uWd.z;

	if ( Vector3DLength(lookDir) > EPSILON )
	{
		if ( 
			upOrient != NULL
			&& Vector3DLength(*upOrient) > EPSILON 
		) Vector3DNormalize(*upOrient, vUp);

		Vector3DNormalize(lookDir, fWd);
		Vector3DMultV(vUp, fWd, rWd);
		Vector3DNormalize(rWd, rWd);
		if ( Vector3DLength(rWd) < EPSILON )
			rWd = VECTOR3D(0, 1, 0) 
				* (FLOAT)((uWd.z >= 0) - (uWd.z < 0));
		Vector3DMultV(fWd, rWd, uWd);
	}
}
void clsObject::LookAt(const clsObject &objToLookAt) 
{ 
	LookAt(objToLookAt.pos, (LPVECTOR3D)&objToLookAt.uWd); 
}

void clsObject::LookAt(float lX, float lY, float lZ) 
{ 
	LookAt(VECTOR3D(lX, lY, lZ), NULL); 
}

void clsObject::GetMoveMatrix(MATRIX3D &mOut) 
{
	mOut._41 = pos.x;
	mOut._42 = pos.y;
	mOut._43 = pos.z;
}

void clsObject::GetLocalScaleMatrix(MATRIX3D &mOut)
{
	mOut._11 = localScale.x;
	mOut._22 = localScale.y;
	mOut._33 = localScale.z;
}

void clsObject::GetScaleMatrix(MATRIX3D &mOut) 
{
	mOut._11 = worldScale.x;
	mOut._22 = worldScale.y;
	mOut._33 = worldScale.z;
}

void clsObject::GetRotationMatrix(MATRIX3D &mOut) 
{
	mOut._11 = rWd.x;
	mOut._21 = fWd.x;
	mOut._31 = uWd.x;
	mOut._41 = .0f;

	mOut._12 = rWd.y;
	mOut._22 = fWd.y;
	mOut._32 = uWd.y;
	mOut._42 = .0f;

	mOut._13 = rWd.z;
	mOut._23 = fWd.z;
	mOut._33 = uWd.z;
	mOut._43 = .0f;

	mOut._14 = 0.f;
	mOut._24 = 0.f;
	mOut._34 = 0.f;
	mOut._44 = 1.0f;
}

VECTOR3D clsObject::getForwardLookDirection() { return fWd; }
VECTOR3D clsObject::getRightLookDirection() { return rWd; }
VECTOR3D clsObject::getUpLookDirection() { return uWd; }
VECTOR3D clsObject::getRotation() { return world; }
VECTOR3D clsObject::getScale() { return worldScale; }

void clsObject::setForwardLookDirection(const VECTOR3D &v) { fWd = v; }
void clsObject::setRightLookDirection(const VECTOR3D &v) { rWd = v; }
void clsObject::setUpLookDirection(const VECTOR3D &v) { uWd = v; }
//void clsObject::inheritOrientation(
//					const VECTOR3D &forward,
//					const VECTOR3D &rightward,
//					const VECTOR3D &upward
//) {
//	fWd = forward;
//	rWd = rightward;
//	uWd = upward;
//}
//void clsObject::inheritOrientation(const clsObject &obj)
//{
//	fWd = obj.fWd;
//	rWd = obj.rWd;
//	uWd = obj.uWd;
//}


void clsObject::getName(LPTSTR objName, size_t bufSize) 
{ 
	UINT bSize = bufSize >= MAX_OBJECT_NAME_LEN 
		? MAX_OBJECT_NAME_LEN - 1 : bufSize;

	_tcsncpy_s(objName, bufSize, Name, bSize);
	objName[bSize] = '\0';
}
void clsObject::setName(LPTSTR objName, size_t srcSize)
{
	UINT bSize = srcSize >= MAX_OBJECT_NAME_LEN 
		? MAX_OBJECT_NAME_LEN - 1 : srcSize;

	_tcsncpy_s(Name, MAX_OBJECT_NAME_LEN, objName, bSize);
	Name[bSize] = '\0';
}

// ============================================================================
// Inplementation of clsScene class:
bool clsScene::findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex)
{
	CONTENT::iterator finder = objects.find(lpObject->clsID());
	bool bResult 
		= lpObject != NULL
		&& finder != objects.end();
	size_t objCount;
 	if ( bResult ) 
	{
		objCount	= finder->second.size();
		bResult		= false;
		for ( 
			size_t i = 0;
			i < objCount;
			i++ 
		) {
			if ( finder->second[i] == lpObject ) 
			{
				if (objIndex != NULL)  *objIndex = i;
				i		= objCount;
				bResult = true;
			}
		}
	}
	return bResult;
}

bool clsScene::findObjectIndex(size_t objID, CLASS_ID *objClsID, size_t *objIndex)
{
	CONTENT::iterator finder = objects.begin();
	size_t	i,
			listCount;

	while ( finder != objects.end() )
	{
		listCount = finder->second.size();
		i = 0;
		while ( i < listCount ) 
		{
			if ( finder->second[i]->objID() == objID ) 
			{
				if ( objClsID != NULL ) *objClsID	= finder->first;
				if ( objIndex != NULL )	*objIndex	= i;
				return true;
			}
			i++;
		}
		finder++;
	}
	return false;
}

clsScene::clsScene() 
{ 
	CONTENT_CLASS clsObjLst;

	clsObjLst.first = CLS_CAMERA;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_MESH;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_LIGHT;
	objects.insert(clsObjLst);

	ambientColor = 0;
}
clsScene::~clsScene() { }

bool clsScene::AddObject(const LPOBJECT3D lpObject)
{
	CONTENT::iterator finder = objects.find(lpObject->clsID());
	bool bResult 
		= lpObject != NULL
		&& finder != objects.end();
 	if ( bResult ) finder->second.push_back(lpObject);
	return bResult;
}

bool clsScene::DeleteObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	bool bResult 
		= finder != objects.end()
		&& objIndex < finder->second.size();
	if ( bResult ) 
		finder->second.erase(finder->second.begin() + objIndex);
	return bResult;
}

bool clsScene::DeleteObject(size_t objID)
{
	CONTENT::iterator finder;
	CLASS_ID objClsID;
	size_t objIndex;
	bool bResult 
		= findObjectIndex(objID, &objClsID, &objIndex);
	if ( bResult ) 
	{
		finder = objects.find(objClsID);
		bResult = finder != objects.end();
		if ( bResult ) 
			finder->second.erase(finder->second.begin() + objIndex);
	}
	return bResult;
}

bool clsScene::DeleteObject(const LPOBJECT3D lpObject)
{
	size_t objIndex;
	return findObjectIndex(lpObject, &objIndex)
			&& DeleteObject(lpObject->clsID(), objIndex);	
}

void clsScene::Clear() {
	objects.clear();
	
	CONTENT_CLASS clsObjLst;

	clsObjLst.first = CLS_CAMERA;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_MESH;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_LIGHT;
	objects.insert(clsObjLst);

	ambientColor = 0;
}

bool clsScene::getFirstObject(CLASS_ID *objID)
{
	CONTENT::iterator i = objects.begin();
	while ( i != objects.end() )
	{
		if ( i->second.size() > 0 ) 
		{
			if (objID != NULL) *objID = i->first;
			return true;
		}
		i++;
	}
	return false;
}

LPOBJECT3D clsScene::getFirstObject()
{
	CLASS_ID idFound;
	if ( getFirstObject(&idFound) )
		return objects.at(idFound).at(0);
	return NULL;
}

LPOBJECT3D clsScene::getObject(CLASS_ID objID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(objID);
	LPOBJECT3D found = NULL;
	if ( finder != objects.end() && objIndex < finder->second.size() ) 
		found = finder->second[objIndex];
	return found;
}

LPOBJECT3D clsScene::getObject(size_t objID)
{
	CLASS_ID clsID; 
	size_t objIndex;
	LPOBJECT3D found = findObjectIndex(objID, &clsID, &objIndex) ?
		objects[clsID][objIndex] : NULL;
	return found;
}

size_t clsScene::getPolygonsCount() {
	UINT count = 0, N = objects[CLS_MESH].size();
	for (UINT i = 0; i < N; i++) {
		LPMESH3D temp = (LPMESH3D)objects[CLS_MESH][i];
		count += temp->getPolygonsCount();
	}
	return count;
}

size_t clsScene::getVerticesCount() {
	UINT count = 0, N = objects[CLS_MESH].size();
	for (UINT i = 0; i < N; i++){
		LPMESH3D temp = (LPMESH3D)objects[CLS_MESH][i];
		count += temp->getVerticesCount();
	}
	return count;
}

COLORREF clsScene::getAmbientColor() { return ambientColor; }

VOID clsScene::setAmbientColor(COLORREF color) { ambientColor = color; }

size_t clsScene::getObjectClassCount(CLASS_ID clsID)
{
	return objects[clsID].size();
}

// ============================================================================
// Implementation of clsHull class:
// ============================================================================
clsHull::clsHull(CLASS_ID clsID) : clsObject(clsID) { color = RGB(0, 0, 0); }

clsHull::clsHull(COLORREF c, CLASS_ID clsID) : clsObject(clsID), color(c) { }

clsHull::clsHull(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue,
		CLASS_ID clsID
) : clsObject(clsID), color(RGB(red, green, blue)) { }

void clsHull::flushVertices()
{
	cache.clear();
	cache.insert(cache.begin(), vertices.begin(), vertices.end());
	cache.shrink_to_fit();
}

size_t clsHull::findVertex(const VECTOR3D &v) 
{
	size_t vCount = vertices.size();

	for (size_t i = 0; i < vCount; i++)
		if (vertices[i] == v)
			return i;
	return vCount;
}

size_t clsHull::findEdge(const EDGE3D &e) 
{
	size_t eCount = edges.size();

	for (size_t i = 0; i < eCount; i++)
		if (edges[i] == e)
			return i;
	return eCount;
}

size_t clsHull::findPolygon(const POLY3D &p) 
{
	size_t pCount = polygons.size();

	for (size_t i = 0; i < pCount; i++)
		if (polygons[i] == p)
			return i;
	return pCount;
}

COLORREF	clsHull::getColor()			{ return color; }
size_t		clsHull::getVerticesCount()	{ return vertices.size(); }
size_t		clsHull::getEdgesCount()	{ return edges.size(); }
size_t		clsHull::getPolygonsCount()	{ return polygons.size(); }
LPVECTOR3D	clsHull::getVerticesRaw()	{ return vertices.data(); }
LPEDGE3D	clsHull::getEdgesRaw()		{ return edges.data(); }
LPPOLY3D	clsHull::getPolygonsRaw()	{ return polygons.data(); }
VERT_LIST	clsHull::getVertices()		{ return vertices; }
EDGE_LIST	clsHull::getEdges()			{ return edges; }
POLY_LIST	clsHull::getPolygons()		{ return polygons; }

POLY3D		clsHull::getPolygon(int i)	{ return polygons[i]; }

void clsHull::getBuffersRaw(LPVECTOR3D *vs, LPEDGE3D *es, LPPOLY3D *ps) 
{
	if ( vs != NULL ) *vs = vertices.data();
	if ( es != NULL ) *es = edges.data();
	if ( ps != NULL ) *ps = polygons.data();
}

void clsHull::getBuffers(LPVERT_LIST vs, LPEDGE_LIST es, LPPOLY_LIST ps) 
{
	if ( vs != NULL ) *vs = vertices;
	if ( es != NULL ) *es = edges;
	if ( ps != NULL ) *ps = polygons;
}

void clsHull::getVerticesTransformed(LPVECTOR3D v)
{
	CopyMemory(v, cache.data(), cache.size() * sizeof(VECTOR3D));
}

void clsHull::Transform()
{
	LPVECTOR3D	v;
	MATRIX3D	mTransScalePos(true),
				mLocalScale(true),
				mLocalRot(true);
	size_t vertCount = getVerticesCount();
	flushVertices();
	
	GetLocalScaleMatrix(mLocalScale);
	GetRotationMatrix(mLocalRot);
	GetMoveMatrix(mTransScalePos);
	GetScaleMatrix(mTransScalePos);
	
	v = cache.data();
	for ( size_t i = 0; i < vertCount; i++ )
	{
		Matrix3DTransformNormal(
				mLocalScale,
				*(v + i),
				*(v + i)
			);		
		Matrix3DTransformNormal(
				mLocalRot,
				*(v + i),
				*(v + i)
			);
		Matrix3DTransformCoord(
				mTransScalePos,
				*(v + i),
				*(v + i)
			);
	}
}

void clsHull::setColor(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
) {
	color = RGB(red, green, blue);
}

void clsHull::setColor(COLORREF c) { color = c; }

// ============================================================================
// Implementation of clsMesh class:
// ============================================================================
MESH_ID clsMesh::MeshID() { return meshID; }

clsMesh::clsMesh(MESH_ID mID) : clsHull(CLS_MESH), meshID(mID) { }

clsMesh::clsMesh(MESH_ID mID, COLORREF c) : clsHull(c, CLS_MESH), meshID(mID) { }

clsMesh::clsMesh(
		MESH_ID mID,
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
) : clsHull(red, green, blue, CLS_MESH), meshID(mID) { }

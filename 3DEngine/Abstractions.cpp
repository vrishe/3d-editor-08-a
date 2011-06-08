
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
	Vector3DMultV(&v1, &v2, &ans);
	return ans;
}

// Implementation of tagPolygon struct:
tagPolygon::tagPolygon() 
	: first(UINT_MAX), second(UINT_MAX), third(UINT_MAX) { }

tagPolygon::tagPolygon(size_t a, size_t b, size_t c) 
	: first(a), second(b), third(c) { }

VECTOR3D tagPolygon::Normal(LPVERT_LIST vs, size_t startVert) {
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
	Vector3DMultV(&v1, &v2, &ans);
	return ans;
}

VECTOR3D tagPolygon::Normal(LPVECTOR3D vs, size_t startVert) {
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
	Vector3DMultV(&v1, &v2, &ans);
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

bool tagPolygon::isContainingEdge(EDGE3D e)
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
			VECTOR3D pt, 
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
	if ( Name != NULL ) delete[] Name;
}

CLASS_ID clsObject::clsID() { return ClassID; }
size_t clsObject::objID()	{ return ID; }

VECTOR3D clsObject::getPosition() { return pos; }

void clsObject::Follow(float units)	{ pos += fWd * units; }
void clsObject::Strafe(float units)	{ pos += rWd * units; }
void clsObject::Fly(float units)	{ pos += uWd * units; }
void clsObject::LocalTranslate(VECTOR3D tV)
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
void clsObject::Translate(VECTOR3D tV) { pos = tV; }
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
	localScale.x *= fY;
	localScale.x *= fZ;
}
void clsObject::Scale(float fX, float fY, float fZ)
{
	worldScale.x *= fX;
	worldScale.x *= fY;
	worldScale.x *= fZ;
}


void clsObject::Pitch(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(&rWd, angle, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);

	Vector3DNormalize(&fWd, &fWd);
	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);
}

void clsObject::Yaw(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(&uWd, angle, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);

	Vector3DNormalize(&fWd, &fWd);
	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);
}

void clsObject::Roll(float angle) 
{
	MATRIX3D M;

	Matrix3DRotateAxis(&fWd, angle, &M);
	Matrix3DTransformNormal(&M, &rWd, &rWd);

	Vector3DNormalize(&fWd, &fWd);
	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);
}

void clsObject::LocalRotate(float pitch, float yaw, float roll)
{
	MATRIX3D M;

	Matrix3DRotateAxis(&rWd, pitch, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Vector3DNormalize(&fWd, &fWd);
	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);

	Matrix3DRotateAxis(&uWd, yaw, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Vector3DNormalize(&fWd, &fWd);
	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);

	Matrix3DRotateAxis(&fWd, roll, &M);
	Matrix3DTransformNormal(&M, &rWd, &rWd);
	Vector3DNormalize(&fWd, &fWd);
	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);
}

void clsObject::Rotate(float y, float z, float x)
{
	MATRIX3D M;

	world.x = x;
	world.y = y;
	world.z = z;

	Matrix3DRotateAxis(&VECTOR3D(.0f, 1.0f, .0f), world.y, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Matrix3DTransformNormal(&M, &rWd, &rWd);
	Matrix3DTransformNormal(&M, &uWd, &uWd);
	Matrix3DTransformCoord(&M, &pos, &pos);

	Matrix3DRotateAxis(&VECTOR3D(.0f, .0f, 1.0f), world.z, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Matrix3DTransformNormal(&M, &rWd, &rWd);
	Matrix3DTransformNormal(&M, &uWd, &uWd);
	Matrix3DTransformCoord(&M, &pos, &pos);

	Matrix3DRotateAxis(&VECTOR3D(1.0f, .0f, .0f), world.x, &M);
	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Matrix3DTransformNormal(&M, &rWd, &rWd);
	Matrix3DTransformNormal(&M, &uWd, &uWd);
	Matrix3DTransformCoord(&M, &pos, &pos);

	Vector3DNormalize(&fWd, &fWd);
	Vector3DNormalize(&rWd, &rWd);
	Vector3DNormalize(&uWd, &uWd);

}

//void clsObject::RotateAxis(const LPVECTOR3D axis, float angle) {
//	MATRIX3D M;
//
//	Matrix3DRotateAxis(axis, angle, &M);
//
//	Matrix3DTransformNormal(&M, &fWd, &fWd);
//	Matrix3DTransformNormal(&M, &rWd, &rWd);
//	Matrix3DTransformNormal(&M, &uWd, &uWd);
//	Matrix3DTransformCoord(&M, &pos, &pos);
//	Vector3DNormalize(&fWd, &fWd);
//	Vector3DNormalize(&rWd, &rWd);
//	Vector3DNormalize(&uWd, &uWd);
//}

void clsObject::LookAt(VECTOR3D lookAt, const LPVECTOR3D upOrient)
{
	VECTOR3D lookDir	= lookAt - pos;
	VECTOR3D vUp		= upOrient != NULL ? 
						*upOrient * uWd.z : VECTOR3D(0, 0, 1);

	if ( Vector3DLength(&lookDir) > EPSILON )
	{
		Vector3DNormalize(&lookDir, &fWd);
		Vector3DMultV(&vUp, &fWd, &rWd);
		Vector3DNormalize(&rWd, &rWd);
		if ( Vector3DLength(&rWd) < EPSILON )
			rWd = VECTOR3D(0, 1, 0) 
				* (FLOAT)((uWd.z >= 0) - (uWd.z < 0));
		Vector3DMultV(&fWd, &rWd, &uWd);
	}
}
void clsObject::LookAt(clsObject *objToLookAt) { LookAt(objToLookAt->pos, &objToLookAt->uWd); }
void clsObject::LookAt(float lX, float lY, float lZ) { LookAt(VECTOR3D(lX, lY, lZ), NULL); }

void clsObject::GetMoveMatrix(LPMATRIX3D mOut) 
{
	mOut->_41 = pos.x;
	mOut->_42 = pos.y;
	mOut->_43 = pos.z;
}

void clsObject::GetLocalScaleMatrix(LPMATRIX3D mOut)
{
	mOut->_11 = localScale.x;
	mOut->_22 = localScale.y;
	mOut->_33 = localScale.z;
}

void clsObject::GetScaleMatrix(LPMATRIX3D mOut) 
{
	mOut->_11 = worldScale.x;
	mOut->_22 = worldScale.y;
	mOut->_33 = worldScale.z;
}

void clsObject::GetRotationMatrix(LPMATRIX3D mOut) 
{
	mOut->_11 = rWd.x;
	mOut->_21 = fWd.x;
	mOut->_31 = uWd.x;
	mOut->_41 = .0f;

	mOut->_12 = rWd.y;
	mOut->_22 = fWd.y;
	mOut->_32 = uWd.y;
	mOut->_42 = .0f;

	mOut->_13 = rWd.z;
	mOut->_23 = fWd.z;
	mOut->_33 = uWd.z;
	mOut->_43 = .0f;

	mOut->_14 = 0.f;
	mOut->_24 = 0.f;
	mOut->_34 = 0.f;
	mOut->_44 = 1.0f;
}

VECTOR3D clsObject::getForwardLookDirection() { return fWd; }
VECTOR3D clsObject::getRightLookDirection() { return rWd; }
VECTOR3D clsObject::getUpLookDirection() { return uWd; }

void clsObject::setForwardLookDirection(LPVECTOR3D v) { fWd.x = v->x; fWd.y = v->y; fWd.z = v->z; }
void clsObject::setRightLookDirection(LPVECTOR3D v) { rWd.x = v->x; rWd.y = v->y; rWd.z = v->z; }
void clsObject::setUpLookDirection(LPVECTOR3D v) { uWd.x = v->x; uWd.y = v->y; uWd.z = v->z; }

void clsObject::getName(LPTSTR objName, size_t bufSize) 
{ 
	UINT bSize = bufSize > MAX_OBJECT_NAME_LEN 
		? MAX_OBJECT_NAME_LEN : bufSize;

	bSize--;
	_tcsncpy_s(objName, bufSize, Name, bSize);
	objName[bSize] = '\0';
}
void clsObject::setName(LPTSTR objName, size_t srcSize)
{
	UINT bSize = srcSize > MAX_OBJECT_NAME_LEN 
		? MAX_OBJECT_NAME_LEN : srcSize;

	bSize--;
	_tcsncpy_s(Name, MAX_OBJECT_NAME_LEN, objName, bSize);
	objName[bSize] = '\0';
}

// ============================================================================
// Inplementation of clsScene class:
bool clsScene::findObjectIndex(LPOBJECT3D lpObject, size_t *objIndex)
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

bool clsScene::AddObject(LPOBJECT3D lpObject)
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

bool clsScene::DeleteObject(LPOBJECT3D lpObject)
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

LPOBJECT3D clsScene::getObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
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
// Implementation of clsMesh class:
MESH_ID clsMesh::MeshID() { return meshID; }

size_t clsMesh::findVertex(VECTOR3D v) 
{
	size_t vCount = vertices.size();

	for (size_t i = 0; i < vCount; i++)
		if (vertices[i] == v)
			return i;
	return vCount;
}

size_t clsMesh::findEdge(EDGE3D e) 
{
	size_t eCount = edges.size();

	for (size_t i = 0; i < eCount; i++)
		if (edges[i] == e)
			return i;
	return eCount;
}

size_t clsMesh::findPolygon(POLY3D p) 
{
	size_t pCount = polygons.size();

	for (size_t i = 0; i < pCount; i++)
		if (polygons[i] == p)
			return i;
	return pCount;
}

// DO NOT DELETE THIS:
//size_t clsMesh::dropUnusedVertices() 
//{
//	size_t result = 0,
//				 vCount = vertices.size(),
//				 pCount = polygons.size();
//
//	for (size_t i  = 0; i < vCount; i++) {
//		bool found = false;
//		for (size_t j = 0; j < pCount; j++)
//			if (polygons[j].first == i || polygons[j].second == i || polygons[j].third) {
//				found = true;
//				break;
//			}
//		if (!found) {
//			delVertex(i);
//			result++;
//		}
//	}
//	return result;
//}
//
//size_t clsMesh::dropRedundantPolygons() 
//{
//	size_t result = 0,
//				 vCount = vertices.size(),
//				 pCount = polygons.size();
//
//	for (size_t i = 0; i < pCount; i++) {
//		bool firstFound = false, secondFound = false, thirdFound = false;
//		for (size_t j = 0; j < vCount; j++) {
//			if (polygons[i].first == j) {
//				firstFound = true;
//				continue;
//			}
//			if (polygons[i].second == j) {
//				secondFound = true;
//				continue;
//			}
//			if (polygons[i].third == j) {
//				secondFound = true;
//				continue;
//			}
//		}
//		if (!firstFound || !secondFound || !thirdFound) {
//			delPolygon(i);
//			result++;
//		}
//	}
//	return result;
//}

clsMesh::clsMesh(MESH_ID mID) : clsObject(CLS_MESH), meshID(mID) { setColor(COLOR3D()); }

clsMesh::clsMesh(MESH_ID mID, COLOR3D c) : clsObject(CLS_MESH), meshID(mID) { setColor(c); }

clsMesh::clsMesh(
		MESH_ID mID,
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
) : clsObject(CLS_MESH), meshID(mID) { setColor( red, green, blue ); }

clsMesh::clsMesh(MESH_ID mID, COLOR3D c, VERT_LIST vs, POLY_LIST ps)
	: clsObject(CLS_MESH), meshID(mID)
{ 
	//addListOfVertices(vs);
	//addListOfPolygons(ps);
	setColor(c);
}

clsMesh::clsMesh(
		MESH_ID mID,
		unsigned char red, 
		unsigned char green, 
		unsigned char blue,
		VERT_LIST vs, 
		POLY_LIST ps
) : clsObject(CLS_MESH), meshID(mID)
{ 
	//addListOfVertices(vs);
	//addListOfPolygons(ps);
	setColor( red, green, blue ); 
}

// DO NOT DELETE THIS:
//void clsMesh::dropRedundant() 
//{	
//	dropRedundantPolygons();	
//	dropUnusedVertices();
//}

COLOR3D		clsMesh::getColor()			{ return color; }

DWORD		clsMesh::getColorRef()		{ return RGB(
													color.Red, 
													color.Green, 
													color.Blue); }
size_t		clsMesh::getVerticesCount()	{ return vertices.size(); }
size_t		clsMesh::getEdgesCount()	{ return edges.size(); }
size_t		clsMesh::getPolygonsCount()	{ return polygons.size(); }
LPVECTOR3D	clsMesh::getVerticesRaw()	{ return vertices.data(); }
LPEDGE3D	clsMesh::getEdgesRaw()		{ return edges.data(); }
LPPOLY3D	clsMesh::getPolygonsRaw()	{ return polygons.data(); }
VERT_LIST	clsMesh::getVertices()		{ return vertices; }
EDGE_LIST	clsMesh::getEdges()			{ return edges; }
POLY_LIST	clsMesh::getPolygons()		{ return polygons; }

POLY3D		clsMesh::getPolygon(int i)	{ return polygons[i]; }

void clsMesh::getBuffersRaw(LPVECTOR3D *vs, LPEDGE3D *es, LPPOLY3D *ps) 
{
	if ( vs != NULL ) *vs = vertices.data();
	if ( es != NULL ) *es = edges.data();
	if ( ps != NULL ) *ps = polygons.data();
}

void clsMesh::getBuffers(LPVERT_LIST vs, LPEDGE_LIST es, LPPOLY_LIST ps) 
{
	if ( vs != NULL ) *vs = vertices;
	if ( es != NULL ) *es = edges;
	if ( ps != NULL ) *ps = polygons;
}

void clsMesh::getVerticesTransformed(LPVECTOR3D v)
{
	VECTOR3D	vertex;
	MATRIX3D	mTransScalePos(true),
				mLocalScale(true),
				mLocalRot(true);
	size_t vertCount = getVerticesCount();
	if ( v != NULL )
	{
		GetLocalScaleMatrix(&mLocalScale);
		GetRotationMatrix(&mLocalRot);
		GetMoveMatrix(&mTransScalePos);
		GetScaleMatrix(&mTransScalePos);
		
		for ( size_t i = 0; i < vertCount; i++ )
		{
			vertex = vertices[i];
			Matrix3DTransformNormal(
					&mLocalScale,
					&vertex,
					&vertex
				);		
			Matrix3DTransformNormal(
					&mLocalRot,
					&vertex,
					&vertex
				);
			Matrix3DTransformCoord(
					&mTransScalePos,
					&vertex,
					&vertex
				);
			*(v+i) = vertex;
		}
	}
}

void clsMesh::setColor(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
) {
	color.Red	= red;
	color.Green = green;
	color.Blue	= blue;
}

void clsMesh::setColor(COLOR3D c)	{ color = c; }

// DO NOT DELETE THIS:
//void clsMesh::addVertex(VECTOR3D v) { vertices.push_back(v); }
//
//void clsMesh::addListOfVertices(VERT_LIST v) 
//{ 
//	vertices.insert(vertices.end(), v.begin(), v.end()); 
//}
//
//bool clsMesh::delVertex(VECTOR3D v) 
//{
//	size_t pos = findVertex(v);
//	return delVertex(pos);
//}
//
//bool clsMesh::delVertex(size_t vi) 
//{ 
//	size_t limit;
//	POLY3D p;
//	EDGE3D e;
//
//	if (vi >= vertices.size()) return false;
//	vertices.erase(vertices.begin() + vi);
//
//	limit = polygons.size();
//	for (size_t i = 0; i < limit; i++)
//	{
//		p = polygons[i];
//		if ( p.isContainingVertex(vi) ) 
//				delPolygon(i);
//	}
//	return true;
//}
//
//size_t clsMesh::delListOfVertices(VERT_LIST v) {
//	size_t	result = 0,
//			vCount = v.size();
//
//	for (size_t i = 0; i < vCount; i++) 
//		if ( delVertex(v[i]) ) result++;
//
//	return result;
//}
//
//bool clsMesh::delEdge(EDGE3D e) 
//{
//	size_t pos = findEdge(e);
//	return delEdge(pos);
//}
//
//bool clsMesh::delEdge(size_t ei) 
//{ 
//	if (ei >= edges.size()) return false;
//	edges.erase(edges.begin() + ei);
//
//	return true;
//}
//
//void clsMesh::addPolygon(POLY3D p) { polygons.push_back(p); }
//
//void clsMesh::addListOfPolygons(POLY_LIST p) { 
//	polygons.insert(polygons.end(), p.begin(), p.end());
//}
//
//bool clsMesh::delPolygon(POLY3D p) {
//	size_t pos = findPolygon(p);
//	return delPolygon(pos);
//}
//
//bool clsMesh::delPolygon(size_t pi) {
//	POLY3D	pErased;
//	EDGE3D	eSupposed;
//	size_t	i,
//			coOwners;
//
//	if ( pi >= polygons.size() ) return false;
//
//	pErased = polygons[pi];
//	polygons.erase(polygons.begin() + pi);
//	for ( 
//		EDGE_LIST::iterator e	= edges.begin(),
//		eLast					= edges.end();
//		e != eLast;
//		e++
//	) {
//		if ( pErased.isContainingEdge(*e) ) 
//		{
//			coOwners = 0;
//			for (
//				POLY_LIST::iterator p	= polygons.begin(),
//				pLast					= polygons.end();
//				p != pLast;
//				p++
//			) { 
//				if ( p->isContainingEdge(*e) ) coOwners++;
//				}
//		}
//	}
//	return true;
//}
//
//size_t clsMesh::delListOfPolygons(POLY_LIST p) {
//	size_t	result = 0,
//			pCount = p.size();
//			
//	for (size_t i = 0; i < pCount; i++) 
//		if ( delPolygon(p[i]) ) result++;			
//
//	return result;
//}


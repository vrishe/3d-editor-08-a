
#include "Abstractions.h"

// ============================================================================
// Implementation of stuctures if necessary

// Implementation of tagVertex struct:
//tagVertex::tagVertex() 
//{ 
//	x = 0; 
//	y = 0; 
//	z = 0; 
//	rhW	= 1.0f; 
//}
//
//tagVertex::tagVertex(float fx, float fy, float fz, float rhw)
//{ 
//	x = fx; 
//	y = fy; 
//	z = fz; 
//	rhW	= rhw; 
//}
//
//bool tagVertex::operator== (const tagVertex &b) 
//{
//	return x == b.x 
//		&& y == b.y 
//		&& z == b.z;
//}

// Implementation of tagEdge struct:
tagEdge::tagEdge() 
	: first(-1), second(-1) { }

tagEdge::tagEdge(size_t a, size_t b) 
	: first(a), second(b) { }

bool tagEdge::operator== (const tagEdge &b) 
{
	return first == b.first 
			&& second == b.second;
}

bool tagEdge::operator!= (const tagEdge &b) {	return !operator==(b); }

// Implementation of tagPolygon struct:
tagPolygon::tagPolygon() 
	: first(UINT_MAX), second(UINT_MAX), third(UINT_MAX) { }

tagPolygon::tagPolygon(size_t a, size_t b, size_t c) 
	: first(a), second(b), third(c) { }

bool tagPolygon::operator== (const tagPolygon &b) {
	return first == b.first 
			&& second == b.second 
			&& third == b.third;
}

bool tagPolygon::operator!= (const tagPolygon &b) {	return !operator==(b); }

// ===========================================================================
// Implementation of clsObject class:
size_t clsObject::Counter = 1;

clsObject::clsObject(CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	VECTOR3D	p(.0f, .0f, .0f),
				fwd(1.0f, .0f, .0f),
				rwd(.0f, 1.0f, .0f),
				uwd(.0f, .0f, 1.0f);

	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];


	pitch	= .0f;
	roll	= .0f; 
	yaw		= .0f;

	pos		= p;
	fWd		= fwd;
	rWd		= rwd;
	uWd		= uwd;
}

clsObject::clsObject(
			VECTOR3D pt, 
			float p, 
			float y, 
			float r, 
			CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	VECTOR3D	fwd(1.0f, .0f, .0f),
				rwd(.0f, 1.0f, .0f),
				uwd(.0f, .0f, 1.0f);

	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];

	pos		= pt;

	pitch	= p;
	yaw		= y;
	roll	= r;

	fWd		= fwd;
	rWd		= rwd;
	uWd		= uwd;
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

	pos.x = pX;
	pos.y = pY;
	pos.z = pZ;

	pitch	= p;
	yaw		= y;
	roll	= r;
}

clsObject::~clsObject() 
{
	Counter--;
	if ( Name != NULL ) delete[] Name;
}

CLASS_ID clsObject::clsID() { return ClassID; }
size_t clsObject::objID()	{ return ID; }

VECTOR3D clsObject::getPosition() { return pos; }

void clsObject::Follow(float units) { pos += fWd * units; }
void clsObject::Strafe(float units) { pos += rWd * units; }
void clsObject::Fly(float units)	{ pos += uWd * units; }

void clsObject::Pitch(float angle) {
	pitch += angle;

	MATRIX3D M;
	
	Matrix3DRotateAxis(&rWd, angle, &M);

	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Vector3DMultV(&fWd, &rWd, &uWd);
}

void clsObject::Yaw(float angle) {
	yaw += angle;

	MATRIX3D M;

	Matrix3DRotateAxis(&uWd, angle, &M);

	Matrix3DTransformNormal(&M, &fWd, &fWd);
	Vector3DMultV(&uWd, &fWd, &rWd);
}

void clsObject::Roll(float angle) {
	roll += angle;

	MATRIX3D M;

	Matrix3DRotateAxis(&fWd, angle, &M);

	Matrix3DTransformNormal(&M, &rWd, &rWd);
	Vector3DMultV(&fWd, &rWd, &uWd);
}

void clsObject::GetMoveMatrix(LPMATRIX3D mOut) {
	Vector3DNormalize(&fWd, &fWd);

	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);

	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);

	//mOut->_11 = 1;
	//mOut->_12 = 0;
	//mOut->_13 = 0;
	//mOut->_14 = .0f;

	//mOut->_21 = 0;
	//mOut->_22 = 1;
	//mOut->_23 = 0;
	//mOut->_24 = .0f;

	//mOut->_31 = 0;
	//mOut->_32 = 0;
	//mOut->_33 = 1;
	//mOut->_34 = .0f;

	mOut->_41 = pos.x;
	mOut->_42 = pos.y;
	mOut->_43 = pos.z;
	//mOut->_44 = 1.0f;
}

void clsObject::GetXRotationMatrix(LPMATRIX3D mOut) {
	Vector3DNormalize(&fWd, &fWd);

	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);

	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);

	//mOut->_11 = 1;
	//mOut->_12 = 0;
	//mOut->_13 = 0;
	//mOut->_14 = .0f;

	//mOut->_21 = 0;
	mOut->_22 = cos(roll);
	mOut->_23 = sin(roll);
	//mOut->_24 = .0f;

	//mOut->_31 = 0;
	mOut->_32 = -sin(roll);
	mOut->_33 = cos(roll);
	//mOut->_34 = .0f;

	//mOut->_41 = pos.x;
	//mOut->_42 = pos.y;
	//mOut->_43 = pos.z;
	//mOut->_44 = 1.0f;
}

void clsObject::GetYRotationMatrix(LPMATRIX3D mOut) {
	Vector3DNormalize(&fWd, &fWd);

	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);

	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);

	mOut->_11 = cos(pitch);
	//mOut->_12 = 0;
	mOut->_13 = -sin(pitch);
	//mOut->_14 = .0f;

	//mOut->_21 = 0;
	//mOut->_22 = 1;
	//mOut->_23 = 0;
	//mOut->_24 = .0f;

	mOut->_31 = sin(pitch);
	//mOut->_32 = 0;
	mOut->_33 = cos(pitch);
	//mOut->_34 = .0f;

	//mOut->_41 = pos.x;
	//mOut->_42 = pos.y;
	//mOut->_43 = pos.z;
	//mOut->_44 = 1.0f;
}

void clsObject::GetZRotationMatrix(LPMATRIX3D mOut) {
	Vector3DNormalize(&fWd, &fWd);

	Vector3DMultV(&fWd, &rWd, &uWd);
	Vector3DNormalize(&uWd, &uWd);

	Vector3DMultV(&uWd, &fWd, &rWd);
	Vector3DNormalize(&rWd, &rWd);

	mOut->_11 = cos(yaw);
	mOut->_12 = sin(yaw);
	//mOut->_13 = 0;
	//mOut->_14 = .0f;

	mOut->_21 = -sin(yaw);
	mOut->_22 = cos(yaw);
	//mOut->_23 = 0;
	//mOut->_24 = .0f;

	//mOut->_31 = 0;
	//mOut->_32 = 0;
	//mOut->_33 = 1;
	//mOut->_34 = .0f;

	//mOut->_41 = pos.x;
	//mOut->_42 = pos.y;
	//mOut->_43 = pos.z;
	//mOut->_44 = 1.0f;
}

void clsObject::GetScaleMatrix(LPMATRIX3D mOut) {

}

//void clsObject::MoveTo(VECTOR3D pt) { pos = pt; }
//void clsObject::MoveTo(float pX, float pY, float pZ)
//{
//	pos.x = pX;
//	pos.y = pY;
//	pos.z = pZ;
//}
//void clsObject::MoveToX(float pX) { pos.x = pX; }
//void clsObject::MoveToY(float pY) { pos.y = pY; }
//void clsObject::MoveToZ(float pZ) { pos.z = pZ; }
//
//void clsObject::MoveAt(VECTOR3D pt) { pos += pt; }
//void clsObject::MoveAt(float pX, float pY, float pZ)
//{
//	pos.x += pX;
//	pos.y += pY;
//	pos.z += pZ;
//}
//void clsObject::MoveAtX(float pX) { pos.x += pX; }
//void clsObject::MoveAtY(float pY) { pos.y += pY; }
//void clsObject::MoveAtZ(float pZ) { pos.z += pZ; }
//
//void clsObject::RotateTo(float pitch, float roll, float yaw)
//{
//	Pitch	= pitch;
//	Roll	= roll;
//	Yaw		= yaw;
//}
//void clsObject::RotateToPitch(float pitch) { Pitch = pitch; }
//void clsObject::RotateToRoll(float roll) { Roll = roll; }
//void clsObject::RotateToYaw(float yaw) { Yaw = yaw; }
//
//void clsObject::RotateAt(float pitch, float roll, float yaw)
//{
//	Pitch	+= pitch;
//	Roll	+= roll;
//	Yaw		+= yaw;
//}
//void clsObject::RotateAtPitch(float pitch) { Pitch += pitch; }
//void clsObject::RotateAtRoll(float roll) { Roll += roll; }
//void clsObject::RotateAtYaw(float yaw) { Yaw += yaw; }

void clsObject::getName(LPTSTR objName, size_t bufSize) 
{ 
	wcscpy_s(objName, bufSize, Name); 
}

void clsObject::setName(LPTSTR objName, size_t srcSize)
{
	wcscpy_s(Name, MAX_OBJECT_NAME_LEN, objName);
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

	ambientColor.Red	= 0;
	ambientColor.Green	= 0;
	ambientColor.Blue	= 0;
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

LPOBJECT3D clsScene::getObject(CLASS_ID clsID, size_t objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	LPOBJECT3D found = NULL;
	if ( finder != objects.end() 
		&& objIndex < finder->second.size() ) 
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

COLOR3D clsScene::getAmbientColor() { return ambientColor; }

DWORD	clsScene::getAmbientColorRef()
{ 
	return RGB(
			ambientColor.Red, 
			ambientColor.Green,
			ambientColor.Blue
		);
}

VOID clsScene::setAmbientColor(COLOR3D c) { ambientColor = c; }

VOID clsScene::setAmbientColor( BYTE red, BYTE green, BYTE blue )
{
	ambientColor.Red	= red;
	ambientColor.Green	= green;
	ambientColor.Blue	= blue;
}

size_t clsScene::getObjectClassCount(CLASS_ID clsID)
{
	return objects[clsID].size();
}

// ============================================================================
// Implementation of clsMesh class:
size_t clsMesh::findVertex(VECTOR3D v) 
{
	size_t vCount = vertices.size();

	for (size_t i = 0; i < vCount; i++)
		if (vertices[i] == v)
			return i;
	return -1;
}

size_t clsMesh::findPolygon(POLY3D p) 
{
	size_t pCount = polygons.size();

	for (size_t i = 0; i < pCount; i++)
		if (polygons[i] == p)
			return i;
	return -1;
}

size_t clsMesh::dropUnusedVertices() 
{
	size_t result = 0,
				 vCount = vertices.size(),
				 pCount = polygons.size();

	for (size_t i  = 0; i < vCount; i++) {
		bool found = false;
		for (size_t j = 0; j < pCount; j++)
			if (polygons[j].first == i || polygons[j].second == i || polygons[j].third) {
				found = true;
				break;
			}
		if (!found) {
			delVertex(i);
			result++;
		}
	}
	return result;
}

size_t clsMesh::dropRedundantPolygons() 
{
	size_t result = 0,
				 vCount = vertices.size(),
				 pCount = polygons.size();

	for (size_t i = 0; i < pCount; i++) {
		bool firstFound = false, secondFound = false, thirdFound = false;
		for (size_t j = 0; j < vCount; j++) {
			if (polygons[i].first == j) {
				firstFound = true;
				continue;
			}
			if (polygons[i].second == j) {
				secondFound = true;
				continue;
			}
			if (polygons[i].third == j) {
				secondFound = true;
				continue;
			}
		}
		if (!firstFound || !secondFound || !thirdFound) {
			delPolygon(i);
			result++;
		}
	}
	return result;
}

clsMesh::clsMesh() : clsObject(CLS_MESH) { setColor(COLOR3D()); }

clsMesh::clsMesh(COLOR3D c) : clsObject(CLS_MESH) { setColor(c); }

clsMesh::clsMesh(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
) : clsObject(CLS_MESH) { setColor( red, green, blue ); }

clsMesh::clsMesh(COLOR3D c, VERT_LIST vs, POLY_LIST ps)
	: clsObject(CLS_MESH)
{ 
	addListOfVertices(vs);
	addListOfPolygons(ps);
	setColor(c);
}

clsMesh::clsMesh(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue,
		VERT_LIST vs, 
		POLY_LIST ps
) : clsObject(CLS_MESH) 
{ 
	addListOfVertices(vs);
	addListOfPolygons(ps);
	setColor( red, green, blue ); 
}

void clsMesh::dropRedundant() 
{	
	dropRedundantPolygons();	
	dropUnusedVertices();
}

COLOR3D		clsMesh::getColor()			{ return color; }

DWORD		clsMesh::getColorRef()		{ return RGB(
													color.Red, 
													color.Green, 
													color.Blue); }
size_t		clsMesh::getVCount()		{ return vertices.size(); }
size_t		clsMesh::getECount()		{ return edges.size(); }
size_t		clsMesh::getPCount()		{ return polygons.size(); }
LPVECTOR3D	clsMesh::getVerticesRaw()	{ return vertices.data(); }
LPEDGE3D	clsMesh::getEdgesRaw()		{ return edges.data(); }
LPPOLY3D	clsMesh::getPolygonsRaw()	{ return polygons.data(); }
VERT_LIST	clsMesh::getVertices()		{ return vertices; }
EDGE_LIST	clsMesh::getEdges()			{ return edges; }
POLY_LIST	clsMesh::getPolygons()		{ return polygons; }

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

void clsMesh::addVertex(VECTOR3D v) { vertices.push_back(v); }

void clsMesh::addListOfVertices(VERT_LIST v) 
{ 
	vertices.insert(vertices.end(), v.begin(), v.end()); 
}

bool clsMesh::delVertex(VECTOR3D v) 
{
	size_t pos = findVertex(v);

	if (pos == -1)
		return false;
	vertices.erase(vertices.begin() + pos);
	return true;
}

bool clsMesh::delVertex(size_t i) 
{ 
	size_t vCount = vertices.size();

	if ( i > -1 && i < vCount) {
		vertices.erase(vertices.begin() + i);
		return true;
	}
	return false; 
}

size_t clsMesh::delListOfVertices(VERT_LIST v) {
	size_t	result = 0,
					vCount = vertices.size(),
					j;

	const size_t N = v.size();
	for (size_t i = 0; i < N; i++) {
		for (j = 0; j < vCount; j++)
			if (vertices[j] == v[i]) {
				vertices.erase(vertices.begin() + j);
				vCount--;
				break;
			}
		if (j >= vCount)
			result--;
	}
	return result;
}

void clsMesh::addPolygon(POLY3D p) { polygons.push_back(p); }

void clsMesh::addListOfPolygons(vector <POLY3D> p) { 
	polygons.insert(polygons.end(), p.begin(), p.end());
}

bool clsMesh::delPolygon(POLY3D p) {
	size_t pos = findPolygon(p);
	if (pos == -1)
		return false;
	polygons.erase(polygons.begin() + pos);
	return true;
}

bool clsMesh::delPolygon(size_t i) { 
	size_t pCount = polygons.size();

	if ( i > -1 && i < pCount) {
		polygons.erase(polygons.begin() + i);
		return true;
	}
	return false; 
}

size_t clsMesh::delListOfPolygons(vector <POLY3D> p) {
	size_t	result = 0,
					pCount = polygons.size(),
					j;

	const size_t N = p.size();
	for (size_t i = 0; i < N; i++) {
		for (j = 0; j < pCount; j++)
			if (polygons[j] == p[i]) {
				polygons.erase(polygons.begin() + j);
				pCount--;
				break;
			}
		if (j >= pCount)
			result--;
	}
	return result;
}

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
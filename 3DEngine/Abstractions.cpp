
#include "Abstractions.h"

// ============================================================================
// Implementation of stuctures if necessary
// Implementation of tagVertex struct:
tagVertex::tagVertex() 
{ 
	x = 0; 
	y = 0; 
	z = 0; 
	rhW	= 1.0f; 
}
tagVertex::tagVertex(float fx, float fy, float fz, float rhw)
{ 
	x = fx; 
	y = fy; 
	z = fz; 
	rhW	= rhw; 
}
bool operator == (tagVertex a, tagVertex b) 
{
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

// Implementation of tagPolygon struct:
tagPolygon::tagPolygon() { first = -1; second = -1; third = -1; }
tagPolygon::tagPolygon(size_t a, size_t b, size_t c) { first = a; second = b; third = c; }
bool operator == (tagPolygon a, tagPolygon b) {
	if (a.first == b.first && a.second == b.second && a.third == b.third)
		return true;
	return false;
}



// ===========================================================================
// Implementation of clsObject class:
size_t clsObject::Counter = 1;

clsObject::clsObject(CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];

	Gizmo.x = 0;
	Gizmo.y = 0;
	Gizmo.z = 0;

	Pitch	= 0;
	Roll	= 0; 
	Yaw		= 0;
}

clsObject::clsObject(
			VECTOR3D pt, 
			float pitch, 
			float roll, 
			float yaw, 
			CLASS_ID clsID) 
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];

	Gizmo	= pt;

	Pitch	= pitch;
	Roll	= roll;
	Yaw		= yaw;
}

clsObject::clsObject(
		float pX, 
		float pY, 
		float pZ, 
		float pitch,
		float roll,
		float yaw, 
		CLASS_ID clsID)
	: ClassID(clsID), ID(Counter++), Name(NULL)
{
	Name	= new TCHAR[MAX_OBJECT_NAME_LEN];

	Gizmo.x = pX;
	Gizmo.y = pY;
	Gizmo.z = pZ;

	Pitch	= pitch;
	Roll	= roll;
	Yaw		= yaw;
}

clsObject::~clsObject() 
{
	Counter--;
	if ( Name != NULL ) delete[] Name;
}

CLASS_ID clsObject::clsID() { return ClassID; }
size_t clsObject::objID()	{ return ID; }

void clsObject::MoveTo(VECTOR3D pt) { Gizmo = pt; }
void clsObject::MoveTo(float pX, float pY, float pZ)
{
	Gizmo.x = pX;
	Gizmo.y = pY;
	Gizmo.z = pZ;
}
void clsObject::MoveToX(float pX) { Gizmo.x = pX; }
void clsObject::MoveToY(float pY) { Gizmo.y = pY; }
void clsObject::MoveToZ(float pZ) { Gizmo.z = pZ; }

void clsObject::MoveAt(VECTOR3D pt) 
{ 
	Gizmo.x += pt.x;
	Gizmo.y += pt.y;
	Gizmo.z += pt.z;
}
void clsObject::MoveAt(float pX, float pY, float pZ)
{
	Gizmo.x += pX;
	Gizmo.y += pY;
	Gizmo.z += pZ;
}
void clsObject::MoveAtX(float pX) { Gizmo.x += pX; }
void clsObject::MoveAtY(float pY) { Gizmo.y += pY; }
void clsObject::MoveAtZ(float pZ) { Gizmo.z += pZ; }

void clsObject::RotateTo(float pitch, float roll, float yaw)
{
	Pitch	= pitch;
	Roll	= roll;
	Yaw		= yaw;
}
void clsObject::RotateToPitch(float pitch) { Pitch = pitch; }
void clsObject::RotateToRoll(float roll) { Roll = roll; }
void clsObject::RotateToYaw(float yaw) { Yaw = yaw; }

void clsObject::RotateAt(float pitch, float roll, float yaw)
{
	Pitch	+= pitch;
	Roll	+= roll;
	Yaw		+= yaw;
}
void clsObject::RotateAtPitch(float pitch) { Pitch += pitch; }
void clsObject::RotateAtRoll(float roll) { Roll += roll; }
void clsObject::RotateAtYaw(float yaw) { Yaw += yaw; }

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
size_t clsMesh::findVertex(VERTEX3D v) 
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
size_t		clsMesh::getVCount()		{ return vertices.size(); }
size_t		clsMesh::getPCount()		{ return polygons.size(); }
LPVERTEX3D	clsMesh::getVerticesRaw()	{ return vertices.data(); }
LPPOLY3D	clsMesh::getPolygonsRaw()	{ return polygons.data(); }
VERT_LIST	clsMesh::getVertices()		{ return vertices; }
POLY_LIST	clsMesh::getPolygons()		{ return polygons; }

void clsMesh::getBuffersRaw(LPVERTEX3D *vs, LPPOLY3D *ps) 
{
	if ( vs != NULL ) *vs = vertices.data();
	if ( ps != NULL ) *ps = polygons.data();
}

void clsMesh::getBuffers(LPVERT_LIST vs, LPPOLY_LIST ps) 
{
	if ( vs != NULL ) *vs = vertices;
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
void clsMesh::addVertex(VERTEX3D v) { vertices.push_back(v); }
void clsMesh::addListOfVertices(VERT_LIST v) 
{ 
	vertices.insert(vertices.end(), v.begin(), v.end()); 
}
bool clsMesh::delVertex(VERTEX3D v) 
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

clsCamera::clsCamera() : clsObject(CLS_CAMERA) { }

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
tagPolygon::tagPolygon(unsigned int a, unsigned int b, unsigned int c) { first = a; second = b; third = c; }
bool operator == (tagPolygon a, tagPolygon b) {
	if (a.first == b.first && a.second == b.second && a.third == b.third)
		return true;
	return false;
}



// ===========================================================================
// Implementation of clsObject class:
clsObject::clsObject(CLASS_ID clsID) : objClassID(clsID)
{
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
: objClassID(clsID) 
{
	Gizmo	= pt;
	Pitch	= pitch;
	Roll	= roll;
	Yaw		= yaw;
}

clsObject::~clsObject() {}

CLASS_ID clsObject::clsID() { return objClassID; }

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


// ============================================================================
// Inplementation of clsScene class:
bool clsScene::findObject(LPOBJECT3D lpObject, unsigned int *objIndex)
{
	CONTENT::iterator finder = objects.find(lpObject->clsID());
	bool bResult 
		= lpObject != NULL
		&& finder != objects.end();
	unsigned int objCount;
 	if ( bResult ) 
	{
		objCount	= finder->second.size();
		bResult		= false;
		for ( 
			unsigned int i = 0;
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

LPOBJECT3D clsScene::getObject(CLASS_ID clsID, unsigned int objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	LPOBJECT3D found = NULL;
	if ( finder != objects.end() 
		&& objIndex < finder->second.size() ) 
		found = finder->second[objIndex];
	return found;
}

clsScene::clsScene() 
{ 
	CONTENT_CLASS clsObjLst;

	clsObjLst.first = CLS_CAMERA;
	objects.insert(clsObjLst);
	clsObjLst.first = CLS_MESH;
	objects.insert(clsObjLst);
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

bool clsScene::DeleteObject(CLASS_ID clsID, unsigned int objIndex)
{
	CONTENT::iterator finder = objects.find(clsID);
	bool bResult 
		= finder != objects.end()
		&& objIndex < finder->second.size();
	if ( bResult ) 
		finder->second.erase(finder->second.begin() + objIndex);
	return bResult;
}

bool clsScene::DeleteObject(LPOBJECT3D lpObject)
{
	unsigned int objIndex;
	return findObject(lpObject, &objIndex)
			&& DeleteObject(lpObject->clsID(), objIndex);	
}

// ============================================================================
// Implementation of clsMesh class:
unsigned int clsMesh::findVertex(VERTEX3D v) 
{
	unsigned int vCount = vertices.size();

	for (unsigned int i = 0; i < vCount; i++)
		if (vertices[i] == v)
			return i;
	return -1;
}
unsigned int clsMesh::findPolygon(POLY3D p) 
{
	unsigned int pCount = polygons.size();

	for (unsigned int i = 0; i < pCount; i++)
		if (polygons[i] == p)
			return i;
	return -1;
}

unsigned int clsMesh::dropUnusedVertices() 
{
	unsigned int result = 0,
				 vCount = vertices.size(),
				 pCount = polygons.size();

	for (unsigned int i  = 0; i < vCount; i++) {
		bool found = false;
		for (unsigned int j = 0; j < pCount; j++)
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
unsigned int clsMesh::dropRedundantPolygons() 
{
	unsigned int result = 0,
				 vCount = vertices.size(),
				 pCount = polygons.size();

	for (unsigned int i = 0; i < pCount; i++) {
		bool firstFound = false, secondFound = false, thirdFound = false;
		for (unsigned int j = 0; j < vCount; j++) {
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
clsMesh::clsMesh(COLOR3D c, VERT_LIST vs, POLY_LIST ps)
	: clsObject(CLS_MESH)
{ 
	addListOfVertices(vs);
	addListOfPolygons(ps);
	setColor(c);
}

void clsMesh::dropRedundant() 
{	
	dropRedundantPolygons();	
	dropUnusedVertices();
}

void clsMesh::FillBuff(LPVERTEX3D_PURE vs, LPPOLY3D ps) 
{
	unsigned int vCount = vertices.size();
	unsigned int pCount = polygons.size();

	for (unsigned int i = 0; i < vCount; i++) {
		vs[i].x = vertices[i].x;
		vs[i].y = vertices[i].y;
		vs[i].z = vertices[i].z;
		vs[i].rhW = vertices[i].rhW;
	}

	for (unsigned int i = 0; i < pCount; i++) {
		ps[i].first = polygons[i].first;
		ps[i].second = polygons[i].second;
		ps[i].third = polygons[i].third;
	}

	VERTEX3D_PURE a[1000];
	POLY3D b[1000];
	for (unsigned int i = 0; i < vCount; i++)
		a[i] = vs[i];
	for (unsigned int i = 0; i < pCount; i++)
		b[i] = ps[i];
	a[999];
}

unsigned int	clsMesh::getVCount() { return vertices.size(); }
unsigned int	clsMesh::getPCount() { return polygons.size(); }
VERT_LIST		clsMesh::getVertices() { return vertices; }
POLY_LIST		clsMesh::getPolygons() { return polygons; }

void clsMesh::setColor(COLOR3D c) {	color = c; }
void clsMesh::addVertex(VERTEX3D v) { vertices.push_back(v); }
void clsMesh::addListOfVertices(VERT_LIST v) 
{ 
	vertices.insert(vertices.end(), v.begin(), v.end()); 
}
bool clsMesh::delVertex(VERTEX3D v) 
{
	unsigned int pos = findVertex(v);

	if (pos == -1)
		return false;
	vertices.erase(vertices.begin() + pos);
	return true;
}
bool clsMesh::delVertex(unsigned int i) 
{ 
	unsigned int vCount = vertices.size();

	if ( i > -1 && i < vCount) {
		vertices.erase(vertices.begin() + i);
		return true;
	}
	return false; 
}
unsigned int clsMesh::delListOfVertices(VERT_LIST v) {
	unsigned int	result = 0,
					vCount = vertices.size(),
					j;

	const unsigned int N = v.size();
	for (unsigned int i = 0; i < N; i++) {
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
	unsigned int pos = findPolygon(p);
	if (pos == -1)
		return false;
	polygons.erase(polygons.begin() + pos);
	return true;
}
bool clsMesh::delPolygon(unsigned int i) { 
	unsigned int pCount = polygons.size();

	if ( i > -1 && i < pCount) {
		polygons.erase(polygons.begin() + i);
		return true;
	}
	return false; 
}
unsigned int clsMesh::delListOfPolygons(vector <POLY3D> p) {
	unsigned int	result = 0,
					pCount = polygons.size(),
					j;

	const unsigned int N = p.size();
	for (unsigned int i = 0; i < N; i++) {
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
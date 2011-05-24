#pragma once

#include <Windows.h>
#include <vector>
#include <map>
using namespace std;


// ============================================================================
// Supporting structures
typedef struct tagVECTOR3
{
	float x;
	float y;
	float z;
} VECTOR3D, *LPVECTOR3D;

typedef struct tagColor
{
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
} COLOR3D, *LPCOLOR3D;

typedef struct tagVertexPure : VECTOR3D {
	float		rhW;
} VERTEX3D_PURE, *LPVERTEX3D_PURE;

typedef struct tagVertex : public VERTEX3D_PURE {
	tagVertex();
	tagVertex(float x, float y, float z, float rhw = 1.0f);
	friend bool operator == (tagVertex, tagVertex);
} VERTEX3D, *LPVERTEX3D;

typedef struct tagPolygon {
	size_t first;
	size_t second;
	size_t third;

	tagPolygon();
	tagPolygon(size_t, size_t, size_t);
	friend bool operator == (tagPolygon, tagPolygon);
} POLY3D, *LPPOLY3D;

enum CLASS_ID {
	CLS_OBJECT		= 0,
	CLS_CAMERA		= 1,
	CLS_MESH		= 2
};

#define MAX_OBJECT_NAME_LEN	256
// ============================================================================
// Object class that represents current object
// space position
class clsObject {
private: 
	static size_t	Counter;
	CLASS_ID		ClassID;
	size_t			ID;
protected:
	LPTSTR			Name;
	VECTOR3D		Gizmo;
	float			Pitch,
					Roll,
					Yaw;

public:
	clsObject(CLASS_ID clsID = CLS_OBJECT);
	clsObject(
		VECTOR3D pt, 
		float pitch, 
		float roll, 
		float yaw, 
		CLASS_ID clsID = CLS_OBJECT
	);
	clsObject(
		float pX, 
		float pY, 
		float pZ, 
		float pitch,
		float roll,
		float yaw, 
		CLASS_ID clsID = CLS_OBJECT
	);
	~clsObject();
	CLASS_ID clsID();
	size_t	 objID();

	void MoveTo(VECTOR3D pt);
	void MoveTo(float pX, float pY, float pZ);
	void MoveToX(float pX);
	void MoveToY(float pY);
	void MoveToZ(float pZ);

	void MoveAt(VECTOR3D pt);
	void MoveAt(float pX, float pY, float pZ);
	void MoveAtX(float pX);
	void MoveAtY(float pY);
	void MoveAtZ(float pZ);

	void RotateTo(float pitch, float roll, float yaw);
	void RotateToPitch(float pitch);
	void RotateToRoll(float roll);
	void RotateToYaw(float yaw);

	void RotateAt(float pitch, float roll, float yaw);
	void RotateAtPitch(float pitch);
	void RotateAtRoll(float roll);
	void RotateAtYaw(float yaw);

	void getName(LPTSTR objName, size_t bufSize);
	void setName(LPTSTR objName, size_t srcSize);
};
typedef clsObject OBJECT3D, *LPOBJECT3D;

typedef vector<LPOBJECT3D> OBJECTS_LIST;
typedef map<CLASS_ID, OBJECTS_LIST> CONTENT;
typedef pair<CLASS_ID, OBJECTS_LIST> CONTENT_CLASS;

// ============================================================================
// World class which represents real world wnumerations and sets the WCS
class clsScene {
private:
	CONTENT objects;
	COLOR3D	ambientColor;

public:
	clsScene();
	~clsScene();

	bool AddObject(LPOBJECT3D lpObject);
	bool DeleteObject(CLASS_ID clsID, size_t objIndex); 
	bool DeleteObject(size_t objID);
	bool DeleteObject(LPOBJECT3D lpObject);

	bool findObjectIndex(LPOBJECT3D lpObject, size_t *objIndex = NULL);
	bool findObjectIndex(
				size_t objID, 
				CLASS_ID *objClsID	= NULL, 
				size_t *objIndex	= NULL
			);

	LPOBJECT3D	getObject(CLASS_ID clsID, size_t objIndex);
	LPOBJECT3D	getObject(size_t objID);
	COLOR3D		getAmbientColor();
	DWORD		getAmbientColorRef();

	VOID		setAmbientColor(COLOR3D c);
	VOID		setAmbientColor(BYTE red, BYTE green, BYTE blue);

	size_t getObjectClassCount(CLASS_ID clsID);
};
typedef clsScene SCENE3D, *LPSCENE3D;

// ============================================================================
// Abstract mesh class that represents EVERY POSSIBLE
// 3d mesh object. Provides basic mesh operations.
typedef vector<VERTEX3D> VERT_LIST, *LPVERT_LIST;
typedef vector<POLY3D> POLY_LIST, *LPPOLY_LIST;

class clsMesh : public clsObject {
protected:
	VERT_LIST		vertices;	// list of vertexes
	POLY_LIST		polygons;	// list of polygons

	COLOR3D			color;

	size_t findVertex(VERTEX3D);		// returns a vertex position
	size_t findPolygon(POLY3D);		// returns a Polygon_ position

	/// delete ver/edge/polys that can't be a part of the object no more
	size_t	dropUnusedVertices();		// delete vertexes if no polygons use them	
	size_t	dropRedundantPolygons();	// delete polygons if there are null vertexes in it

public:
	clsMesh();
	clsMesh(COLOR3D c);
	clsMesh(						
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
	);
	clsMesh(COLOR3D c, VERT_LIST vs, POLY_LIST ps);
	clsMesh(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue, 
		VERT_LIST vs, 
		POLY_LIST ps
	);

	// functionality
	void			dropRedundant();
	// add some modifying functionality here

	// getters
	COLOR3D			getColor();
	size_t			getVCount();
	size_t			getPCount();
	LPVERTEX3D		getVerticesRaw();
	LPPOLY3D		getPolygonsRaw();
	VERT_LIST		getVertices();
	POLY_LIST		getPolygons();

	void			getBuffersRaw(LPVERTEX3D *vs, LPPOLY3D *ps);
	void			getBuffers(LPVERT_LIST vs, LPPOLY_LIST ps);

	// setters
	void			setColor(
						unsigned char red, 
						unsigned char green, 
						unsigned char blue
					);
	void			setColor(COLOR3D c);
	void			addVertex(VERTEX3D);
	void			addListOfVertices(VERT_LIST);
	bool			delVertex(VERTEX3D);
	bool			delVertex(size_t);
	size_t	delListOfVertices(VERT_LIST);	// returns 0 if everything's deleted, 
													// -n otherwise, where n - number of 
													// undeleted elements

	void			addPolygon(POLY3D);
	void			addListOfPolygons(POLY_LIST);
	bool			delPolygon(POLY3D);
	bool			delPolygon(size_t);
	size_t	delListOfPolygons(POLY_LIST);
};
typedef clsMesh MESH3D, *LPMESH3D;

// ============================================================================
// Camera class
class clsCamera : public clsObject {
private:

public:
	clsCamera();
};
typedef clsCamera CAMERA3D, *LPCAMERA3D;

#pragma once

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
	unsigned short Red;
	unsigned short Green;
	unsigned short Blue;
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
	unsigned int first;
	unsigned int second;
	unsigned int third;

	tagPolygon();
	tagPolygon(unsigned int, unsigned int, unsigned int);
	friend bool operator == (tagPolygon, tagPolygon);
} POLY3D, *LPPOLY3D;

enum CLASS_ID {
	CLS_OBJECT		= 0,
	CLS_CAMERA		= 1,
	CLS_MESH		= 2
};

// ============================================================================
// Object class that represents current object
// space position
class clsObject {
private: 
	CLASS_ID	objClassID;
protected:
	VECTOR3D Gizmo;
	float	Pitch,
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
	clsObject(float pX, float pY, float pZ, float pitch, float roll, float yaw);
	~clsObject();
	CLASS_ID clsID();

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

	bool findObject(LPOBJECT3D lpObject, unsigned int *objIndex);
public:
	clsScene();
	~clsScene();

	bool AddObject(LPOBJECT3D lpObject);
	bool DeleteObject(CLASS_ID clsID, unsigned int objIndex); 
	bool DeleteObject(LPOBJECT3D lpObject);

	LPOBJECT3D getObject(CLASS_ID clID, unsigned int objIndex);
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

	unsigned int findVertex(VERTEX3D);		// returns a vertex position
	unsigned int findPolygon(POLY3D);		// returns a Polygon_ position

	/// delete ver/edge/polys that can't be a part of the object no more
	unsigned int	dropUnusedVertices();		// delete vertexes if no polygons use them	
	unsigned int	dropRedundantPolygons();	// delete polygons if there are null vertexes in it

public:
	clsMesh();
	clsMesh(COLOR3D c);
	clsMesh(COLOR3D c, VERT_LIST vs, POLY_LIST ps);

	// functionality
	void			dropRedundant();
	void			FillBuff(LPVERTEX3D_PURE vs, LPPOLY3D ps);
	// add some modifying functionality here

	// getters
	unsigned int	getVCount();
	unsigned int	getPCount();
	VERT_LIST		getVertices();
	POLY_LIST		getPolygons();

	// setters
	void			setColor(COLOR3D c);
	void			addVertex(VERTEX3D);
	void			addListOfVertices(VERT_LIST);
	bool			delVertex(VERTEX3D);
	bool			delVertex(unsigned int);
	unsigned int	delListOfVertices(VERT_LIST);	// returns 0 if everything's deleted, 
													// -n otherwise, where n - number of 
													// undeleted elements

	void			addPolygon(POLY3D);
	void			addListOfPolygons(POLY_LIST);
	bool			delPolygon(POLY3D);
	bool			delPolygon(unsigned int);
	unsigned int	delListOfPolygons(POLY_LIST);
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

#pragma once

#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <vector>
#include <map>

#include "Math3D.h"
using namespace std;


// ============================================================================
// Supporting structures

//typedef struct tagVertex : public tagVector3D {
//	
//} VERTEX3D, *LPVERTEX3D;

typedef struct tagEdge {
	size_t first;
	size_t second;

	tagEdge();
	tagEdge(size_t nFirst, size_t nSecond);

	tagEdge& operator+= (const UINT& p);
	bool operator==(const tagEdge &b) const;
	bool operator!=(const tagEdge &b) const;
	bool isContianingVertex(size_t vi);
} EDGE3D, *LPEDGE3D;

typedef vector<VECTOR3D> VERT_LIST, *LPVERT_LIST;

typedef struct tagDirectPolygon {
	VECTOR3D first;
	VECTOR3D second;
	VECTOR3D third;

	DWORD colorRef;

	VECTOR3D Normal(size_t startVert);
} DIRECTPOLY3D, *LPDIRECTPOLY3D;

typedef struct tagPolygon {
	size_t first;
	size_t second;
	size_t third;

	tagPolygon();
	tagPolygon(size_t a, size_t b, size_t c);
	VECTOR3D Normal(const LPVERT_LIST, size_t startVert);
	VECTOR3D Normal(const LPVECTOR3D, size_t startVert);

	tagPolygon& operator+= (const UINT& p);
	bool operator==(const tagPolygon &b) const;
	bool operator!=(const tagPolygon &b) const;
	bool isContainingEdge (const EDGE3D &e);
	bool isContainingVertex (size_t vi);
} POLY3D, *LPPOLY3D;

typedef struct tagColor
{
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
	tagColor(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);

	bool operator==(const float &b) const;
	bool operator!=(const float &b) const;
} COLOR3D, *LPCOLOR3D;

#define BLACK 0

// This is for simple colorref colo extraction 
#define RED(c)		(BYTE)(c)
#define GREEN(c)	(BYTE)(c>>8)
#define BLUE(c)		(BYTE)(c>>16)

// ============================================================================
// Object class that represents current object
// space position
#define MAX_OBJECT_NAME_LEN	256
#define OBJECTS_NUM			4

enum CLASS_ID {
	CLS_OBJECT		= 0,
	CLS_LIGHT		= 1,
	CLS_HULL		= 2,
	CLS_MESH		= 4,
	CLS_CAMERA		= 3
};

enum MESH_ID {
	MSH_PYRAMID		= 0,
	MSH_CONE		= 1,
	MSH_EXCONE		= 2,
	MSH_HOLE		= 3,
	MSH_MIC			= 4,
	MSH_TANK		= 5
};

enum CONSTRAINTS {
	C_FREE	= 0,
	C_XY	= 1,	
	C_ZX	= 2,
	C_ZY	= 3
};

typedef struct tagCOORD_SPACE3D {
	VECTOR3D X;
	VECTOR3D Y;
	VECTOR3D Z;

	tagCOORD_SPACE3D();
	tagCOORD_SPACE3D(
			VECTOR3D aX,
			VECTOR3D aY,
			VECTOR3D aZ
	);
} COORD_SPACE3D, *LPCOORD_SPACE3D;

class clsObject {
private: 	
	CLASS_ID		ClassID;
	size_t			ID;

	static size_t	Counter;
protected:

	LPTSTR			Name;

	// Position
	VECTOR3D		pos;
	// Orientation
	VECTOR3D		fWd;
	VECTOR3D		rWd;
	VECTOR3D		uWd;
	VECTOR3D		world;

	// Scaling
	VECTOR3D		worldScale;
	VECTOR3D		localScale;

	CONSTRAINTS		moveConst;

	void PitchYawRoll();

public:
	clsObject(CLASS_ID clsID = CLS_OBJECT);
	clsObject(
		const VECTOR3D	&pt, 
		float		p, 
		float		y, 
		float		r, 
		CLASS_ID	clsID = CLS_OBJECT
	);
	clsObject(
		float pX, 
		float pY, 
		float pZ, 
		float p,
		float y,
		float r, 
		CLASS_ID clsID = CLS_OBJECT
	);
	virtual ~clsObject();
	CLASS_ID clsID();
	size_t	 objID();

	// Positioning
	void Follow(float units);	// Along local-x
	void Strafe(float units);	// Along local-y
	void Fly(float units);		// Along local-z
	void LocalTranslate(const VECTOR3D &tV);
	void LocalTranslate(float tX, float tY, float tZ);
	void Translate(const VECTOR3D &tV);
	void Translate(float tX, float tY, float tZ);	 

	// Scaling
	void ScaleAlong(float factor);
	void ScaleHAcross(float factor);
	void ScaleVAcross(float factor);
	void LocalScale(float fx, float fy, float fz);
	void Scale(float fx, float fy, float fz);
	
	// Orientation
	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);
	void LocalRotate(float pitch, float yaw, float roll);
	void Rotate(float y, float z, float x);

	float Distance(const VECTOR3D &vDst);
	float Distance(const clsObject &objDst);

	// Take a look at point or object
	void LookAt(const VECTOR3D &lookAt, const LPVECTOR3D upOrient);
	void LookAt(const clsObject &objToLookAt);
	void LookAt(float lX, float lY, float lZ);

	void GetMoveMatrix(MATRIX3D &mOut);
	void GetLocalScaleMatrix(MATRIX3D &mOut);
	void GetScaleMatrix(MATRIX3D &mOut);
	void GetRotationMatrix(MATRIX3D &mOut);

	VECTOR3D getForwardLookDirection();
	VECTOR3D getRightLookDirection();
	VECTOR3D getUpLookDirection();	
	VECTOR3D getPosition();
	VECTOR3D getRotation();
	VECTOR3D getScale();

	void setForwardLookDirection(const VECTOR3D &);
	void setRightLookDirection(const VECTOR3D &);
	void setUpLookDirection(const VECTOR3D &);
	//void inheritOrientation(
	//			const VECTOR3D &forward,
	//			const VECTOR3D &rightward,
	//			const VECTOR3D &upward
	//		);
	//void inheritOrientation(const clsObject &obj);

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
	CONTENT		objects;
	COLORREF	ambientColor;

public:
	clsScene();
	~clsScene();

	bool AddObject(const LPOBJECT3D lpObject);
	bool DeleteObject(CLASS_ID clsID, size_t objIndex); 
	bool DeleteObject(size_t objID);
	bool DeleteObject(const LPOBJECT3D lpObject);
	void Clear();

	bool findObjectIndex(const LPOBJECT3D lpObject, size_t *objIndex = NULL);
	bool findObjectIndex(
				size_t objID, 
				CLASS_ID *objClsID	= NULL, 
				size_t *objIndex		= NULL
			);

	bool		getFirstObject(CLASS_ID *objID);
	LPOBJECT3D	getFirstObject();
	LPOBJECT3D	getObject(CLASS_ID objID, size_t objIndex);
	LPOBJECT3D	getObject(size_t objID);
	size_t		getPolygonsCount();
	size_t		getVerticesCount();
	COLORREF	getAmbientColor();

	VOID		setAmbientColor(COLORREF color);

	size_t getObjectClassCount(CLASS_ID clsID);
};
typedef clsScene SCENE3D, *LPSCENE3D;

// ============================================================================
// Class clsHull
// ============================================================================
typedef vector<EDGE3D> EDGE_LIST, *LPEDGE_LIST;
typedef vector<POLY3D> POLY_LIST, *LPPOLY_LIST;

class clsHull : public clsObject {
protected:
	VERT_LIST		vertices;	// list of vertexes
	VERT_LIST		cache;		// list of transformed vertices
	EDGE_LIST		edges;		// list of edges
	POLY_LIST		polygons;	// list of polygons

	COLORREF		color;

	void flushVertices();
	size_t findVertex(const VECTOR3D &v);	// returns a vertex position
	size_t findEdge(const EDGE3D &e);
	size_t findPolygon(const POLY3D &p);	// returns a Polygon_ position

public:
	clsHull(CLASS_ID clID = CLS_HULL);
	clsHull(COLORREF c, CLASS_ID clID = CLS_HULL);
	clsHull(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue,
		CLASS_ID clID = CLS_HULL
	);

	// getters
	COLORREF		getColor();
	size_t			getVerticesCount();
	size_t			getEdgesCount();
	size_t			getPolygonsCount();
	LPVECTOR3D		getVerticesRaw();
	LPEDGE3D		getEdgesRaw();
	LPPOLY3D		getPolygonsRaw();
	VERT_LIST		getVertices();
	EDGE_LIST		getEdges();
	POLY_LIST		getPolygons();

	POLY3D			getPolygon(int);

	void			getBuffersRaw(LPVECTOR3D *vs, LPEDGE3D *es, LPPOLY3D *ps);
	void			getBuffers(LPVERT_LIST vs, LPEDGE_LIST es, LPPOLY_LIST ps);
	void			getVerticesTransformed(LPVECTOR3D v);
	void			Transform();

	// setters
	void			setColor(COLORREF c);
	void			setColor(
						unsigned char red, 
						unsigned char green, 
						unsigned char blue
					);

	void			addVertices(const LPVECTOR3D vs, UINT vsCount);
	bool			delVertices();
	
	void			addEdges(const LPEDGE3D es, UINT esCount);
	bool			delEdges(EDGE3D);

	void			addPolygons(const LPPOLY3D ps, UINT psCount);
	bool			delPolygon(POLY3D);
};
typedef clsHull HULL3D, *LPHULL3D;

// ============================================================================
// Abstract mesh class that represents EVERY POSSIBLE
// 3d mesh object. Provides basic mesh operations.
class clsMesh : public clsHull {
protected:
	MESH_ID			meshID;

public:
	clsMesh(MESH_ID mID);
	clsMesh(MESH_ID mID, COLORREF c);
	clsMesh(
		MESH_ID mID,
		unsigned char red, 
		unsigned char green, 
		unsigned char blue
	);

	MESH_ID MeshID();
};
typedef clsMesh MESH3D, *LPMESH3D;


#pragma once

#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
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
	bool operator==(const tagEdge &b) const;
	bool operator!=(const tagEdge &b) const;
	bool isContianingVertex(size_t vi);
} EDGE3D, *LPEDGE3D;

typedef vector<VECTOR3D> VERT_LIST, *LPVERT_LIST;

typedef struct tagDirectPolygon {
	VECTOR3D first;
	VECTOR3D second;
	VECTOR3D third;

	VECTOR3D normal;

	void setNormal(size_t startVert);
} DIRECTPOLY3D, *LPDIRECTPOLY3D;

typedef struct tagPolygon {
	size_t first;
	size_t second;
	size_t third;
	
	VECTOR3D normal;

	tagPolygon();
	tagPolygon(size_t a, size_t b, size_t c);
	void setNormal(LPVERT_LIST, size_t startVert);
	bool operator==(const tagPolygon &b) const;
	bool operator!=(const tagPolygon &b) const;
	bool isContainingEdge (EDGE3D e);
	bool isContainingVertex (size_t vi);
} POLY3D, *LPPOLY3D;

typedef struct tagColor
{
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
} COLOR3D, *LPCOLOR3D;

// ============================================================================
// Object class that represents current object
// space position
#define MAX_OBJECT_NAME_LEN	256

enum CLASS_ID {
	CLS_OBJECT		= 0,
	CLS_CAMERA		= 1,
	CLS_MESH		= 2
};

enum CONSTRAINTS {
	C_FREE	= 0,
	C_XY	= 1,	
	C_ZX	= 2,
	C_ZY	= 3
};

class clsObject {
private: 
	static size_t	Counter;
	CLASS_ID		ClassID;
	size_t			ID;

protected:
	LPTSTR			Name;

	float			pitch,
					yaw,
					roll;

	float			xScale,
					yScale,
					zScale;

	VECTOR3D		pos,
					fWd,
					rWd,
					uWd;

	CONSTRAINTS		moveConst;

public:
	clsObject(CLASS_ID clsID = CLS_OBJECT);
	clsObject(
		VECTOR3D	pt, 
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
	~clsObject();
	CLASS_ID clsID();
	size_t	 objID();

	VECTOR3D getPosition();

	void Follow(float units);	// change Y coord
	void Strafe(float units);	// change X coord
	void Fly(float units);		// change Z coord

	void ScaleByX(float factor);
	void ScaleByY(float factor);
	void ScaleByZ(float factor);

	void Pitch(float angle);	// right (Y)
	void Yaw(float angle);		// up	 (Z)
	void Roll(float angle);		// front (X)

	void GetMoveMatrix(LPMATRIX3D mOut);
	void GetRollRotationMatrix(LPMATRIX3D mOut);
	void GetPitchRotationMatrix(LPMATRIX3D mOut);
	void GetYawRotationMatrix(LPMATRIX3D mOut);
	void GetScaleMatrix(LPMATRIX3D mOut);

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
	size_t		getPolygonsCount();
	size_t		getVerticesCount();
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
typedef vector<EDGE3D> EDGE_LIST, *LPEDGE_LIST;
typedef vector<POLY3D> POLY_LIST, *LPPOLY_LIST;

class clsMesh : public clsObject {
protected:
	VERT_LIST		vertices;	// list of vertexes
	EDGE_LIST		edges;		// list of edges
	POLY_LIST		polygons;	// list of polygons

	COLOR3D			color;

	size_t findVertex(VECTOR3D v);	// returns a vertex position
	size_t findEdge(EDGE3D e);
	size_t findPolygon(POLY3D p);	// returns a Polygon_ position

	//// delete ver/edge/polys that can't be a part of the object no more
	//size_t	dropUnusedVertices();		// delete vertexes if no polygons use them	
	//size_t	dropRedundantPolygons();	// delete polygons if there are null vertexes in it

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
	//void			dropRedundant();
	// add some modifying functionality here

	// getters
	COLOR3D			getColor();
	DWORD			getColorRef();
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

	// setters
	void			setColor(
						unsigned char red, 
						unsigned char green, 
						unsigned char blue
					);
	void			setColor(COLOR3D c);

	//void			addVertex(VECTOR3D);
	//void			addListOfVertices(VERT_LIST);
	//bool			delVertex(VECTOR3D);
	//bool			delVertex(size_t);
	//size_t	delListOfVertices(VERT_LIST);	// returns 0 if everything's deleted, 
	//												// -n otherwise, where n - number of 
	//												// undeleted elements

	//bool			delEdge(EDGE3D);
	//bool			delEdge(size_t);

	//void			addPolygon(POLY3D);
	//void			addListOfPolygons(POLY_LIST);
	//bool			delPolygon(POLY3D);
	//bool			delPolygon(size_t);
	//size_t	delListOfPolygons(POLY_LIST);
};
typedef clsMesh MESH3D, *LPMESH3D;



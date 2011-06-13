#pragma once

#include "Primitives.h"

class clsTankBody : public clsMesh {
private:
	float Length;
	float Width;
	float Height;
	
	float fbA;
	float ftA;
	float bbA;
	float btA;
	float tA;
	float sA;

	void Triangulate();

public:
	clsTankBody(
			float bL,
			float bW,
			float bH,
			float fba,
			float fta,
			float bba,
			float bta,
			float ta,
			float sa
		);
	clsTankBody(
			float bL,
			float bW,
			float bH,
			float fba,
			float fta,
			float bba,
			float bta,
			float ta,
			float sa,
			COLORREF c
		);
	clsTankBody(
			float bL,
			float bW,
			float bH,
			float fba,
			float fta,
			float bba,
			float bta,
			float ta,
			float sa,
			unsigned char red, 
			unsigned char green,
			unsigned char blue
		);
};
typedef clsTankBody TANKBODY, *LPTANKBODY;


#define APPROX_DEFAULT_PREC 18
#define JGPZ_PARTS_NUM 3

enum JGPZ_PART {
	PZ_BODY		= 0,
	PZ_CANNON	= 1,
	PZ_TRACK	= 2,
	PZ_LEAD_WH	= 3,
	PZ_BASE_WH	= 4,
	PZ_TAIL_WH	= 5
};

class clsJagdpanther : public clsMesh {
	LPMESH3D	*jgPzParts;
	float		cLen;	
public:
	clsJagdpanther(
			float bL,
			float bW,
			float bH,
			float fba,
			float fta,
			float bba,
			float bta,
			float ta,
			float sa,
			float clen,
			unsigned char red, 
			unsigned char green,
			unsigned char blue		
		);
	virtual ~clsJagdpanther();

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists
};
typedef clsJagdpanther JAGDPANTHER3D, *LPJAGDPANTHER3D;
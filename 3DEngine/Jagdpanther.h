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

class clsCaterpillar : public clsMesh {
private:
	VECTOR3D O1;
	float	 o1Radius;
	VECTOR3D O2;
	float	 o2Radius;
	VECTOR3D O3;
	float	 o3Radius;
	VECTOR3D O4;
	float	 o4Radius;

	float	conRad1;
	float	conRad2;
	float	conRad3;
	float	conRad4;

	int		precision;

	void Triangulate();
public:
	clsCaterpillar(
		const VECTOR3D &o1,
		float o1rad,
		const VECTOR3D &o2,
		float o2rad,
		const VECTOR3D &o3,
		float o3rad,
		const VECTOR3D &o4,
		float o4rad,
		float crad1,
		float crad2,
		float crad3,
		float crad4,
		int	  prec
	);
};
typedef clsCaterpillar CATERPILLAR, *LPCATERPILLAR;


#define APPROX_DEFAULT_PREC 18
#define JGPZ_PARTS_NUM 28

enum JGPZ_PART {
	PZ_BODY				= 0,
	PZ_CANNON_BASE		= 1,
	PZ_CANNON_DEMPFER	= 2,
	PZ_CANNON			= 3,
	PZ_MG_BASE			= 4,
	PZ_MG				= 5,
	PZ_LEAD_WHL			= 6,
	PZ_LEAD_WHR			= 7,
	PZ_REAR_WHL			= 8,
	PZ_REAR_WHR			= 9,
	PZ_BASE_WH1			= 10,
	PZ_BASE_WH2			= 11,
	PZ_BASE_WH3			= 12,
	PZ_BASE_WH4			= 13,
	PZ_BASE_WH5			= 14,
	PZ_BASE_WH6			= 15,
	PZ_BASE_WH7			= 16,
	PZ_BASE_WH8			= 17,
	PZ_BASE_WH9			= 18,
	PZ_BASE_WH10		= 19,
	PZ_BASE_WH11		= 20,
	PZ_BASE_WH12		= 21,
	PZ_BASE_WH13		= 22,
	PZ_BASE_WH14		= 23,
	PZ_BASE_WH15		= 24,
	PZ_BASE_WH16		= 25,
	PZ_TRACK_L			= 26,
	PZ_TRACK_R			= 27
};

class clsJagdpanther : public clsMesh {
private:
	LPMESH3D	*Parts;
	float		Length;
	float		Width;
	float		Height;
	
	float		fbA;
	float		ftA;
	float		bbA;
	float		btA;
	float		tA;
	float		sA;
	float		cLen;	

	bool getMeshPosition(
		JGPZ_PART part,
		int& vs,
		int& es,
		int& ps
		);

	void addMesh	(JGPZ_PART part, MESH3D&); // adding vertices, edges and polygons as a specific part
	void deleteMesh (JGPZ_PART part);

	void replaceMesh		(JGPZ_PART part, MESH3D&);
	void recalcMeshVertices (JGPZ_PART part, MESH3D&);

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

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

	float getBodyLength();
	float getBodyWidth();
	float getBodyHeight();
	float getFTArmSlope();
	float getFBArmSlope();
	float getBTArmSlope();
	float getBBArmSlope();
	float getTopArmSlope();
	float getSideArmSlope();
	float getCannonLength();
};
typedef clsJagdpanther JAGDPANTHER3D, *LPJAGDPANTHER3D;
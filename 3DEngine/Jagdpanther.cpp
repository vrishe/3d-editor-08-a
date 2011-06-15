#include "Jagdpanther.h"


clsTankBody::clsTankBody(
			float bL,
			float bW,
			float bH,
			float fba,
			float fta,
			float bba,
			float bta,
			float ta,
			float sa
) : clsMesh(MSH_TANK) {
	float cnv2rad	= (FLOAT)M_PI / 180.0f;

	Length			= bL;
	Width			= bW;
	Height			= bH;
	fbA				= fba * cnv2rad;
	ftA				= fta * cnv2rad;
	bbA				= bba * cnv2rad;
	btA				= bta * cnv2rad;
	tA				= ta  * cnv2rad;
	sA				= sa  * cnv2rad;

	Triangulate();
}

clsTankBody::clsTankBody(
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
) : clsMesh(MSH_TANK, c) {
	float cnv2rad	= (FLOAT)M_PI / 180.0f;

	Length			= bL;
	Width			= bW;
	Height			= bH;
	fbA				= fba * cnv2rad;
	ftA				= fta * cnv2rad;
	bbA				= bba * cnv2rad;
	btA				= bta * cnv2rad;
	tA				= ta  * cnv2rad;
	sA				= sa  * cnv2rad;

	Triangulate();
}

clsTankBody::clsTankBody(
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
) : clsMesh(MSH_TANK, red, green, blue) {
	float cnv2rad	= (FLOAT)M_PI / 180.0f;

	Length			= bL;
	Width			= bW;
	Height			= bH;
	fbA				= fba * cnv2rad;
	ftA				= fta * cnv2rad;
	bbA				= bba * cnv2rad;
	btA				= bta * cnv2rad;
	tA				= ta  * cnv2rad;
	sA				= sa  * cnv2rad;

	Triangulate();
}

void clsTankBody::Triangulate()
{
	VECTOR3D v[11];

	vertices.clear();
	edges.clear();
	polygons.clear();

	// Symmetry projection above ZoX
	v[1].x = Length	/ 2.0f;									// half length
	v[1].z = Height * 0.2f;									// frontal slice
	v[2].x = v[1].x - (v[1].z / tan((FLOAT)(M_PI_2 - fbA)));
	v[7].x = -v[1].x;
	v[7].z = Height * 0.65f;									// back slice
	v[6].x = v[7].x;
	v[6].z = Height * 0.575f;
	v[3].x = v[6].x + (v[6].z / tan((FLOAT)(M_PI_2 - bbA))); 
	v[4].z = Height * 0.3f;
	v[4].x = v[7].x + ((v[7].z - v[4].z) / tan((FLOAT)(M_PI_2 - bbA)));
	float l = v[7].z - v[6].z;
	v[5] = v[4] - VECTOR3D(
						l * sin(2 * bbA),
						.0f,
						l * -cos(2 * bbA)
					);
	v[8].x = v[7].x * 0.4f;
	v[8].z = v[7].z;
	v[9].z = Height;
	v[9].x = v[8].x - (v[9].z / tan((FLOAT)(M_PI_2 + btA)));
	float	tangentA	= tan((FLOAT)M_PI_2 + ftA),
			tangentB	= tan(-tA);
	v[10].x = (v[1].z - tangentA * v[1].x 
						- v[9].z + tangentB * v[9].x) 
					/ (tangentB - tangentA);
	v[10].z = tangentA * (v[10].x - v[1].x) + v[1].z;
	v[0].z = Height * 0.4f;
	v[0].x = (v[0].z + tangentA * v[1].x - v[1].z) / tangentA;


	// Copy vertices and reallocate em. as should be.

	float halfWidth		= Width		/ 2.0f;
	float baseHalfWidth	= halfWidth * 0.568f;

	vertices.insert(vertices.end(), v, v + 5);
	for( UINT i = 0; i < 5; i++ )
		vertices[i].y += baseHalfWidth;

	vertices.insert(vertices.end(), v + 4, v + 11);
	vertices.push_back(v[0]);
	vertices[5].y	+= halfWidth;
	vertices[12].y	+= halfWidth * 0.92f;

	float x12x5 = vertices[12].x - vertices[5].x; 
	float y12y5 = vertices[12].y - vertices[5].y;
	float z12z5 = vertices[12].z - vertices[5].z; 
	for( UINT i = 6; i < 12; i++ )
	{
		VECTOR3D temp1, temp2;
		vertices[i].y += vertices[5].y;
		if ( i > 8 ) 
		{
			vertices[i].y += (vertices[i].x - vertices[5].x)
							* y12y5 / x12x5;
		}

		temp1 = vertices[i];
		temp1.z = (temp1.x - vertices[5].x) 
				* z12z5 / x12x5 
				+ vertices[5].z;
		temp2.x = temp1.x;
		temp2.z = temp1.y * tan((FLOAT)M_PI_2 - sA);
		vertices[i].y = (vertices[i].z - temp2.z) 
						* (temp1.y - temp2.y) 
						/ (temp1.z - temp2.z)
						+ temp2.y;

	}

	vertices.insert(vertices.end(), vertices.begin(), vertices.end());
	for ( UINT i = 13; i < 26; i++ )
		vertices[i].y *= -1.0f;

	for ( UINT i = 0; i < 5; i++ )
	{
		edges.push_back(EDGE3D(i, i + 13));
		if ( i > 1 ) 
		{
				edges.push_back(EDGE3D(0, i));
				edges.push_back(EDGE3D(13, i + 13));
		}
		if ( i < 4 )
		{
			polygons.push_back(POLY3D(0, i + 2, i + 1));
			polygons.push_back(POLY3D(13, i + 14, i + 15));
			edges.push_back(EDGE3D(i, i + 1));
			edges.push_back(EDGE3D(i, i + 14));
			edges.push_back(EDGE3D(i + 13, i + 14));
			polygons.push_back(POLY3D(i, i + 14, i + 13));
			polygons.push_back(POLY3D(i, i + 1, i + 14));
		}
	}
	edges.push_back(EDGE3D(0, 12));
	edges.push_back(EDGE3D(4, 5));
	edges.push_back(EDGE3D(13, 25));
	edges.push_back(EDGE3D(17, 18));
	polygons.push_back(POLY3D(0, 12, 5));
	polygons.push_back(POLY3D(13, 18, 25));

	for ( UINT i = 6; i < 12; i++ )
	{
		edges.push_back(EDGE3D(i, i + 13));
		edges.push_back(EDGE3D(i, i + 1));
		edges.push_back(EDGE3D(i + 13, i + 14));
		edges.push_back(EDGE3D(i, i + 14));
		if ( i < 11 )
		{
			polygons.push_back(POLY3D(i, i + 14, i + 13));
			polygons.push_back(POLY3D(i, i + 1, i + 14));
		}
	}
	edges.push_back(EDGE3D(24, 13));
	edges.push_back(EDGE3D(11, 0));
	edges.push_back(EDGE3D(5,6));
	edges.push_back(EDGE3D(18, 19));
	edges.push_back(EDGE3D(4, 6));
	edges.push_back(EDGE3D(17, 6));
	edges.push_back(EDGE3D(17, 19));
	edges.push_back(EDGE3D(5, 12));
	edges.push_back(EDGE3D(18, 25));
	edges.push_back(EDGE3D(5, 12));
	edges.push_back(EDGE3D(7, 9));
	edges.push_back(EDGE3D(6, 9));
	edges.push_back(EDGE3D(5, 9));
	edges.push_back(EDGE3D(12, 9));
	edges.push_back(EDGE3D(12, 10));
	edges.push_back(EDGE3D(20, 22));
	edges.push_back(EDGE3D(19, 22));
	edges.push_back(EDGE3D(18, 22));
	edges.push_back(EDGE3D(25, 22));
	edges.push_back(EDGE3D(25, 23));

	polygons.push_back(POLY3D(12, 0, 11));
	polygons.push_back(POLY3D(13, 25, 24));
	polygons.push_back(POLY3D(12, 13, 11));
	polygons.push_back(POLY3D(11, 13, 24));
	polygons.push_back(POLY3D(6, 4, 5));
	polygons.push_back(POLY3D(6, 17, 4));
	polygons.push_back(POLY3D(6, 19, 17));
	polygons.push_back(POLY3D(17, 19, 18));
	polygons.push_back(POLY3D(7, 9, 8));
	polygons.push_back(POLY3D(6, 9, 7));
	polygons.push_back(POLY3D(5, 9, 6));
	polygons.push_back(POLY3D(5, 12, 9));
	polygons.push_back(POLY3D(9, 12, 10));
	polygons.push_back(POLY3D(10, 12, 11));
	polygons.push_back(POLY3D(20, 21, 22));
	polygons.push_back(POLY3D(19, 20, 22));
	polygons.push_back(POLY3D(18, 19, 22));
	polygons.push_back(POLY3D(18, 22, 25));
	polygons.push_back(POLY3D(22, 23, 25));
	polygons.push_back(POLY3D(23, 24, 25));

	for( UINT i = 0, max = vertices.size(); i < max; i++ )
		swap(vertices[i].x, vertices[i].y);

	Transform();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

clsCaterpillar::clsCaterpillar(
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
						int   prec
) : clsMesh(MSH_CAT) {
	O1 = o1;
	O2 = o2;
	O3 = o3;
	O4 = o4;
	o1Radius = o1rad;
	o2Radius = o2rad;
	o3Radius = o3rad;	
	o4Radius = o4rad;

	if ( crad1 - o1Radius > EPSILON 
		&& crad1 - o2Radius > EPSILON ) 
		conRad1 = crad1;
	else
		conRad1 = o1Radius + o2Radius;
	conRad2 = crad2;
	if ( crad1 - o4Radius > EPSILON 
		&& crad1 - o3Radius > EPSILON ) 
		conRad3 = crad3;
	else
		conRad3 = o4Radius + o3Radius;
	conRad4 = crad4;

	precision = prec;
	Triangulate();
}

void clsCaterpillar::Triangulate()
{
	vertices.clear();
	edges.clear();
	polygons.clear();

	float circle1rad = conRad1 - o1Radius;
	float circle2rad = conRad1 - o2Radius;
	float circleDist = Vector3DLength(O2 - O1); 
	if ( circleDist > EPSILON ) 
	{
		float a		 = (circle1rad * circle2rad
					- circle2rad * circle2rad
					+ circleDist * circleDist ) / (2 * circleDist);
		float height = sqrt(circle1rad * circle1rad  - a * a); 
		VECTOR3D Direction;
		Vector3DNormalize(O2 - O1, Direction);
		VECTOR3D P2( O1 + Direction * a );
		VECTOR3D Intersection ( 
						P2.x + height * ( O2.z - O1.z ) / circleDist,
						.0f,
						P2.z + height * ( O2.x - O1.x ) / circleDist
					);
		VECTOR3D	v1, 
					v2;
		Vector3DNormalize(VECTOR3D(O1 - Intersection), v1);
		Vector3DNormalize(VECTOR3D(O2 - Intersection), v2);
		float cAngle = Vector3DMultS( v1, v2 );
		Sleep(20);

	}

	Transform();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}


clsJagdpanther::clsJagdpanther(
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
) : clsMesh( MSH_TANK, red, green, blue ) {
	Length	= bL;
	Width	= bW;
	Height	= bH;
	fbA		= fba;
	ftA		= fta;
	bbA		= bba;
	btA		= bta;
	tA		= ta;
	sA		= sa;
	cLen	= clen;

	Parts = new LPMESH3D[JGPZ_PARTS_NUM];
	Triangulate();
}

clsJagdpanther::~clsJagdpanther() { delete[] Parts; }

void clsJagdpanther::Triangulate() {
	vertices.clear();
	edges.clear();
	polygons.clear();

	TANKBODY tBody(Length, Width, Height, fbA, ftA, bbA, btA, tA, sA);
	tBody.Translate(.0, .0, Height * 0.25f);
	tBody.Yaw(-(FLOAT)M_PI_2);
	tBody.Transform();
		addMesh(PZ_BODY,	tBody);

	float CannonZ = Height * 0.93f;
	float CannonBaseH  = Length * .11f;
	float CannonBaseR1 = Height * .14f;
	float CannonBaseR2 = Height * 0.075f;
	float CannonBaseX = Length * .5f - (CannonZ + CannonBaseR1) * cos(ftA * (FLOAT)(M_PI / 180.0f));
	CONE3D	CannonBase(Length * 0.11f, CannonBaseR1, CannonBaseR2, APPROX_DEFAULT_PREC);
	CannonBase.Yaw(-(FLOAT)M_PI_2);
	CannonBase.Pitch(-(FLOAT)M_PI_2);
	CannonBase.Translate(.0f, CannonBaseX - CannonBaseH, CannonZ);
	CannonBase.Transform();
		addMesh(PZ_CANNON_BASE,	CannonBase);

	float CannonDpfrR1 = Height * 0.06f;
	float CannonDpfrH  = cLen * .3f;
	CONE3D	CannonDpfr(CannonDpfrH, CannonDpfrR1, CannonDpfrR1, APPROX_DEFAULT_PREC);
	CannonDpfr.Yaw(-(FLOAT)M_PI_2);
	CannonDpfr.Pitch(-(FLOAT)M_PI_2);
	CannonDpfr.Translate(.0f, CannonBaseX, CannonZ);
	CannonDpfr.Transform();
		addMesh(PZ_CANNON_DEMPFER,	CannonDpfr);

	float CannonR1 = Height * 0.045f;
	float CannonR2 = Height * .04f;
	float Caliber  = .8f;
	float CannonH  = cLen * .7f;
	HOLE3D	Cannon(CannonH, CannonR1, CannonR1 * Caliber, CannonR2, CannonR2 * Caliber, APPROX_DEFAULT_PREC);
	Cannon.Yaw(-(FLOAT)M_PI_2);
	Cannon.Pitch(-(FLOAT)M_PI_2);
	Cannon.Translate(.0f, CannonBaseX + CannonDpfrH, CannonZ);
	Cannon.Transform();
		addMesh(PZ_CANNON,	Cannon);

	float MgBaseR = Height * .125f;
	float MgBaseZ = Height * .82f; 
	float MgBaseX = Length * .5f - (MgBaseZ + 2 * MgBaseR) * cos(ftA * (FLOAT)(M_PI / 180.0f));
	float MgBaseY = Width * .5f - MgBaseZ * sin(sA * (FLOAT)(M_PI / 180.0f));
	SPHERE3D MgBase(MgBaseR, .5f, 0, (FLOAT)M_PI * 2, APPROX_DEFAULT_PREC, 0x00fffff);
	MgBase.Roll((90 - ftA) * (FLOAT)(M_PI / 180.0f));
	MgBase.Translate(MgBaseY, MgBaseX, MgBaseZ);
	MgBase.Transform();
		addMesh(PZ_MG_BASE, MgBase);

	float MgLength	= Length * .07f;
	float MgR		= Height * .02f;
	CONE3D Mg(MgLength, MgR, MgR, APPROX_DEFAULT_PREC);
	Mg.Roll(-(FLOAT)M_PI_2);
	Mg.Translate(MgBaseY, MgBaseX + MgBaseR + MgLength, MgBaseZ);
	Mg.Transform();
		addMesh(PZ_MG, Mg);

	CONE3D  FrontWheel(Width * .16f, Height * .16f, Height * .16f, APPROX_DEFAULT_PREC); 
	FrontWheel.Pitch((FLOAT)M_PI_2);
	FrontWheel.Translate(-Width * .45f + Width * .16f, (Length / 2.0f) * .85f, Height * 0.44f);
	FrontWheel.Transform();
		addMesh(PZ_LEAD_WHL, FrontWheel);
	FrontWheel.Pitch((FLOAT)M_PI);
	FrontWheel.Translate(Width * .45f - Width * .16f, (Length / 2.0f) * .85f, Height * 0.44f);
	FrontWheel.Transform();
		addMesh(PZ_LEAD_WHR, FrontWheel);

	CONE3D RearWheel(Width * .16f, Height * .11f, Height * .11f, APPROX_DEFAULT_PREC); 
	RearWheel.Pitch((FLOAT)M_PI_2);
	RearWheel.Translate(-Width * .45f + Width * .16f, -(Length / 2.0f) * .8f, Height * 0.37f);
	RearWheel.Transform();
		addMesh(PZ_REAR_WHL, RearWheel);
	RearWheel.Pitch((FLOAT)M_PI);
	RearWheel.Translate(Width * .45f - Width * .16f, -(Length / 2.0f) * .8f, Height * 0.37f);
	RearWheel.Transform();
		addMesh(PZ_REAR_WHR, RearWheel);

	float delta = (Length * 0.68f) / 4;
	CONE3D Wheel(Width * .06f, Height * .22f, Height * .22f, APPROX_DEFAULT_PREC);
	Wheel.Pitch((FLOAT)M_PI_2);
	Wheel.Translate(-Width * .45f + Width * .16f, (Length / 2.0f) * .63f, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH1, Wheel);
	Wheel.Translate(-Width * .45f + Width * .16f, (Length / 2.0f) * .63f - delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH2, Wheel);
	Wheel.Translate(-Width * .45f + Width * .16f, (Length / 2.0f) * .63f - 2 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH3, Wheel);
	Wheel.Translate(-Width * .45f + Width * .16f, (Length / 2.0f) * .63f - 3 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH4, Wheel);
	Wheel.Pitch((FLOAT)M_PI);
	Wheel.Translate(Width * .45f - Width * .16f, (Length / 2.0f) * .63f, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH5, Wheel);
	Wheel.Translate(Width * .45f - Width * .16f, (Length / 2.0f) * .63f - delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH6, Wheel);
	Wheel.Translate(Width * .45f - Width * .16f, (Length / 2.0f) * .63f - 2 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH7, Wheel);
	Wheel.Translate(Width * .45f - Width * .16f, (Length / 2.0f) * .63f - 3 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH8, Wheel);

	Wheel.Translate(-Width * .45f , (Length / 2.0f) * .44f, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH9, Wheel);
	Wheel.Translate(-Width * .45f , (Length / 2.0f) * .44f - delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH10, Wheel);
	Wheel.Translate(-Width * .45f , (Length / 2.0f) * .44f - 2 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH11, Wheel);
	Wheel.Translate(-Width * .45f , (Length / 2.0f) * .44f - 3 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH12, Wheel);
	Wheel.Pitch(-(FLOAT)M_PI);
		Wheel.Translate(Width * .45f, (Length / 2.0f) * .44f, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH13, Wheel);
	Wheel.Translate(Width * .45f, (Length / 2.0f) * .44f - delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH14, Wheel);
	Wheel.Translate(Width * .45f, (Length / 2.0f) * .44f - 2 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH15, Wheel);
	Wheel.Translate(Width * .45f, (Length / 2.0f) * .44f - 3 * delta, Height * 0.25f);
	Wheel.Transform();
		addMesh(PZ_BASE_WH16, Wheel);

	Transform();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

bool clsJagdpanther::getMeshPosition(JGPZ_PART part, int& vs, int& es, int& ps) {
	vs = es = ps = -1;
	if ( part < 0 ) return false;
	vs = es = ps = 0;
	for (int i = 0; i < JGPZ_PARTS_NUM; i++) {
		if (part == i) return true;
		vs += Parts[i]->getVerticesCount();
		es += Parts[i]->getEdgesCount();
		ps += Parts[i]->getPolygonsCount();
	}
	return false;
}

void clsJagdpanther::addMesh(JGPZ_PART part, MESH3D& m) {
	Parts[part] = new MESH3D(m);
	
	int vNum, eNum, pNum;
	getMeshPosition(part, vNum, eNum, pNum);
	LPVECTOR3D vs = new VECTOR3D[m.getVerticesCount()];
	m.getVerticesTransformed(vs);
	vertices.insert(vertices.begin() + vNum, vs, vs + m.getVerticesCount());
	delete [] vs;

	EDGE_LIST es = m.getEdges();
	int num = es.size();
	for (int i = 0; i < num; i++ )
		es[i] += vNum;
	edges.insert(edges.begin() + eNum, es.begin(), es.end());

	POLY_LIST ps = m.getPolygons();
	num = ps.size();
	for (int i = 0; i < num; i++ ) {
		ps[i] += vNum;
		size_t tmp = ps[i].first;
		ps[i].first = ps[i].second;
		ps[i].second = tmp;
	}
	polygons.insert(polygons.begin() + pNum, ps.begin(), ps.end());
}

void clsJagdpanther::deleteMesh(JGPZ_PART part) {
	int vNum, eNum, pNum;
	getMeshPosition(part, vNum, eNum, pNum);
	vertices.erase(vertices.begin() + vNum, vertices.begin() + Parts[part]->getVerticesCount() + vNum);
	edges.erase   (edges.begin() + eNum,    edges.begin() + Parts[part]->getEdgesCount() + eNum);
	polygons.erase(polygons.begin() + pNum, polygons.begin() + Parts[part]->getPolygonsCount() + pNum);
}

void clsJagdpanther::replaceMesh(JGPZ_PART part, MESH3D& m) {
	deleteMesh(part);
	addMesh(part, m);
}

void clsJagdpanther::recalcMeshVertices(JGPZ_PART part, MESH3D& m) {
	int vFrom, vTo;
	getMeshPosition(part, vFrom, vTo, vTo);
	vTo = Parts[part]->getVerticesCount();
	LPVECTOR3D vs = new VECTOR3D[vTo];
	Parts[part]->getVerticesTransformed(vs);	
	vertices.erase(vertices.begin() + vFrom, vertices.begin() + vTo + vFrom);
	vertices.insert(vertices.begin() + vFrom, vs, vs + vTo);
	delete [] vs;
}

float clsJagdpanther::getBodyLength() { return Length; }
float clsJagdpanther::getBodyWidth() { return Width; }
float clsJagdpanther::getBodyHeight() { return Height; }
float clsJagdpanther::getFTArmSlope() { return ftA; }
float clsJagdpanther::getFBArmSlope() { return fbA; }
float clsJagdpanther::getBTArmSlope() { return btA; }
float clsJagdpanther::getBBArmSlope() { return bbA; }
float clsJagdpanther::getTopArmSlope() { return tA; }
float clsJagdpanther::getSideArmSlope() { return sA; }
float clsJagdpanther::getCannonLength() { return cLen; } 
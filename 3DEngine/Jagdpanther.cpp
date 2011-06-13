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
	jgPzParts = new LPMESH3D[JGPZ_PARTS_NUM];
	cLen = clen;

	Triangulate();
}

clsJagdpanther::~clsJagdpanther() { delete[] jgPzParts; }

void clsJagdpanther::Triangulate() {
	vertices.clear();
	edges.clear();
	polygons.clear();


	Transform();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}
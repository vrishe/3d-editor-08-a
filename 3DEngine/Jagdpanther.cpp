#include "Jagdpanther.h"


clsTankBody::clsTankBody(
			float bL,
			float bW,
			float bH,
			float fbA
) : clsMesh(MSH_TANK) {
	Length		= bL;
	Width		= bW;
	Height		= bH;
	frontBottom = fbA * (FLOAT)M_PI / 180.0f;

	Triangulate();
}

void clsTankBody::Triangulate()
{
	vertices.clear();
	edges.clear();
	polygons.clear();

	float halfLength	= Length	/ 2.0f;
	float tempLength;
	float halfWidth		= Width		/ 2.0f;
	float tempWidth;
	float halfHeight	= Height	/ 2.0f;
	float tempHeight;

	// Base armor plate
	tempWidth	= halfWidth * 0.563f;
	tempHeight	= .0f;
	vertices.push_back(VECTOR3D(halfLength , tempWidth, tempHeight));	// 0
	vertices.push_back(VECTOR3D(-halfLength, tempWidth, tempHeight));	
	vertices.push_back(VECTOR3D(-halfLength, -tempWidth, tempHeight));	
	vertices.push_back(VECTOR3D(halfLength, -tempWidth, tempHeight));	// 3	
	// Bottom front armor plate
	tempHeight	= Height * 0.23f;
	vertices.push_back(VECTOR3D(halfLength, tempWidth, tempHeight));
	vertices.push_back(VECTOR3D(halfLength, -tempWidth, tempHeight));	// 5
	// Top front armor plate
	tempHeight	= Height * 0.41f;
	vertices.push_back(VECTOR3D(halfLength, tempWidth, tempHeight));
	vertices.push_back(VECTOR3D(halfLength, -tempWidth, tempHeight));	// 7
	tempWidth	= halfWidth;
	tempHeight	= Height * 0.88f;
	vertices.push_back(VECTOR3D(halfLength, tempWidth, tempHeight));
	vertices.push_back(VECTOR3D(halfLength, -tempWidth, tempHeight));	// 9
	tempHeight	= Height * 0.41f;
	vertices.push_back(VECTOR3D(halfLength, tempWidth, tempHeight));	 
	vertices.push_back(VECTOR3D(halfLength, -tempWidth, tempHeight));	// 11
	// Top armor plate
	tempLength	= halfLength * 0.66f;
	tempHeight	= Height * 0.88f;
	vertices.push_back(VECTOR3D(-tempLength, tempWidth, tempHeight));
	vertices.push_back(VECTOR3D(-tempLength, -tempWidth, tempHeight));	// 13
	// Top back armor plate
	tempLength = halfLength * 0.71f;
	tempHeight = Height * 0.65f;
	vertices.push_back(VECTOR3D(-tempLength, tempWidth, tempHeight));
	vertices.push_back(VECTOR3D(-tempLength, -tempWidth, tempHeight));	// 15
	tempLength = halfLength;
	vertices.push_back(VECTOR3D(-tempLength, tempWidth, tempHeight));
	vertices.push_back(VECTOR3D(-tempLength, -tempWidth, tempHeight));	// 17
	//

	//vertices.push_back(VECTOR3D(Length / 2.0f, Width * 0.2815f, Height * 0.23f));
	//vertices.push_back(VECTOR3D(Length / 2,.0f -Width * 0.2815f, Height * 0.23f));
	//vertices.push_back(VECTOR3D(
	//						(Length / 2.0f 
	//						+ Height * 0.23f 
	//						/ -tan((FLOAT)M_PI_2 - frontBottom)),
	//						-Width * 0.2815f,
	//						.0f));
	//vertices.push_back(VECTOR3D(
	//						(Length / 2.0f 
	//						+ Height * 0.23f 
	//						/ -tan((FLOAT)M_PI_2 - frontBottom)),
	//						Width * 0.2815f,
	//						.0f));

	//vertices.push_back(VECTOR3D(Length / 2.0f, Width * 0.9f, Height * 0.23f));
	//vertices.push_back(VECTOR3D(Length / 2.0f, -Width * 0.9f, Height * 0.23f));

	// Base armor plate
	edges.push_back(EDGE3D(0, 1));
	edges.push_back(EDGE3D(0, 2));
	edges.push_back(EDGE3D(1, 2));
	edges.push_back(EDGE3D(2, 3));
	edges.push_back(EDGE3D(3, 0));
	//Bottom front armor plate
	edges.push_back(EDGE3D(3, 5));
	edges.push_back(EDGE3D(0, 5));
	edges.push_back(EDGE3D(0, 4));
	edges.push_back(EDGE3D(4, 5));
	// Top front armor plate
	edges.push_back(EDGE3D(4, 6));
	edges.push_back(EDGE3D(4, 7));
	edges.push_back(EDGE3D(6, 7));
	edges.push_back(EDGE3D(7, 5));
	edges.push_back(EDGE3D(6, 8));
	edges.push_back(EDGE3D(6, 9));
	edges.push_back(EDGE3D(8, 9));
	edges.push_back(EDGE3D(9, 7));
	edges.push_back(EDGE3D(10, 6));
	edges.push_back(EDGE3D(10, 8));
	edges.push_back(EDGE3D(11, 9));
	edges.push_back(EDGE3D(11, 7));
	// Top armor plate
	edges.push_back(EDGE3D(8, 12));
	edges.push_back(EDGE3D(8, 13));
	edges.push_back(EDGE3D(12, 13));
	edges.push_back(EDGE3D(13, 9));
	// Top back armor plate
	edges.push_back(EDGE3D(12, 14));
	edges.push_back(EDGE3D(12, 15));
	edges.push_back(EDGE3D(14, 15));
	edges.push_back(EDGE3D(15, 13));
	edges.push_back(EDGE3D(14, 16));
	edges.push_back(EDGE3D(14, 17));
	edges.push_back(EDGE3D(16, 17));
	edges.push_back(EDGE3D(17, 15));


	//Base armor plate
	polygons.push_back(POLY3D(2, 3, 0));
	polygons.push_back(POLY3D(1, 2, 0));
	//Bottom front armor plate
	polygons.push_back(POLY3D(5, 4, 0));
	polygons.push_back(POLY3D(0, 3, 5));
	// Top front armor plate
	polygons.push_back(POLY3D(4, 7, 6));
	polygons.push_back(POLY3D(4, 5, 7));
	polygons.push_back(POLY3D(6, 9, 8));
	polygons.push_back(POLY3D(6, 7, 9));
	polygons.push_back(POLY3D(10, 6, 8));
	polygons.push_back(POLY3D(7, 11, 9));
	// Top armor plate
	polygons.push_back(POLY3D(8, 13, 12));
	polygons.push_back(POLY3D(8, 9, 13));
	// Top back armor plate
	polygons.push_back(POLY3D(12, 15, 14));
	polygons.push_back(POLY3D(12, 13, 15));
	polygons.push_back(POLY3D(14, 17, 16));
	polygons.push_back(POLY3D(14, 15, 17));


	flushVertices();
	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}
#include "3DGenerator.h"
#include <math.h>
#include <Windows.h>

#ifndef STRUCTS_CPP
#define STRUCTS_CPP

/* ------------------------ STRUCTS ------------------------ */

Vertex::Vertex() { x = 0; y = 0; z = 0; rhw = 1; }
Vertex::Vertex(float a, float b, float c) { x = a; y = b; z = c; rhw = 1; }
bool operator == (Vertex a, Vertex b) {
	if (a.x == b.x && a.y == b.y && a.z == b.z)
		return true;
	return false;
}

Polygon_::Polygon_() { first = -1; second = -1; third = -1; }
Polygon_::Polygon_(int a, int b, int c) { first = a; second = b; third = c; }
bool operator == (Polygon_ a, Polygon_ b) {
	if (a.first == b.first && a.second == b.second && a.third == b.third)
		return true;
	return false;
}

#endif // STRUCTS_CPP

#ifndef PRIMITIVE_CPP
#define PRIMITIVE_CPP

/* ----------------------- PRIMITIVE ------------------------ */

int Primitive::findVertex(Vertex v) {
	for (int i = 0; i < vCount; i++)
		if (vertexes[i] == v)
			return i;
	return -1;
}
int Primitive::findPolygon(Polygon_ p) {
	for (int i = 0; i < pCount; i++)
		if (polygons[i] == p)
			return i;
	return -1;
}

int Primitive::dropUnusedVertexes() {
	int result = 0;
	for (int i  = 0; i < vCount; i++) {
		bool found = false;
		for (int j = 0; j < pCount; j++)
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
int Primitive::dropRedundantPolygons() {
	int result = 0;
	for (int i = 0; i < pCount; i++) {
		bool firstFound = false, secondFound = false, thirdFound = false;
		for (int j = 0; j < vCount; j++) {
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
void Primitive::dropRedundant() {	
	dropRedundantPolygons();	
	dropUnusedVertexes();
}

void Primitive::Triangulate() {}
void Primitive::FillBuff(D3DVertex* vs) {
	for (int i = 0; i < pCount; i++) {
		int j = i*3;
		vs[j]	= (D3DVertex)vertexes[polygons[i].first];
		vs[j+1] = (D3DVertex)vertexes[polygons[i].second];
		vs[j+2] = (D3DVertex)vertexes[polygons[i].third];
	}

	D3DVertex a[1000];
	for (int i = 0; i < pCount * 3; i++)
		a[i] = vs[i];
	a[999];
}

/* ---------------------- constructors --------------------- */

Primitive::Primitive() : vCount(0), pCount(0) {}
Primitive::Primitive(int v, vector <Vertex> vs, int p, vector <Polygon_> ps) :
	vCount(v), vertexes(vs), pCount(p), polygons(ps) {}


/* ------------------------ getters ------------------------ */

int Primitive::getVCount() { return vCount; }
int Primitive::getPCount() { return pCount; }
vector <Vertex> Primitive::getVertexes() { return vertexes; }
vector <Polygon_> Primitive::getPolygons() { return polygons; }


/* ------------------------ setters ------------------------ */

void Primitive::addVertex(Vertex v) { vertexes.push_back(v); vCount++; }
void Primitive::addListOfVertexes(vector <Vertex> v) { vertexes.insert(vertexes.end(), v.begin(), v.end()); vCount += v.size(); }
bool Primitive::delVertex(Vertex v) {
	int pos = findVertex(v);
	if (pos == -1)
		return false;
	vertexes.erase(vertexes.begin() + pos);
	vCount--;
	return true;
}
bool Primitive::delVertex(int i) { 
	if ( i > -1 && i < vCount) {
		vertexes.erase(vertexes.begin() + i);
		return true;
	}
	return false; 
}
int Primitive::delListOfVertexes(vector <Vertex> v) {
	int result = 0, j;
	const int N = v.size();
	for (int i = 0; i < N; i++) {
		for (j = 0; j < vCount; j++)
			if (vertexes[j] == v[i]) {
				vertexes.erase(vertexes.begin() + j);
				vCount--;
				break;
			}
		if (j >= vCount)
			result--;
	}
	return result;
}

void Primitive::addPolygon(Polygon_ p) { polygons.push_back(p); pCount++; }
void Primitive::addListOfPolygons(vector <Polygon_> p) { polygons.insert(polygons.end(), p.begin(), p.end()); pCount += p.size(); }
bool Primitive::delPolygon(Polygon_ p) {
	int pos = findPolygon(p);
	if (pos == -1)
		return false;
	polygons.erase(polygons.begin() + pos);
	pCount--;
	return true;
}
bool Primitive::delPolygon(int i) { 
	if ( i > -1 && i < pCount) {
		polygons.erase(polygons.begin() + i);
		return true;
	}
	return false; 
}
int Primitive::delListOfPolygons(vector <Polygon_> p) {
	int result = 0, j;
	const int N = p.size();
	for (int i = 0; i < N; i++) {
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

#endif // PRIMITIVE_CPP

#ifndef PYRAMID_CPP
#define PYRAMID_CPP

/* ----------------------- PYRAMID ------------------------ */

void Pyramid::Triangulate() {
		// setting base vertexes
	addVertex(Vertex(-bL/2 + 300,-bW/2 + 300, 0));	// 0
	addVertex(Vertex(-bL/2 + 300,bW/2 + 300, 0));	// 1
	addVertex(Vertex(bL/2 + 300,bW/2 + 300, 0));	// 2
	addVertex(Vertex(bL/2 + 300,-bW/2 + 300, 0));	// 3
		// setting top vertexes
	addVertex(Vertex(-tL/2 + 300,-tW/2 + 300, h));	// 4
	addVertex(Vertex(-tL/2 + 300,tW/2 + 300, h));	// 5
	addVertex(Vertex(tL/2 + 300,tW/2 + 300, h));	// 6
	addVertex(Vertex(tL/2 + 300,-tW/2 + 300, h));	// 7

		// setting base polygons
	addPolygon(Polygon_(0,1,2));	// 0
	addPolygon(Polygon_(0,2,3));	// 1
		// setting top polygons
	addPolygon(Polygon_(4,5,7));	// 2
	addPolygon(Polygon_(5,6,7));	// 3
		// setting side polygons
	addPolygon(Polygon_(0,4,5));	// 4
	addPolygon(Polygon_(0,5,1));	// 5
	addPolygon(Polygon_(1,5,2));	// 6
	addPolygon(Polygon_(5,2,6));	// 7
	addPolygon(Polygon_(2,7,6));	// 8
	addPolygon(Polygon_(7,3,2));	// 9
	addPolygon(Polygon_(0,7,3));	// 10
	addPolygon(Polygon_(7,0,4));	// 11

	vertexes.shrink_to_fit();
	polygons.shrink_to_fit();
}

/* ---------------------- constructors --------------------- */

Pyramid::Pyramid() : Primitive(), h(0), bL(0), bW(0), tL(0), tW(0) {}
Pyramid::Pyramid(float height, float bLength, float bWidth, float tLength, float tWidth) :
	Primitive(), h(height), bL(bLength), bW(bWidth), tL(tLength), tW(tWidth) {
		Triangulate();
}


/* ------------------------ getters ------------------------ */

float Pyramid::getHeight() { return h; }
float Pyramid::getBLength() { return bL; }
float Pyramid::getBWidth() { return bW; }
float Pyramid::getTLength() {return tL; }
float Pyramid::getTWidth() {return tW; }


/* ------------------------ setters ------------------------ */

void Pyramid::setHeight(float n) { h = n; }
void Pyramid::setBLength(float n) { bL = n; }
void Pyramid::setBWidth(float n) { bW = n; }
void Pyramid::setTLength(float n) { tL = n; }
void Pyramid::setTWidth(float n) {tW = n; }


#endif // PYRAMID_CPP

#ifndef CONE_CPP
#define CONE_CPP

/* ----------------------- CONE ------------------------ */

void Cone::Triangulate() {
	int step = 360 / precission;
		// setting vertexes
	addVertex(Vertex(0, 0, 0));
	addVertex(Vertex(0, 0, h));
	addVertex(Vertex(bR, 0, 0));
	addVertex(Vertex(tR, 0, h));
	for (int i = 1; i < precission; i++) {
		float k = tan((step*i) * 3.1415926535 / 180);
		float d = bR*bR - k*k;
		float x = (-k + sqrt(d)) / 2;
		float y = k * x;
		addVertex(Vertex(x, y, 0));
		d = tR*tR - k*k;
		x = (-k + sqrt(d)) / 2;
		y = k * x;
		addVertex(Vertex(x, y, h));
	}

		// setting base polygons
	for (int i = 1; i < precission; i++)
		addPolygon(Polygon_(0, i*2, i*2 + 2));
	addPolygon(Polygon_(0, precission*2, 2));
		// setting top polygons
	for (int i = 1; i < precission; i++)
		addPolygon(Polygon_(1, i*2 + 1, i*2 + 3));
	addPolygon(Polygon_(1, precission*2 + 1, 3));
		// setting side polygons
	for (int i = 1; i < precission; i++) {
		addPolygon(Polygon_(i*2, i*2 + 1, i*2 + 2));
		addPolygon(Polygon_(i*2 + 1, i*2 + 2, i*2 + 3));
	}
	addPolygon(Polygon_(precission*2, precission*2 + 1, 2));
	addPolygon(Polygon_(precission*2 + 1, 2, 3));

	vertexes.shrink_to_fit();
	polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

Cone::Cone() : Primitive(), h(0), bR(0), tR(0), precission(0) {}
Cone::Cone(float height, float bRadius, float tRadius, int prec) : 
	Primitive(), h(height), bR(bRadius), tR(tRadius), precission(prec) {
	Triangulate();
}


/* ------------------------ getters ------------------------ */

float Cone::getHeight() { return h; }
float Cone::getBRadius() { return bR; }
float Cone::getTRadius() { return tR; }
int Cone::getPrecission() { return precission; }


/* ------------------------ setters ------------------------ */

void Cone::setHeight(float n) { h = n; } 
void Cone::setBRadius(float n) { bR = n; }
void Cone::setTRadius(float n) { tR = n; }
void Cone::setPrecission(int n) { precission = n; }


#endif // CONE_CPP

#ifndef HOLE_CPP
#define HOLE_CPP

/* ----------------------- HOLE ------------------------ */

void Hole::Triangulate() {
	int step = 360 / precission;
		// setting vertexes
	addVertex(Vertex(bR, 0, 0)); // setting outter circle
	addVertex(Vertex(tR, 0, h));
	for (int i = 1; i < precission; i++) {
		float k = tan((step*i) * 3.1415926535 / 180);
		float d = bR*bR - k*k;
		float x = (-k + sqrt(d)) / 2;
		float y = k * x;
		addVertex(Vertex(x, y, 0));
		d = tR*tR - k*k;
		x = (-k + sqrt(d)) / 2;
		y = k * x;
		addVertex(Vertex(x, y, h));
	}
	addVertex(Vertex(bRh, 0, 0)); // setting inner circle
	addVertex(Vertex(tRh, 0, h));
	for (int i = 1; i < precission; i++) {
		float k = tan((step*i) * 3.1415926535 / 180);
		float d = bRh*bRh - k*k;
		float x = (-k + sqrt(d)) / 2;
		float y = k * x;
		addVertex(Vertex(x, y, 0));
		d = tRh*tRh - k*k;
		x = (-k + sqrt(d)) / 2;
		y = k * x;
		addVertex(Vertex(x, y, h));
	}

		// setting base polygons
	for (int i = 0; i < precission - 1; i++) {
		addPolygon(Polygon_(i*2, precission*2 + i*2, precission*2 + i*2 + 2));
		addPolygon(Polygon_(precission*2 + i*2 + 2, i*2 + 2, i*2));
	}
	addPolygon(Polygon_(precission*2 - 2, precission*4 - 2, precission*2));
	addPolygon(Polygon_(precission*2, 0, precission*2 - 2));
		// setting top polygons
	for (int i = 0; i < precission - 1; i++) {
		addPolygon(Polygon_(i*2 + 1, precission*2 + i*2 + 1, precission*2 + i*2 + 3));
		addPolygon(Polygon_(precission*2 + i*2 + 3, i*2 + 3, i*2 + 1));
	}
	addPolygon(Polygon_(precission*2 - 1, precission*4 - 1, precission*2 + 1));
	addPolygon(Polygon_(precission*2 + 1, 1, precission*2 - 1));
		// setting side polygons
	for (int i = 0; i < precission - 1; i++) {
		addPolygon(Polygon_(i*2, i*2 + 1, i*2 + 2));
		addPolygon(Polygon_(i*2 + 1, i*2 + 3, i*2 + 2));
	}
	addPolygon(Polygon_(precission*2 - 2, precission*2 - 1, 0));
	addPolygon(Polygon_(precission*2 - 1, 1, 0));

	vertexes.shrink_to_fit();
	polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

Hole::Hole() : Primitive(), h(0), bR(0), bRh(0), tR(0), tRh(0), precission(0) {}
Hole::Hole(float height, float bRadius, float bHoleRadius, float tRadius, float tHoleRadius, int prec) : 
	Primitive(), h(height), bR(bRadius), bRh(bHoleRadius), tR(tRadius), tRh(tHoleRadius), precission(prec) {
	Triangulate();
}


/* ------------------------ getters ------------------------ */

float Hole::getHeight() { return h; }
float Hole::getBRadius() { return bR; }
float Hole::getTRadius() { return tR; }
float Hole::getBHoleRadius() { return bRh; }
float Hole::getTHoleRadius() { return tRh; }


/* ------------------------ setters ------------------------ */

void Hole::setHeight(float n) { h = n; } 
void Hole::setBRadius(float n) { bR = n; }
void Hole::setTRadius(float n) { tR = n; }
void Hole::setBHoleRadius(float n) { bRh = n; }
void Hole::setTHoleRadius(float n) { tRh = n; }

#endif // HOLE_CPP
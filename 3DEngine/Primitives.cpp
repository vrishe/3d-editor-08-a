#include "Primitives.h"
#include <math.h>
#include <Windows.h>

#ifndef PYRAMID_CPP
#define PYRAMID_CPP

/* ----------------------- PYRAMID ------------------------ */

void Pyramid::Triangulate() {
		// setting base vertices
	vertices.push_back(VERTEX3D(-bL/2,-bW/2, 0));	// 0
	vertices.push_back(VERTEX3D(-bL/2,bW/2, 0));	// 1
	vertices.push_back(VERTEX3D(bL/2,bW/2, 0));		// 2
	vertices.push_back(VERTEX3D(bL/2,-bW/2, 0));	// 3
		// setting top vertices
	vertices.push_back(VERTEX3D(-tL/2,-tW/2, h));	// 4
	vertices.push_back(VERTEX3D(-tL/2,tW/2, h));	// 5
	vertices.push_back(VERTEX3D(tL/2,tW/2, h));		// 6
	vertices.push_back(VERTEX3D(tL/2,-tW/2, h));	// 7

		// setting base polygons
	polygons.push_back(POLY3D(0,1,2));	// 0
	polygons.push_back(POLY3D(0,2,3));	// 1
		// setting top polygons
	polygons.push_back(POLY3D(4,5,7));	// 2
	polygons.push_back(POLY3D(5,6,7));	// 3
		// setting side polygons
	polygons.push_back(POLY3D(0,4,5));	// 4
	polygons.push_back(POLY3D(0,5,1));	// 5
	polygons.push_back(POLY3D(1,5,2));	// 6
	polygons.push_back(POLY3D(5,2,6));	// 7
	polygons.push_back(POLY3D(2,7,6));	// 8
	polygons.push_back(POLY3D(7,3,2));	// 9
	polygons.push_back(POLY3D(0,7,3));	// 10
	polygons.push_back(POLY3D(7,0,4));	// 11

	vertices.shrink_to_fit();
	polygons.shrink_to_fit();
}

/* ---------------------- constructors --------------------- */

Pyramid::Pyramid() : clsMesh(), h(0), bL(0), bW(0), tL(0), tW(0) {}
Pyramid::Pyramid(float height, float bLength, float bWidth, float tLength, float tWidth) :
	clsMesh(), h(height), bL(bLength), bW(bWidth), tL(tLength), tW(tWidth) {
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
		// setting vertices
	vertices.push_back(VERTEX3D(0, 0, 0));
	vertices.push_back(VERTEX3D(0, 0, h));
	for (int i = 0; i < precission; i++) {
		switch (step*i) {
		case 0:
			vertices.push_back(VERTEX3D(bR, 0, 0));
			vertices.push_back(VERTEX3D(tR, 0, h));
			break;
		case 90:
			vertices.push_back(VERTEX3D(0, bR, 0));
			vertices.push_back(VERTEX3D(0, tR, h));
			break;
		case 180:
			vertices.push_back(VERTEX3D(-bR, 0, 0));
			vertices.push_back(VERTEX3D(-tR, 0, h));
			break;
		case 270:
			vertices.push_back(VERTEX3D(0, -bR, 0));
			vertices.push_back(VERTEX3D(0, -tR, h));
			break;
		default:
			double k = tan((step*i) * 3.1415926535 / 180);
			float d = 2*bR*bR - k*k;
 			float x = (-k + sqrt(d)) / 2;
			float y = k * x;
			vertices.push_back(VERTEX3D(x, y, 0));
			d = 2*tR*tR - k*k;
			x = (-k + sqrt(d)) / 2;
			y = k * x;
			vertices.push_back(VERTEX3D(x, y, h));
		}
	}

		// setting base polygons
	for (int i = 1; i < precission; i++)
		polygons.push_back(POLY3D(0, i*2, i*2 + 2));
	polygons.push_back(POLY3D(0, precission*2, 2));
		// setting top polygons
	for (int i = 1; i < precission; i++)
		polygons.push_back(POLY3D(1, i*2 + 1, i*2 + 3));
	polygons.push_back(POLY3D(1, precission*2 + 1, 3));
		// setting side polygons
	for (int i = 1; i < precission; i++) {
		polygons.push_back(POLY3D(i*2, i*2 + 1, i*2 + 2));
		polygons.push_back(POLY3D(i*2 + 1, i*2 + 2, i*2 + 3));
	}
	polygons.push_back(POLY3D(precission*2, precission*2 + 1, 2));
	polygons.push_back(POLY3D(precission*2 + 1, 2, 3));

	vertices.shrink_to_fit();
	polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

Cone::Cone() : clsMesh(), h(0), bR(0), tR(0), precission(0) {}
Cone::Cone(float height, float bRadius, float tRadius, int prec) : 
	clsMesh(), h(height), bR(bRadius), tR(tRadius), precission(prec) {
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

#ifndef EXCONE_CPP
#define EXCONE_CPP

/* ------------------------ EXCONE ------------------------- */

void ExCone::Triangulate() {
	int step = 360 / precission;
		// setting vertices
	if ( secant >= 0 ) {
		vertices.push_back(VERTEX3D(0, 0, 0));
		vertices.push_back(VERTEX3D(0, 0, h));
	}
	else {
		vertices.push_back(VERTEX3D(0, secant, 0));
		vertices.push_back(VERTEX3D(0, secant, h));
	}
		// secant vertices
	bool secBase = true, secTop = true;
	VERTEX3D bV1, bV2, tV1, tV2;
	if ( abs(secant) >= bR )
		secBase = false;
	else {
		bV1 = VERTEX3D(sqrt(bR*bR - secant * secant), secant, 0);
		bV2 = VERTEX3D(-sqrt(bR*bR - secant * secant), secant, 0);
	}
	if ( abs(secant) >= tR )
		secBase = false;
	else {	
		tV1 = VERTEX3D(sqrt(tR*tR - secant * secant), secant, h);
		tV2 = VERTEX3D(-sqrt(tR*tR - secant * secant), secant, h);
	}
	bool secSet = false;
	for (int i = 0; i < precission; i++) { // circle vertices
		switch (step*i) {
		case 0:
			if ( secant >= 0 ) {
				vertices.push_back(VERTEX3D(bR, 0, 0));
				vertices.push_back(VERTEX3D(tR, 0, h));
			}
			else {
				vertices.push_back(bV1);
				vertices.push_back(bV2);
				vertices.push_back(tV1);
				vertices.push_back(tV2);
				secSet = true;
			}
			break;
		case 90:
			if ( secant > 0 ) {
				if ( !secSet ) {
					if ( !secBase )
						vertices.push_back(VERTEX3D(0, bR, 0));
					else {
						vertices.push_back(bV1);
						vertices.push_back(bV2);
					}
					if ( !secTop )
						vertices.push_back(VERTEX3D(0, tR, 0));
					else {
						vertices.push_back(tV1);
						vertices.push_back(tV2);
					}
					secSet = true;
				}
			}
			break;
		case 180:
			if ( secant >= 0 ) {
				vertices.push_back(VERTEX3D(-bR, 0, 0));
				vertices.push_back(VERTEX3D(-tR, 0, h));
			}
			break;
		case 270:
			vertices.push_back(VERTEX3D(0, -bR, 0));
			vertices.push_back(VERTEX3D(0, -tR, h));
			break;
		default:
			double k = tan((step*i) * 3.1415926535 / 180);
			float d = 2*bR*bR - k*k;
 			float x = (-k + sqrt(d)) / 2;
			float y = k * x;
			if ( y >= secant ) {
				if ( !secSet && secBase ) {
					vertices.push_back(bV1);
					vertices.push_back(bV2);
				}
			}
			else
				vertices.push_back(VERTEX3D(x, y, 0));

			d = 2*tR*tR - k*k;
			x = (-k + sqrt(d)) / 2;
			y = k * x;
			if ( y >= secant ) {
				if ( !secSet && secTop ) {
					vertices.push_back(tV1);
					vertices.push_back(tV2);
				}
			}
			else
				vertices.push_back(VERTEX3D(x, y, h));
		}
	}

		// setting base polygons
	int N = vertices.size() / 2, index = ( N == precission / 2 + 1 ? 2 : 1 );
	for (int i = index; i < N; i++)
		polygons.push_back(POLY3D(0, i*2, i*2 + 2));
	polygons.push_back(POLY3D(0, N*2, 2));
		// setting top polygons
	for (int i = index; i < N; i++)
		polygons.push_back(POLY3D(1, i*2 + 1, i*2 + 3));
	polygons.push_back(POLY3D(1, N*2 + 1, 3));
		// setting side polygons
	for (int i = index; i < N; i++) {
		polygons.push_back(POLY3D(i*2, i*2 + 1, i*2 + 2));
		polygons.push_back(POLY3D(i*2 + 1, i*2 + 2, i*2 + 3));
	}
	polygons.push_back(POLY3D(N*2, N*2 + 1, 2));
	polygons.push_back(POLY3D(N*2 + 1, 2, 3));

	vertices.shrink_to_fit();
	polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

ExCone::ExCone() : Cone() { secant = max(bR, tR); }
ExCone::ExCone(float height, float bRadius, float tRadius, int prec, float s) :
	Cone(height, bRadius, tRadius, prec), secant(s) {
		Triangulate();
}


/* ------------------------- getters ----------------------- */

float ExCone::getSecant() { return secant; }


/* ------------------------- setters ----------------------- */

void ExCone::setSecant(float n) { secant = n; }
 

#endif // EXCONE_CPP

#ifndef HOLE_CPP
#define HOLE_CPP

/* ----------------------- HOLE ------------------------ */

void Hole::Triangulate() {
	int step = 360 / precission;
		// setting vertices
	for (int i = 0; i < precission; i++) { // setting outter circle
		double a = (step*i) * 3.1415926535 / 180;
		switch (step*i) {
		case 0:
			vertices.push_back(VERTEX3D(bR, 0, 0));
			vertices.push_back(VERTEX3D(tR, 0, h));
			break;
		case 90:
			vertices.push_back(VERTEX3D(0, bR, 0));
			vertices.push_back(VERTEX3D(0, tR, h));
			break;
		case 180:
			vertices.push_back(VERTEX3D(-bR, 0, 0));
			vertices.push_back(VERTEX3D(-tR, 0, h));
			break;
		case 270:
			vertices.push_back(VERTEX3D(0, -bR, 0));
			vertices.push_back(VERTEX3D(0, -tR, h));
			break;
		default:
			float k = tan(a);

			float d = 2*bR*bR - k*k;
			float x = (-k + sqrt(d)) / 2;
			float y = k * x;
			vertices.push_back(VERTEX3D(x, y, 0));

			d = 2*tR*tR - k*k;
			x = (-k + sqrt(d)) / 2;
			y = k * x;
			vertices.push_back(VERTEX3D(x, y, h));
		}
	}
	for (int i = 0; i < precission; i++) {
		switch (step*i) {
		case 0:
			vertices.push_back(VERTEX3D(bRh, 0, 0));
			vertices.push_back(VERTEX3D(tRh, 0, h));
			break;
		case 90:
			vertices.push_back(VERTEX3D(0, bRh, 0));
			vertices.push_back(VERTEX3D(0, tRh, h));
			break;
		case 180:
			vertices.push_back(VERTEX3D(-bRh, 0, 0));
			vertices.push_back(VERTEX3D(-tRh, 0, h));
			break;
		case 270:
			vertices.push_back(VERTEX3D(0, -bRh, 0));
			vertices.push_back(VERTEX3D(0, -tRh, h));
			break;
		default:
			float k = tan((step*i) * 3.1415926535 / 180);
			float d = 2*bRh*bRh - k*k;
			float x = (-k + sqrt(d)) / 2;
			float y = k * x;
			vertices.push_back(VERTEX3D(x, y, 0));
			d = 2*tRh*tRh - k*k;
			x = (-k + sqrt(d)) / 2;
			y = k * x;
			vertices.push_back(VERTEX3D(x, y, h));
		}
	}

		// setting base polygons
	for (int i = 0; i < precission - 1; i++) {
		polygons.push_back(POLY3D(i*2, precission*2 + i*2, precission*2 + i*2 + 2));
		polygons.push_back(POLY3D(precission*2 + i*2 + 2, i*2 + 2, i*2));
	}
	polygons.push_back(POLY3D(precission*2 - 2, precission*4 - 2, precission*2));
	polygons.push_back(POLY3D(precission*2, 0, precission*2 - 2));
		// setting top polygons
	for (int i = 0; i < precission - 1; i++) {
		polygons.push_back(POLY3D(i*2 + 1, precission*2 + i*2 + 1, precission*2 + i*2 + 3));
		polygons.push_back(POLY3D(precission*2 + i*2 + 3, i*2 + 3, i*2 + 1));
	}
	polygons.push_back(POLY3D(precission*2 - 1, precission*4 - 1, precission*2 + 1));
	polygons.push_back(POLY3D(precission*2 + 1, 1, precission*2 - 1));
		// setting side polygons
	for (int i = 0; i < precission - 1; i++) {
		polygons.push_back(POLY3D(i*2, i*2 + 1, i*2 + 2));
		polygons.push_back(POLY3D(i*2 + 1, i*2 + 3, i*2 + 2));
	}
	polygons.push_back(POLY3D(precission*2 - 2, precission*2 - 1, 0));
	polygons.push_back(POLY3D(precission*2 - 1, 1, 0));

	vertices.shrink_to_fit();
	polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

Hole::Hole() : clsMesh(), h(0), bR(0), bRh(0), tR(0), tRh(0), precission(0) {}
Hole::Hole(float height, float bRadius, float bHoleRadius, float tRadius, float tHoleRadius, int prec) : 
	clsMesh(), h(height), bR(bRadius), bRh(bHoleRadius), tR(tRadius), tRh(tHoleRadius), precission(prec) {
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
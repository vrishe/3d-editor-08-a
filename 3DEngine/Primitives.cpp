#include "Primitives.h"
#include <math.h>
#include <Windows.h>

#ifndef PYRAMID_CPP
#define PYRAMID_CPP

/* ----------------------- PYRAMID ------------------------ */

void Pyramid::Triangulate() {
	vertices.clear();
	polygons.clear();

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

		// setting edges


		// setting base polygons
	polygons.push_back(POLY3D(0,1,2));	// 0
	polygons.push_back(POLY3D(0,2,3));	// 1
		// setting top polygons
	polygons.push_back(POLY3D(4,5,6));	// 2
	polygons.push_back(POLY3D(4,6,7));	// 3
		// setting side polygons
	polygons.push_back(POLY3D(0,4,3));	// 4
	polygons.push_back(POLY3D(4,7,3));	// 5
	polygons.push_back(POLY3D(3,7,2));	// 6
	polygons.push_back(POLY3D(7,6,2));	// 7
	polygons.push_back(POLY3D(2,6,1));	// 8
	polygons.push_back(POLY3D(6,5,1));	// 9
	polygons.push_back(POLY3D(1,5,0));	// 10
	polygons.push_back(POLY3D(5,4,0));	// 11

	vertices.shrink_to_fit();
	polygons.shrink_to_fit();
}

/* ---------------------- constructors --------------------- */

Pyramid::Pyramid(COLOR3D c) 
	: clsMesh(c), h(0), bL(0), bW(0), tL(0), tW(0) { }

Pyramid::Pyramid(
		float height, 
		float bLength, 
		float bWidth, 
		float tLength, 
		float tWidth,
		COLOR3D	c
) : clsMesh(c)
{ 
	h  = height;
	bL = bLength;
	bW = bWidth;
	tL = tLength;
	tW = tWidth;
	Triangulate();
}

Pyramid::Pyramid(
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : clsMesh( red, green, blue ) 
{
	h	= 0;
	bL	= 0;
	bW	= 0;
	tL	= 0;
	tW	= 0;
}

Pyramid::Pyramid(
		float height, 
		float bLength, 
		float bWidth, 
		float tLength, 
		float tWidth,
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : clsMesh( red, green, blue )
{
	h  = height;
	bL = bLength;
	bW = bWidth;
	tL = tLength;
	tW = tWidth;
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
	vertices.clear();
	polygons.clear();

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
			double k = tan((step*i) * 3.1415926535 / 180), x;
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			double y = k * x;
			vertices.push_back(VERTEX3D((float)x, (float)y, 0));
			if (step*i < 90 || step*i > 270)
 				x = sqrt(tR*tR / (1 + k*k));
			else
				x = -sqrt(tR*tR / (1 + k*k));
			y = k * x;
			vertices.push_back(VERTEX3D((float)x, (float)y, h));
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

Cone::Cone(COLOR3D c) 
	: clsMesh(c), h(0), bR(0), tR(0), precission(0) { }

Cone::Cone(
		float height, 
		float bRadius, 
		float tRadius, 
		int	  prec,
		COLOR3D	c
) : clsMesh(c)
{ 
	h			= height;
	bR			= bRadius;
	tR			= tRadius;
	precission	= prec;
	Triangulate();
}

Cone::Cone(
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : clsMesh( red, green, blue ) 
{
	h			= 0;
	bR			= 0;
	tR			= 0;
	precission	= 0;
}

Cone::Cone(
		float height, 
		float bRadius, 
		float tRadius, 
		int	  prec,
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : clsMesh( red, green, blue )
{
	h			= height;
	bR			= bRadius;
	tR			= tRadius;
	precission	= prec;
	Triangulate();
}

/* ------------------------ getters ------------------------ */

float Cone::getHeight()		{ return h; }
float Cone::getBRadius()	{ return bR; }
float Cone::getTRadius()	{ return tR; }
int Cone::getPrecission()	{ return precission; }


/* ------------------------ setters ------------------------ */

void Cone::setHeight(float n)	{ h = n; } 
void Cone::setBRadius(float n)	{ bR = n; }
void Cone::setTRadius(float n)	{ tR = n; }
void Cone::setPrecission(int n) { precission = n; }


#endif // CONE_CPP

#ifndef EXCONE_CPP
#define EXCONE_CPP

/* ------------------------ EXCONE ------------------------- */

void ExCone::Triangulate() {
	vertices.clear();
	polygons.clear();

	int step = 360 / precission; // шаг между точками в градусах
		// setting vertices
	if ( secant >= 0 ) {
		vertices.push_back(VERTEX3D(0, 0, 0));
		vertices.push_back(VERTEX3D(0, 0, h));
	}
	else { // нулевая вершина лежит на секущей плоскости т.к. центр конуса "отрезан"
		vertices.push_back(VERTEX3D(0, secant, 0));
		vertices.push_back(VERTEX3D(0, secant, h));
	}
		// secant vertices
	int secBaseLeft = 2, secTopLeft = 2; // кол-во еще недобавленных вершин пересачения плоскости сечения с реберами основания и верхней плоскости конуса
	VERTEX3D bV1, bV2, tV1, tV2; // точки пересечения плоскости сечения с ребрами основания и верхней плоскости конуса
	if ( abs(secant) >= bR ) // плоскость сечения не пересекается/касается основания конуса
		secBaseLeft = 0; // отдельная обработка вершины касания не требуется
	else {
		bV1 = VERTEX3D((float)sqrt(bR*bR - secant * secant), secant, 0);
		bV2 = VERTEX3D((float)-sqrt(bR*bR - secant * secant), secant, 0);
	}
	if ( abs(secant) >= tR ) // аналогично основанию
		secTopLeft = 0;
	else {	
		tV1 = VERTEX3D((float)sqrt(tR*tR - secant * secant), secant, h);
		tV2 = VERTEX3D((float)-sqrt(tR*tR - secant * secant), secant, h);
	}
	for (int i = 0; i < precission; i++) { // нахождение вершин кривых основания и верхней плоскости конуса
		switch (step*i) {
		case 0:
			if ( secant >= 0 ) {
				vertices.push_back(VERTEX3D(bR, 0, 0));
				vertices.push_back(VERTEX3D(tR, 0, h));
			}
			else { // вершина не лежит на оси OX, т.к. центр конуса "отрезан"
				if (secBaseLeft) // проверки для сохранения правильного порядка добавления вершин (аналогично не усеченному конусу)
					vertices.push_back(bV1);
				if (secTopLeft)
					vertices.push_back(tV1);
				if (secBaseLeft)
					vertices.push_back(bV2);
				if (secTopLeft)
					vertices.push_back(tV2);
				secBaseLeft = secTopLeft = 0; // все вершины пересечения добавлены
			}
			break;
		case 90:
			if ( secant > 0 ) {
				switch ( secBaseLeft ) {
				case 2: // вместо одной вершины на оси ОУ будут добавлены две вершины пересечения
					vertices.push_back(bV1);
					if (secTopLeft) { 
						vertices.push_back(tV1);
						vertices.push_back(bV2);
						vertices.push_back(tV2);
					}
					else { // в верхней плоскости конуса дублируем вершину для быстрого построения полигонов
						vertices.push_back(VERTEX3D(0, tR, h));
						vertices.push_back(bV2);
						vertices.push_back(VERTEX3D(0, tR, h));
					}	
					secBaseLeft = secTopLeft = 0; // все вершины пересечения добавлены
					break;
				case 1: // добвление одной вершины на оси ОУ
					if ( secant < bR )
						vertices.push_back(VERTEX3D(0, secant, 0));
					else
						vertices.push_back(VERTEX3D(0, bR, h));
					if ( secant < tR )
						vertices.push_back(VERTEX3D(0, secant, h));
					else
						vertices.push_back(VERTEX3D(0, tR, h));
					break;
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
			/// находим точку пересечения окружности и очередной прямой в плоскости ОУ,
			/// построенной под углом step*i относительно ОХ и проходящей через начало координат
			double k = tan((step*i) * 3.1415926535 / 180), x;
 			if (step*i < 90 || step*i > 270) // находим квадрант где должна быть очередная точка
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			double y = k * x;
			if ( y >= secant ) { // если найденная вершина "отрезана" плоскостью сечения
				if ( secBaseLeft ) {
					if ( x > 0 ) {
						vertices.push_back(bV1);
						secBaseLeft--;
					}
					else {
						vertices.push_back(bV2);
						secBaseLeft--;
					}
				}
			}
			else
				vertices.push_back(VERTEX3D((float)x, (float)y, 0));
			/// аналогично добавляем вершину для верхней плоскости конуса
			if (step*i < 90 || step*i > 270)
 				x = sqrt(tR*tR / (1 + k*k));
			else
				x = -sqrt(tR*tR / (1 + k*k));
			y = k * x;
			if ( y >= secant ) {
				if ( secTopLeft ) {
					if ( x > 0 ) {
						vertices.push_back(tV1);
						secTopLeft--;
					}
					else {
						vertices.push_back(tV2);
						secTopLeft--;
					}
				}
			}
			else
				vertices.push_back(VERTEX3D((float)x, (float)y, h));
		}
	}

		// setting base polygons
	int N = (vertices.size() - 2) / 2, index = ( N == precission / 2 + 1 ? 2 : 1 );
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

ExCone::ExCone(COLOR3D c) 
	: Cone( c ) { secant = max(bR, tR); }

ExCone::ExCone(
		float height, 
		float bRadius, 
		float tRadius,
		float s,
		int	  prec,
		COLOR3D	c
) : Cone( c ) 
{ 
	secant		= s;
	h			= height;
	bR			= bRadius;
	tR			= tRadius;
	precission	= prec;
	Triangulate();
}

ExCone::ExCone(
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : Cone( red, green, blue ) { secant = max(bR, tR); }

ExCone::ExCone(
		float height, 
		float bRadius, 
		float tRadius,
		float s,
		int	  prec,
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : Cone( red, green, blue ) 
{ 
	secant		= s;
	h			= height;
	bR			= bRadius;
	tR			= tRadius;
	precission	= prec;
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
	vertices.clear();
	polygons.clear();

	int step = 360 / precission;
		// setting vertices
	for (int i = 0; i < precission; i++) { // setting outter circle
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
			double k = tan((step*i) * 3.1415926535 / 180), x;
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			double y = k * x;
			vertices.push_back(VERTEX3D((float)x, (float)y, 0));

			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			y = k * x;
			vertices.push_back(VERTEX3D((float)x, (float)y, h));
		}
	}
	for (int i = 0; i < precission; i++) { // setting inner circle
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
			double k = tan((step*i) * 3.1415926535 / 180), x;
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			double y = k * x;
			vertices.push_back(VERTEX3D((float)x, (float)y, 0));
			if (step*i < 90 || step*i > 270)
 				x = sqrt(bR*bR / (1 + k*k));
			else
				x = -sqrt(bR*bR / (1 + k*k));
			y = k * x;
			vertices.push_back(VERTEX3D((float)x, (float)y, h));
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
Hole::Hole(COLOR3D c) 
	: clsMesh(c), h(0), bR(0), bRh(0), tR(0), tRh(0), precission(0) { }

Hole::Hole(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		COLOR3D	c
) : clsMesh(c)
{ 
	h			= height;
	bR			= bRadius;
	bRh			= bHoleRadius;
	tR			= tRadius;
	tRh			= tHoleRadius;
	precission	= prec;
	Triangulate();
}

Hole::Hole(
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : clsMesh( red, green, blue ) 
{
	h			= 0;
	bR			= 0;
	bRh			= 0;
	tR			= 0;
	tRh			= 0;
	precission	= 0;
}

Hole::Hole(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		unsigned char red,
		unsigned char green,
		unsigned char blue
) : clsMesh( red, green, blue )
{
	h			= height;
	bR			= bRadius;
	bRh			= bHoleRadius;
	tR			= tRadius;
	tRh			= tHoleRadius;
	precission	= prec;
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
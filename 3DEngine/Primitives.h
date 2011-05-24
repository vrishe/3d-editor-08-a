#pragma once

#include "Abstractions.h"

#ifndef PYRAMID_H
#define PYRAMID_H

class Pyramid : public clsMesh {
protected:
	float h;	// height
	float bL;	// base length
	float bW;	// base width
	float tL;	// top length
	float tW;	// top width

public:
	Pyramid();
	Pyramid(float height, float bLength, float bWidth, float tLength, float tWidth);

	// functionality
	void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBLength();
	float getBWidth();
	float getTLength();
	float getTWidth();

	// setters
	void setHeight(float n);
	void setBLength(float n);
	void setBWidth(float n);
	void setTLength(float n);
	void setTWidth(float n);
};

#endif // PYRAMID_H

#ifndef CONE_H
#define CONE_H

class Cone : public clsMesh {
protected:
	float h;			// cone height
	float bR;			// base radius
	float tR;			// top radius
	int precission;	// precission of circle approximation in polygons

public:
	Cone();
	Cone(float height, float bRadius, float tRadius, int prec);

	// functionality
	void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	int getPrecission();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setPrecission(int);
};

#endif // CONE_H

#ifndef EXCONE_H
#define EXCONE_H

class ExCone : public Cone {
	/// �����, ���������� ����������, ����������� OXZ � ������������ OY � �. (0, secant, 0)
	/// ������ ���������� ����������� -min(bR,tR) <= secant < max(bR, tR),
	/// ����� ������ ������������
	float secant;

public:
	ExCone();
	ExCone(float height, float bRadius, float tRadius, int prec, float s);

	// functionality
	void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getSecant();

	// setters
	void setSecant(float); 
};

#endif // EXCONE_H

#ifndef HOLE_H
#define HOLE_H

class Hole : public clsMesh {
protected:
	float h;			// cone height
	float bR;			// base radius
	float bRh;			// base hole radius
	float tR;			// top radius
	float tRh;			// top hole radius
	int precission;	// precission of circle approximation in polygons

public:
	Hole();
	Hole(float height, float bRadius, float bRHole, float tRadius, float tRHole, int prec);

	// functionality
	void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	float getBHoleRadius();
	float getTHoleRadius();
	int getPrecission();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setBHoleRadius(float);
	void setTHoleRadius(float);
	void setPrecission(int);
};

#endif // HOLE_H
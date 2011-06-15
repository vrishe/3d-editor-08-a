#pragma once

#include "Abstractions.h"

#ifndef PYRAMID_H
#define PYRAMID_H

class Pyramid : public clsMesh {
protected:
	float h;		// height
	float bL;		// base length
	float bW;		// base width
	float tL;		// top length
	float tW;		// top width
	float shift;    // get your head out by OX

public:
	Pyramid(COLORREF c);
	Pyramid(
		float height, 
		float bLength, 
		float bWidth, 
		float tLength, 
		float tWidth,
		COLORREF	c,
		float sh = 0
	);
	Pyramid(
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);
	Pyramid(
		float height, 
		float bLength, 
		float bWidth, 
		float tLength, 
		float tWidth,
		float sh = 0,
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

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
typedef Pyramid PYRAMID3D, *LPPYRAMID3D;

#endif // PYRAMID_H

#ifndef CONE_H
#define CONE_H

class Cone : public clsMesh {
protected:
	float h;			// cone height
	float bR;			// base radius
	float tR;			// top radius
	int precision;	// precision of circle approximation in polygons

public:
	Cone(COLORREF c, MESH_ID mID = MSH_CONE);
	Cone(
		float height, 
		float bRadius, 
		float tRadius, 
		int	  prec,
		COLORREF	c
	);
	Cone(
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff,
		MESH_ID mID = MSH_CONE
	);
	Cone(
		float height, 
		float bRadius, 
		float tRadius, 
		int	  prec,
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	int getPrecision();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setPrecission(int);
};
typedef Cone CONE3D, *LPCONE3D;

#endif // CONE_H

#ifndef EXCONE_H
#define EXCONE_H

class ExCone : public Cone {
	/// конус, отсеченный плоскостью, паралельной OXZ и пересекающей OY в т. (0, secant, 0)
	/// должно выполнятся неравенство -min(bR,tR) <= secant <= max(bR, tR),
	/// иначе фигура неопределена
	float secant;

public:
	ExCone(COLORREF c);
	ExCone(
		float height, 
		float bRadius, 
		float tRadius,
		float s,
		int	  prec,
		COLORREF	c
	);
	ExCone(
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);
	ExCone(
		float height, 
		float bRadius, 
		float tRadius, 
		float s,
		int	  prec,
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getSecant();

	// setters
	void setSecant(float); 
};
typedef ExCone EXCONE3D, *LPEXCONE3D;

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
	int precision;	// precision of circle approximation in polygons

public:
	Hole(COLORREF c);
	Hole(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		COLORREF	c
	);
	Hole(
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);
	Hole(
		float height, 
		float bRadius, 
		float bHoleRadius,
		float tRadius,
		float tHoleRadius,
		int	  prec,
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
	);

	// functionality
	void Triangulate(); // generate vertices, edges and polygons lists

	// getters
	float getHeight();
	float getBRadius();
	float getTRadius();
	float getBHoleRadius();
	float getTHoleRadius();
	int getPrecision();

	// setters
	void setHeight(float); 
	void setBRadius(float);
	void setTRadius(float);
	void setBHoleRadius(float);
	void setTHoleRadius(float);
	void setPrecission(int);
};
typedef Hole HOLE3D, *LPHOLE3D;

#endif // HOLE_H

class clsSphere : public clsMesh {
protected:
	float			radius;
	float			cropMult;
	float			angleFrom;
	float			angleTo;
	unsigned int	precision;

	void			Triangulate();
public:
	clsSphere(
		float			Radius,
		float			Crop,
		float			From,
		float			To,
		unsigned int	Prec,
		COLORREF		c
	);
	clsSphere(
		float			Radius, 
		float			Crop,
		float			From,
		float			To,
		unsigned int	Prec,
		unsigned char	red,
		unsigned char	green,
		unsigned char	blue
	);

	void	setRadius(float Radius);
	void	setCrop(float Plane);
	void	setPrecision(int Prec);

	float	getRadius();
	float	getCrop();
	int		getPrecision();
};
typedef clsSphere SPHERE3D, *LPSPHERE3D;
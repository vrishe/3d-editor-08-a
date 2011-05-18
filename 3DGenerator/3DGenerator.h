#include <vector>
using namespace std;

#ifndef STRUCTS_H
#define STRUCTS_H

struct D3DVertex {
	float x;
	float y;
	float z;
	float rhw;
};

struct Vertex : public D3DVertex {
	Vertex();
	Vertex(float, float, float);
	friend bool operator == (Vertex, Vertex);
};

struct Polygon_ {
	int first;
	int second;
	int third;

	Polygon_();
	Polygon_(int, int, int);
	friend bool operator == (Polygon_, Polygon_);
};

#endif // STRUCTS_H

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

class Primitive {
protected:
	int vCount; // number of vertexes
	int pCount; // number of polygons

	vector <Vertex> vertexes;  // list of vertexes
	vector <Polygon_> polygons; // list of polygons

	int findVertex(Vertex);		// returns a vertex position
	int findPolygon(Polygon_);	// returns a Polygon_ position

	/// delete ver/edge/polys that can't be a part of the object no more
	int dropUnusedVertexes();		// delete vertexes if no polygons use them	
	int dropRedundantPolygons();	// delete polygons if there are null vertexes in it

public:
	Primitive();
	Primitive(int v, vector <Vertex> vs, /*int e, vector <Edge> es,*/ int p, vector <Polygon_> ps);

	// functionality
	void dropRedundant();
	virtual void Triangulate(); // generate vertexes, edges and polygons lists
	void FillBuff(D3DVertex* vs);
	// add some modifying functionality here

	// getters
	int getVCount();
	int getPCount();
	vector <Vertex> getVertexes();
	vector <Polygon_> getPolygons();

	// setters
	void addVertex(Vertex);
	void addListOfVertexes(vector <Vertex>);
	bool delVertex(Vertex);
	bool delVertex(int);
	int delListOfVertexes(vector <Vertex>);  // returns 0 if everything's deleted, -n otherwise, where n - number of undeleted elements

	void addPolygon(Polygon_);
	void addListOfPolygons(vector <Polygon_>);
	bool delPolygon(Polygon_);
	bool delPolygon(int);
	int delListOfPolygons(vector <Polygon_>);  // returns 0 if everything's deleted, -n otherwise, where n - number of undeleted elements
};

#endif // PRIMITIVE_H

#ifndef PYRAMID_H
#define PYRAMID_H

class Pyramid : public Primitive {
	float h;	// height
	float bL;	// base length
	float bW;	// base width
	float tL;	// top length
	float tW;	// top width

public:
	Pyramid();
	Pyramid(float height, float bLength, float bWidth, float tLength, float tWidth);

	// functionality
	void Triangulate(); // generate vertexes, edges and polygons lists

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

class Cone : public Primitive {
	float h;			// cone height
	float bR;			// base radius
	float tR;			// top radius
	int precission;	// precission of circle approximation in polygons

public:
	Cone();
	Cone(float height, float bRadius, float tRadius, int prec);

	// functionality
	void Triangulate(); // generate vertexes, edges and polygons lists

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

#ifndef HOLE_H
#define HOLE_H

class Hole : public Primitive {
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
	void Triangulate(); // generate vertexes, edges and polygons lists

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
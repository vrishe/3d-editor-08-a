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
	void Triangulate();
};
typedef clsTankBody TANKBODY, *LPTANKBODY;
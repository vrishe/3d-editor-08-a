#pragma once

#include "Primitives.h"

class clsTankBody : public clsMesh {
private:
	float Length;

	float Width;
	float FrontWidth;	// 0.9f
	float BaseWidth;	// 0.563f

	float Height;
	float bottomSlopeHeight; // 0.23
	float backTopHeight;	 // 0.636

	// Angles of armor slopes
	float frontTop;
	float frontBottom;
	float top;
	float backTop;
	float backBottom;
	float side;

public:
	clsTankBody(
			float bL,
			float bW,
			float bH,
			float fbA
		);
	void Triangulate();
};
typedef clsTankBody TANKBODY, *LPTANKBODY;
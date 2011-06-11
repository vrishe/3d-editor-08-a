#pragma once

#include "Abstractions.h"

#define DARK_SIDE 0.1
// ============================================================================
// clsDifLight class

class clsDifLight : public clsObject {
	float	power;		// must be 0 <= power
	COLORREF color;

public:
	clsDifLight( 
		unsigned char red		= 255,
		unsigned char green		= 255,
		unsigned char blue		= 255
		);
	clsDifLight(float p);

	float getPower();
	COLORREF getColor();

	void setPower(float);
	void setColor(
			unsigned char r, 
			unsigned char g, 
			unsigned char b
		);
	void setColor(COLORREF c);
};
typedef clsDifLight DIFLIGHT3D, *LPDIFLIGHT3D;


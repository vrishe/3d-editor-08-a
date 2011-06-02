#pragma once

#include "Abstractions.h"

// ============================================================================
// clsDifLight class

class clsDifLight : public clsObject {
	float	power;		// must be 0 <= power <= 1
	COLOR3D color;

public:
	clsDifLight( 
		unsigned char red	= 255,
		unsigned char green	= 255,
		unsigned char blue	= 255
		);
	clsDifLight(float p);

	float getPower();
	COLOR3D getColor();

	void setPower(float);
	void setColor(unsigned char r, unsigned char g, unsigned char b);
};
typedef clsDifLight DIFLIGHT3D, *LPDIFLIGHT3D;


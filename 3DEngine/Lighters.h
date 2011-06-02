#pragma once

#include "Abstractions.h"

// ============================================================================
// clsDifLight class

class clsDifLight : public clsObject {
	float power;		// must be 0 <= power <= 1

public:
	clsDifLight();
	clsDifLight(float p);

	float getPower();

	void setPower(float);
};
typedef clsDifLight DIFLIGHT3D, *LPDIFLIGHT3D;


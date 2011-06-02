
#include "Lighters.h"

// ============================================================================
// Implementation of clsDifLight class:
clsDifLight::clsDifLight(unsigned char red,	unsigned char green, unsigned char blue) : 
	clsObject(CLS_LIGHT), power(0.2f) {
		color.Red	= red;
		color.Green = green;
		color.Blue	= blue;
}

clsDifLight::clsDifLight(float p) : clsObject(CLS_LIGHT), power(p) {}

float clsDifLight::getPower()	{ return power; }
COLOR3D clsDifLight::getColor() { return color; }

void clsDifLight::setPower(float p)	{ power = (p > 1 ? 1 : (p < 0 ? 0 : p)); }
void clsDifLight::setColor(unsigned char r, unsigned char g, unsigned char b) {
	color.Red	= r;
	color.Green = g;
	color.Blue	= b;
}
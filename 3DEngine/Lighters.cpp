
#include "Lighters.h"

// ============================================================================
// Implementation of clsDifLight class:
clsDifLight::clsDifLight(unsigned char,	unsigned char, unsigned char) : 
	clsObject(CLS_LIGHT), power(0.2f) {}

clsDifLight::clsDifLight(float p) : clsObject(CLS_LIGHT), power(p) {}

float clsDifLight::getPower()	{ return power; }
COLOR3D clsDifLight::getColor() { return color; }

void clsDifLight::setPower(float p)		{ power = p; }
void clsDifLight::setColor(unsigned char r, unsigned char g, unsigned char b) {
	color.Red	= r;
	color.Green = g;
	color.Blue	= b;
}
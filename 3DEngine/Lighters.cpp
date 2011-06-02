
#include "Lighters.h"

// ============================================================================
// Implementation of clsDifLight class:
clsDifLight::clsDifLight() : clsObject(CLS_LIGHT), power(0.2f) {}

clsDifLight::clsDifLight(float p) : clsObject(CLS_LIGHT), power(p) {}

float clsDifLight::getPower() { return power; }

void clsDifLight::setPower(float p) { power = p; }
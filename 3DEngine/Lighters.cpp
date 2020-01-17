
#include "Lighters.h"

// ============================================================================
// Implementation of clsDifLight class:
clsDifLight::clsDifLight(
  unsigned char red,  
  unsigned char green, 
  unsigned char blue
) : clsObject(CLS_LIGHT), power(0.2f) { setColor(red, green, blue); }

clsDifLight::clsDifLight(float p) : clsObject(CLS_LIGHT), power(p) {}

float clsDifLight::getPower() { return power; }
COLORREF clsDifLight::getColor() { return color; }

void clsDifLight::setPower(float p) { power = (p < 0 ? 0 : p); }
void clsDifLight::setColor(
      unsigned char r, 
      unsigned char g, 
      unsigned char b
) {
  color = RGB(r, g, b);
}
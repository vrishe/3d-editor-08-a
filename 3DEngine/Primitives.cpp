#include "Primitives.h"
#include <math.h>
#include <Windows.h>

#ifndef PYRAMID_CPP
#define PYRAMID_CPP

/* ----------------------- PYRAMID ------------------------ */

void Pyramid::Triangulate() {
  vertices.clear();
  edges.clear();
  polygons.clear();

    // setting base vertices
  vertices.push_back(VECTOR3D(-bL/2,-bW/2, 0)); // 0
  vertices.push_back(VECTOR3D(-bL/2,bW/2, 0));  // 1
  vertices.push_back(VECTOR3D(bL/2,bW/2, 0));   // 2
  vertices.push_back(VECTOR3D(bL/2,-bW/2, 0));  // 3
    // setting top vertices
  vertices.push_back(VECTOR3D(-tL/2 + shift,-tW/2, h)); // 4
  vertices.push_back(VECTOR3D(-tL/2 + shift,tW/2, h));  // 5
  vertices.push_back(VECTOR3D(tL/2 + shift,tW/2, h));   // 6
  vertices.push_back(VECTOR3D(tL/2 + shift,-tW/2, h));  // 7

    // setting edges
  // setting base edges
  edges.push_back(EDGE3D(0,1)); 
  edges.push_back(EDGE3D(1,2));
  edges.push_back(EDGE3D(2,3));
  edges.push_back(EDGE3D(3,0));
  edges.push_back(EDGE3D(0,2));
    // setting top edges
  edges.push_back(EDGE3D(4,5)); 
  edges.push_back(EDGE3D(5,6));
  edges.push_back(EDGE3D(6,7));
  edges.push_back(EDGE3D(7,4));
  edges.push_back(EDGE3D(4,6)); 
    // setting side edges
  edges.push_back(EDGE3D(0,4));
  edges.push_back(EDGE3D(0,5));
  edges.push_back(EDGE3D(1,5));
  edges.push_back(EDGE3D(1,6));
  edges.push_back(EDGE3D(2,6)); 
  edges.push_back(EDGE3D(2,7)); 
  edges.push_back(EDGE3D(3,7)); 
  edges.push_back(EDGE3D(3,4));

    // setting base polygons
  polygons.push_back(POLY3D(0,1,2)); // 0
  polygons.push_back(POLY3D(0,2,3)); // 1
    // setting top polygons
  polygons.push_back(POLY3D(5,4,6)); // 2
  polygons.push_back(POLY3D(6,4,7)); // 3
    // setting side polygons
  polygons.push_back(POLY3D(4,0,3));  // 4
  polygons.push_back(POLY3D(7,4,3));  // 5
  polygons.push_back(POLY3D(7,3,2));  // 6
  polygons.push_back(POLY3D(6,7,2));  // 7
  polygons.push_back(POLY3D(6,2,1));  // 8
  polygons.push_back(POLY3D(5,6,1));  // 9
  polygons.push_back(POLY3D(5,1,0));  // 10
  polygons.push_back(POLY3D(4,5,0));  // 11

  flushVertices();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}

/* ---------------------- constructors --------------------- */

Pyramid::Pyramid(COLORREF c) 
  : clsMesh(MSH_PYRAMID, c), h(0), bL(0), bW(0), tL(0), tW(0), shift(0) { }

Pyramid::Pyramid(
    float   height, 
    float   bLength, 
    float   bWidth, 
    float   tLength, 
    float   tWidth,
    COLORREF  c,
    float   sh
) : clsMesh(MSH_PYRAMID, c)
{ 
  h  = height;
  bL = bLength;
  bW = bWidth;
  tL = tLength;
  tW = tWidth;
  shift = sh;
  Triangulate();
}

Pyramid::Pyramid(
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : clsMesh(MSH_PYRAMID, red, green, blue ) 
{
  h = 0;
  bL  = 0;
  bW  = 0;
  tL  = 0;
  tW  = 0;
  shift = 0;
}

Pyramid::Pyramid(
    float height, 
    float bLength, 
    float bWidth, 
    float tLength, 
    float tWidth,
    float sh,
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : clsMesh(MSH_PYRAMID, red, green, blue )
{
  h  = height;
  bL = bLength;
  bW = bWidth;
  tL = tLength;
  tW = tWidth;
  shift = sh;
  Triangulate();
}


/* ------------------------ getters ------------------------ */

float Pyramid::getHeight() { return h; }
float Pyramid::getBLength() { return bL; }
float Pyramid::getBWidth() { return bW; }
float Pyramid::getTLength() {return tL; }
float Pyramid::getTWidth() {return tW; }


/* ------------------------ setters ------------------------ */

void Pyramid::setHeight(float n) { h = n; }
void Pyramid::setBLength(float n) { bL = n; }
void Pyramid::setBWidth(float n) { bW = n; }
void Pyramid::setTLength(float n) { tL = n; }
void Pyramid::setTWidth(float n) {tW = n; }


#endif // PYRAMID_CPP

#ifndef CONE_CPP
#define CONE_CPP

/* ----------------------- CONE ------------------------ */

void Cone::Triangulate() {
  vertices.clear();
  edges.clear();
  polygons.clear();

  int step = 360 / precision;
    // setting vertices
  vertices.push_back(VECTOR3D(0, 0, 0));
  vertices.push_back(VECTOR3D(0, 0, h));
  for (int i = 0; i < precision; i++) {
    switch (step*i) {
    case 0:
      vertices.push_back(VECTOR3D(bR, 0, 0));
      vertices.push_back(VECTOR3D(tR, 0, h));
      break;
    case 90:
      vertices.push_back(VECTOR3D(0, bR, 0));
      vertices.push_back(VECTOR3D(0, tR, h));
      break;
    case 180:
      vertices.push_back(VECTOR3D(-bR, 0, 0));
      vertices.push_back(VECTOR3D(-tR, 0, h));
      break;
    case 270:
      vertices.push_back(VECTOR3D(0, -bR, 0));
      vertices.push_back(VECTOR3D(0, -tR, h));
      break;
    default:
      float k = (float)tan((step*i) * 3.1415926535 / 180), x;
      if (step*i < 90 || step*i > 270)
        x = sqrt(bR*bR / (1 + k*k));
      else
        x = -sqrt(bR*bR / (1 + k*k));
      float y = k * x;
      vertices.push_back(VECTOR3D(x, y, 0));
      if (step*i < 90 || step*i > 270)
        x = sqrt(tR*tR / (1 + k*k));
      else
        x = -sqrt(tR*tR / (1 + k*k));
      y = k * x;
      vertices.push_back(VECTOR3D(x, y, h));
    }
  }

    // setting base edges
  for (int i = 1; i < precision; i++) {
    edges.push_back(EDGE3D(0, i*2));
    edges.push_back(EDGE3D(i*2, i*2 + 2));
  }
  edges.push_back(EDGE3D(0, precision*2));
  edges.push_back(EDGE3D(precision*2, 2));
    // setting top edges
  for (int i = 1; i < precision; i++) {
    edges.push_back(EDGE3D(1, i*2 + 1));
    edges.push_back(EDGE3D(i*2 + 1, i*2 + 3));
  }
  edges.push_back(EDGE3D(1, precision*2 + 1));
  edges.push_back(EDGE3D(precision*2 + 1, 3));
    // setting side edges
  for (int i = 1; i < precision; i++) {
    edges.push_back(EDGE3D(i*2, i*2 + 1));
    edges.push_back(EDGE3D(i*2 + 1, i*2 + 2));
  }
  edges.push_back(EDGE3D(precision*2, precision*2 + 1));
  edges.push_back(EDGE3D(precision*2 + 1, 2));

    // setting base polygons
  for (int i = 1; i < precision; i++)
    polygons.push_back(POLY3D(i*2, 0, i*2 + 2));
  polygons.push_back(POLY3D(precision*2, 0, 2));
    // setting top polygons
  for (int i = 1; i < precision; i++)
    polygons.push_back(POLY3D(1, i*2 + 1, i*2 + 3));
  polygons.push_back(POLY3D(1, precision*2 + 1, 3));
    // setting side polygons
  for (int i = 1; i < precision; i++) {
    polygons.push_back(POLY3D(i*2 + 1, i*2, i*2 + 2));
    polygons.push_back(POLY3D(i*2 + 1, i*2 + 2, i*2 + 3));
  }
  polygons.push_back(POLY3D(precision*2 + 1, precision*2, 2));
  polygons.push_back(POLY3D(precision*2 + 1, 2, 3));

  flushVertices();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

Cone::Cone(COLORREF c, MESH_ID mID) 
  : clsMesh(mID, c), h(0), bR(0), tR(0), precision(24) { }

Cone::Cone(
    float height, 
    float bRadius, 
    float tRadius, 
    int   prec,
    COLORREF  c
) : clsMesh(MSH_CONE, c)
{ 
  h     = height;
  bR      = bRadius;
  tR      = tRadius;
  precision = prec;
  Triangulate();
}

Cone::Cone(
    unsigned char red,
    unsigned char green,
    unsigned char blue,
    MESH_ID mID
) : clsMesh( mID, red, green, blue ) 
{
  h     = 0;
  bR      = 0;
  tR      = 0;
  precision = 24;
}

Cone::Cone(
    float height, 
    float bRadius, 
    float tRadius, 
    int   prec,
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : clsMesh( MSH_CONE, red, green, blue )
{
  h     = height;
  bR      = bRadius;
  tR      = tRadius;
  precision = prec;
  Triangulate();
}

/* ------------------------ getters ------------------------ */

float Cone::getHeight()   { return h; }
float Cone::getBRadius()  { return bR; }
float Cone::getTRadius()  { return tR; }
int Cone::getPrecision()  { return precision; }


/* ------------------------ setters ------------------------ */

void Cone::setHeight(float n) { h = n; } 
void Cone::setBRadius(float n)  { bR = n; }
void Cone::setTRadius(float n)  { tR = n; }
void Cone::setPrecission(int n) { precision = n; }


#endif // CONE_CPP

#ifndef EXCONE_CPP
#define EXCONE_CPP

/* ------------------------ EXCONE ------------------------- */

void ExCone::Triangulate() {
  vertices.clear();
  edges.clear();
  polygons.clear();

  int step = 360 / precision; // ��� ����� ������� � ��������
    // setting vertices
  if ( secant >= 0 ) {
    vertices.push_back(VECTOR3D(0, 0, 0));
    vertices.push_back(VECTOR3D(0, 0, h));
  }
  else { // ������� ������� ����� �� ������� ��������� �.�. ����� ������ "�������"
    vertices.push_back(VECTOR3D(0, secant, 0));
    vertices.push_back(VECTOR3D(0, secant, h));
  }
    // secant vertices
  int secBaseLeft = 2, secTopLeft = 2; // ���-�� ��� ������������� ������ ����������� ��������� ������� � �������� ��������� � ������� ��������� ������
  VECTOR3D bV1, bV2, tV1, tV2; // ����� ����������� ��������� ������� � ������� ��������� � ������� ��������� ������
  if ( abs(secant) >= bR ) // ��������� ������� �� ������������/�������� ��������� ������
    secBaseLeft = 0; // ��������� ��������� ������� ������� �� ���������
  else {
    bV1 = VECTOR3D((float)sqrt(bR*bR - secant * secant), secant, 0);
    bV2 = VECTOR3D((float)-sqrt(bR*bR - secant * secant), secant, 0);
  }
  if ( abs(secant) >= tR ) // ���������� ���������
    secTopLeft = 0;
  else {  
    tV1 = VECTOR3D((float)sqrt(tR*tR - secant * secant), secant, h);
    tV2 = VECTOR3D((float)-sqrt(tR*tR - secant * secant), secant, h);
  }
  for (int i = 0; i < precision; i++) { // ���������� ������ ������ ��������� � ������� ��������� ������
    switch (step*i) {
    case 0:
      if ( secant >= 0 ) {
        vertices.push_back(VECTOR3D(bR, 0, 0));
        vertices.push_back(VECTOR3D(tR, 0, h));
      }
      else { // ������� �� ����� �� ��� OX, �.�. ����� ������ "�������"
        if (secBaseLeft) // �������� ��� ���������� ����������� ������� ���������� ������ (���������� �� ���������� ������)
          vertices.push_back(bV1);
        if (secTopLeft)
          vertices.push_back(tV1);
        if (secBaseLeft)
          vertices.push_back(bV2);
        if (secTopLeft)
          vertices.push_back(tV2);
        secBaseLeft = secTopLeft = 0; // ��� ������� ����������� ���������
      }
      break;
    case 90:
      if ( secant > 0 ) {
        switch ( secBaseLeft ) {
        case 2: // ������ ����� ������� �� ��� �� ����� ��������� ��� ������� �����������
          vertices.push_back(bV1);
          if (secTopLeft) { 
            vertices.push_back(tV1);
            vertices.push_back(bV2);
            vertices.push_back(tV2);
          }
          else { // � ������� ��������� ������ ��������� ������� ��� �������� ���������� ���������
            vertices.push_back(VECTOR3D(0, tR, h));
            vertices.push_back(bV2);
            vertices.push_back(VECTOR3D(0, tR, h));
          } 
          secBaseLeft = secTopLeft = 0; // ��� ������� ����������� ���������
          break;
        case 1: // ��������� ����� ������� �� ��� ��
          if ( secant < bR )
            vertices.push_back(VECTOR3D(0, secant, 0));
          else
            vertices.push_back(VECTOR3D(0, bR, h));
          if ( secant < tR )
            vertices.push_back(VECTOR3D(0, secant, h));
          else
            vertices.push_back(VECTOR3D(0, tR, h));
          break;
        //default:
        //  vertices.push_back(VECTOR3D(0, bR, h));
        //  vertices.push_back(VECTOR3D(0, tR, h));
        }
      }
      break;
    case 180:
      if ( secant >= 0 ) {
        vertices.push_back(VECTOR3D(-bR, 0, 0));
        vertices.push_back(VECTOR3D(-tR, 0, h));
      }
      break;
    case 270:
      vertices.push_back(VECTOR3D(0, -bR, 0));
      vertices.push_back(VECTOR3D(0, -tR, h));
      break;
    default:
      /// ������� ����� ����������� ���������� � ��������� ������ � ��������� ��,
      /// ����������� ��� ����� step*i ������������ �� � ���������� ����� ������ ���������
      float k = (float)tan((step*i) * 3.1415926535 / 180), x;
      if (step*i < 90 || step*i > 270) // ������� �������� ��� ������ ���� ��������� �����
        x = sqrt(bR*bR / (1 + k*k));
      else
        x = -sqrt(bR*bR / (1 + k*k));
      float y = k * x;
      bool pushed = false; // ���� �� ������� ��������� � ������
      if ( y >= secant ) { // ���� ��������� ������� "��������" ���������� �������
        if ( secBaseLeft ) {
          if ( x > 0 ) {
            vertices.push_back(bV1);
            secBaseLeft = 1;
          }
          else {
            vertices.push_back(bV2);
            secBaseLeft = 0;
          }
          pushed = true;
        }
      }
      else {
        vertices.push_back(VECTOR3D(x, y, 0));
        pushed = true;
      }
      /// ���������� ��������� ������� ��� ������� ��������� ������
      if (step*i < 90 || step*i > 270)
        x = sqrt(tR*tR / (1 + k*k));
      else
        x = -sqrt(tR*tR / (1 + k*k));
      y = k * x;
      if ( y >= secant ) {
        if ( secTopLeft ) {
          if ( !pushed )
            if ( x > 0 )
              vertices.push_back(bV1);
            else
              vertices.push_back(bV2);
          if ( x > 0 ) {
            vertices.push_back(tV1);
            secTopLeft = 1;
          }
          else {
            vertices.push_back(tV2);
            secTopLeft = 0;
          }
        }
        else if ( pushed ) {
          if ( x > 0 )
            vertices.push_back(tV1);
          else
            vertices.push_back(tV2);
          pushed = false;
        }
      }
      else {
        if ( !pushed )
          if ( x > 0 )
            vertices.push_back(bV1);
          else
            vertices.push_back(bV2);
        vertices.push_back(VECTOR3D(x, y, h));
      }

    }
  }

    // setting base edges
  int N = (vertices.size() - 2) / 2;
  for (int i = 1; i < N; i++) {
    edges.push_back(EDGE3D(0, i*2));
    edges.push_back(EDGE3D(i*2, i*2 + 2));
  }
  edges.push_back(EDGE3D(0, N*2));
  edges.push_back(EDGE3D(N*2, 2));
    // setting top edges
  for (int i = 1; i < N; i++) {
    edges.push_back(EDGE3D(1, i*2 + 1));
    edges.push_back(EDGE3D(i*2 + 1, i*2 + 3));
  }
  edges.push_back(EDGE3D(1, N*2 + 1));
  edges.push_back(EDGE3D(N*2 + 1, 3));
    // setting side edges
  for (int i = 1; i < N; i++) {
    edges.push_back(EDGE3D(i*2, i*2 + 1));
    edges.push_back(EDGE3D(i*2 + 1, i*2 + 2));
  }
  edges.push_back(EDGE3D(N*2, N*2 + 1));
  edges.push_back(EDGE3D(N*2 + 1, 2));

    // setting base polygons
  N = (vertices.size() - 2) / 2; int index = ( N == precision / 2 + 1 ? 2 : 1 );
  for (int i = index; i < N; i++)
    polygons.push_back(POLY3D(i*2, 0, i*2 + 2));
  polygons.push_back(POLY3D(N*2, 0, 2));
    // setting top polygons
  for (int i = index; i < N; i++)
    polygons.push_back(POLY3D(1, i*2 + 1, i*2 + 3));
  polygons.push_back(POLY3D(1, N*2 + 1, 3));
    // setting side polygons
  for (int i = index; i < N; i++) {
    polygons.push_back(POLY3D(i*2 + 1, i*2, i*2 + 2));
    polygons.push_back(POLY3D(i*2 + 1, i*2 + 2, i*2 + 3));
  }
  polygons.push_back(POLY3D(N*2 + 1, N*2, 2));
  polygons.push_back(POLY3D(N*2 + 1, 2, 3));

  flushVertices();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */

ExCone::ExCone(COLORREF c) 
  : Cone( c, MSH_EXCONE ) { secant = max(bR, tR); }

ExCone::ExCone(
    float height, 
    float bRadius, 
    float tRadius,
    float s,
    int   prec,
    COLORREF  c
) : Cone( c, MSH_EXCONE ) 
{ 
  secant    = s;
  h     = height;
  bR      = bRadius;
  tR      = tRadius;
  precision = prec;
  Triangulate();
}

ExCone::ExCone(
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : Cone( red, green, blue, MSH_EXCONE ) { secant = max(bR, tR); }

ExCone::ExCone(
    float height, 
    float bRadius, 
    float tRadius,
    float s,
    int   prec,
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : Cone( red, green, blue, MSH_EXCONE ) 
{ 
  secant    = s;
  h     = height;
  bR      = bRadius;
  tR      = tRadius;
  precision = prec;
  Triangulate();
}


/* ------------------------- getters ----------------------- */

float ExCone::getSecant() { return secant; }


/* ------------------------- setters ----------------------- */

void ExCone::setSecant(float n) { secant = n; }
 

#endif // EXCONE_CPP

#ifndef HOLE_CPP
#define HOLE_CPP

/* ----------------------- HOLE ------------------------ */

void Hole::Triangulate() {
  vertices.clear();
  edges.clear();
  polygons.clear();

  int step = 360 / precision;
    // setting vertices
  for (int i = 0; i < precision; i++) { // setting outter circle
    switch (step*i) {
    case 0:
      vertices.push_back(VECTOR3D(bR, 0, 0));
      vertices.push_back(VECTOR3D(tR, 0, h));
      break;
    case 90:
      vertices.push_back(VECTOR3D(0, bR, 0));
      vertices.push_back(VECTOR3D(0, tR, h));
      break;
    case 180:
      vertices.push_back(VECTOR3D(-bR, 0, 0));
      vertices.push_back(VECTOR3D(-tR, 0, h));
      break;
    case 270:
      vertices.push_back(VECTOR3D(0, -bR, 0));
      vertices.push_back(VECTOR3D(0, -tR, h));
      break;
    default:
      float k = tan((float)((step*i) * M_PI / 180)), x;
      if (step*i < 90 || step*i > 270)
        x = sqrt(bR*bR / (1 + k*k));
      else
        x = -sqrt(bR*bR / (1 + k*k));
      float y = k * x;
      vertices.push_back(VECTOR3D(x, y, 0));

      if (step*i < 90 || step*i > 270)
        x = sqrt(tR*tR / (1 + k*k));
      else
        x = -sqrt(tR*tR / (1 + k*k));
      y = k * x;
      vertices.push_back(VECTOR3D(x, y, h));
    }
  }
  for (int i = 0; i < precision; i++) { // setting inner circle
    switch (step*i) {
    case 0:
      vertices.push_back(VECTOR3D(bRh, 0, 0));
      vertices.push_back(VECTOR3D(tRh, 0, h));
      break;
    case 90:
      vertices.push_back(VECTOR3D(0, bRh, 0));
      vertices.push_back(VECTOR3D(0, tRh, h));
      break;
    case 180:
      vertices.push_back(VECTOR3D(-bRh, 0, 0));
      vertices.push_back(VECTOR3D(-tRh, 0, h));
      break;
    case 270:
      vertices.push_back(VECTOR3D(0, -bRh, 0));
      vertices.push_back(VECTOR3D(0, -tRh, h));
      break;
    default:
      float k = tan((float)((step*i) * M_PI / 180)), x;
      if (step*i < 90 || step*i > 270)
        x = sqrt(bRh*bRh / (1 + k*k));
      else
        x = -sqrt(bRh*bRh / (1 + k*k));
      float y = k * x;
      vertices.push_back(VECTOR3D(x, y, 0));
      if (step*i < 90 || step*i > 270)
        x = sqrt(tRh*tRh / (1 + k*k));
      else
        x = -sqrt(tRh*tRh / (1 + k*k));
      y = k * x;
      vertices.push_back(VECTOR3D(x, y, h));
    }
  }

    // setting base edges
  for (int i = 0; i < precision - 1; i++) {
    edges.push_back(EDGE3D(i*2, precision*2 + i*2));
    edges.push_back(EDGE3D(i*2, precision*2 + i*2 + 2));
    edges.push_back(EDGE3D(i*2, i*2 + 2));
    edges.push_back(EDGE3D(precision*2 + i*2, precision*2 + i*2 + 2));  
  }
  edges.push_back(EDGE3D(precision*2 - 2, precision*4 - 2));
  edges.push_back(EDGE3D(precision*2 - 2, precision*2));
  edges.push_back(EDGE3D(precision*2 - 2, 0));
  edges.push_back(EDGE3D(precision*4 - 2, precision*2));
    // setting top edges
  for (int i = 0; i < precision - 1; i++) {
    edges.push_back(EDGE3D(i*2 + 1, precision*2 + i*2 + 1));
    edges.push_back(EDGE3D(i*2 + 1, precision*2 + i*2 + 3));
    edges.push_back(EDGE3D(i*2 + 1, i*2 + 3));
    edges.push_back(EDGE3D(precision*2 + i*2 + 1, precision*2 + i*2 + 3));
  }
  edges.push_back(EDGE3D(precision*2 - 1, precision*4 - 1));
  edges.push_back(EDGE3D(precision*2 - 1, precision*2 + 1));
  edges.push_back(EDGE3D(precision*2 - 1, 1));
  edges.push_back(EDGE3D(precision*4 - 1, precision*2 + 1));
    // setting side edges
  for (int i = 0; i < precision - 1; i++) { // outter circle
    edges.push_back(EDGE3D(i*2, i*2 + 1));
    edges.push_back(EDGE3D(i*2 + 1, i*2 + 2));
  }
  edges.push_back(EDGE3D(precision*2 - 2, precision*2 - 1));
  edges.push_back(EDGE3D(precision*2 - 1, 0));
  for (int i = 0; i < precision - 1; i++) { // inner circle
    edges.push_back(EDGE3D(precision*2 + i*2, precision*2 + i*2 + 1));
    edges.push_back(EDGE3D(precision*2 + i*2 + 1, precision*2 + i*2 + 2));
  }
  edges.push_back(EDGE3D(precision*4 - 2, precision*4 - 1));
  edges.push_back(EDGE3D(precision*4 - 1, precision*2));

    // setting base polygons
  for (int i = 0, j = 0; i < precision - 1; i++, j += 2) {
    polygons.push_back(POLY3D(i*2, precision*2 + i*2, precision*2 + i*2 + 2));
    polygons.push_back(POLY3D(precision*2 + i*2 + 2, i*2 + 2, i*2));
  }
  polygons.push_back(POLY3D(precision*2 - 2, precision*4 - 2, precision*2));
  polygons.push_back(POLY3D(precision*2, 0, precision*2 - 2));
    // setting top polygons
  for (int i = 0, j = precision*2; i < precision - 1; i++, j += 2) {
    polygons.push_back(POLY3D(precision*2 + i*2 + 1, i*2 + 1, precision*2 + i*2 + 3));
    polygons.push_back(POLY3D(i*2 + 3, precision*2 + i*2 + 3, i*2 + 1));
  }
  polygons.push_back(POLY3D(precision*4 - 1, precision*2 - 1, precision*2 + 1));
  polygons.push_back(POLY3D(1, precision*2 + 1, precision*2 - 1));
    // setting side polygons
  for (int i = 0, j = precision*4; i < precision - 1; i++, j+= 2) { // outter circle
    polygons.push_back(POLY3D(i*2 + 1, i*2, i*2 + 2));
    polygons.push_back(POLY3D(i*2 + 3, i*2 + 1, i*2 + 2));
  }
  polygons.push_back(POLY3D(precision*2 - 1, precision*2 - 2, 0));
  polygons.push_back(POLY3D(1, precision*2 - 1, 0));
  for (int i = 0, j = precision*6; i < precision - 1; i++, j += 2) { // inner circle
    polygons.push_back(POLY3D(precision*2 + i*2, precision*2 + i*2 + 1, precision*2 + i*2 + 2));
    polygons.push_back(POLY3D(precision*2 + i*2 + 1, precision*2 + i*2 + 3, precision*2 + i*2 + 2));
  }
  polygons.push_back(POLY3D(precision*4 - 2, precision*4 - 1, precision*2));
  polygons.push_back(POLY3D(precision*4 - 1, precision*2 + 1, precision*2));

  flushVertices();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}


/* ---------------------- constructors --------------------- */
Hole::Hole(COLORREF c) 
  : clsMesh(MSH_HOLE, c), h(0), bR(0), bRh(0), tR(0), tRh(0), precision(0) { }

Hole::Hole(
    float height, 
    float bRadius, 
    float bHoleRadius,
    float tRadius,
    float tHoleRadius,
    int   prec,
    COLORREF  c
) : clsMesh(MSH_HOLE, c)
{ 
  h     = height;
  bR      = bRadius;
  bRh     = bHoleRadius;
  tR      = tRadius;
  tRh     = tHoleRadius;
  precision = prec;
  Triangulate();
}

Hole::Hole(
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : clsMesh( MSH_HOLE, red, green, blue ) 
{
  h     = 0;
  bR      = 0;
  bRh     = 0;
  tR      = 0;
  tRh     = 0;
  precision = 24;
}

Hole::Hole(
    float height, 
    float bRadius, 
    float bHoleRadius,
    float tRadius,
    float tHoleRadius,
    int   prec,
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : clsMesh( MSH_HOLE, red, green, blue )
{
  h     = height;
  bR      = bRadius;
  bRh     = bHoleRadius;
  tR      = tRadius;
  tRh     = tHoleRadius;
  precision = prec;
  Triangulate();
}


/* ------------------------ getters ------------------------ */

float Hole::getHeight() { return h; }
float Hole::getBRadius() { return bR; }
float Hole::getTRadius() { return tR; }
float Hole::getBHoleRadius() { return bRh; }
float Hole::getTHoleRadius() { return tRh; }
int Hole::getPrecision() { return precision; }


/* ------------------------ setters ------------------------ */

void Hole::setHeight(float n) { h = n; } 
void Hole::setBRadius(float n) { bR = n; }
void Hole::setTRadius(float n) { tR = n; }
void Hole::setBHoleRadius(float n) { bRh = n; }
void Hole::setTHoleRadius(float n) { tRh = n; }
void Hole::setPrecission(int n) { precision = n; }

#endif // HOLE_CPP

clsSphere::clsSphere(
    float     Radius,
    float     Crop,
    float     From,
    float     To,
    unsigned int  Prec,
    COLORREF    c
) : clsMesh(MSH_SPHERE, c) {
  radius    = abs(Radius);
  precision = Prec;
  cropMult  = Crop;
  angleFrom = From;
  angleTo   = To;
  if ( cropMult < EPSILON )     cropMult = .0f;
  if ( cropMult - 1.0f > EPSILON )  cropMult = 1.0f;

  Triangulate();
}

clsSphere::clsSphere(
    float     Radius, 
    float     Crop,
    float     From,
    float     To,
    unsigned int  Prec,
    unsigned char red,
    unsigned char green,
    unsigned char blue
) : clsMesh(MSH_SPHERE, red, green, blue) {
  radius    = abs(Radius);
  precision = Prec;
  cropMult  = Crop;
  angleFrom = From;
  angleTo   = To;
  if ( cropMult < EPSILON )     cropMult = .0f;
  if ( cropMult - 1.0f > EPSILON )  cropMult = 1.0f;

  Triangulate();
}

void clsSphere::Triangulate()
{
  if ( cropMult < 1.0f )
  {
    vertices.clear();
    edges.clear();
    polygons.clear();

    float cropZ = radius *  (2 * cropMult - 1),
        vAngle  = asin(cropZ / radius),
        vDelta  = ((float)M_PI_2 - vAngle) 
              / (float)(precision / 2), 
        hDelta  = abs((angleTo - angleFrom) / precision),
        radius2p = radius * radius;

    BOOL isSliced = cos(angleFrom) != 1.0f || cos(angleTo) != 1.0f;
    UINT perCircle  = isSliced ? precision + 2 : precision;
    if ( abs(cropZ) < radius - EPSILON ) 
      vertices.push_back(VECTOR3D(.0f, .0f, cropZ));

    while ( (FLOAT)M_PI_2 - vAngle > -EPSILON )
    {
      float cropX = radius * cos(vAngle);
      UINT vCount = cropX > EPSILON ? perCircle : 1;

      LPVECTOR3D v  = new VECTOR3D[vCount];

      float hAngle = angleFrom;
      UINT max = isSliced ? vCount - 1 : vCount;
      for( UINT i = 0; i < max; i++ ) 
      {
        v[i].x = cropX * cos(hAngle);
        v[i].y = cropX * sin(hAngle);
        v[i].z = cropZ;

        hAngle += hDelta;
      }
      if ( isSliced ) v[max].z = cropZ;
      vertices.insert(vertices.end(), v, v + vCount);
      
      delete[] v;

      vAngle += vDelta;
      cropZ = radius * sin(vAngle); 
    }

    UINT  i = 1,
        max = vertices.size();
    while ( i < max )
    {
      for ( UINT j = i, m = i + perCircle; j < m; j++ )
      {
        UINT current  = j >= max ? max - 1 : j,
           upward   = ((INT)(current - perCircle)) <= 0 
                      ? 0 : (current - perCircle),
           forward  = current >= m - 1 ? i : current + 1,
           diagonal = upward == 0 ? 0 : upward + 1;

        if ( forward == max ) 
        {
          upward    += j - i;
          diagonal  = upward + 1;
          if ( diagonal == i ) diagonal = i - perCircle;
          polygons.push_back(POLY3D(diagonal, current, upward));
        }
        else
        {
          edges.push_back(EDGE3D(current, forward));
          if ( diagonal != 0 )
          {
            if ( diagonal == i ) diagonal = i - perCircle;
            edges.push_back(EDGE3D(current, diagonal));
            polygons.push_back(POLY3D(current, diagonal, forward));
            polygons.push_back(POLY3D(upward, diagonal, current));
          }
          else
          {
            polygons.push_back(POLY3D(upward, forward, current));
          }
        }
        edges.push_back(EDGE3D(current, upward));
      }
      i += perCircle;
    }

    flushVertices();
    vertices.shrink_to_fit();
    edges.shrink_to_fit();
    polygons.shrink_to_fit();
  }
}
#include "Jagdpanther.h"


clsTankBody::clsTankBody(
      float bL,
      float bW,
      float bH,
      float fba,
      float fta,
      float bba,
      float bta,
      float ta,
      float sa
) : clsMesh(MSH_TANK) {
  float cnv2rad = (FLOAT)M_PI / 180.0f;

  Length      = bL;
  Width     = bW;
  Height      = bH;
  fbA       = fba * cnv2rad;
  ftA       = fta * cnv2rad;
  bbA       = bba * cnv2rad;
  btA       = bta * cnv2rad;
  tA        = ta  * cnv2rad;
  sA        = sa  * cnv2rad;

  Triangulate();
}

clsTankBody::clsTankBody(
      float bL,
      float bW,
      float bH,
      float fba,
      float fta,
      float bba,
      float bta,
      float ta,
      float sa,
      COLORREF c
) : clsMesh(MSH_TANK, c) {
  float cnv2rad = (FLOAT)M_PI / 180.0f;

  Length      = bL;
  Width     = bW;
  Height      = bH;
  fbA       = fba * cnv2rad;
  ftA       = fta * cnv2rad;
  bbA       = bba * cnv2rad;
  btA       = bta * cnv2rad;
  tA        = ta  * cnv2rad;
  sA        = sa  * cnv2rad;

  Triangulate();
}

clsTankBody::clsTankBody(
      float bL,
      float bW,
      float bH,
      float fba,
      float fta,
      float bba,
      float bta,
      float ta,
      float sa,
      unsigned char red, 
      unsigned char green,
      unsigned char blue
) : clsMesh(MSH_TANK, red, green, blue) {
  float cnv2rad = (FLOAT)M_PI / 180.0f;

  Length      = bL;
  Width     = bW;
  Height      = bH;
  fbA       = fba * cnv2rad;
  ftA       = fta * cnv2rad;
  bbA       = bba * cnv2rad;
  btA       = bta * cnv2rad;
  tA        = ta  * cnv2rad;
  sA        = sa  * cnv2rad;

  Triangulate();
}

void clsTankBody::Triangulate()
{
  VECTOR3D v[11];

  vertices.clear();
  edges.clear();
  polygons.clear();

  // Symmetry projection above ZoX
  v[1].x = Length / 2.0f;                 // half length
  v[1].z = Height * 0.2f;                 // frontal slice
  v[2].x = v[1].x - (v[1].z / tan((FLOAT)(M_PI_2 - fbA)));
  v[7].x = -v[1].x;
  v[7].z = Height * 0.65f;                  // back slice
  v[6].x = v[7].x;
  v[6].z = Height * 0.575f;
  v[3].x = v[6].x + (v[6].z / tan((FLOAT)(M_PI_2 - bbA))); 
  v[4].z = Height * 0.3f;
  v[4].x = v[7].x + ((v[7].z - v[4].z) / tan((FLOAT)(M_PI_2 - bbA)));
  float l = v[7].z - v[6].z;
  v[5] = v[4] - VECTOR3D(
            l * sin(2 * bbA),
            .0f,
            l * -cos(2 * bbA)
          );
  v[8].x = v[7].x * 0.4f;
  v[8].z = v[7].z;
  v[9].z = Height;
  v[9].x = v[8].x - (v[9].z / tan((FLOAT)(M_PI_2 + btA)));
  float tangentA  = tan((FLOAT)M_PI_2 + ftA),
      tangentB  = tan(-tA);
  v[10].x = (v[1].z - tangentA * v[1].x 
            - v[9].z + tangentB * v[9].x) 
          / (tangentB - tangentA);
  v[10].z = tangentA * (v[10].x - v[1].x) + v[1].z;
  v[0].z = Height * 0.4f;
  v[0].x = (v[0].z + tangentA * v[1].x - v[1].z) / tangentA;
  
  symmetryPoints.insert(symmetryPoints.begin(), v, v + _countof(v));
  // Copy vertices and reallocate em. as should be.

  float halfWidth   = Width   / 2.0f;
  float baseHalfWidth = halfWidth * 0.568f;

  vertices.insert(vertices.end(), v, v + 5);
  for( UINT i = 0; i < 5; i++ )
    vertices[i].y += baseHalfWidth;

  vertices.insert(vertices.end(), v + 4, v + 11);
  vertices.push_back(v[0]);
  vertices[5].y += halfWidth;
  vertices[12].y  += halfWidth * 0.92f;

  float x12x5 = vertices[12].x - vertices[5].x; 
  float y12y5 = vertices[12].y - vertices[5].y;
  float z12z5 = vertices[12].z - vertices[5].z; 
  for( UINT i = 6; i < 12; i++ )
  {
    VECTOR3D temp1, temp2;
    vertices[i].y += vertices[5].y;
    if ( i > 8 ) 
    {
      vertices[i].y += (vertices[i].x - vertices[5].x)
              * y12y5 / x12x5;
    }

    temp1 = vertices[i];
    temp1.z = (temp1.x - vertices[5].x) 
        * z12z5 / x12x5 
        + vertices[5].z;
    temp2.x = temp1.x;
    temp2.z = temp1.y * tan((FLOAT)M_PI_2 - sA);
    vertices[i].y = (vertices[i].z - temp2.z) 
            * (temp1.y - temp2.y) 
            / (temp1.z - temp2.z)
            + temp2.y;

  }

  vertices.insert(vertices.end(), vertices.begin(), vertices.end());
  for ( UINT i = 13; i < 26; i++ )
    vertices[i].y *= -1.0f;

  for ( UINT i = 0; i < 5; i++ )
  {
    edges.push_back(EDGE3D(i, i + 13));
    if ( i > 1 ) 
    {
        edges.push_back(EDGE3D(0, i));
        edges.push_back(EDGE3D(13, i + 13));
    }
    if ( i < 4 )
    {
      polygons.push_back(POLY3D(0, i + 2, i + 1));
      polygons.push_back(POLY3D(13, i + 14, i + 15));
      edges.push_back(EDGE3D(i, i + 1));
      edges.push_back(EDGE3D(i, i + 14));
      edges.push_back(EDGE3D(i + 13, i + 14));
      polygons.push_back(POLY3D(i, i + 14, i + 13));
      polygons.push_back(POLY3D(i, i + 1, i + 14));
    }
  }
  edges.push_back(EDGE3D(0, 12));
  edges.push_back(EDGE3D(4, 5));
  edges.push_back(EDGE3D(13, 25));
  edges.push_back(EDGE3D(17, 18));
  polygons.push_back(POLY3D(0, 12, 5));
  polygons.push_back(POLY3D(13, 18, 25));

  for ( UINT i = 6; i < 12; i++ )
  {
    edges.push_back(EDGE3D(i, i + 13));
    edges.push_back(EDGE3D(i, i + 1));
    edges.push_back(EDGE3D(i + 13, i + 14));
    edges.push_back(EDGE3D(i, i + 14));
    if ( i < 11 )
    {
      polygons.push_back(POLY3D(i, i + 14, i + 13));
      polygons.push_back(POLY3D(i, i + 1, i + 14));
    }
  }
  edges.push_back(EDGE3D(24, 13));
  edges.push_back(EDGE3D(11, 0));
  edges.push_back(EDGE3D(5,6));
  edges.push_back(EDGE3D(18, 19));
  edges.push_back(EDGE3D(4, 6));
  edges.push_back(EDGE3D(17, 6));
  edges.push_back(EDGE3D(17, 19));
  edges.push_back(EDGE3D(5, 12));
  edges.push_back(EDGE3D(18, 25));
  edges.push_back(EDGE3D(5, 12));
  edges.push_back(EDGE3D(7, 9));
  edges.push_back(EDGE3D(6, 9));
  edges.push_back(EDGE3D(5, 9));
  edges.push_back(EDGE3D(12, 9));
  edges.push_back(EDGE3D(12, 10));
  edges.push_back(EDGE3D(20, 22));
  edges.push_back(EDGE3D(19, 22));
  edges.push_back(EDGE3D(18, 22));
  edges.push_back(EDGE3D(25, 22));
  edges.push_back(EDGE3D(25, 23));

  polygons.push_back(POLY3D(12, 0, 11));
  polygons.push_back(POLY3D(13, 25, 24));
  polygons.push_back(POLY3D(12, 13, 11));
  polygons.push_back(POLY3D(11, 13, 24));
  polygons.push_back(POLY3D(6, 4, 5));
  polygons.push_back(POLY3D(6, 17, 4));
  polygons.push_back(POLY3D(6, 19, 17));
  polygons.push_back(POLY3D(17, 19, 18));
  polygons.push_back(POLY3D(7, 9, 8));
  polygons.push_back(POLY3D(6, 9, 7));
  polygons.push_back(POLY3D(5, 9, 6));
  polygons.push_back(POLY3D(5, 12, 9));
  polygons.push_back(POLY3D(9, 12, 10));
  polygons.push_back(POLY3D(10, 12, 11));
  polygons.push_back(POLY3D(20, 21, 22));
  polygons.push_back(POLY3D(19, 20, 22));
  polygons.push_back(POLY3D(18, 19, 22));
  polygons.push_back(POLY3D(18, 22, 25));
  polygons.push_back(POLY3D(22, 23, 25));
  polygons.push_back(POLY3D(23, 24, 25));

  for( UINT i = 0, max = vertices.size(); i < max; i++ )
    swap(vertices[i].x, vertices[i].y);

  Transform();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}

VERT_LIST clsTankBody::getSymmetry() { return symmetryPoints; }

void CircleIntersect(
      float o1X, float o1Y, float o1R, 
      float o2X, float o2Y, float o2R, 
      float *out, size_t outCount
) {
  if ( out != NULL )
  {
    float res[4]  = {0};
    float d     = sqrtf(powf((o1X - o2X), 2) + powf((o1Y - o2Y), 2));

    VECTOR3D v(o2X - o1X, o2Y - o1Y, .0f);
    VECTOR3D P0(o1X, o1Y, .0f);
    Vector3DNormalize(v, v);
    if ( abs(d - (o1R + o2R)) < EPSILON )
    {
      P0 += v * o1R;
      res[0] = P0.x;
      res[1] = P0.y;
    }
    else
    {
      if ( (o1R + o2R) - d > EPSILON )
      {
        float a = (o1R*o1R - o2R*o2R + d*d) / (2*d);
        float h = sqrt(o1R*o1R - a*a);
        
        P0 += v * a;
        res[0] = P0.x + h * (o2Y - o1Y) / d;
        res[1] = P0.y - h * (o2X - o1X) / d;
        res[2] = P0.x - h * (o2Y - o1Y) / d;
        res[3] = P0.y + h * (o2X - o1X) / d;
      }
    }
    UINT cpCount = outCount <= _countof(res) ? outCount : _countof(res);
    CopyMemory(out, res, cpCount * sizeof(float));
  }
}

clsCaterpillar::clsCaterpillar(
            const VECTOR3D &o1,
            float o1rad,
            const VECTOR3D &o2,
            float o2rad,
            const VECTOR3D &o3,
            float o3rad,
            const VECTOR3D &o4,
            float o4rad,
            float height,
            float thickness,
            unsigned int   prec
) : clsMesh(MSH_CAT) {
  O1      = o1;
  o1Radius  =o1rad;
  O2      = o2;
  o2Radius  = o2rad;
  O3      = o3;
  o3Radius  = o3rad;
  O4      = o4;
  o4Radius  = o4rad;
  Height    = height;
  Thickness = thickness;

  precision = prec;
  Triangulate();
}

void clsCaterpillar::Triangulate()
{
  vertices.clear();
  edges.clear();
  polygons.clear();

  float cjRadius = 2 * max(o1Radius, o2Radius);
  float halfHeight = Height / 2;

  float *pts = new float[4];
  CircleIntersect(
        O2.x, O2.z, cjRadius - o2Radius,
        O1.x, O1.z, cjRadius - o1Radius,    
        pts, 4
      );
  VECTOR3D beg(O1.x - pts[0], 0, O1.z - pts[1]),
       end(O2.x - pts[0], 0, O2.z - pts[1]);
  Vector3DNormalize(beg, beg);
  Vector3DNormalize(end, end);

  VECTOR3D circle(0, 0, 1);
  float da = fabs(acos(Vector3DMultS(beg, circle))) / precision;

  for ( UINT i = 0; i < precision; i++ )
  {
    vertices.push_back(VECTOR3D(O1.x, -halfHeight, O1.z) + (circle * o1Radius));
    vertices.push_back(VECTOR3D(O1.x, -halfHeight, O1.z) + (circle * (o1Radius + Thickness)));
    MATRIX3D M(true);
    Matrix3DRotateAxis(VECTOR3D(0, 1, 0), da, M);
    Matrix3DTransformNormal(M, circle, circle);
    Vector3DNormalize(circle, circle);
  }

  da  = fabs(acos(Vector3DMultS(beg, end))) / precision;
  for ( UINT i = 0; i <= precision; i++ )
  {
    vertices.push_back(VECTOR3D(pts[0], -halfHeight, pts[1]) + (beg * cjRadius));
    vertices.push_back(VECTOR3D(pts[0], -halfHeight, pts[1]) + (beg * (cjRadius + Thickness)));
    MATRIX3D M(true);
    Matrix3DRotateAxis(VECTOR3D(0, 1, 0), da, M);
    Matrix3DTransformNormal(M, beg, beg);
    Vector3DNormalize(beg, beg);
  } 

  CircleIntersect(
        O4.x, O4.z, cjRadius - o4Radius,  
        O3.x, O3.z, cjRadius - o3Radius,
        pts, 4
      );
  beg.x = O3.x - pts[0];
  beg.y = 0;
  beg.z = O3.z - pts[1];
  end.x = O4.x - pts[0];
  end.y = 0;
  end.z = O4.z - pts[1];
  Vector3DNormalize(beg, beg);
  Vector3DNormalize(end, end);

  da  = fabs(acos(Vector3DMultS(beg, end))) / precision;
  for ( UINT i = 0; i <= precision; i++ )
  {
    vertices.push_back(VECTOR3D(pts[0], -halfHeight, pts[1]) + (beg * cjRadius));
    vertices.push_back(VECTOR3D(pts[0], -halfHeight, pts[1]) + (beg * (cjRadius + Thickness)));
    MATRIX3D M(true);
    Matrix3DRotateAxis(VECTOR3D(0, 1, 0), da, M);
    Matrix3DTransformNormal(M, beg, beg);
    Vector3DNormalize(beg, beg);
  } 

  da = fabs(acos(Vector3DMultS(beg, VECTOR3D(0, 0, 1)))) / precision;
  for ( UINT i = 0; i <= precision; i++ )
  {
    vertices.push_back(VECTOR3D(O4.x, -halfHeight, O4.z) + (beg * o4Radius));
    vertices.push_back(VECTOR3D(O4.x, -halfHeight, O4.z) + (beg * (o4Radius + Thickness)));
    MATRIX3D M(true);
    Matrix3DRotateAxis(VECTOR3D(0, 1, 0), da, M);
    Matrix3DTransformNormal(M, beg, beg);
    Vector3DNormalize(beg, beg);
  }

  delete[] pts;

  for ( UINT i = 0, max = vertices.size() - 1; i < max; i++ )
    edges.push_back(EDGE3D(i , i + 1));
  edges.push_back(EDGE3D(vertices.size() - 1, 0));
  for ( UINT i = 0, max = vertices.size() - 2; i < max; i++ )
    edges.push_back(EDGE3D(i , i + 2));
  edges.push_back(EDGE3D(vertices.size() - 1, 1));
  edges.push_back(EDGE3D(vertices.size() - 2, 0));
  for ( UINT i = 0, max = vertices.size() - 2; i < max; i++ )
  {
    if ( i % 2 == 0 )
      polygons.push_back(POLY3D(i + 1, i, i + 2));
    else
      polygons.push_back(POLY3D(i + 2, i, i + 1));
  }
  polygons.push_back(POLY3D(0, vertices.size() - 1, vertices.size() - 2));
  polygons.push_back(POLY3D(1, vertices.size() - 1, 0));

  UINT baseVCount = vertices.size(),
     baseECount = edges.size(),
     basePCount = polygons.size();
  vertices.insert(vertices.end(), vertices.begin(), vertices.end());
  for ( UINT max = vertices.size(), i = max / 2 ; i < max; i++ )
    vertices[i].y += Height;
  edges.insert(edges.end(), edges.begin(), edges.end());
  for ( UINT max = edges.size(), i = baseECount; i < max; i++ )
    edges[i] += baseVCount;
  polygons.insert(polygons.end(), polygons.begin(), polygons.end());
  for ( UINT max = polygons.size(), i = basePCount; i < max; i++ )
  {
    polygons[i] += baseVCount;
    swap(polygons[i].first, polygons[i].second);
  }
  bool parity;
  for ( UINT i = 0; i < baseVCount; i++ )
  {
    parity = i % 2 == 0;
    edges.push_back(EDGE3D(i, i + baseVCount));
    if ( i < baseVCount - 2 )
    {
      edges.push_back(EDGE3D(i, i + 2));
      edges.push_back(EDGE3D(i, i + 2 + baseVCount));
      if ( parity )
      {
        polygons.push_back(POLY3D(i, i + baseVCount, i + 2 + baseVCount));
        polygons.push_back(POLY3D(i, i + 2 + baseVCount, i + 2));
      }
      else
      {
        polygons.push_back(POLY3D(i + baseVCount, i, i + 2 + baseVCount));
        polygons.push_back(POLY3D(i + 2 + baseVCount, i, i + 2));
      }
    }
    else
    {
      edges.push_back(EDGE3D(i, 0));
      edges.push_back(EDGE3D(i, baseVCount));
      if ( parity )
      {
        polygons.push_back(POLY3D(i, i + baseVCount, baseVCount));
        polygons.push_back(POLY3D(i, baseVCount, 0));
      }
      else
      {
        polygons.push_back(POLY3D(i + baseVCount, i, baseVCount + 1));
        polygons.push_back(POLY3D(baseVCount + 1, i, 1));
      }
    }
  }

  Transform();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}


clsJagdpanther::clsJagdpanther(
  float bL,
  float bW,
  float bH,
  float fba,
  float fta,
  float bba,
  float bta,
  float ta,
  float sa,
  float clen,
  unsigned char red, 
  unsigned char green,
  unsigned char blue    
) : clsMesh( MSH_TANK, red, green, blue ) {
  Length  = bL;
  Width = bW;
  Height  = bH;
  fbA   = fba;
  ftA   = fta;
  bbA   = bba;
  btA   = bta;
  tA    = ta;
  sA    = sa;
  cLen  = clen;

  Parts = new LPMESH3D[JGPZ_PARTS_NUM];
  Triangulate();
}

clsJagdpanther::~clsJagdpanther() { delete[] Parts; }

void clsJagdpanther::Triangulate() {
  vertices.clear();
  edges.clear();
  polygons.clear();

  TANKBODY tBody(Length, Width, Height, fbA, ftA, bbA, btA, tA, sA);
  tBody.Yaw(-(FLOAT)M_PI_2);
  tBody.Transform();
    addMesh(PZ_BODY,  tBody);

  float conv2rad  = (FLOAT)M_PI / 180.0f;

  float CannonBaseH = Length * .11f;
  float CannonBaseR1  = Height * .14f;
  float CannonBaseR2  = Height * 0.075f;
  //float crossFront  = Height 
  float CannonZ   = tBody.getSymmetry().at(10).z - 1.5f * CannonBaseR1;
  float CannonBaseX = tBody.getSymmetry().at(10).x;
  CONE3D  CannonBase(Length * 0.11f, CannonBaseR1, CannonBaseR2, APPROX_DEFAULT_PREC);
  CannonBase.Yaw(-(FLOAT)M_PI_2);
  CannonBase.Pitch(-(FLOAT)M_PI_2);
  CannonBase.Translate(.0f, CannonBaseX, CannonZ);
  CannonBase.Transform();
    addMesh(PZ_CANNON_BASE, CannonBase);

  float CannonDpfrR1 = Height * 0.06f;
  float CannonDpfrH  = cLen * .3f;
  CONE3D  CannonDpfr(CannonDpfrH, CannonDpfrR1, CannonDpfrR1, APPROX_DEFAULT_PREC);
  CannonDpfr.Yaw(-(FLOAT)M_PI_2);
  CannonDpfr.Pitch(-(FLOAT)M_PI_2);
  CannonDpfr.Translate(.0f, CannonBaseX + CannonBaseH, CannonZ);
  CannonDpfr.Transform();
    addMesh(PZ_CANNON_DEMPFER,  CannonDpfr);

  float CannonR1 = Height * 0.045f;
  float CannonR2 = Height * .04f;
  float Caliber  = .8f;
  float CannonH  = cLen * .7f;
  HOLE3D  Cannon(CannonH, CannonR1, CannonR1 * Caliber, CannonR2, CannonR2 * Caliber, APPROX_DEFAULT_PREC);
  Cannon.Yaw(-(FLOAT)M_PI_2);
  Cannon.Pitch(-(FLOAT)M_PI_2);
  Cannon.Translate(.0f, CannonBaseX + CannonBaseH + CannonDpfrH, CannonZ);
  Cannon.Transform();
    addMesh(PZ_CANNON,  Cannon);

  float MgBaseR = Height * .12f;
  VECTOR3D MgBaseDir = tBody.getSymmetry().at(1) - tBody.getSymmetry().at(10);
  MgBaseDir *= .46f;
  MgBaseDir = tBody.getSymmetry().at(10) + MgBaseDir;
  MgBaseDir.y = Width / 2 * ( 1 - sin( sA * conv2rad )) - MgBaseR;
  SPHERE3D MgBase(MgBaseR, .5f, 0, (FLOAT)M_PI * 2, APPROX_DEFAULT_PREC, 0x00fffff);
  MgBase.Roll((90 - ftA) * (FLOAT)(M_PI / 180.0f));
  MgBase.Translate(MgBaseDir.y, MgBaseDir.x, MgBaseDir.z);
  MgBase.Transform();
    addMesh(PZ_MG_BASE, MgBase);

  float MgLength  = Length * .07f;
  float MgR   = Height * .02f;
  CONE3D Mg(MgLength, MgR, MgR, APPROX_DEFAULT_PREC);
  Mg.Roll(-(FLOAT)M_PI_2);
  Mg.Translate(MgBaseDir.y, MgBaseDir.x + MgBaseR + MgLength, MgBaseDir.z);
  Mg.Transform();
    addMesh(PZ_MG, Mg);

  float WheelR = Height * .15f;
  float WheelH = Width  * .16f;
  float WheelY = Width  * .45f;
  float WheelX = Length * .425f;
  float WheelZ = Height * .2f;

  VECTOR3D O4(WheelX, 0, WheelZ);
  float o4Radius = WheelR;

  CONE3D  FrontWheel(WheelH, WheelR, WheelR, APPROX_DEFAULT_PREC); 
  FrontWheel.Pitch((FLOAT)M_PI_2);
  FrontWheel.Translate(-WheelY + WheelH, WheelX, WheelZ);
  FrontWheel.Transform();
    addMesh(PZ_LEAD_WHL, FrontWheel);
  FrontWheel.Pitch((FLOAT)M_PI);
  FrontWheel.Translate(WheelY - WheelH, WheelX, WheelZ);
  FrontWheel.Transform();
    addMesh(PZ_LEAD_WHR, FrontWheel);


  WheelR = Height   * .11f;
  WheelX = -Length  * .4f;
  WheelZ = Height   * .12f;

  VECTOR3D O1(WheelX, 0, WheelZ);
  float o1Radius = WheelR;

  CONE3D RearWheel(WheelH, WheelR, WheelR, APPROX_DEFAULT_PREC); 
  RearWheel.Pitch((FLOAT)M_PI_2);
  RearWheel.Translate(-WheelY + WheelH, WheelX, WheelZ);
  RearWheel.Transform();
    addMesh(PZ_REAR_WHL, RearWheel);
  RearWheel.Pitch((FLOAT)M_PI);
  RearWheel.Translate(WheelY - WheelH, WheelX, WheelZ);
  RearWheel.Transform();
    addMesh(PZ_REAR_WHR, RearWheel);

  float delta   = Length  * .0425f;
  float offset  = Width   * .08f;

  WheelX      = Length  * .3f;
  WheelR      = Height  * .22f;
  WheelZ      = Height  * .05f;
  WheelY      = Width   * .38f;

  VECTOR3D O3(WheelX, 0, WheelZ);
  float o3Radius = WheelR;

  CONE3D Wheel(Width * .06f, Height * .22f, Height * .22f, APPROX_DEFAULT_PREC);
  Wheel.Pitch((FLOAT)M_PI_2 * 3.0f);
  for ( UINT pair = 0, i = pair; pair < 8; pair++, i = pair * 2 )
  {
    float tOffset = pair % 2 == 0 ? offset : .0f,
        tDelta = WheelX - delta * i;
    Wheel.Pitch((FLOAT)M_PI);
    Wheel.Translate(-WheelY + tOffset, tDelta, WheelZ);
    Wheel.Transform();
      addMesh((JGPZ_PART)(PZ_BASE_WH1 + i), Wheel);
    Wheel.Pitch((FLOAT)M_PI);
    Wheel.Translate(WheelY - tOffset, tDelta, WheelZ);
    Wheel.Transform();
      addMesh((JGPZ_PART)(PZ_BASE_WH1 + i + 1), Wheel);
  }

  VECTOR3D O2(WheelX - delta * 14, 0, WheelZ);
  float o2Radius  = o3Radius;
  float TrackW  = Width * .17f;
  float Thick   = TrackW * .08f;  
  float TrackY  = Width * .45f - TrackW * .5f; 

  CATERPILLAR Track(O1, o1Radius, O2, o2Radius, O3, o3Radius, O4, o4Radius, TrackW, Thick, 5);
  Track.Translate(TrackY, 0, 0);
  Track.Transform();
      addMesh(PZ_TRACK_L, Track);   
  Track.Translate(-TrackY, 0, 0);
  Track.Transform();
      addMesh(PZ_TRACK_R, Track);   

  Translate(.0, .0, Height * 0.25f);
  Transform();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();
}

bool clsJagdpanther::getMeshPosition(JGPZ_PART part, int& vs, int& es, int& ps) {
  vs = es = ps = -1;
  if ( part < 0 ) return false;
  vs = es = ps = 0;
  for (int i = 0; i < JGPZ_PARTS_NUM; i++) {
    if (part == i) return true;
    vs += Parts[i]->getVerticesCount();
    es += Parts[i]->getEdgesCount();
    ps += Parts[i]->getPolygonsCount();
  }
  return false;
}

void clsJagdpanther::addMesh(JGPZ_PART part, MESH3D& m) {
  Parts[part] = new MESH3D(m);
  
  int vNum, eNum, pNum;
  getMeshPosition(part, vNum, eNum, pNum);
  LPVECTOR3D vs = new VECTOR3D[m.getVerticesCount()];
  m.getVerticesTransformed(vs);
  vertices.insert(vertices.begin() + vNum, vs, vs + m.getVerticesCount());
  delete [] vs;

  EDGE_LIST es = m.getEdges();
  int num = es.size();
  for (int i = 0; i < num; i++ )
    es[i] += vNum;
  edges.insert(edges.begin() + eNum, es.begin(), es.end());

  POLY_LIST ps = m.getPolygons();
  num = ps.size();
  for (int i = 0; i < num; i++ ) {
    ps[i] += vNum;
    size_t tmp = ps[i].first;
    ps[i].first = ps[i].second;
    ps[i].second = tmp;
  }
  polygons.insert(polygons.begin() + pNum, ps.begin(), ps.end());
}

void clsJagdpanther::deleteMesh(JGPZ_PART part) {
  int vNum, eNum, pNum;
  getMeshPosition(part, vNum, eNum, pNum);
  vertices.erase(vertices.begin() + vNum, vertices.begin() + Parts[part]->getVerticesCount() + vNum);
  edges.erase   (edges.begin() + eNum,    edges.begin() + Parts[part]->getEdgesCount() + eNum);
  polygons.erase(polygons.begin() + pNum, polygons.begin() + Parts[part]->getPolygonsCount() + pNum);
}

void clsJagdpanther::replaceMesh(JGPZ_PART part, MESH3D& m) {
  deleteMesh(part);
  addMesh(part, m);
}

void clsJagdpanther::recalcMeshVertices(JGPZ_PART part, MESH3D& m) {
  int vFrom, vTo;
  getMeshPosition(part, vFrom, vTo, vTo);
  vTo = Parts[part]->getVerticesCount();
  LPVECTOR3D vs = new VECTOR3D[vTo];
  Parts[part]->getVerticesTransformed(vs);  
  vertices.erase(vertices.begin() + vFrom, vertices.begin() + vTo + vFrom);
  vertices.insert(vertices.begin() + vFrom, vs, vs + vTo);
  delete [] vs;
}

float clsJagdpanther::getBodyLength() { return Length; }
float clsJagdpanther::getBodyWidth() { return Width; }
float clsJagdpanther::getBodyHeight() { return Height; }
float clsJagdpanther::getFTArmSlope() { return ftA; }
float clsJagdpanther::getFBArmSlope() { return fbA; }
float clsJagdpanther::getBTArmSlope() { return btA; }
float clsJagdpanther::getBBArmSlope() { return bbA; }
float clsJagdpanther::getTopArmSlope() { return tA; }
float clsJagdpanther::getSideArmSlope() { return sA; }
float clsJagdpanther::getCannonLength() { return cLen; } 
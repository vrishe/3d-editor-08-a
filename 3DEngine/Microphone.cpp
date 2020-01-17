#include "Microphone.h"

// ============================================================================
// Implementation of Microphone class:
Microphone::Microphone(unsigned char red, unsigned char green, unsigned char blue, float r, float h, int prec) : clsMesh( MSH_MIC, red, green, blue ) {
  precision = prec;

  tH = h;

  bR = r;
  bH = tH * 0.061f;
  bW = bR * 0.47f;

  uR = bR * 0.1412f;
  uH = tH * 0.58436f;
  uG = tH * 0.02057f;
  hI = bR * 0.09f;

  hR = tH * 0.165f;
  hD = bR * 0.353f;
  cR = tH * 0.12345679f;

  micParts = new LPMESH3D[PARTS_NUM];
  Triangulate();
}

Microphone::Microphone(unsigned char red, unsigned char green, unsigned char blue, 
            float br, 
            float bh,
            float bw,   
            float ur,   
            float uh,     
            float ug,     
            float hi, 
            float hr,   
            float hd,     
            float cr,   
            int prec
            ) : clsMesh( MSH_MIC, red, green, blue ) {
  precision = prec;

  bR = br;
  bH = bh;
  bW = bw;

  uR = ur;
  uH = uh;
  uG = ug;
  hI = hi;

  hR = hr;
  hD = hd;
  cR = cr;

  tH = bH + uH + uG + hR;

  micParts = new LPMESH3D[PARTS_NUM];
  Triangulate();
}

void Microphone::Triangulate() {
  vertices.clear();
  edges.clear();
  polygons.clear();

  ExCone      base(bH, bR, bR, bR * 0.824f, precision);
  Pyramid     buttonL(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, -bW * 0.1375f),
          buttonR(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, -bW * 0.1375f);
  
  Cone      shroudLow(uH * 0.21127f, uR, uR, precision),
          bridge   (uH * 0.084537f, uR * 0.66667f, uR * 0.41667f, precision),
          shroudHi (uH - uG - shroudLow.getHeight() - uH * .04f - bridge.getHeight(), uR, uR, precision),
          upright  (uH - bridge.getHeight(), uR * 0.66667f, uR * 0.66667f, precision);

  Pyramid     handleBridgeUp  (uH * 0.09155f, uR * 0.653f, hI, uR * 0.653f, hI),
          handleBridgeDown(uH * 0.077f,   uR * 0.653f, hI, uR * 0.653f, hI),
          handle      (uH * 0.7598f,  uR * 0.792f, uR * 0.5418f, uR * 0.5418f, uR * 0.5418f, uR * 0.125f),
          handleTop   (uH * 0.05647f, uR * 0.792f, uR * 0.5418f, uR * 0.792f,  uR * 0.5418f,-uR * 0.3542f);

  Hole      head   (hD,  hR,   cR, hR, cR, precision),
          headFront(hD / 10.f, hR * 1.075f, cR * 0.9f, hR * 1.075f, cR * 0.9f, precision),
          headBack (hD / 10.f, hR * 1.075f, cR * 0.9f, hR * 1.075f, cR * 0.9f, precision);
  Cone      core   (hD * 1.43333f,  cR, cR, precision);

  base.Yaw(-(FLOAT)M_PI_2);
  base.Transform();

  buttonL.Translate(-bR * 0.2588f, bR * 0.824f, bH * 1.1f);
  buttonL.Pitch((FLOAT)M_PI);
  buttonL.Transform();

  buttonR.Translate(bR * 0.2588f, bR * 0.824f, bH * 1.1f);
  buttonR.Pitch((FLOAT)M_PI);
  buttonR.Transform();

  upright.Fly(bH);
  upright.Transform();

  shroudLow.Fly(base.getHeight() + uH * .04f);
  shroudLow.Transform();

  shroudHi.Fly(shroudLow.getPosition().z + shroudLow.getHeight() + uG);
  shroudHi.Transform();

  bridge.Fly(shroudHi.getPosition().z + shroudHi.getHeight());
  bridge.Transform();

  handleBridgeUp.Fly(uH * 0.8f);
  handleBridgeUp.Follow(uR + hI / 2);
  handleBridgeUp.Transform();

  handleBridgeDown.Fly(bH + uH * 0.15f);
  handleBridgeDown.Follow(handleBridgeUp.getPosition().x);
  handleBridgeDown.Transform();

  handle.Translate(uR * 1.5f + hI, bR * 0.0059f, uH * .95f);
  handle.Pitch((FLOAT)M_PI);
  handle.Yaw((FLOAT)M_PI_2);
  handle.Transform();

  handleTop.Translate(handle.getPosition().x, handle.getPosition().y, handle.getPosition().z);
  handleTop.Yaw((FLOAT)M_PI_2);
  handleTop.Transform();

  head.Fly(bridge.getPosition().z + bridge.getHeight() + hR);
  head.Strafe(-hD/2);
  head.Pitch((FLOAT)M_PI_2);
  head.Roll((FLOAT)M_PI_2);
  head.Transform();

  headFront.Fly(head.getPosition().z);
  headFront.Strafe(-head.getPosition().y);
  headFront.Pitch((FLOAT)M_PI_2);
  headFront.Roll((FLOAT)M_PI_2);
  headFront.Transform();

  headBack.Translate(head.getPosition());
  headBack.Pitch((FLOAT)M_PI_2);
  headBack.Roll((FLOAT)M_PI_2);
  headBack.Transform();

  core.Fly(head.getPosition().z);
  core.Strafe(-core.getHeight() / 2.1f);
  core.Pitch((FLOAT)M_PI_2);
  core.Roll((FLOAT)M_PI_2);
  core.Transform();

  addMesh(MIC_BASE,   base);
  addMesh(MIC_BUTTON_L, buttonL);
  addMesh(MIC_BUTTON_R, buttonR);
  addMesh(MIC_UPRIGHT,  upright);
  addMesh(MIC_SHROUD_LOW, shroudLow);
  addMesh(MIC_SHROUD_HI,  shroudHi);
  addMesh(MIC_BRIDGE,   bridge);
  addMesh(MIC_HANDLE_BU,  handleBridgeUp);
  addMesh(MIC_HANDLE_BD,  handleBridgeDown);
  addMesh(MIC_HANDLE,   handle);
  addMesh(MIC_HANDLE_TOP, handleTop);
  addMesh(MIC_HEAD,   head);
  addMesh(MIC_HEAD_FRONT, headFront);
  addMesh(MIC_HEAD_BACK,  headBack);
  addMesh(MIC_CORE,   core);

  Transform();
  vertices.shrink_to_fit();
  edges.shrink_to_fit();
  polygons.shrink_to_fit();

//  flipNormals(0, polygons.size());
}

Microphone::~Microphone() { delete [] micParts; }

bool Microphone::getMeshPosition(MIC_PART part, int& vs, int& es, int& ps) {
  vs = es = ps = -1;
  if ( part < 0 ) return false;
  vs = es = ps = 0;
  for (int i = 0; i < PARTS_NUM; i++) {
    if (part == i) return true;
    vs += micParts[i]->getVerticesCount();
    es += micParts[i]->getEdgesCount();
    ps += micParts[i]->getPolygonsCount();
  }
  return false;
}

void Microphone::addMesh(MIC_PART part, MESH3D& m) {
  micParts[part] = new MESH3D(m);
  
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

void Microphone::deleteMesh(MIC_PART part) {
  int vNum, eNum, pNum;
  getMeshPosition(part, vNum, eNum, pNum);
  vertices.erase(vertices.begin() + vNum, vertices.begin() + micParts[part]->getVerticesCount() + vNum);
  edges.erase   (edges.begin() + eNum,    edges.begin() + micParts[part]->getEdgesCount() + eNum);
  polygons.erase(polygons.begin() + pNum, polygons.begin() + micParts[part]->getPolygonsCount() + pNum);
}

void Microphone::replaceMesh(MIC_PART part, MESH3D& m) {
  deleteMesh(part);
  addMesh(part, m);
}

void Microphone::recalcMeshVertices(MIC_PART part, MESH3D& m) {
  int vFrom, vTo;
  getMeshPosition(part, vFrom, vTo, vTo);
  vTo = micParts[part]->getVerticesCount();
  LPVECTOR3D vs = new VECTOR3D[vTo];
  micParts[part]->getVerticesTransformed(vs); 
  vertices.erase(vertices.begin() + vFrom, vertices.begin() + vTo + vFrom);
  vertices.insert(vertices.begin() + vFrom, vs, vs + vTo);
  delete [] vs;
}


/* ---------------------------------- setters ---------------------------------- */

void Microphone::setTotalHeight (float h) {
  tH = h;

  bH = tH * 0.061f;

  uH = tH * 0.58436f;
  uG = tH * 0.02057f;

  hR = tH * 0.165f;
  cR = tH * 0.12345679f;

  delete [] micParts;
  micParts = new LPMESH3D[PARTS_NUM];
  Triangulate();
}
void Microphone::setBaseRadius (float r) {
  bR = r;
  ExCone  base(bH, bR, bR, bR * 0.824f, 24);
  base.Yaw(-(FLOAT)M_PI_2);
  replaceMesh(MIC_BASE, base);

  micParts[MIC_BUTTON_L]->Translate(-bR * 0.2588f, bR * 0.824f, bH * 1.1f);
  micParts[MIC_BUTTON_R]->Translate(bR * 0.2588f, bR * 0.824f, bH * 1.1f);

  recalcMeshVertices(MIC_BUTTON_L, *micParts[MIC_BUTTON_L]);
  recalcMeshVertices(MIC_BUTTON_R, *micParts[MIC_BUTTON_R]);
}
void Microphone::setBaseHeight (float h) {
  float shift = h - bH;
  bH = h; tH += shift;
  ExCone  base(bH, bR, bR, bR * 0.824f, 24);
  base.Yaw(-(FLOAT)M_PI_2);
  replaceMesh(MIC_BASE, base);

  for (int i = MIC_BUTTON_L; i < PARTS_NUM; i++){
    micParts[i]->Translate(micParts[i]->getPosition().x, micParts[i]->getPosition().y, micParts[i]->getPosition().z + shift);
    recalcMeshVertices((MIC_PART)i, *micParts[i]);
  }
}
void Microphone::setButtonWidth (float w) {
  bW = w;
  Pyramid     buttonL(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, -bW * 0.1375f),
          buttonR(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, -bW * 0.1375f);
  buttonL.Translate( -bR * 0.2588f, bR * 0.824f, bH * 1.1f);
  buttonL.Pitch((FLOAT)M_PI);
  buttonR.Translate(bR * 0.2588f, bR * 0.824f, bH * 1.1f);
  buttonR.Pitch((FLOAT)M_PI);
  replaceMesh(MIC_BUTTON_L, buttonL);
  replaceMesh(MIC_BUTTON_R, buttonR);
}
void Microphone::setUprightRadius (float r) {
  float shift = r - uR;
  uR = r;

  Cone    upright  (uH * 0.35211f, uR * 0.66667f, uR * 0.66667f, 24),
        shroudLow(uH * 0.21127f, uR, uR, 24),
        shroudHi (uH * 0.63317f, uR, uR, 24),
        bridge   (uH * 0.084537f,uR * 0.66667f, uR * 0.41667f, 24);
  upright.Fly(bH);
  shroudLow.Fly(tH * 0.0823f);
  shroudHi.Fly(shroudLow.getPosition().z + shroudLow.getHeight() + uG);
  bridge.Fly(shroudHi.getPosition().z + shroudHi.getHeight());

  replaceMesh(MIC_UPRIGHT, upright);
  replaceMesh(MIC_SHROUD_LOW, shroudLow);
  replaceMesh(MIC_SHROUD_HI, shroudHi);
  replaceMesh(MIC_BRIDGE, bridge);

  for (int i = MIC_HANDLE_BU; i < MIC_HEAD; i++){
    micParts[i]->Translate( micParts[i]->getPosition().x + shift,
                micParts[i]->getPosition().y,
                micParts[i]->getPosition().z);
    recalcMeshVertices((MIC_PART)i, *micParts[i]);
  }
}
void Microphone::setUprightHeight (float h) {
  float shift = h - uH;
  tH += shift; uH = tH * 0.58436f;

  Cone      upright  (uH * 0.21127f + 2 * uG, uR * 0.66667f, uR * 0.66667f, 24),
          shroudLow(uH * 0.21127f, uR, uR, 24),
          shroudHi (uH - upright.getHeight() - uH * 0.084537f, uR, uR, 24),
          bridge   (uH * 0.084537f,uR * 0.66667f, uR * 0.41667f, 24);

  Pyramid     handleBridgeUp  (uH * 0.09155f, uR * 0.653f, hI, uR * 0.653f, hI),
          handleBridgeDown(uH * 0.077f,   uR * 0.653f, hI, uR * 0.653f, hI),
          handle      (uH * 0.7598f,  uR * 0.792f, uR * 0.5418f, uR * 0.5418f, uR * 0.5418f, uR * 0.125f),
          handleTop   (uH * 0.05647f, uR * 0.792f, uR * 0.5418f, uR * 0.792f,  uR * 0.5418f,-uR * 0.3542f);

  upright.Fly(bH);
  shroudLow.Fly(tH * 0.0823f);
  shroudHi.Fly(shroudLow.getPosition().z + shroudLow.getHeight() + uG);
  bridge.Fly(shroudHi.getPosition().z + shroudHi.getHeight());

  handleBridgeUp.Fly(tH * 0.46f);
  handleBridgeUp.Follow(bR * 0.2f);
  handleBridgeDown.Fly(tH * 0.15f);
  handleBridgeDown.Follow(handleBridgeUp.getPosition().x);
  handle.Translate(bR * 0.306f, bR * 0.0059f, tH * 0.547f);
  handle.Pitch((FLOAT)M_PI);
  handle.Yaw((FLOAT)M_PI_2);
  handleTop.Translate(handle.getPosition().x, handle.getPosition().y, handle.getPosition().z);
  handleTop.Yaw((FLOAT)M_PI_2);

  replaceMesh(MIC_UPRIGHT, upright);
  replaceMesh(MIC_SHROUD_LOW, shroudLow);
  replaceMesh(MIC_SHROUD_HI, shroudHi);
  replaceMesh(MIC_BRIDGE, bridge);
  replaceMesh(MIC_HANDLE_BU, handleBridgeUp);
  replaceMesh(MIC_HANDLE_BD, handleBridgeDown);
  replaceMesh(MIC_HANDLE, handle);
  replaceMesh(MIC_HANDLE_TOP, handleTop);

  for (int i = MIC_HEAD; i < PARTS_NUM; i++){
    micParts[i]->Translate( micParts[i]->getPosition().x,
                micParts[i]->getPosition().y,
                bridge.getPosition().z + bridge.getHeight() + hR);
    recalcMeshVertices((MIC_PART)i, *micParts[i]);
  }
}
void Microphone::setUprightGap (float g) {
  float shift = g - uG;

  Cone      upright  (uH * 0.21127f + 2 * g, uR * 0.66667f, uR * 0.66667f, 24),
          shroudLow(uH * 0.21127f, uR, uR, 24),
          shroudHi ( uH * 0.7042f - 2*uG - shift, uR, uR, 24);


  upright.Fly(bH);
  shroudLow.Fly(tH * 0.0823f);
  shroudHi.Fly(shroudLow.getPosition().z + shroudLow.getHeight() + g);


  replaceMesh(MIC_UPRIGHT, upright);
  replaceMesh(MIC_SHROUD_LOW, shroudLow);
  replaceMesh(MIC_SHROUD_HI, shroudHi);
}
void Microphone::setHandleIndent (float i) {
  float shift = i - hI;
  hI = i;
  Pyramid     handleBridgeUp  (uH * 0.09155f, uR * 0.653f, hI * 1.5f, uR * 0.653f, hI * 1.5f),
          handleBridgeDown(uH * 0.077f,   uR * 0.653f, hI * 1.5f, uR * 0.653f, hI * 1.5f);
  handleBridgeUp.Fly(tH * 0.46f);
  handleBridgeUp.Follow(uR + hI / 2.f);
  handleBridgeDown.Fly(tH * 0.15f);
  handleBridgeDown.Follow(handleBridgeUp.getPosition().x);
  replaceMesh(MIC_HANDLE_BU, handleBridgeUp);
  replaceMesh(MIC_HANDLE_BD, handleBridgeDown);

  for (int i = MIC_HANDLE; i < MIC_HEAD; i++){
    micParts[i]->Translate( micParts[i]->getPosition().x + shift,
                micParts[i]->getPosition().y,
                micParts[i]->getPosition().z);
    recalcMeshVertices((MIC_PART)i, *micParts[i]);
  }
}
void Microphone::setHeadRadius (float r) {
  float shift = r - hR;
  hR = r; tH += shift;

  Hole      head   (hD,  hR,   cR, hR, cR, 24),
          headFront(hD / 10.f, hR * 1.075f, cR * 0.9f, hR * 1.075f, cR * 0.9f, 24),
          headBack (hD / 10.f, hR * 1.075f, cR * 0.9f, hR * 1.075f, cR * 0.9f, 24);

  head.Fly(micParts[MIC_HEAD]->getPosition().z + shift);
  head.Strafe(-hD/2);
  head.Pitch((FLOAT)M_PI_2);
  head.Roll((FLOAT)M_PI_2);
  headFront.Fly(head.getPosition().z);
  headFront.Strafe(-head.getPosition().y);
  headFront.Pitch((FLOAT)M_PI_2);
  headFront.Roll((FLOAT)M_PI_2);
  headBack.Translate(head.getPosition());
  headBack.Pitch((FLOAT)M_PI_2);
  headBack.Roll((FLOAT)M_PI_2);

  replaceMesh(MIC_HEAD, head);
  replaceMesh(MIC_HEAD_FRONT, headFront);
  replaceMesh(MIC_HEAD_BACK, headBack);

  micParts[MIC_CORE]->Translate(  micParts[MIC_CORE]->getPosition().x,
                micParts[MIC_CORE]->getPosition().y,
                micParts[MIC_CORE]->getPosition().z + shift);
  recalcMeshVertices(MIC_CORE, *micParts[MIC_CORE]);
}
void Microphone::setHeadDepth (float d) {
  float shift = d - hD;
  hD = d;

  Hole      head   (hD,  hR,   cR, hR, cR, 24),
          headFront(hD / 10.f, hR * 1.075f, cR * 0.9f, hR * 1.075f, cR * 0.9f, 24),
          headBack (hD / 10.f, hR * 1.075f, cR * 0.9f, hR * 1.075f, cR * 0.9f, 24);
  Cone      core   (hD * 1.43333f,  cR, cR, 24);

  head.Fly(micParts[MIC_HEAD]->getPosition().z);
  head.Strafe(-hD/2);
  head.Pitch((FLOAT)M_PI_2);
  head.Roll((FLOAT)M_PI_2);

  headFront.Fly(head.getPosition().z);
  headFront.Strafe(-head.getPosition().y);
  headFront.Pitch((FLOAT)M_PI_2);
  headFront.Roll((FLOAT)M_PI_2);

  headBack.Translate(head.getPosition());
  headBack.Pitch((FLOAT)M_PI_2);
  headBack.Roll((FLOAT)M_PI_2);

  core.Fly(head.getPosition().z);
  core.Strafe(-core.getHeight() / 2.1f);
  core.Pitch((FLOAT)M_PI_2);
  core.Roll((FLOAT)M_PI_2);

  replaceMesh(MIC_HEAD,   head);
  replaceMesh(MIC_HEAD_FRONT, headFront);
  replaceMesh(MIC_HEAD_BACK,  headBack);
  replaceMesh(MIC_CORE,   core);
}
void Microphone::setCoreRadius (float r) {
  float shift = r - cR;
  cR =r;
  setHeadDepth(hD);
}
void Microphone::setPrecision (int prec) {
  precision = prec;

  delete [] micParts;
  micParts = new LPMESH3D[PARTS_NUM];
  Triangulate();
}


/* ---------------------------------- getters ---------------------------------- */
float Microphone::getBaseRadius   () { return bR; }
float Microphone::getBaseHeight   () { return bH; }
float Microphone::getButtonWidth  () { return bW; }
float Microphone::getUprightRadius  () { return uR; }
float Microphone::getUprightHeight  () { return uH; }
float Microphone::getUprightGap   () { return uG; }
float Microphone::getHandleIndent () { return hI; }
float Microphone::getHeadRadius   () { return hR; }
float Microphone::getHeadDepth    () { return hD; }
float Microphone::getCoreRadius   () { return cR; }
float Microphone::getTotalHeight  () { return tH; }
int Microphone::getPrecision    () { return precision; }
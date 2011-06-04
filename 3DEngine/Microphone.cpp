#include "Microphone.h"

// ============================================================================
// Implementation of Microphone class:
Microphone::Microphone(unsigned char red, unsigned char green, unsigned char blue, float r, float h, float prec) : clsMesh(red, green, blue) {
	precission = prec;

	tH = h;

	bR = r;
	bH = tH * 0.061f;
	bW = bR * 0.47f;

	uR = bR * 0.1412f;
	uH = tH * 0.58436f;
	uG = tH * 0.411522f;
	hI = bR * 0.09f;

	hR = tH * 0.165f;
	hD = bR * 0.353f;
	cR = tH * 0.12345679f;

	micParts = new LPMESH3D[PARTS_NUM];
	Triangulate();
}

void Microphone::Triangulate() {
	vertices.clear();
	edges.clear();
	polygons.clear();

	ExCone			base(bH, bR, bR, bR * 0.824f, 24);
	Pyramid			buttonL(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, bW * 0.1375f),
					buttonR(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, bW * 0.1375f);

	Cone			upright	 (uH * 0.35211f, uR * 0.66667f, uR * 0.66667f, 24),
					shroudLow(uH * 0.21127f, uR, uR, 24),
					shroudHi (uH * 0.63317f, uR, uR, 24),
					bridge   (uH * 0.084537f,uR * 0.66667f, uR * 0.41667f, 24);

	Pyramid			handleBridgeUp	(uH * 0.09155f, uR * 0.653f, hI, uR * 0.653f, hI),
					handleBridgeDown(uH * 0.077f,   uR * 0.653f, hI, uR * 0.653f, hI),
					handle			(uH * 0.7598f,  uR * 0.792f, uR * 0.5418f, uR * 0.5418f, uR * 0.5418f, uR * 0.125f),
					handleTop		(uH * 0.05647f, uR * 0.792f, uR * 0.5418f, uR * 0.792f,  uR * 0.5418f,-uR * 0.3542f);

	Hole			head	 (hD,  hR,	 cR, hR, cR, 24),
					headFront(hD / 10.f, hR * 1.075f, hD * 0.2167f, hR * 1.075f, cR * 0.9f, 24),
					headBack (hD / 10.f, hR * 1.075f, hD * 0.2167f, hR * 1.075f, cR * 0.9f, 24);
	Cone			core	 (hD * 1.43333f,  cR, cR, 24);

	base.YawTo(-(FLOAT)M_PI_2);

	buttonL.Translate(bR * 0.824f, -bR * 0.2588f, bH * 1.1f);
	buttonL.PitchTo((FLOAT)M_PI);
	buttonR.Translate(bR * 0.824f, bR * 0.2588f, bH * 1.1f);
	buttonR.PitchTo((FLOAT)M_PI);

	upright.Fly(base.getHeight());
	shroudLow.Fly(tH * 0.0823f);
	shroudHi.Fly(tH * 0.2263f);
	bridge.Fly(shroudHi.getPosition().z + shroudHi.getHeight());

	handleBridgeUp.Fly(tH * 0.46f);
	handleBridgeUp.Strafe(bR * 0.2f);
	handleBridgeDown.Fly(tH * 0.15f);
	handleBridgeDown.Strafe(handleBridgeUp.getPosition().y);
	handle.Fly(tH * 0.547f);
	handle.Strafe(bR * 0.306f);
	handle.Follow(bR * 0.0059f);
	handle.PitchTo((FLOAT)M_PI);
	handle.YawTo((FLOAT)M_PI_2);
	handleTop.Fly(handle.getPosition().z);
	handleTop.Strafe(handle.getPosition().y);
	handleTop.YawTo((FLOAT)M_PI_2);

	head.Fly(bridge.getPosition().z + bridge.getHeight() + head.getBRadius());
	head.Follow(-head.getHeight() / 2);
	head.PitchTo((FLOAT)M_PI_2);
	headFront.Fly(head.getPosition().z);
	headFront.Follow(-head.getPosition().x);
	headFront.PitchTo((FLOAT)M_PI_2);
	headBack.Translate(&head.getPosition());
	headBack.PitchTo((FLOAT)M_PI_2);
	core.Fly(head.getPosition().z);
	core.Follow(-bR * 0.235f);
	core.PitchTo((FLOAT)M_PI_2);

	addMesh(MIC_BASE,		base);
	addMesh(MIC_BUTTON_L,	buttonL);
	addMesh(MIC_BUTTON_R,	buttonR);
	addMesh(MIC_UPRIGHT,	upright);
	addMesh(MIC_SHROUD_LOW,	shroudLow);
	addMesh(MIC_SHROUD_HI,	shroudHi);
	addMesh(MIC_BRIDGE,		bridge);
	addMesh(MIC_HANDLE_BU,	handleBridgeUp);
	addMesh(MIC_HANDLE_BD,	handleBridgeDown);
	addMesh(MIC_HANDLE,		handle);
	addMesh(MIC_HANDLE_TOP,	handleTop);
	addMesh(MIC_HEAD,		head);
	addMesh(MIC_HEAD_FRONT,	headFront);
	addMesh(MIC_HEAD_BACK,	headBack);
	addMesh(MIC_CORE,		core);

	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

Microphone::~Microphone() {
	delete [] micParts;				
}

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
		for (int i = 0; i < num; i++ )
		ps[i] += vNum;
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

void Microphone::setBaseRadius (float r) {
	bR = r;
	ExCone	base(bH, bR, bR, bR * 0.824f, 24);
	base.YawTo(-(FLOAT)M_PI_2);
	replaceMesh(MIC_BASE, base);

	micParts[MIC_BUTTON_L]->Translate(bR * 0.824f, -bR * 0.2588f, bH * 1.1f);
	micParts[MIC_BUTTON_R]->Translate(bR * 0.824f, bR * 0.2588f, bH * 1.1f);

	recalcMeshVertices(MIC_BUTTON_L, *micParts[MIC_BUTTON_L]);
	recalcMeshVertices(MIC_BUTTON_R, *micParts[MIC_BUTTON_R]);
}
void Microphone::setBaseHeight (float h) {
	float shift = h - bH;
	bH = h;
	ExCone	base(bH, bR, bR, bR * 0.824f, 24);
	base.YawTo(-(FLOAT)M_PI_2);
	replaceMesh(MIC_BASE, base);

	for (int i = MIC_BUTTON_L; i < PARTS_NUM; i++){
		micParts[i]->Translate(micParts[i]->getPosition().x, micParts[i]->getPosition().y, micParts[i]->getPosition().z + shift);
		recalcMeshVertices((MIC_PART)i, *micParts[i]);
	}
}
void Microphone::setButtonWidth (float w) {
	bW = w;
	Pyramid			buttonL(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, bW * 0.1375f),
					buttonR(bH * 0.6f, bW * 0.575f, bW, bW * 0.3f, bW, bW * 0.1375f);
	buttonL.Translate(bR * 0.824f, -bR * 0.2588f, bH * 1.1f);
	buttonL.PitchTo((FLOAT)M_PI);
	buttonR.Translate(bR * 0.824f, bR * 0.2588f, bH * 1.1f);
	buttonR.PitchTo((FLOAT)M_PI);
	replaceMesh(MIC_BUTTON_L, buttonL);
	replaceMesh(MIC_BUTTON_R, buttonR);
}
void Microphone::setUprightRadius (float r) {}
void Microphone::setUprightHeight (float h) {}
void Microphone::setUprightGap (float g) {}
void Microphone::setHandleIndent (float i) {}
void Microphone::setHeadRadius (float r) {}
void Microphone::setHeadDepth (float d) {}
void Microphone::setCoreRadius (float r) {}


/* ---------------------------------- getters ---------------------------------- */
float Microphone::getBaseRadius		() { return bR; }
float Microphone::getBaseHeight		() { return bH; }
float Microphone::getButtonWidth	() { return bW; }
float Microphone::getUprightRadius	() { return uR; }
float Microphone::getUprightHeight	() { return uH; }
float Microphone::getUprightGap		() { return uG; }
float Microphone::getHandleIndent	() { return hI; }
float Microphone::getHeadRadius		() { return hR; }
float Microphone::getHeadDepth		() { return hD; }
float Microphone::getCoreRadius		() { return cR; }
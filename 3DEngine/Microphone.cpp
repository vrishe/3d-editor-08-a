#include "Microphone.h"

// ============================================================================
// Implementation of Microphone class:
Microphone::Microphone(unsigned char red, unsigned char green, unsigned char blue, float bR, float tH) : clsMesh(red, green, blue) {
	base			= new ExCone (tH * 0.062f, bR, bR, bR * 0.824f, 24);
	buttonL			= new Pyramid(tH * 0.037f, bR * 0.27f, bR * 0.47f, bR * 0.1412f, bR * 0.47f, 5.5);
	buttonR			= new Pyramid(tH * 0.037f, bR * 0.27f, bR * 0.47f, bR * 0.1412f, bR * 0.47f, 5.5);

	upright			= new Cone(tH * 0.2057f,	 bR * 0.0941f, bR * 0.0941f, 24);
	shroudLow		= new Cone(tH * 0.12345679f, bR * 0.1412f, bR * 0.1412f, 24);
	shroudHi		= new Cone(tH * 0.37f,		 bR * 0.1412f, bR * 0.1412f, 24);
	bridge			= new Cone(tH * 0.0494f,	 bR * 0.0941f, bR * 0.0588f, 24);

	handleBridgeUp	= new Pyramid(tH * 0.0535f, bR * 0.1682f, bR * 0.0941f, bR * 0.1682f, bR * 0.0941f);
	handleBridgeDown= new Pyramid(tH * 0.045f,	bR * 0.1682f, bR * 0.0941f, bR * 0.1682f, bR * 0.0941f);
	handle			= new Pyramid(tH * 0.444f,	bR * 0.112f,  bR * 0.0765f, bR * 0.0765f, bR * 0.0765f, bR * 0.0174f);
	handleTop		= new Pyramid(tH * 0.033f,   bR * 0.112f,  bR * 0.0765f, bR * 0.112f,  bR * 0.0765f,-bR * 0.05f);

	head			= new Hole(bR * 0.353f,  tH * 0.165f, tH * 0.12345679f, tH * 0.165f, tH * 0.12345679f, 24);
	headFront		= new Hole(bR * 0.0353f, tH * 0.177f, tH * 0.12345679f, tH * 0.177f, tH * 0.1111f,	   24);
	headBack		= new Hole(bR * 0.0353f, tH * 0.177f, tH * 0.12345679f, tH * 0.177f, tH * 0.1111f,	   24);
	core			= new Cone(bR * 0.506f,  tH * 0.12345679f, tH * 0.12345679f, 24);

	base->YawTo(-(FLOAT)M_PI_2);

	buttonL->Fly(tH * 0.0679f); 	
	buttonL->Follow(bR * 0.824f);
	buttonL->Strafe(-bR * 0.2588f);
	buttonL->PitchTo((FLOAT)M_PI);
	buttonR->Fly(tH * 0.0679f); 	
	buttonR->Follow(bR * 0.824f);
	buttonR->Strafe(bR * 0.2588f);
	buttonR->PitchTo((FLOAT)M_PI);

	upright->Fly(base->getHeight());
	shroudLow->Fly(tH * 0.0823f);
	shroudHi->Fly(tH * 0.2263f);
	bridge->Fly(shroudHi->getPosition().z + shroudHi->getHeight());

	handleBridgeUp->Fly(tH * 0.46f);
	handleBridgeUp->Strafe(bR * 0.2f);
	handleBridgeDown->Fly(tH * 0.15f);
	handleBridgeDown->Strafe(handleBridgeUp->getPosition().y);
	handle->Fly(tH * 0.547f);
	handle->Strafe(bR * 0.306f);
	handle->Follow(bR * 0.0059f);
	handle->PitchTo((FLOAT)M_PI);
	handle->YawTo((FLOAT)M_PI_2);
	handleTop->Fly(handle->getPosition().z);
	handleTop->Strafe(handle->getPosition().y);
	handleTop->YawTo((FLOAT)M_PI_2);

	head->Fly(bridge->getPosition().z + bridge->getHeight() + head->getBRadius());
	head->Follow(-head->getHeight() / 2);
	head->PitchTo((FLOAT)M_PI_2);
	headFront->Fly(head->getPosition().z);
	headFront->Follow(-head->getPosition().x);
	headFront->PitchTo((FLOAT)M_PI_2);
	headBack->Translate(&head->getPosition());
	headBack->PitchTo((FLOAT)M_PI_2);
	core->Fly(head->getPosition().z);
	core->Follow(-bR * 0.235f);
	core->PitchTo((FLOAT)M_PI_2);

	Triangulate();
}

Microphone::~Microphone() {
	delete	base;				
	delete	upright;
	delete	shroudHi;
	delete	shroudLow;
	delete	bridge;
	delete	core;
	delete	head;
	delete	headFront;
	delete	headBack;		
	delete  buttonL;
	delete	buttonR;
	delete	handle;
	delete	handleBridgeUp;
	delete	handleBridgeDown;
	delete	handleTop;
}

void Microphone::addMesh(LPMESH3D m) {
	LPVECTOR3D vs = new VECTOR3D[m->getVerticesCount()];
	m->getVerticesTransformed(vs);
	int alredy = vertices.size();
	vertices.insert(vertices.end(), vs, vs + m->getVerticesCount());
	delete [] vs;

	EDGE_LIST es = m->getEdges();
	int num = es.size();
	for (int i = 0; i < num; i++ ) {
		es[i].first += alredy; es[i].second += alredy;
	}
	edges.insert(edges.end(), es.begin(), es.end());

	POLY_LIST ps = m->getPolygons();
	num = ps.size();
		for (int i = 0; i < num; i++ ) {
		ps[i].first += alredy; ps[i].second += alredy; ps[i].third += alredy;
	}
	polygons.insert(polygons.end(), ps.begin(), ps.end());
}

void Microphone::Triangulate() {
	addMesh(base);
	addMesh(buttonL);
	addMesh(buttonR);
	addMesh(upright);
	addMesh(shroudLow);
	addMesh(shroudHi);
	addMesh(bridge);
	addMesh(handleBridgeUp);
	addMesh(handleBridgeDown);
	addMesh(handle);
	addMesh(handleTop);
	addMesh(head);
	addMesh(headFront);
	addMesh(headBack);
	addMesh(core);

	vertices.shrink_to_fit();
	edges.shrink_to_fit();
	polygons.shrink_to_fit();
}

void setBaseRadius (float) {

}

/* ---------------------------------- setters ---------------------------------- */
void Microphone::setBaseRadius (float bR) {}
void Microphone::setBaseHeight (float bH) {}
void Microphone::setButtonWidth (float bW) {}
void Microphone::setUprightRadius (float uR) {}
void Microphone::setUprightHeight (float uH) {}
void Microphone::setUprightGap (float uG) {}
void Microphone::setHandleIndent (float hI) {}
void Microphone::setHeadRadius (float hR) {}
void Microphone::setHeadDepth (float hD) {}
void Microphone::setCoreRadius (float cR) {}
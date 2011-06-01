#include "Microphone.h"

Microphone::Microphone(unsigned char red, unsigned char green, unsigned char blue) : clsMesh(red, green, blue) {
	// make a microphone here!
	base			= new ExCone(15, 85, 85, 70, 24);
	buttonL			= new Pyramid(9, 40, 8, 40, 23);
	buttonR			= new Pyramid(9, 40, 8, 40, 23);

	upright			= new Cone(50, 8, 8, 24);
	shroudLow		= new Cone(30, 12, 12, 24);
	shroudHi		= new Cone(90, 12, 12, 24);
	bridge			= new Cone(12, 8, 5, 24);

	handleBridgeUp	= new Pyramid(13, 9, 5, 9, 5);
	handleBridgeDown= new Pyramid(10, 9, 5, 9, 5);
	handle			= new Pyramid(108, 9.5, 6.5, 6.5, 6.5, -1.5);
	handleTop		= new Pyramid(8.f, 9.5, 6.5, 9.5, 6.5, 4.3);

	head			= new Hole(30, 40, 30, 40, 30, 24);
	headFront		= new Hole(3,  43, 30, 43, 27, 24);
	headBack		= new Hole(3,  43, 30, 43, 27, 24);
	core			= new Cone(39, 30, 30, 24);

	buttonL->Fly(7.5); 
	buttonL->Strafe(70); buttonL->Follow(-22.);
	buttonR->Fly(7.5); 	
	buttonR->Strafe(70); buttonR->Follow(22.);

	upright->Fly(15);
	shroudLow->Fly(20);
	shroudHi->Fly(55);
	bridge->Fly(145);

	handleBridgeUp->Fly(112);
	handleBridgeUp->Follow(-17);
	handleBridgeDown->Fly(36.5);
	handleBridgeDown->Follow(-17);
	handle->Fly(133);
	handle->Follow(-26);
	handle->PitchTo((FLOAT)M_PI);
	handleTop->Fly(133);
	handleTop->Follow(-26);

	head->Fly(197);
	head->Strafe(-15);
	head->PitchTo(90.0f * (FLOAT)M_PI / 180.0f);
	head->YawTo(90.0f * (FLOAT)M_PI / 180.0f);
	headFront->Fly(197);
	headFront->Strafe(15);
	headFront->PitchTo(90.0f * (FLOAT)M_PI / 180.0f);
	headFront->YawTo(90.0f * (FLOAT)M_PI / 180.0f);
	headBack->Fly(197);
	headBack->Strafe(-15);
	headBack->PitchTo(90.0f * (FLOAT)M_PI / 180.0f);
	headBack->YawTo(90.0f * (FLOAT)M_PI / 180.0f);
	core->Fly(197);
	core->Strafe(-18);
	core->PitchTo(90.0f * (FLOAT)M_PI / 180.0f);
	core->YawTo(90.0f * (FLOAT)M_PI / 180.0f);

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
}

Microphone::Microphone(float bR, float totalH) : clsMesh() {
	// make a microphone here!
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
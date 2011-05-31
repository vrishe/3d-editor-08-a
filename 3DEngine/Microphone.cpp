#include "Microphone.h"

Microphone::Microphone() : clsMesh(240,240,240) {
	// make a microphone here!
	base			= ExCone(15, 90, 90, 70, 24);
	button1			= Pyramid(9, 23, 8, 44, 23);
	button2			= Pyramid(9, 23, 8, 44, 23);

	upright			= Cone(50, 8, 8, 24);
	shroudLow		= Cone(30, 12, 12, 24);
	shroudHi		= Cone(90, 12, 12, 24);
	bridge			= Cone(12, 8, 5, 24);

	head;

	button1.Fly(7); button1.Strafe(70); button1.Follow(-23.5);
	button2.Fly(7); button2.Strafe(70); button2.Follow(23.5);
	upright.Fly(15);
	shroudLow.Fly(20);
	shroudHi.Fly(55);
	bridge.Fly(145);

	addMesh(&base);
	addMesh(&button1);
	addMesh(&button2);
	addMesh(&upright);
	addMesh(&shroudLow);
	addMesh(&shroudHi);
	addMesh(&bridge);	
}

Microphone::Microphone(float bR, float totalH) : clsMesh() {
	// make a microphone here!
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
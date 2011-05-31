#pragma once

#include "Primitives.h"

class Microphone : public clsMesh {
	ExCone	base;			// 0
	Cone	upright,		// 3
			shroudHi,		// 5
			shroudLow,		// 4
			bridge,			// 6
			core;
	Hole	head,			// 7
			headFront,
			headBack;
	Pyramid button1,		// 1
			button2,		// 2
			handle,
			handleBridge1,
			handleBridge2;

	void addMesh(LPMESH3D); // adding vertices, edges and polygons to global microphone lists

public:
	Microphone();
	Microphone(float bR, float totalH);

	// setters
	void setBaseRadius		(float);
	void setBaseHeight		(float);
	void setButtonWidth		(float);
	void setUprightRadius	(float);
	void setUprightHeight	(float);
	void setUprightGap		(float);
	void setHandleIndent	(float);
	void setHeadRadius		(float);
	void setHeadDepth		(float);
	void setCoreRadius		(float);
};
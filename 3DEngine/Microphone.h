#pragma once

#include "Primitives.h"

// ============================================================================
// Microphone class
class Microphone : public clsMesh {
	ExCone	*base;				// 0
	Cone	*upright,			// 3
			*shroudHi,			// 5
			*shroudLow,			// 4
			*bridge,			// 6
			*core;				// 13
	Hole	*head,				// 10
			*headFront,			// 11
			*headBack;			// 12
	Pyramid *buttonL,			// 1
			*buttonR,			// 2
			*handle,			// 9
			*handleBridgeUp,	// 7
			*handleBridgeDown,	// 8
			*handleTop;			// 14

	void addMesh(LPMESH3D); // adding vertices, edges and polygons to global microphone lists

public:
	Microphone(
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff
		);
	Microphone(float bR, float totalH);
	~Microphone();

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
typedef Microphone MICROPHONE3D, *LPMICROPHONE3D;
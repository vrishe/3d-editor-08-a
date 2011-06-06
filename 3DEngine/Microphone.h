#pragma once

#include "Primitives.h"

#define PARTS_NUM 15

enum MIC_PART {
	MIC_BASE		= 0,
	MIC_BUTTON_L	= 1,
	MIC_BUTTON_R	= 2,
	MIC_UPRIGHT		= 3,
	MIC_SHROUD_LOW	= 4,
	MIC_SHROUD_HI	= 5,
	MIC_BRIDGE		= 6,
	MIC_HANDLE_BU	= 7,
	MIC_HANDLE_BD	= 8,
	MIC_HANDLE		= 9,
	MIC_HANDLE_TOP	= 10,
	MIC_HEAD		= 11,
	MIC_HEAD_FRONT	= 12,
	MIC_HEAD_BACK	= 13,
	MIC_CORE		= 14
};

// ============================================================================
// Microphone class
class Microphone : public clsMesh {
	LPMESH3D	*micParts;
	float		tH,			// total height
				bR,			// base radius
				bH,			// base height
				bW,			// button width
				uR,			// upright radius
				uH,			// upright height
				uG,			// upright gap
				hI,			// handle indent
				hR,			// head radius
				hD,			// head depth
				cR;			// core radius
	int			precission;


	bool getMeshPosition(
		MIC_PART part,
		int& vs,
		int& es,
		int& ps
		);

	void addMesh	(MIC_PART part, MESH3D&); // adding vertices, edges and polygons as a specific part
	void deleteMesh (MIC_PART part);

	void replaceMesh		(MIC_PART part, MESH3D&);
	void recalcMeshVertices (MIC_PART part, MESH3D&);

public:
	Microphone(
		unsigned char red	= 0xff,
		unsigned char green	= 0xff,
		unsigned char blue	= 0xff,
		float r = 85, 
		float h = 243,
		int prec = 24
		);
	~Microphone();

	// functionality
	virtual void Triangulate(); // generate vertices, edges and polygons lists

	// setters
	void setTotalHeight		(float);
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
	void setPrecission		(int);

	// getters
	float getTotalHeight	();
	float getBaseRadius		();
	float getBaseHeight		();
	float getButtonWidth	();
	float getUprightRadius	();
	float getUprightHeight	();
	float getUprightGap		();
	float getHandleIndent	();
	float getHeadRadius		();
	float getHeadDepth		();
	float getCoreRadius		();
	int getPrecission		();
};
typedef Microphone MICROPHONE3D, *LPMICROPHONE3D;
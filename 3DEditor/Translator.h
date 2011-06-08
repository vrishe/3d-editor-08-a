#pragma once

#include "3DEditor.h"
#include <string>

class clsTranslator {
private:
		// write to file
	static void pasteColor(COLOR3D, FILE*);
	static void pasteName(LPOBJECT3D, FILE*);

	static void pastePosition(LPOBJECT3D, FILE*);
	static void pasteRotation(LPOBJECT3D, FILE*);

	static void pastePyramid(LPPYRAMID3D, FILE*);
	static void pasteCone(LPCONE3D, FILE*);
	static void pasteExCone(LPEXCONE3D, FILE*);
	static void pasteHole(LPHOLE3D, FILE*);
	static void pasteMic(LPMICROPHONE3D, FILE*);

	static void pasteCam(LPCAMERA3D, FILE*);
	static void pasteLight(LPDIFLIGHT3D, FILE*);

		// read from file
	static VECTOR3D readVector(FILE*);
	static void readPosition(LPOBJECT3D, FILE*);
	static void readRotation(VECTOR3D& fwd, VECTOR3D& uwd, VECTOR3D& rwd, FILE*);
	static MESH_ID readMeshID(FILE*);

	//static void readPyramid(LPPYRAMID3D, FILE* dataFile, LPSCENE3D);
	//static void readCone(LPCONE3D, FILE* dataFile, LPSCENE3D Scene);
	//static void readExCone(LPEXCONE3D, FILE* dataFile, LPSCENE3D Scene);
	//static void readHole(LPHOLE3D, FILE* dataFile, LPSCENE3D Scene);
	static void readMic(FILE* dataFile, LPSCENE3D);
public:
	static bool saveSceneScript(LPSCENE3D, TCHAR *fileName);
	static bool loadSceneScript(LPSCENE3D, TCHAR *fileName);
};
typedef clsTranslator TRANSLATOR3D, *LPTRANSLATOR3D;
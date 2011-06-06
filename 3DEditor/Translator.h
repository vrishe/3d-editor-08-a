#pragma once

#include "Internal.h"
#include "External.h"S
#include <string>

class clsTranslator {
private:
		// write to file
	static bool pasteFloat(float, HANDLE hFile);
	static bool pasteColor(COLOR3D, HANDLE hFile);
	static bool pasteName(LPOBJECT3D, HANDLE hFile);

	static bool pastePosition(LPOBJECT3D, HANDLE hFile);
	static bool pasteRotation(LPOBJECT3D, HANDLE hFile);

	static bool pastePyramid(LPPYRAMID3D, HANDLE hFile);
	static bool pasteCone(LPCONE3D, HANDLE hFile);
	static bool pasteExCone(LPEXCONE3D, HANDLE hFile);
	static bool pasteHole(LPHOLE3D, HANDLE hFile);
	static bool pasteMic(LPMICROPHONE3D, HANDLE hFile);

		// read from file
	static float readFloat(string&);
	static VECTOR3D readVector(string&);
	static void readPosition(LPOBJECT3D, string&);
	static void readRotation(VECTOR3D& fwd, VECTOR3D& uwd, VECTOR3D& rwd, string&);
	static MESH_ID readMeshID(string&);

	static void readPyramid(LPPYRAMID3D, string& dataFile, LPSCENE3D);
	static void readCone(LPCONE3D, string& dataFile, LPSCENE3D Scene);
	static void readExCone(LPEXCONE3D, string& dataFile, LPSCENE3D Scene);
	static void readHole(LPHOLE3D, string& dataFile, LPSCENE3D Scene);
	static void readMic(LPMICROPHONE3D, string& dataFile, LPSCENE3D);
public:
	static bool saveSceneScript(LPSCENE3D, TCHAR *fileName);
	static bool loadSceneScript(LPSCENE3D, TCHAR *fileName);
};
typedef clsTranslator TRANSLATOR3D, *LPTRANSLATOR3D;
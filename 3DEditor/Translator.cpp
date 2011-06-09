
#include "Translator.h"

#define _SCL_SECURE_NO_WARNINGS

// ============================================================================
// Implementation of clsTranslator class:
// write to file
void clsTranslator::pasteName(LPOBJECT3D obj, FILE* file) {
	TCHAR *dataBuffer = new TCHAR[MAX_OBJECT_NAME_LEN];
	obj->getName(dataBuffer, 256);
	
	errno_t bErrorFlag = _ftprintf_s(file, _T("Name: %s\n"), dataBuffer );
	
	dataBuffer[MAX_OBJECT_NAME_LEN - 1] = '\0';
	delete [] dataBuffer;
}
void clsTranslator::pasteColor(COLOR3D in, FILE* file) {
	errno_t bErrorFlag = _ftprintf_s(file, _T("Color: %d %d %d\n"), in.Red, in.Green, in.Blue );
}
void clsTranslator::pastePosition(LPOBJECT3D obj, FILE* file) {
	VECTOR3D pos = obj->getPosition();
	errno_t bErrorFlag = _ftprintf_s(file, _T("Position: %.3f %.3f %.3f\n"), pos.x, pos.y, pos.z );
}
void clsTranslator::pasteRotation(LPOBJECT3D obj, FILE* file) {
	VECTOR3D fWd = obj->getForwardLookDirection();
	VECTOR3D uWd = obj->getUpLookDirection();
	VECTOR3D rWd = obj->getRightLookDirection();

	errno_t bErrorFlag = _ftprintf_s(file, _T("View vectors:\nForward: %.3f %.3f %.3f\nUpward: %.3f %.3f %.3f\nRightward: %.3f %.3f %.3f\n"), 
											fWd.x, fWd.y, fWd.z, uWd.x, uWd.y, uWd.z, rWd.x, rWd.y, rWd.z );
}

void clsTranslator::pastePyramid(LPPYRAMID3D obj, FILE* file) {
	errno_t bErrorFlag = _ftprintf_s(file, _T("------------------------------------- PYRAMID --------------------------------------\n") );

	pasteName(obj, file);
	pastePosition(obj, file);
	pasteRotation(obj, file);

	bErrorFlag = _ftprintf_s(file, _T("Height: %.3f\n"), obj->getHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Base length: %.3f\n"), obj->getBLength() );
	bErrorFlag = _ftprintf_s(file, _T("Base width: %.3f\n"), obj->getBWidth() );
	bErrorFlag = _ftprintf_s(file, _T("Top length: %.3f\n"), obj->getTLength() );
	bErrorFlag = _ftprintf_s(file, _T("Top width: %.3f\n"), obj->getTWidth() );
	pasteColor(obj->getColor(), file);
}
void clsTranslator::pasteCone(LPCONE3D obj, FILE* file) {
	errno_t bErrorFlag = _ftprintf_s(file, _T("------------------------------------- CONE --------------------------------------\n") );

	pasteName(obj, file);
	pastePosition(obj, file);
	pasteRotation(obj, file);

	bErrorFlag = _ftprintf_s(file, _T("Height: %.3f\n"), obj->getHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Base radius: %.3f\n"), obj->getBRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Top radius: %.3f\n"), obj->getTRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Precission: %.3f\n"), obj->getPrecission() );
	pasteColor(obj->getColor(), file);
}
void clsTranslator::pasteExCone(LPEXCONE3D obj, FILE* file) {
	errno_t bErrorFlag = _ftprintf_s(file, _T("------------------------------------- EXCONE --------------------------------------\n") );

	pasteName(obj, file);
	pastePosition(obj, file);
	pasteRotation(obj, file);

	bErrorFlag = _ftprintf_s(file, _T("Height: %.3f\n"), obj->getHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Base radius: %.3f\n"), obj->getBRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Top radius: %.3f\n"), obj->getTRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Secant: %.3f\n"), obj->getSecant() );
	bErrorFlag = _ftprintf_s(file, _T("Precission: %.3f\n"), obj->getPrecission() );
	pasteColor(obj->getColor(), file);
}
void clsTranslator::pasteHole(LPHOLE3D obj, FILE* file) {
	errno_t bErrorFlag = _ftprintf_s(file, _T("------------------------------------- HOLE --------------------------------------\n") );

	pasteName(obj, file);
	pastePosition(obj, file);
	pasteRotation(obj, file);

	bErrorFlag = _ftprintf_s(file, _T("Height: %.3f\n"), obj->getHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Base radius: %.3f\n"), obj->getBRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Base hole radius: %.3f\n"), obj->getBHoleRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Top radius: %.3f\n"), obj->getTRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Top hole radius: %.3f\n"), obj->getTHoleRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Precission: %.3f\n"), obj->getPrecission() );
	pasteColor(obj->getColor(), file);
}
void clsTranslator::pasteMic(LPMICROPHONE3D obj, FILE* file) {
	errno_t bErrorFlag = _ftprintf_s(file, _T("------------------------------------- MICROPHONE --------------------------------------\n") );

	pasteName(obj, file);
	pastePosition(obj, file);
	pasteRotation(obj, file);

	bErrorFlag = _ftprintf_s(file, _T("Total height: %.3f\n"), obj->getTotalHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Base radius: %.3f\n"), obj->getBaseRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Base height: %.3f\n"), obj->getBaseHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Button width: %.3f\n"), obj->getButtonWidth() );
	bErrorFlag = _ftprintf_s(file, _T("Upright radius: %.3f\n"), obj->getUprightRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Upright height: %.3f\n"), obj->getUprightHeight() );
	bErrorFlag = _ftprintf_s(file, _T("Upright gap: %.3f\n"), obj->getUprightGap() );
	bErrorFlag = _ftprintf_s(file, _T("Handle indent: %.3f\n"), obj->getHandleIndent() );
	bErrorFlag = _ftprintf_s(file, _T("Head radius: %.3f\n"), obj->getHeadRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Head deapth: %.3f\n"), obj->getHeadDepth() );
	bErrorFlag = _ftprintf_s(file, _T("Core radius: %.3f\n"), obj->getCoreRadius() );
	bErrorFlag = _ftprintf_s(file, _T("Precission: %.3f\n"), obj->getPrecission() );
	pasteColor(obj->getColor(), file);
}

void clsTranslator::pasteCam(LPCAMERA3D obj, FILE* file) {
	pasteName(obj, file);
	pastePosition(obj, file);
	pasteRotation(obj, file);
	errno_t bErrorFlag;
	bErrorFlag = _ftprintf_s(file, _T("hFOV: %.3f\n"), obj->getHFov() );
	bErrorFlag = _ftprintf_s(file, _T("vFOV: %.3f\n"), obj->getVFov() );
	bErrorFlag = _ftprintf_s(file, _T("Near clip: %.3f\n"), obj->getNearCP() );
	bErrorFlag = _ftprintf_s(file, _T("Far clip: %.3f\n"), obj->getFarCP() );
}
void clsTranslator::pasteLight(LPDIFLIGHT3D obj, FILE* file) {
	pasteName(obj, file);
	pasteRotation(obj, file);
	errno_t bErrorFlag;
	bErrorFlag = _ftprintf_s(file, _T("Power: %.3f\n"), obj->getPower() );
	pasteColor(obj->getColor(), file);
}

// read from file
VECTOR3D clsTranslator::readVector(FILE* file) {
	float x, y, z;
	_ftscanf_s(file, _T("%f"), &x);
	_ftscanf_s(file, _T("%f"), &y);
	_ftscanf_s(file, _T("%f"), &z);
	return VECTOR3D(x, y, z);
}
void clsTranslator::readPosition(LPOBJECT3D obj, FILE* file) {
	VECTOR3D v = readVector(file);
	obj->Translate(v);	
}
void clsTranslator::readRotation(VECTOR3D& fwd, VECTOR3D& uwd, VECTOR3D& rwd, FILE* file) {
	TCHAR buffer[256];
	fwd = readVector(file);
	_ftscanf_s(file, _T("%s"), buffer, 256);

	uwd = readVector(file);
	_ftscanf_s(file, _T("%s"), buffer, 256);

	rwd = readVector(file);
}
MESH_ID clsTranslator::readMeshID(FILE* file) {
	TCHAR name[256];
	errno_t bErrorFlag = _ftscanf_s(file, _T("%s"), name, 256);

	if ( !_tcscmp(name, _T("MICROPHONE")) )
		return MSH_MIC;
	if ( !_tcscmp(name, _T("PYRAMID")) )
		return MSH_PYRAMID;
	if ( !_tcscmp(name, _T("CONE")) ) 
		return MSH_CONE;
	if ( !_tcscmp(name, _T("EXCONE")) )
		return MSH_EXCONE;
	if ( !_tcscmp(name, _T("HOLE")) )
		return MSH_HOLE;

	return MSH_MIC;
}

//void clsTranslator::readPyramid(LPPYRAMID3D obj, FILE* file, LPSCENE3D Scene) {
//	VECTOR3D pos = readVector(file);
//	file.erase(0, 27);
//	VECTOR3D fwd, rwd, uwd;
//	readRotation(fwd, uwd, rwd, file);
//	file.erase(0, 10);
//	obj->Translate(pos);
//	obj->setForwardLookDirection(&fwd);
//	obj->setUpLookDirection(&uwd);
//	obj->setRightLookDirection(&rwd);
//
//	float h = readFloat(file);
//	file.erase(0, 14);
//	float bl = readFloat(file);
//	file.erase(0, 13);
//	float bw = readFloat(file);
//	file.erase(0, 13);
//	float tl = readFloat(file);
//	file.erase(0, 12);
//	float tw = readFloat(file);
//	file.erase(0, 14);
//	pos = readVector(file);
//	obj->setColor((UCHAR)pos.x, (UCHAR)pos.y, (UCHAR)pos.z);
//
//	obj = new PYRAMID3D(h, bl, bw, tl, tw);
//	Scene->AddObject(obj);
//}
//void clsTranslator::readCone(LPCONE3D obj, FILE* file, LPSCENE3D Scene) {
//	VECTOR3D pos = readVector(file);
//	file.erase(0, 27);
//	VECTOR3D fwd, rwd, uwd;
//	readRotation(fwd, uwd, rwd, file);
//	file.erase(0, 10);
//	obj->Translate(pos);
//	obj->setForwardLookDirection(&fwd);
//	obj->setUpLookDirection(&uwd);
//	obj->setRightLookDirection(&rwd);
//
//	float h = readFloat(file);
//	file.erase(0, 14);
//	float br = readFloat(file);
//	file.erase(0, 13);
//	float tr = readFloat(file);
//	file.erase(0, 13);
//	float prec = readFloat(file);
//	file.erase(0, 14);
//	pos = readVector(file);
//	obj->setColor((UCHAR)pos.x, (UCHAR)pos.y, (UCHAR)pos.z);
//
//	obj = new CONE3D(h, br, tr, (UINT)prec);
//	Scene->AddObject(obj);
//}
//void clsTranslator::readExCone(LPEXCONE3D obj, FILE* file, LPSCENE3D Scene) {
//	VECTOR3D pos = readVector(file);
//	file.erase(0, 27);
//	VECTOR3D fwd, rwd, uwd;
//	readRotation(fwd, uwd, rwd, file);
//	file.erase(0, 10);
//	obj->Translate(pos);
//	obj->setForwardLookDirection(&fwd);
//	obj->setUpLookDirection(&uwd);
//	obj->setRightLookDirection(&rwd);
//
//	float h = readFloat(file);
//	file.erase(0, 14);
//	float br = readFloat(file);
//	file.erase(0, 13);
//	float tr = readFloat(file);
//	file.erase(0, 9);
//	float sec = readFloat(file);
//	file.erase(0, 13);
//	float prec = readFloat(file);
//	file.erase(0, 14);
//	pos = readVector(file);
//	obj->setColor((UCHAR)pos.x, (UCHAR)pos.y, (UCHAR)pos.z);
//
//	obj = new EXCONE3D(h, br, tr, sec, (UINT)prec);
//	Scene->AddObject(obj);
//}
//void clsTranslator::readHole(LPHOLE3D obj, FILE* file, LPSCENE3D Scene) {
//	VECTOR3D pos = readVector(file);
//	file.erase(0, 27);
//	VECTOR3D fwd, rwd, uwd;
//	readRotation(fwd, uwd, rwd, file);
//	file.erase(0, 10);
//	obj->Translate(pos);
//	obj->setForwardLookDirection(&fwd);
//	obj->setUpLookDirection(&uwd);
//	obj->setRightLookDirection(&rwd);
//
//	float h = readFloat(file);
//	file.erase(0, 14);
//	float br = readFloat(file);
//	file.erase(0, 19);
//	float bh = readFloat(file);
//	file.erase(0, 13);
//	float tr = readFloat(file);
//	file.erase(0, 18);
//	float th = readFloat(file);
//	file.erase(0, 13);
//	float prec = readFloat(file);
//	file.erase(0, 14);
//	pos = readVector(file);
//	obj->setColor((UCHAR)pos.x, (UCHAR)pos.y, (UCHAR)pos.z);
//
//	obj = new HOLE3D(h, br, bh, tr, th, (UINT)prec);
//	Scene->AddObject(obj);
//}
void clsTranslator::readMic(FILE* file, LPSCENE3D Scene) {
	LPMICROPHONE3D obj;
	TCHAR buffer[256], name[256];
	errno_t bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), name, 256);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	VECTOR3D pos = readVector(file);

	bErrorFlag = _ftscanf_s(file, _T("%24c"), buffer);
	VECTOR3D fwd, rwd, uwd;
	readRotation(fwd, uwd, rwd, file);

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
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &tH);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &bR);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &bH);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &bW);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &uR);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &uH);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &uG);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &hI);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &hR);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &hD);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &cR);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	bErrorFlag = _ftscanf_s(file, _T("%f"), &precission);

	bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	int r, g, j;
	bErrorFlag = _ftscanf_s(file, _T("%d %d %d"), &r, &g, &j);

	obj = new MICROPHONE3D(r, g, j, bR, bH, bW, uR, uH, uG, hI, hR, hD, cR);
	obj->setName(name, 256);
	obj->Translate(pos);
	obj->setForwardLookDirection(&fwd);
	obj->setUpLookDirection(&uwd);
	obj->setRightLookDirection(&rwd);

	Scene->AddObject(obj);
}

bool clsTranslator::saveSceneScript(LPSCENE3D Scene, TCHAR *fileName) {
	FILE* file;
	errno_t bErrorFlag = _wfopen_s(&file, fileName, _T("w"));
	if (bErrorFlag)
		return false;

	bErrorFlag = _ftprintf_s(file, _T("3DEditor scene file\n"), NULL );
	bErrorFlag = _ftprintf_s(file, _T("===================================== Cameras =======================================\n"), NULL );
	bErrorFlag = _ftprintf_s(file, _T("Total: "), NULL );
	UINT sceneObjCount = Scene->getObjectClassCount(CLS_CAMERA);
	bErrorFlag = _ftprintf_s(file, _T("%d\n\n"), sceneObjCount );
	
	for ( UINT i = 0; i < sceneObjCount; i++ ) {
 		pasteCam((LPCAMERA3D)Scene->getObject(CLS_CAMERA, i), file);
		bErrorFlag = _ftprintf_s(file, _T("----------------------------------------------------------------------------\n"), NULL );
	}

	bErrorFlag = _ftprintf_s(file, _T("===================================== Lighters ======================================\n"), NULL );
	bErrorFlag = _ftprintf_s(file, _T("Total: "), NULL );
	sceneObjCount = Scene->getObjectClassCount(CLS_LIGHT);
	bErrorFlag = _ftprintf_s(file, _T("%d\n\n"), sceneObjCount );
	
	for ( UINT i = 0; i < sceneObjCount; i++ ) {
		pasteLight((LPDIFLIGHT3D)Scene->getObject(CLS_LIGHT, i), file);
		bErrorFlag = _ftprintf_s(file, _T("----------------------------------------------------------------------------\n"), NULL );
	}

	bErrorFlag = _ftprintf_s(file, _T("===================================== Meshes ========================================\n"), NULL );
	bErrorFlag = _ftprintf_s(file, _T("Total: "), NULL );
	sceneObjCount = Scene->getObjectClassCount(CLS_MESH);
	bErrorFlag = _ftprintf_s(file, _T("%d\n\n"), sceneObjCount );
	
	for ( UINT i = 0; i < sceneObjCount; i++ ) {
		LPMESH3D meshToSave = (LPMESH3D)Scene->getObject(CLS_MESH, i);
		switch ( meshToSave->MeshID() ) {
		case MSH_PYRAMID:
			pastePyramid((LPPYRAMID3D)meshToSave, file);
			break;
		case MSH_CONE:
			pasteCone((LPCONE3D)meshToSave, file);
			break;
		case MSH_EXCONE:
			pasteExCone((LPEXCONE3D)meshToSave, file);
			break;
		case MSH_HOLE:
			pasteHole((LPHOLE3D)meshToSave, file);
			break;
		case MSH_MIC:
			pasteMic((LPMICROPHONE3D)meshToSave, file);
			break;
		}
	}
	bErrorFlag = _ftprintf_s(file, _T("----------------------------------------------------------------------------\n"), NULL );

	fclose(file);
	return ( bErrorFlag != 0 );
}

bool clsTranslator::loadSceneScript(LPSCENE3D Scene, TCHAR *fileName) {
	FILE* file;
	errno_t bErrorFlag = _wfopen_s(&file, fileName, _T("r"));
	if (bErrorFlag)
		return false;

	TCHAR buffer[256]; buffer[19] = '\0';
	bErrorFlag = _ftscanf_s(file, _T("%19c"), buffer);
	if ( wcscmp(buffer, _T("3DEditor scene file")) )
		return false;

	Scene->Clear();

		// loading cameras
	bErrorFlag = _ftscanf_s(file, _T("%94c\0"), buffer);
	UINT objCount;
	bErrorFlag = _ftscanf_s(file, _T("%d"), &objCount);
	for (UINT i = 0; i < objCount; i++) {
		LPCAMERA3D camera = new CAMERA3D;

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		camera->setName(buffer, 256);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		readPosition(camera, file);

		bErrorFlag = _ftscanf_s(file, _T("%24c"), buffer);
		VECTOR3D fwd, rwd, uwd;
		readRotation(fwd, uwd, rwd, file);
		camera->setForwardLookDirection(&fwd);
		camera->setUpLookDirection(&uwd);
		camera->setRightLookDirection(&rwd);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		float tmp;
		bErrorFlag = _ftscanf_s(file, _T("%f"), &tmp);
		camera->setHFov(tmp);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag =_ftscanf_s(file, _T("%f"), &tmp);
		camera->setVFov(tmp);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag = _ftscanf_s(file, _T("%f"), &tmp);
		camera->setNearCP(tmp);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag = _ftscanf_s(file, _T("%f"), &tmp);
		camera->setFarCP(tmp);

		bErrorFlag = _ftscanf_s(file, _T("%77c"), buffer, 256);
		Scene->AddObject(camera);
	}

		// loading lighters
	bErrorFlag = _ftscanf_s(file, _T("%94c\0"), buffer);
	bErrorFlag = _ftscanf_s(file, _T("%d"), &objCount);
	for (UINT i = 0; i < objCount; i++) {
		LPDIFLIGHT3D light = new DIFLIGHT3D;

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		light->setName(buffer, 256);

		bErrorFlag = _ftscanf_s(file, _T("%24c"), buffer);
		VECTOR3D fwd, rwd, uwd;
		readRotation(fwd, uwd, rwd, file);
		light->setForwardLookDirection(&fwd);
		light->setUpLookDirection(&uwd);
		light->setRightLookDirection(&rwd);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		float tmp;
		bErrorFlag = _ftscanf_s(file, _T("%f"), &tmp);
		light->setPower(tmp);

		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
		int r, g, k;
		bErrorFlag = _ftscanf_s(file, _T("%d %d %d"), &r, &g, &k);
		light->setColor(r, g, k);

		bErrorFlag = _ftscanf_s(file, _T("%77c"), buffer, 256);
		Scene->AddObject(light);
	}

		// loading meshes
	bErrorFlag = _ftscanf_s(file, _T("%94c"), buffer);
	bErrorFlag = _ftscanf_s(file, _T("%d"), &objCount);
	for (UINT i = 0; i < objCount; i++) {
		bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
	//	LPMESH3D mesh = new MESH3D(readMeshID(file));
		switch ( readMeshID(file) ) {
		/*case MSH_PYRAMID:
			readPyramid((LPPYRAMID3D)mesh, file, Scene);
			break;
		case MSH_CONE:
			readCone((LPCONE3D)mesh, file, Scene);
			break;
		case MSH_EXCONE:
			readExCone((LPEXCONE3D)mesh, file, Scene);
			break;
		case MSH_HOLE:
			readHole((LPHOLE3D)mesh, file, Scene);
			break;*/
		case MSH_MIC:
			bErrorFlag = _ftscanf_s(file, _T("%s"), buffer, 256);
			readMic(file, Scene);
			break;
		}
	}
	
	//return ( bErrorFlag != 0 );
	return true;
}

void clsTranslator::cleanSceneUp(LPSCENE3D Scene)
{
	LPOBJECT3D objToDelete;
	while ( (objToDelete = Scene->getFirstObject()) != NULL )
	{
		Scene->DeleteObject(objToDelete);
		delete objToDelete;
	}
}
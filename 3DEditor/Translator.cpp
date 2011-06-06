
#include "Translator.h"

// ============================================================================
// Implementation of clsTranslator class:
// write to file
bool clsTranslator::pasteFloat(float in, HANDLE hFile) {
	int  decimal, sign;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;
	char *dataBuffer = _fcvt( in, 5, &decimal, &sign );
	if ( sign )
		bErrorFlag = WriteFile(hFile, "-", 1, &dwBytesWritten, NULL);
	if ( strlen(dataBuffer) ) {
		if  ( decimal > 0 )
			bErrorFlag = WriteFile(hFile, dataBuffer, decimal, &dwBytesWritten, NULL);
		else
			bErrorFlag = WriteFile(hFile, "0", 1, &dwBytesWritten, NULL);
		bErrorFlag = WriteFile(hFile, ".", 1, &dwBytesWritten, NULL);
		dataBuffer += decimal;
		bErrorFlag = WriteFile(hFile, dataBuffer, (DWORD)(strlen(dataBuffer) - decimal), &dwBytesWritten, NULL);
	}
	else
		bErrorFlag = WriteFile(hFile, "0.0000", 6, &dwBytesWritten, NULL);
	return bErrorFlag;
}
bool clsTranslator::pasteName(LPOBJECT3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;
	TCHAR *dataBuffer = new TCHAR[MAX_OBJECT_NAME_LEN];

	bErrorFlag = WriteFile( hFile, "Name: ", 7, &dwBytesWritten, NULL);
	obj->getName(dataBuffer, 256 * sizeof(TCHAR));
	bErrorFlag = WriteFile(hFile, dataBuffer, (DWORD)wcslen(dataBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pasteColor(COLOR3D in, HANDLE hFile) {
	char dataBuffer[5];
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	_itoa(in.Red, dataBuffer, 10);
	bErrorFlag = WriteFile(hFile, &dataBuffer, (DWORD)strlen(dataBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	_itoa(in.Green, dataBuffer, 10);
	bErrorFlag = WriteFile(hFile, &dataBuffer, (DWORD)strlen(dataBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	_itoa(in.Blue, dataBuffer, 10);
	bErrorFlag = WriteFile(hFile, &dataBuffer, (DWORD)strlen(dataBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pastePosition(LPOBJECT3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	bErrorFlag = WriteFile( hFile, "Position: ", 10, &dwBytesWritten, NULL);
	VECTOR3D tmp = obj->getPosition();
	pasteFloat(tmp.x, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.y, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.z, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pasteRotation(LPOBJECT3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	bErrorFlag = WriteFile(hFile, "View vectors: ", 14, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, "Forward: ", 9, &dwBytesWritten, NULL);
	VECTOR3D tmp = obj->getForwardLookDirection();
	pasteFloat(tmp.x, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.y, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.z, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);

	tmp = obj->getUpLookDirection();
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, "Upward: ", 8, &dwBytesWritten, NULL);
	pasteFloat(tmp.x, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.y, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.z, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);

	tmp = obj->getRightLookDirection();
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, "Rightward: ", 11, &dwBytesWritten, NULL);
	pasteFloat(tmp.x, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.y, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	pasteFloat(tmp.z, hFile);
	bErrorFlag = WriteFile(hFile, " ", 1, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pastePyramid(LPPYRAMID3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	char split[] = "-------------------------------------- PYRAMID --------------------------------------";
	bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	pastePosition(obj, hFile);
	pasteRotation(obj, hFile);

	bErrorFlag = WriteFile( hFile, "Height: ", 8, &dwBytesWritten, NULL);
	pasteFloat(((LPPYRAMID3D)obj)->getHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base length: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPPYRAMID3D)obj)->getBLength(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base width: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPPYRAMID3D)obj)->getBWidth(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Top length: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPPYRAMID3D)obj)->getTLength(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Top width: ", 11, &dwBytesWritten, NULL);
	pasteFloat(((LPPYRAMID3D)obj)->getTWidth(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Color (RGB): ", 13, &dwBytesWritten, NULL);
	pasteColor(obj->getColor(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pasteCone(LPCONE3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	char split[] = "-------------------------------------- CONE --------------------------------------";
	bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	pastePosition(obj, hFile);
	pasteRotation(obj, hFile);

	bErrorFlag = WriteFile( hFile, "Height: ", 8, &dwBytesWritten, NULL);
	pasteFloat(((LPCONE3D)obj)->getHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base radius: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPCONE3D)obj)->getBRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Top radius: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPCONE3D)obj)->getTRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Precission: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPCONE3D)obj)->getPrecission(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Color (RGB): ", 13, &dwBytesWritten, NULL);
	pasteColor(obj->getColor(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pasteExCone(LPEXCONE3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	char split[] = "-------------------------------------- EXCONE --------------------------------------";
	bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	pastePosition(obj, hFile);
	pasteRotation(obj, hFile);

	bErrorFlag = WriteFile( hFile, "Height: ", 8, &dwBytesWritten, NULL);
	pasteFloat(((LPEXCONE3D)obj)->getHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base radius: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPEXCONE3D)obj)->getBRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Top radius: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPEXCONE3D)obj)->getTRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Secant: ", 8, &dwBytesWritten, NULL);
	pasteFloat(((LPEXCONE3D)obj)->getSecant(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Precission: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPEXCONE3D)obj)->getPrecission(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Color (RGB): ", 13, &dwBytesWritten, NULL);
	pasteColor(obj->getColor(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pasteHole(LPHOLE3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	char split[] = "-------------------------------------- HOLE --------------------------------------";
	bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	pastePosition(obj, hFile);
	pasteRotation(obj, hFile);

	bErrorFlag = WriteFile( hFile, "Height: ", 8, &dwBytesWritten, NULL);
	pasteFloat(((LPHOLE3D)obj)->getHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base radius: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPHOLE3D)obj)->getBRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base hole radius: ", 18, &dwBytesWritten, NULL);
	pasteFloat(((LPHOLE3D)obj)->getBHoleRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Top radius: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPHOLE3D)obj)->getTRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Top hole radius: ", 17, &dwBytesWritten, NULL);
	pasteFloat(((LPHOLE3D)obj)->getTHoleRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Precission: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPHOLE3D)obj)->getPrecission(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Color (RGB): ", 13, &dwBytesWritten, NULL);
	pasteColor(obj->getColor(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}
bool clsTranslator::pasteMic(LPMICROPHONE3D obj, HANDLE hFile) {
	char new_line[]="\x0D\x0A";
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag;

	char split[] = "-------------------------------------- MICROPHONE --------------------------------------";
	bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	pastePosition(obj, hFile);
	pasteRotation(obj, hFile);

	bErrorFlag = WriteFile( hFile, "Total height: ", 14, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getTotalHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base radius: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getBaseRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Base height: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getBaseHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Button width: ", 14, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getButtonWidth(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Upright radius: ", 16, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getUprightRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Upright height: ", 16, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getUprightHeight(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Upright gap: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getUprightGap(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Handle indent: ", 15, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getHandleIndent(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Head radius: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getHeadRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Head deapth: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getHeadDepth(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Core radius: ", 13, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getCoreRadius(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Precission: ", 12, &dwBytesWritten, NULL);
	pasteFloat(((LPMICROPHONE3D)obj)->getPrecission(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	bErrorFlag = WriteFile( hFile, "Color (RGB): ", 13, &dwBytesWritten, NULL);
	pasteColor(obj->getColor(), hFile);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	return bErrorFlag;
}

// read from file
float clsTranslator::readFloat(string& data) {
	size_t N = min(data.find_first_of(' '), data.find_first_of("\x0D\x0A"));
	char *tmp = new char[N + 1];
	for ( size_t i = 0; i < N; i++ )
		tmp[i] = data[i];
	tmp[N] = '\0';
	float x = atof(tmp);
	delete [] tmp;
	data.erase(0, N + 1);
	return x;
}
VECTOR3D clsTranslator::readVector(string& data) {
	float x = readFloat(data);
	float y = readFloat(data);
	float z = readFloat(data);	
	return VECTOR3D(x, y, z);
}
void clsTranslator::readPosition(LPOBJECT3D obj, string& fileData) {
	VECTOR3D v = readVector(fileData);
	obj->Translate(&v);	
}
void clsTranslator::readRotation(VECTOR3D& fwd, VECTOR3D& uwd, VECTOR3D& rwd, string& fileData) {
	fwd = readVector(fileData);
	fileData.erase(0, 10);

	uwd = readVector(fileData);
	fileData.erase(0, 13);

	rwd = readVector(fileData);
}
MESH_ID clsTranslator::readMeshID(string& fileData) {
	size_t N = fileData.find_first_of(' ');
	char *name = new char[N + 1];
	for ( size_t i = 0; i < N; i++ )
		name[i] = fileData[i];
	name[N] = '\0';
	fileData.erase(0, N + 1);

	if ( !strcmp(name, "MICROPHONE") ) {
		delete [] name;
		return MSH_MIC;
	}
	if ( !strcmp(name, "PYRAMID") ) {
		delete [] name;
		return MSH_PYRAMID;
	}
	if ( !strcmp(name, "CONE") ) {
		delete [] name;
		return MSH_CONE;
	}
	if ( !strcmp(name, "EXCONE") ) {
		delete [] name;
		return MSH_EXCONE;
	}
	if ( !strcmp(name, "HOLE") ) {
		delete [] name;
		return MSH_HOLE;
	}
	return MSH_MIC;
}

void clsTranslator::readPyramid(LPPYRAMID3D obj, string& fileData, LPSCENE3D Scene) {
	VECTOR3D pos = readVector(fileData);
	fileData.erase(0, 27);
	VECTOR3D fwd, rwd, uwd;
	readRotation(fwd, uwd, rwd, fileData);
	fileData.erase(0, 10);
	obj->Translate(&pos);
	obj->setForwardLookDirection(&fwd);
	obj->setUpLookDirection(&uwd);
	obj->setRightLookDirection(&rwd);

	float h = readFloat(fileData);
	fileData.erase(0, 14);
	float bl = readFloat(fileData);
	fileData.erase(0, 13);
	float bw = readFloat(fileData);
	fileData.erase(0, 13);
	float tl = readFloat(fileData);
	fileData.erase(0, 12);
	float tw = readFloat(fileData);
	fileData.erase(0, 14);
	pos = readVector(fileData);
	obj->setColor(pos.x, pos.y, pos.z);

	obj = new PYRAMID3D(h, bl, bw, tl, tw);
	Scene->AddObject(obj);
}
void clsTranslator::readCone(LPCONE3D obj, string& fileData, LPSCENE3D Scene) {
	VECTOR3D pos = readVector(fileData);
	fileData.erase(0, 27);
	VECTOR3D fwd, rwd, uwd;
	readRotation(fwd, uwd, rwd, fileData);
	fileData.erase(0, 10);
	obj->Translate(&pos);
	obj->setForwardLookDirection(&fwd);
	obj->setUpLookDirection(&uwd);
	obj->setRightLookDirection(&rwd);

	float h = readFloat(fileData);
	fileData.erase(0, 14);
	float br = readFloat(fileData);
	fileData.erase(0, 13);
	float tr = readFloat(fileData);
	fileData.erase(0, 13);
	float prec = readFloat(fileData);
	fileData.erase(0, 14);
	pos = readVector(fileData);
	obj->setColor(pos.x, pos.y, pos.z);

	obj = new CONE3D(h, br, tr, prec);
	Scene->AddObject(obj);
}
void clsTranslator::readExCone(LPEXCONE3D obj, string& fileData, LPSCENE3D Scene) {
	VECTOR3D pos = readVector(fileData);
	fileData.erase(0, 27);
	VECTOR3D fwd, rwd, uwd;
	readRotation(fwd, uwd, rwd, fileData);
	fileData.erase(0, 10);
	obj->Translate(&pos);
	obj->setForwardLookDirection(&fwd);
	obj->setUpLookDirection(&uwd);
	obj->setRightLookDirection(&rwd);

	float h = readFloat(fileData);
	fileData.erase(0, 14);
	float br = readFloat(fileData);
	fileData.erase(0, 13);
	float tr = readFloat(fileData);
	fileData.erase(0, 9);
	float sec = readFloat(fileData);
	fileData.erase(0, 13);
	float prec = readFloat(fileData);
	fileData.erase(0, 14);
	pos = readVector(fileData);
	obj->setColor(pos.x, pos.y, pos.z);

	obj = new EXCONE3D(h, br, tr, sec, prec);
	Scene->AddObject(obj);
}
void clsTranslator::readHole(LPHOLE3D obj, string& fileData, LPSCENE3D Scene) {
	VECTOR3D pos = readVector(fileData);
	fileData.erase(0, 27);
	VECTOR3D fwd, rwd, uwd;
	readRotation(fwd, uwd, rwd, fileData);
	fileData.erase(0, 10);
	obj->Translate(&pos);
	obj->setForwardLookDirection(&fwd);
	obj->setUpLookDirection(&uwd);
	obj->setRightLookDirection(&rwd);

	float h = readFloat(fileData);
	fileData.erase(0, 14);
	float br = readFloat(fileData);
	fileData.erase(0, 19);
	float bh = readFloat(fileData);
	fileData.erase(0, 13);
	float tr = readFloat(fileData);
	fileData.erase(0, 18);
	float th = readFloat(fileData);
	fileData.erase(0, 13);
	float prec = readFloat(fileData);
	fileData.erase(0, 14);
	pos = readVector(fileData);
	obj->setColor(pos.x, pos.y, pos.z);

	obj = new HOLE3D(h, br, bh, tr, th, prec);
	Scene->AddObject(obj);
}
void clsTranslator::readMic(LPMICROPHONE3D obj, string& fileData, LPSCENE3D Scene) {
	VECTOR3D pos = readVector(fileData);
	fileData.erase(0, 27);
	VECTOR3D fwd, rwd, uwd;
	readRotation(fwd, uwd, rwd, fileData);
	fileData.erase(0, 16);

	float tmp = readFloat(fileData);
	fileData.erase(0, 14);
	float tmp2 = readFloat(fileData);
	fileData.erase(0, 14);

	obj = new MICROPHONE3D(255, 255, 255, tmp2, tmp);
	obj->Translate(&pos);
	obj->setForwardLookDirection(&fwd);
	obj->setUpLookDirection(&uwd);
	obj->setRightLookDirection(&rwd);

	tmp = readFloat(fileData);
	obj->setBaseHeight(tmp);
	fileData.erase(0, 15);
	tmp = readFloat(fileData);
	obj->setButtonWidth(tmp);
	fileData.erase(0, 17);
	tmp = readFloat(fileData);
	obj->setUprightRadius(tmp);
	fileData.erase(0, 17);
	tmp = readFloat(fileData);
	obj->setUprightHeight(tmp);
	fileData.erase(0, 14);
	tmp = readFloat(fileData);
	obj->setUprightGap(tmp);
	fileData.erase(0, 16);
	tmp = readFloat(fileData);
	obj->setHandleIndent(tmp);
	fileData.erase(0, 14);
	tmp = readFloat(fileData);
	obj->setHeadRadius(tmp);
	fileData.erase(0, 14);
	tmp = readFloat(fileData);
	obj->setHeadDepth(tmp);
	fileData.erase(0, 14);
	tmp = readFloat(fileData);
	obj->setCoreRadius(tmp);
	fileData.erase(0, 13);
	tmp = readFloat(fileData);
	obj->setPrecission(tmp);
	fileData.erase(0, 14);
	pos = readVector(fileData);
	obj->setColor(pos.x, pos.y, pos.z);

	Scene->AddObject(obj);
}

bool clsTranslator::saveSceneScript(LPSCENE3D Scene, TCHAR *fileName) {
	HANDLE hFile; 
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;
	char new_line[]="\x0D\x0A";

	hFile = CreateFile(fileName,               // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_ALWAYS,          // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE) 
		return false;

	char *splitBuffer = "===================================== Cameras =======================================";
	bErrorFlag = WriteFile(hFile, splitBuffer, (DWORD)strlen(splitBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, "Total: ", 7, &dwBytesWritten, NULL);
	UINT sceneObjCount = Scene->getObjectClassCount(CLS_CAMERA);
	TCHAR *dataBuffer = new TCHAR[256];
	_itow(sceneObjCount, dataBuffer, 10);
	bErrorFlag = WriteFile( hFile, dataBuffer, (DWORD)wcslen(dataBuffer), &dwBytesWritten, NULL);
	delete [] dataBuffer;
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	
	for ( UINT i = 0; i < sceneObjCount; i++ ) {
 		LPCAMERA3D camToSave = (LPCAMERA3D)Scene->getObject(CLS_CAMERA, i);
//		pasteName(camToSave, hFile);
		pastePosition(camToSave, hFile);
		pasteRotation(camToSave, hFile);
		
		bErrorFlag = WriteFile( hFile, "hFOV: ", 6, &dwBytesWritten, NULL);
		pasteFloat(camToSave->getHFov(), hFile);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

		bErrorFlag = WriteFile( hFile, "vFOV: ", 6, &dwBytesWritten, NULL);
		pasteFloat(camToSave->getVFov(), hFile);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

		bErrorFlag = WriteFile( hFile, "Near clip: ", 11, &dwBytesWritten, NULL);
		pasteFloat(camToSave->getNearCP(), hFile);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

		bErrorFlag = WriteFile( hFile, "Far clip: ", 10, &dwBytesWritten, NULL);
		pasteFloat(camToSave->getFarCP(), hFile);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

		char split[] = "----------------------------------------------------------------------------";
		bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	}

	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	splitBuffer = "===================================== Lighters ======================================";
	bErrorFlag = WriteFile(hFile, splitBuffer, (DWORD)strlen(splitBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, "Total: ", 7, &dwBytesWritten, NULL);
	sceneObjCount = Scene->getObjectClassCount(CLS_LIGHT);
	dataBuffer = new TCHAR[256];
	_itow(sceneObjCount, dataBuffer, 10);
	bErrorFlag = WriteFile( hFile, dataBuffer, (DWORD)wcslen(dataBuffer), &dwBytesWritten, NULL);
	delete [] dataBuffer;
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	
	for ( UINT i = 0; i < sceneObjCount; i++ ) {
		LPDIFLIGHT3D lightToSave = (LPDIFLIGHT3D)Scene->getObject(CLS_LIGHT, i);
	//	pasteName(lightToSave, hFile);
		pasteRotation(lightToSave, hFile);

		bErrorFlag = WriteFile( hFile, "Power: ", 7, &dwBytesWritten, NULL);
		pasteFloat(lightToSave->getPower(), hFile);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

		bErrorFlag = WriteFile( hFile, "Color (RGB): ", 13, &dwBytesWritten, NULL);
		pasteColor(lightToSave->getColor(), hFile);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

		char split[] = "----------------------------------------------------------------------------";
		bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
		bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	}

	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	splitBuffer = "===================================== Meshes ========================================";
	bErrorFlag = WriteFile(hFile, splitBuffer, (DWORD)strlen(splitBuffer), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, "Total: ", 7, &dwBytesWritten, NULL);
	sceneObjCount = Scene->getObjectClassCount(CLS_MESH);
	dataBuffer = new TCHAR[256];
	_itow(sceneObjCount, dataBuffer, 10);
	bErrorFlag = WriteFile( hFile, dataBuffer, (DWORD)wcslen(dataBuffer), &dwBytesWritten, NULL);
	delete [] dataBuffer;
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);
	
	for ( UINT i = 0; i < sceneObjCount; i++ ) {
		LPMESH3D meshToSave = (LPMESH3D)Scene->getObject(CLS_MESH, i);
//		pasteName(meshToSave, hFile);
		
		switch ( meshToSave->MeshID() ) {
		case MSH_PYRAMID:
			pastePyramid((LPPYRAMID3D)meshToSave, hFile);
			break;
		case MSH_CONE:
			pasteCone((LPCONE3D)meshToSave, hFile);
			break;
		case MSH_EXCONE:
			pasteExCone((LPEXCONE3D)meshToSave, hFile);
			break;
		case MSH_HOLE:
			pasteHole((LPHOLE3D)meshToSave, hFile);
			break;
		case MSH_MIC:
			pasteMic((LPMICROPHONE3D)meshToSave, hFile);
			break;
		}
	}
	char split[] = "----------------------------------------------------------------------------";
	bErrorFlag = WriteFile( hFile, split, (DWORD)strlen(split), &dwBytesWritten, NULL);
	bErrorFlag = WriteFile(hFile, new_line, 2, &dwBytesWritten, NULL);

	CloseHandle(hFile);
	return bErrorFlag;
}

bool clsTranslator::loadSceneScript(LPSCENE3D Scene, TCHAR *fileName) {
	HANDLE hFile; 
    DWORD dwBytesRead = 0, fSizeHi, fSizeLow;
    BOOL bErrorFlag = FALSE;

	hFile = CreateFile(fileName,               // name of the write
                       GENERIC_READ,           // open for reading
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,          // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE) 
		return false;

	fSizeLow = GetFileSize(hFile, &fSizeHi);
	char *buffer = new char[fSizeLow];
	bErrorFlag = ReadFile(hFile, buffer, fSizeLow, &dwBytesRead, NULL);

	CloseHandle(hFile);

	string fileData(buffer);
	delete [] buffer;

		// loading cameras
	if ( !fileData.size() )
		return false;
	fileData.erase(0, 94);
	UINT objCount = (UINT)readFloat(fileData);
	fileData.erase(0, 13);
	for (UINT i = 0; i < objCount; i++) {
		LPCAMERA3D camera = new CAMERA3D;
		readPosition(camera, fileData);
		fileData.erase(0, 27);
		VECTOR3D fwd, rwd, uwd;
		readRotation(fwd, uwd, rwd, fileData);
		fileData.erase(0, 8);

		camera->setForwardLookDirection(&fwd);
		camera->setUpLookDirection(&uwd);
		camera->setRightLookDirection(&rwd);

		float tmp = readFloat(fileData);
		camera->setHFov(tmp);
		fileData.erase(0, 7);
		tmp = readFloat(fileData);
		camera->setVFov(tmp);
		fileData.erase(0, 12);
		tmp = readFloat(fileData);
		camera->setNearCP(tmp);
		fileData.erase(0, 11);
		tmp = readFloat(fileData);
		camera->setFarCP(tmp);

		fileData.erase(0, 89);
	//	Scene->AddObject(camera);
	}

		// loading lighters
	fileData.erase(0, 86);
	objCount = (UINT)readFloat(fileData);
	fileData.erase(0, 28);
	for (UINT i = 0; i < objCount; i++) {
		LPDIFLIGHT3D light = new DIFLIGHT3D;
		VECTOR3D fwd, rwd, uwd;
		readRotation(fwd, uwd, rwd, fileData);
		fileData.erase(0, 9);

		light->setForwardLookDirection(&fwd);
		light->setUpLookDirection(&uwd);
		light->setRightLookDirection(&rwd);

		float tmp = readFloat(fileData);
		light->setPower(tmp);
		fileData.erase(0, 14);
		VECTOR3D v = readVector(fileData);
		light->setColor(v.x, v.y, v.z);

		fileData.erase(0, 105);
		//Scene->AddObject(light);
	}

		// loading meshes
	fileData.erase(0,71);
	objCount = (UINT)readFloat(fileData);
	fileData.erase(0, 4);
	for (UINT i = 0; i < objCount; i++) {
		fileData.erase(0,38);
		LPMESH3D mesh = new MESH3D(readMeshID(fileData));
		fileData.erase(0, fileData.find_first_of(' ') + 1);
		switch ( mesh->MeshID() ) {
		case MSH_PYRAMID:
			readPyramid((LPPYRAMID3D)mesh, fileData, Scene);
			break;
		case MSH_CONE:
			readCone((LPCONE3D)mesh, fileData, Scene);
			break;
		case MSH_EXCONE:
			readExCone((LPEXCONE3D)mesh, fileData, Scene);
			break;
		case MSH_HOLE:
			readHole((LPHOLE3D)mesh, fileData, Scene);
			break;
		case MSH_MIC:
			readMic((LPMICROPHONE3D)mesh, fileData, Scene);
			break;
		}

		fileData.erase(0, 3);
	//	Scene->AddObject(mesh);
	}
	
	return bErrorFlag;
}
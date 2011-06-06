// 3DEditor.cpp: определяет точку входа для приложения.
//

#include "3DEditor.h"

// Глобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Remove this stuff:
FORM				mainForm;
UINT				ufWidth, ufHeight;

BUTTON				testButton1, testButton2, testButton3, testButton4;
LPWINBASE			testLpButton1;

LPRENDER_POOL		testPool;

SCENE3D				testScene;

TARGCAMERA3D		CameraTop, 
					CameraFront, 
					CameraRight, 
					CameraPersp;

DIFLIGHT3D			testLight1, testLight2;

PYRAMID3D			cubeX(100, 120, 100, 80, 60, 20, 80, 80, 200);
Cone				testCone(100.0f, 100.0f, 50.0f, 24);
ExCone				testExCone(100.0f, 100.0f, 50.0f, 40.f, 24);
Hole				testHole(70.0f, 70.0f, 30.0f, 30.0f, 10.0f, 20); 
MICROPHONE3D		testMic( 55, 55, 124 );

// Win API entry point:
// ===================================

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	INT		iResult;
	HACCEL	hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY3DEDITOR, szWindowClass, MAX_LOADSTRING);

	// Get this stuff below out of here!
	mainForm.Create(
				_T("Main form class"),
				(FORM_TYPE)(APP_FORM | WS_CLIPCHILDREN), 
				WS_EX_APPWINDOW | WS_EX_CONTROLPARENT,
				CW_USEDEFAULT, 0,
				1024, 768,
				NULL,
				LoadMenu(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR))
			); 
	mainForm.AssignEventHandler(WM_DESTROY, mainForm_OnDestroy, TRUE);
	mainForm.AssignEventHandler(WM_COMMAND, mainForm_menuClick, TRUE);
	mainForm.AssignEventHandler(WM_PAINT, mainForm_OnPaint, TRUE);
	mainForm.AssignEventHandler(WM_KEYDOWN, mainForm_keyPressed, TRUE);
	mainForm.AssignEventHandler(WM_GETDLGCODE, mainForm_ProcKeys, TRUE);
	mainForm.getClientSize(&ufWidth, &ufHeight);

	testScene.setAmbientColor(128, 125, 125);

	// Scene assembly here:
	testScene.AddObject(&CameraTop);
	testScene.AddObject(&CameraFront);
	testScene.AddObject(&CameraRight);
	testScene.AddObject(&CameraPersp);
	testScene.AddObject(&testLight1);
	testScene.AddObject(&testLight2);
	//testScene.AddObject(&testMic);
	testScene.AddObject(&cubeX);
	testScene.AddObject(&testCone);
	testScene.AddObject(&testExCone);
	testScene.AddObject(&testHole);

	// Objects here:
	testMic.Fly(-120);
	//testMic.Follow(-120);
	//testMic.setBaseRadius(100);
	//testMic.setBaseHeight(50);
	//testMic.setButtonWidth(80);
	//testMic.setUprightRadius(60);
	//testMic.setUprightHeight(132);
	//testMic.setUprightGap(25);

	cubeX.Follow(60);
	cubeX.Fly(40);

	// Lighters here:
	testLight1.LookAt(-1, 0, 0);
	testLight1.setColor(30, 80, 30);
	testLight1.setPower(0.4f);
	testLight2.LookAt(0, 0, -1);
	testLight2.setColor(5, 5, 5);
	testLight2.setPower(0.5f);
	
	// Cameras here:
	CameraTop.Translate(.0f, .0f, 200.f);
//	CameraTop.LookAt(.0f, .0f, .0f);
	CameraTop.Roll(-(FLOAT)M_PI_2);

	CameraFront.Translate(200.0f, .0f, .0f);
//	CameraFront.LookAt(.0f, .0f, .0f);

	CameraRight.Translate(.0f, 200.0f, .0f);
//	CameraRight.LookAt(.0f, .0f, .0f);

	CameraPersp.Translate(200.0f, 200.0f, 200.f);
//	CameraPersp.LookAt(.0f, .0f, .0f);

	// Viewports here:
	ufWidth -= 40;
	ufHeight -= 40;
	testPool = new RENDER_POOL(&mainForm, &testScene);
	testPool->addViewport(
				20, 20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				CameraTop.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				25 + ufWidth / 2, 
				20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				CameraFront.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				20, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				CameraRight.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				25 + ufWidth / 2, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				CameraPersp.objID(),
				RM_SHADEDWF
			);
	//testButton1.Create(
	//				0,
	//				_T("Button1"),
	//				&mainForm,
	//				25 + ufWidth / 2,
	//				25 + ufHeight / 2,
	//				110,
	//				30,
	//				FALSE);
	//testButton2.Create(
	//				1,
	//				_T("Button2"),
	//				&mainForm,
	//				25 + ufWidth / 2,
	//				25 + ufHeight / 2 + 35,
	//				110,
	//				30,
	//				FALSE);
	//testButton3.Create(
	//				1,
	//				_T("Button3"),
	//				&mainForm,
	//				25 + ufWidth / 2,
	//				25 + ufHeight / 2 + 70,
	//				110,
	//				30,
	//				FALSE);
	//testButton4.Create(
	//				1,
	//				_T("Button4"),
	//				&mainForm,
	//				25 + ufWidth / 2,
	//				25 + ufHeight / 2 + 105,
	//				110,
	//				30,
	//				FALSE);

	//testButton4.setTabOrder(2);
	//testButton1.setTabOrder(4);
	//TRANSLATOR3D::loadSceneScript(&testScene, TEXT("new.txt"));
	mainForm.Show();
	//mainForm.setColor(128, 125, 125);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR));
	iResult = mainForm.DoMSGCycle(hAccelTable);

	delete testPool; 
	mainForm.Destroy();
	return iResult;
}

 //Form event handler functions
 //============================================================================
LRESULT mainForm_OnPaint(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	((LPFORM)Sender)->Validate();
	testPool->RenderWorld();
	return 0L;
}

LRESULT mainForm_keyPressed(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	FLOAT	strafeDir	= 3.0f,
			flyDir		= 3.0f;
	FLOAT	rotRad		= 0.1f;
	switch ( wParam )
	{
		case VK_SPACE:
 			if ( CameraRight.getProjectionType() == PT_PARALLEL ) {
				CameraTop.setProjectionType(PT_CENTRAL);
				CameraRight.setProjectionType(PT_CENTRAL);
				CameraFront.setProjectionType(PT_CENTRAL);
				CameraPersp.setProjectionType(PT_CENTRAL);
			}
			else {
				CameraTop.setProjectionType(PT_PARALLEL);
				CameraRight.setProjectionType(PT_PARALLEL);
				CameraFront.setProjectionType(PT_PARALLEL);
				CameraPersp.setProjectionType(PT_PARALLEL);
			}
	//		TRANSLATOR3D::saveSceneScript(&testScene, TEXT("new.txt"));
			break;

		case VK_LEFT:
				//cubeX.Strafe(-strafeDir);
				CameraPersp.Strafe(-strafeDir);
			break;

		case VK_RIGHT:
				//cubeX.Strafe(strafeDir);
				CameraPersp.Strafe(strafeDir);
			break;

		case VK_DOWN:
				//cubeX.Follow(-flyDir);
				CameraPersp.Follow(-flyDir);
			break;

		case VK_UP:
				//cubeX.Follow(flyDir);
				CameraPersp.Follow(flyDir);
			break;
	}
	mainForm.Invalidate();

	return 0L;
}

LRESULT mainForm_menuClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME ofn;
	TCHAR *fName = new TCHAR[256];
	switch (LOWORD(wParam))
	{
		case IDM_ABOUT:
			((LPFORM)Sender)->DBShow(
					MAKEINTRESOURCE(IDD_ABOUTBOX), 
					About_DialogBox_Handler
				);
			break;
		case IDM_EXIT:
			((LPFORM)Sender)->Destroy();
			break;
		case IDM_SAVE:
			if ( SaveFileDialog((HWND)lParam, ofn) ) {
				CopyMemory(fName, ofn.lpstrFile, ofn.nMaxFile);
				if ( !TRANSLATOR3D::saveSceneScript(&testScene, fName) )
					MessageBox((HWND)lParam, _T("Неправильное имя файла"), _T("Ошибка записи"), MB_ICONERROR | MB_OK );
			}
			break;
		case IDM_LOAD:
			if ( OpenFileDialog((HWND)lParam, ofn) ) {
				CopyMemory(fName, ofn.lpstrFile, ofn.nMaxFile);
				if ( !TRANSLATOR3D::loadSceneScript(&testScene, fName) )
					MessageBox((HWND)lParam, _T("Невозможноо открыть данный файл"), _T("Ошибка чтения"), MB_ICONERROR | MB_OK );
			}
			break;
		case 0: 
			//((LPFORM)Sender)->MBShow(
			//		_T("Okay! You've done that!"),
			//		_T("Lol!"),
			//		MB_OK
			//	);
			break;
	}
	return 0L;
}

LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam) 
{	return DLGC_WANTARROWS;		}

LRESULT mainForm_OnDestroy(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

// "About" dialog box handler
INT_PTR CALLBACK About_DialogBox_Handler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL OpenFileDialog(HWND hWnd, OPENFILENAME& ofn) {	
	TCHAR szFile[260] = {'\0'};  // buffer for file name
	HANDLE hf;					 // file handle

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Text\0*.3de\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box.
	return GetOpenFileName(&ofn);
}

BOOL SaveFileDialog(HWND hWnd, OPENFILENAME& ofn) {	
	TCHAR szFile[260];  // buffer for file name
	HANDLE hf;			// file handle

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Text\0*.3de\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	// Display the Open dialog box.
	return GetSaveFileName(&ofn);	 
}

// 3DEditor.cpp: определ€ет точку входа дл€ приложени€.
//

#include "3DEditor.h"

// √лобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// “екст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// им€ класса главного окна

// Remove this stuff:
FORM				mainForm;
UINT				ufWidth, ufHeight;

BUTTON				testButton1, testButton2, testButton3, testButton4;
LPWINBASE			testLpButton1;

LPRENDER_POOL		testPool;

SCENE3D				testScene;

CAMERA3D			CameraTop, 
					CameraFront, 
					CameraRight, 
					CameraPersp;

DIFLIGHT3D			testLight1, testLight2;

PYRAMID3D			cubeX(100, 100, 100, 100, 100, 0, 80, 80, 200);
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
	mainForm.getClientSize(&ufWidth, &ufHeight);

	testScene.setAmbientColor(128, 125, 125);

	// Scene assembly here:
	testScene.AddObject(&CameraTop);
	testScene.AddObject(&CameraFront);
	testScene.AddObject(&CameraRight);
	testScene.AddObject(&CameraPersp);
	testScene.AddObject(&testLight1);
	testScene.AddObject(&testLight2);
	testScene.AddObject(&testMic);
	//testScene.AddObject(&cubeX);

	// Objects here:
	testMic.Fly(-120);

	// Lighters here:
	testLight1.LookAt(-1, 0, 0);
	testLight1.setColor(60, 30, 30);
	testLight1.setPower(0.1f);
	testLight2.LookAt(0, 0, -1);
	testLight2.setColor(5, 5, 5);
	testLight2.setPower(0.1f);
	
	// Cameras here:
	CameraTop.Translate(.0f, .0f, 200.f);
	CameraTop.LookAt(.0f, .0f, .0f);
	CameraTop.RollTo(-90.0f * (FLOAT)M_PI / 180.0f);

	CameraFront.Translate(200.0f, .0f, .0f);
	CameraFront.LookAt(.0f, .0f, .0f);

	CameraRight.Translate(.0f, 200.0f, .0f);
	CameraRight.LookAt(.0f, .0f, .0f);

	CameraPersp.Translate(200.0f, 200.0f, 200.f);
	CameraPersp.LookAt(.0f, .0f, .0f);

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
	//testPool->addViewport(
	//			25 + ufWidth / 2, 
	//			25 + ufHeight / 2,
	//			ufWidth / 2 - 5,
	//			ufHeight / 2 - 5,
	//			CameraPersp.objID(),
	//			RM_SHADED
	//		);
	testButton1.Create(
					0,
					_T("Button1"),
					&mainForm,
					25 + ufWidth / 2,
					25 + ufHeight / 2,
					110,
					30,
					FALSE);
	testButton2.Create(
					1,
					_T("Button2"),
					&mainForm,
					25 + ufWidth / 2,
					25 + ufHeight / 2 + 35,
					110,
					30,
					FALSE);
	testButton3.Create(
					1,
					_T("Button3"),
					&mainForm,
					25 + ufWidth / 2,
					25 + ufHeight / 2 + 70,
					110,
					30,
					FALSE);
	testButton4.Create(
					1,
					_T("Button4"),
					&mainForm,
					25 + ufWidth / 2,
					25 + ufHeight / 2 + 105,
					110,
					30,
					FALSE);

	testButton4.setTabOrder(2);
	testButton1.setTabOrder(4);
	mainForm.Show();

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
			break;

		/*case VK_LEFT:
				testPyramid1.Strafe(-strafeDir);
				CameraRight.LookAt(&testPyramid1);
			break;

		case VK_RIGHT:
				testPyramid1.Strafe(strafeDir);
				CameraRight.LookAt(&testPyramid1);
			break;

		case VK_DOWN:
				testPyramid1.Fly(-flyDir);
				CameraRight.LookAt(&testPyramid1);
			break;

		case VK_UP:
				testPyramid1.Fly(flyDir);
				CameraRight.LookAt(&testPyramid1);
			break;*/
	}
	mainForm.Invalidate();

	return 0L;
}

LRESULT mainForm_menuClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
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
		case 0: 
			((LPFORM)Sender)->MBShow(
					_T("Okay! You've done that!"),
					_T("Lol!"),
					MB_OK
				);
			break;
	}
	return 0L;
}


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
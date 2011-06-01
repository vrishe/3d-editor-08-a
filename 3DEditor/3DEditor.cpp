// 3DEditor.cpp: определ€ет точку входа дл€ приложени€.
//

#include "3DEditor.h"

// √лобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// “екст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// им€ класса главного окна

// Remove this stuff:
FORM				mainForm;
UINT				ufWidth, ufHeight;

LPRENDER_POOL		testPool;

SCENE3D				testScene;

CAMERA3D			CameraTop, 
					CameraFront, 
					CameraRight, 
					CameraPersp;

CONTROL				testButton;

Microphone			testMic( 200, 80, 80 );

// Win API entry point:
// ===================================

LRESULT button_hover(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	UINT width, height;
	((LPCONTROL)Sender)->getSize(&width, &height);
	((LPCONTROL)Sender)->MoveTo(
		25 + ufWidth / 2 + rand() % ((ufWidth / 2 - 5) - width), 
		25 + ufHeight / 2 + rand() % ((ufHeight / 2 - 5) - height) 
	);
	return 0L;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY3DEDITOR, szWindowClass, MAX_LOADSTRING);

	// Get this stuff below out of here!
	mainForm.Create(
				_T("Main form class"),
				(FORM_TYPE)(APP_FORM | WS_CLIPCHILDREN), 
				NULL,
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

	testScene.setAmbientColor(132, 128, 128);

	// Scene assembly here:
	testScene.AddObject(&CameraTop);
	testScene.AddObject(&CameraFront);
	testScene.AddObject(&CameraRight);
	testScene.AddObject(&CameraPersp);
	testScene.AddObject(&testMic);

	// Objects here:
	testMic.Fly(-120);
	
	// Cameras here:
	CameraTop.Translate(.0f, .0f, 200.f);
	CameraTop.LookAt(.0f, .0f, .0f);
	CameraTop.RollTo(90.0f * (FLOAT)M_PI / 180.0f);

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
	//			RM_SHADEDWF
	//		);

	testButton.Create(
				_T("button"),
				WS_CHILD | BS_PUSHBUTTON,
				NULL,
				25 + ufWidth / 2,
				25 + ufHeight / 2,
				70,
				30,
				&mainForm
			);
	testButton.setText(_T("Click!"));
	testButton.AssignEventHandler(WM_MOUSEMOVE, button_hover, TRUE); 
	mainForm.Show();

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete testPool; 
	mainForm.Destroy();
	return (int) msg.wParam;
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
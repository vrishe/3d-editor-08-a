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

CAMERA3D			testCamera1, 
					testCamera2, 
					testCamera3, 
					testCamera4;

Hole				testHole(50.0f, 75.0f, 50.0f, 45.0f, 20.0f, 60); 

// Win API entry point:
// ===================================

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
	//mainForm.AssignEventHandler(WM_KEYDOWN, mainForm_keyPressed, TRUE);
	mainForm.getClientSize(&ufWidth, &ufHeight);

	testScene.setAmbientColor(132, 128, 128);

	// Scene assembly here:
	testScene.AddObject(&testCamera1);
	testScene.AddObject(&testCamera2);
	testScene.AddObject(&testCamera3);
	testScene.AddObject(&testCamera4);
	testScene.AddObject(&testHole);

	// Objects here:
	
	// Cameras here:
	testCamera1.Translate(.0f, .0f, 200.f);
	testCamera1.LookAt(.0f, .0f, .0f);

	testCamera2.Translate(200.0f, .0f, .0f);
	testCamera2.LookAt(.0f, .0f, .0f);

	testCamera3.Translate(.0f, 200.0f, .0f);
	testCamera3.LookAt(.0f, .0f, .0f);

	testCamera4.Translate(200.0f, 200.0f, 200.f);
	testCamera4.LookAt(.0f, .0f, .0f);

	// Viewports here:
	ufWidth -= 40;
	ufHeight -= 40;
	testPool = new RENDER_POOL(&mainForm, &testScene);
	testPool->addViewport(
				20, 20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera1.objID(),
				RM_SHADED
			);
	testPool->addViewport(
				25 + ufWidth / 2, 
				20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera2.objID(),
				RM_SHADED
			);
	testPool->addViewport(
				20, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera3.objID(),
				RM_SHADED
			);
	testPool->addViewport(
				25 + ufWidth / 2, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera4.objID(),
				RM_SHADED
			);
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
LRESULT mainForm_OnPaint(LPVOID Sender, WPARAM wParam, LPARAM lParam)
{
	((LPFORM)Sender)->Validate();
	testPool->RenderWorld();
	return 0L;
}

//LRESULT mainForm_keyPressed(LPVOID Sender, WPARAM wParam, LPARAM lParam)
//{
//	FLOAT	strafeDir	= 3.0f,
//			flyDir		= 3.0f;
//	switch ( wParam )
//	{
//		case VK_SPACE:
// 			if ( testCamera3.getProjectionType() == PT_PARALLEL ) {
//				testCamera1.setProjectionType(PT_CENTRAL);
//				testCamera3.setProjectionType(PT_CENTRAL);
//			}
//			else {
//				testCamera1.setProjectionType(PT_PARALLEL);
//				testCamera3.setProjectionType(PT_PARALLEL);
//			}
//			break;
//
//		case VK_LEFT:
//				testPyramid1.Strafe(-strafeDir);
//				testCamera3.LookAt(&testPyramid1);
//			break;
//
//		case VK_RIGHT:
//				testPyramid1.Strafe(strafeDir);
//				testCamera3.LookAt(&testPyramid1);
//			break;
//
//		case VK_DOWN:
//				testPyramid1.Fly(-flyDir);
//				testCamera3.LookAt(&testPyramid1);
//			break;
//
//		case VK_UP:
//				testPyramid1.Fly(flyDir);
//				testCamera3.LookAt(&testPyramid1);
//			break;
//	}
//	mainForm.Invalidate();
//
//	return 0L;
//}

LRESULT mainForm_menuClick(LPVOID Sender, WPARAM wParam, LPARAM lParam)
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
	}
	return 0L;
}


LRESULT mainForm_OnDestroy(LPVOID Sender, WPARAM wParam, LPARAM lParam)
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
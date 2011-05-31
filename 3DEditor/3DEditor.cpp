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
CAMERA3D			testCamera1, testCamera2, testCamera3;
Pyramid				testPyramid(140.0f, 140.0f, 140.0f, 140.0f, 140.0f),
					testPyramid2(50.0f, 140.0f, 110.0f, 90.0f, 75.0f);
Cone				testCone(30.0f, 75.0f, 50.0f, 24);
ExCone				testExCone(40.0f, 150.0f, 120.0f, 120.0f, 13);
Hole				testHole(30.0f, 50.0f, 30.0f, 30.0f, 10.0f, 20); 

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
	mainForm.AssignEventHandler(WM_KEYDOWN, mainForm_keyPressed, TRUE);
	mainForm.getClientSize(&ufWidth, &ufHeight);

	testScene.setAmbientColor(132, 128, 128);

	testScene.AddObject(&testPyramid);
	//testScene.AddObject(&testPyramid2);
	//testScene.AddObject(&testCone);
	//testScene.AddObject(&testExCone);
	//testScene.AddObject(&testHole);
	testScene.AddObject(&testCamera1);
	testScene.AddObject(&testCamera2);
	testScene.AddObject(&testCamera3);

	testPyramid.setColor(200, 30, 30);
	testPyramid.Fly(-70);
	//testPyramid.Strafe(-70);

	testCamera1.Follow(-200);
	testCamera3.Follow(-200);
	//testCamera3.Fly(200);
	//testCamera3.PitchTo(-45.0f * (FLOAT)M_PI/180.0f);
	//testCamera3.LookAt(VECTOR3D(1.0f, 1.0f, .3f));
	//testCamera2.LookAt(&testPyramid);
	//testCamera3.LookAt(&testPyramid);
	//testCamera2.Follow(200);

	ufWidth -= 40;
	ufHeight -= 40;
	testPool = new RENDER_POOL(&mainForm, &testScene);
	testPool->addViewport(
				20, 20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera1.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				20, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera2.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				25 + ufWidth / 2, 
				20,
				ufWidth / 2 - 5,
				ufHeight,
				testCamera3.objID(),
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

LRESULT mainForm_keyPressed(LPVOID Sender, WPARAM wParam, LPARAM lParam)
{
	FLOAT	strafeDir	= 3.0f,
			flyDir		= 3.0f;
	switch ( wParam )
	{
		case VK_SPACE:
 			if ( testCamera3.getProjectionType() == PT_PARALLEL ) {
				testCamera1.setProjectionType(PT_CENTRAL);
				testCamera3.setProjectionType(PT_CENTRAL);
			}
			else {
				testCamera1.setProjectionType(PT_PARALLEL);
				testCamera3.setProjectionType(PT_PARALLEL);
			}
			break;

		case VK_LEFT:
				testPyramid.Strafe(-strafeDir);
				//testCamera2.LookAt(&testPyramid);
				//testCamera3.LookAt(&testPyramid);
			break;

		case VK_RIGHT:
				testPyramid.Strafe(strafeDir);
				//testCamera2.LookAt(&testPyramid);
				//testCamera3.LookAt(&testPyramid);
			break;

		case VK_DOWN:
				testPyramid.Fly(-flyDir);
				//testCamera2.LookAt(&testPyramid);
				//testCamera3.LookAt(&testPyramid);
			break;

		case VK_UP:
				testPyramid.Fly(flyDir);
				//testCamera2.LookAt(&testPyramid);
				//testCamera3.LookAt(&testPyramid);
			break;
	}
	mainForm.Invalidate();

	return 0L;
}

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
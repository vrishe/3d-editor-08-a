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
	testScene.AddObject(&testCamera1);
	testScene.AddObject(&testCamera2);
	testScene.AddObject(&testCamera3);

	testCamera1.setNearCP(1.0);
	testCamera2.setNearCP(1.0);
	testCamera3.setNearCP(1.0);

	//testCamera1.Fly(-5.0f);
	//testCamera1.Pitch((float)(0.0*M_PI/180));
	//testCamera1.setProjectionType(PT_CENTRAL);
	testCamera2.Fly(100);
	//testCamera2.Follow(0);
	//testCamera2.Strafe(22);
	testCamera2.Pitch(90.0f*(FLOAT)M_PI/180.0f);
	//testCamera3.Strafe(200);
	
	testCamera3.Follow(-100);
	testCamera3.Fly(100);
	testCamera3.Pitch(-45.0f*(FLOAT)M_PI/180.0f);
	//testCamera3.setHFov(60.0f * (FLOAT)M_PI/ 180.0f);

	testScene.AddObject(&testPyramid);
	//testScene.AddObject(&testPyramid2);
	//testScene.AddObject(&testCone);
	//testScene.AddObject(&testExCone);
	//testScene.AddObject(&testHole);

	testPyramid.setColor(200, 30, 30);
	//testPyramid.Strafe(115);
	//testPyramid.Follow(450);
	//testPyramid.Fly(40);
	//testPyramid.Yaw((float)(50.0*M_PI/180));

	//testPyramid2.Strafe(-115);
	//testPyramid2.Follow(200);

	//testExCone.setColor(20, 150, 30);
	//testExCone.Strafe(-75);
	//testExCone.Fly(40);
	//testExCone.Roll((float)(50.0*M_PI/180));
	//
	//testHole.setColor(100, 50, 100);
	//testHole.Strafe(175);
	//testHole.Follow(10);
	//testHole.Fly(80);
	//testHole.Pitch((float)(35.0*M_PI/180));
	//testHole.ScaleByX(1.3f);

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
 	if ( wParam == VK_SPACE )
	{
 		if ( testCamera3.getProjectionType() == PT_PARALLEL ) {
			testCamera1.setProjectionType(PT_CENTRAL);
			testCamera3.setProjectionType(PT_CENTRAL);
		}
		else {
			testCamera1.setProjectionType(PT_PARALLEL);
			testCamera3.setProjectionType(PT_PARALLEL);
		}

		mainForm.Invalidate();
	}
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
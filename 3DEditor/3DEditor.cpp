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
DIFLIGHT3D			testLight;
Pyramid				testPyramid1(100.0f, 100.0f, 80.0f, 80.0f, 60.0f),
					testPyramid2(100.0f, 100.0f, 80.0f, 80.0f, 60.0f),
					testPyramid3(100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
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

	// Scene assembly here:
	testScene.AddObject(&testLight);
	testScene.AddObject(&testPyramid1);
	testScene.AddObject(&testPyramid2);
	testScene.AddObject(&testPyramid3);
	testScene.AddObject(&testCamera1);
	testScene.AddObject(&testCamera2);
	testScene.AddObject(&testCamera3);
	//testScene.AddObject(&testCone);
	//testScene.AddObject(&testExCone);
	//testScene.AddObject(&testHole);

	//Lights here:
	testLight.setColor(255, 255, 255);
	testLight.setPower(.1f);
	testLight.LookAt(.0f, .0f, -1.0);

	// Objects here:
	testPyramid1.setColor(200, 30, 30);
	testPyramid1.Translate(.0f, .0f, -50.0f);

	testPyramid2.setColor(30, 200, 30);
	testPyramid2.Translate(-150.0f, .0f, -170.0f);
	testPyramid2.LookAt(&testPyramid1);

	testPyramid3.setColor(170, 170, 170);
	testPyramid3.Translate(120.0f, .0f, -50.0f);
	
	// Cameras here:
	testCamera1.Translate(450.0f, .0f, .0f);
	testCamera1.LookAt(.0f, .0f, .0f);
	testCamera1.setProjectionType(PT_CENTRAL);

	testCamera2.Translate(.0f, 450.0f, .0f);
	testCamera2.LookAt(.0f, .0f, .0f);
	testCamera2.setProjectionType(PT_CENTRAL);

	testCamera3.Translate(-450.0f, 450.0f, 450.0f);
	testCamera3.LookAt(.0f, .0f, .0f);
	//testCamera3.LookAt(&testPyramid1);
	//testCamera3.RollTo((FLOAT)M_PI);
	testCamera3.setProjectionType(PT_CENTRAL);

	// Viewports here:
	ufWidth		-= 40;
	ufHeight	-= 40;
	testPool	= new RENDER_POOL(&mainForm, &testScene);
	testPool->addViewport(
				20, 20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera1.objID(),
				RM_SHADED
			);
	testPool->addViewport(
				20, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera2.objID(),
				RM_SHADED
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
	mainForm.setColor(132, 128, 128);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		testPyramid2.LookAt(&testPyramid1);
		//testCamera3.LookAt(&testPyramid1);
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
				testPyramid1.Strafe(-strafeDir);
			break;

		case VK_RIGHT:
				testPyramid1.Strafe(strafeDir);
			break;

		case VK_DOWN:
				testPyramid1.Fly(-flyDir);
			break;

		case VK_UP:
				testPyramid1.Fly(flyDir);
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
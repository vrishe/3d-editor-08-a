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
CAMERA3D			testCamera;
Pyramid				testPyramid(50.0f, 140.0f, 110.0f, 20.0f, 30.0f);
Cone				testCone(30.0f, 75.0f, 50.0f, 24);
ExCone				testExCone(86.0f, 150.0f, 100.0f, 120.0f, 13);
Hole				testHole(30.0f, 75.0f, 50.0f, 75.0f, 50.0f, 24); 

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
	mainForm.getClientSize(&ufWidth, &ufHeight);
	ufWidth -= 40;
	ufHeight -= 40;
	testScene.AddObject(&testCamera);
	//testScene.AddObject(&testPyramid);
	//testScene.AddObject(&testCone);
	//testScene.AddObject(&testExCone);
	testScene.AddObject(&testHole);
	testScene.setAmbientColor(132, 128, 128);

	testPool = new RENDER_POOL(&mainForm, &testScene);
	testPool->addViewport(
				20, 20,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				20, 
				25 + ufHeight / 2,
				ufWidth / 2 - 5,
				ufHeight / 2 - 5,
				testCamera.objID(),
				RM_WIREFRAME
			);
	testPool->addViewport(
				25 + ufWidth / 2, 
				20,
				ufWidth / 2 - 5,
				ufHeight,
				testCamera.objID(),
				RM_WIREFRAME
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
//	BYTE anchor = testForm1->getAnchors();
//	TCHAR number[20];
// 	if ( wParam == 32 )
//	{
//		if ( ++anchor == 16 ) anchor = 0;
//		_itow_s(anchor, number, 20, 10);
//		mainForm->MBShow(number, _T("Anchor changed"), MB_OK);
//		testForm1->setAnchors(anchor);
//	}
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
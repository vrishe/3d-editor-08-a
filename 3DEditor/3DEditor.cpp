// 3DEditor.cpp: определ€ет точку входа дл€ приложени€.
//

#include "3DEditor.h"

// √лобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// “екст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// им€ класса главного окна

// Remove this stuff:
FORM				mainForm;
UINT				ufWidth, ufHeight;
RENDER_POOL			testPool;
SCENE3D				testScene;
CAMERA3D			testCamera;

TRIVERTEX			testTriangle[3];
GRADIENT_TRIANGLE	grTestTriangle;

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
				APP_FORM, 
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
	testPool.assignScene(&testScene);
	testScene.AddObject(&testCamera);
	testPool.addViewport(
					20, 20,
					ufWidth / 2 - 20,
					ufHeight,
					0,
					RM_WIREFRAME
				);
	testPool.addViewport(
					ufWidth / 2 + 10, 20,
					ufWidth / 2 - 20,
					ufHeight,
					0,
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

	mainForm.Destroy();
	return (int) msg.wParam;
}

 //Form event handler functions
 //============================================================================
LRESULT mainForm_OnPaint(LPVOID Sender, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	((LPFORM)Sender)->Validate();
	hDC		= ((LPFORM)Sender)->getDC();
		testPool.RenderWorld(hDC);
	((LPFORM)Sender)->dropDC();
	return 0;
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
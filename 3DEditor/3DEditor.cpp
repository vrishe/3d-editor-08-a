// 3DEditor.cpp: определ€ет точку входа дл€ приложени€.
//

#include "3DEditor.h"

// √лобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// “екст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// им€ класса главного окна

// Remove this stuff:
LPFORM	mainForm, testForm1, testForm2;
UINT	ufWidth, ufHeight;
TRIVERTEX	testTriangle[3];
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
	mainForm = new FORM( szWindowClass );
	testForm1 = new FORM( _T("testForm class"), (HBRUSH)(COLOR_WINDOW + 3) );
	testForm2 = new FORM( _T("testForm2 class"), (HBRUSH)(COLOR_WINDOW + 3) );

	mainForm->Create(
				APP_FORM, 
				NULL,
				CW_USEDEFAULT, 0,
				1024, 768,
				NULL,
				LoadMenu(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR))
			); 
	mainForm->getFormClientSize(&ufWidth, &ufHeight);
	ufWidth -= 40;
	ufHeight -= 40;
	testForm1->Create(
				CHILD_FORM,
				NULL,
				20, 20,
				ufWidth / 2	- 5, 
				ufHeight / 2 - 5,
				mainForm,
				NULL
			);
	testForm2->Create(
				CHILD_FORM,
				NULL,
				ufWidth / 2 + 25,
				20,
				ufWidth / 2	- 5, 
				ufHeight / 2 - 5,
				mainForm,
				NULL
			);

	mainForm->AssignEventHandler(WM_DESTROY, mainForm_OnDestroy, TRUE);
	mainForm->AssignEventHandler(WM_COMMAND, mainForm_menuClick, TRUE);
	//mainForm->AssignEventHandler(WM_CHAR, mainForm_keyPressed, TRUE);
	testForm1->AssignEventHandler(WM_PAINT, testForm_OnPaint, TRUE);

	mainForm->Show();

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete mainForm;

	return (int) msg.wParam;
}

// Form event handler functions
// ============================================================================
LRESULT testForm_OnPaint(LPVOID Sender, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = ((LPFORM)Sender)->gethWnd();
	HDC	hDC,
		hDCForm,
		hDCBitmap;
	HBITMAP backBuffer;
	HGDIOBJ oldBitmap;
	PAINTSTRUCT pStruct;
	BOOL bResult;

	hDC = BeginPaint(hWnd, &pStruct);
		hDCForm = GetDC(hWnd);
		hDCBitmap = CreateCompatibleDC(hDCForm);
		backBuffer = CreateCompatibleBitmap(
								hDCBitmap, 
								pStruct.rcPaint.right, 
								pStruct.rcPaint.bottom
							); 
		oldBitmap = SelectObject(hDCBitmap, backBuffer);
		// Let's set triangle's vertices
			testTriangle[0].x		= 10;
			testTriangle[0].y		= 10;
			testTriangle[0].Red		= 0xFFFF;
			testTriangle[0].Green	= 0x0000;
			testTriangle[0].Blue	= 0x0000;
			testTriangle[0].Alpha	= 0x0000;

			testTriangle[1].x		= 40;
			testTriangle[1].y		= 300;
			testTriangle[1].Red		= 0x0000;
			testTriangle[1].Green	= 0xFFFF;
			testTriangle[1].Blue	= 0x0000;
			testTriangle[1].Alpha	= 0x0000;

			testTriangle[2].x		= 450;
			testTriangle[2].y		= 80;
			testTriangle[2].Red		= 0x0000;
			testTriangle[2].Green	= 0x0000;
			testTriangle[2].Blue	= 0xFFFF;
			testTriangle[2].Alpha	= 0x0000;

			grTestTriangle.Vertex1	= 0;
			grTestTriangle.Vertex2	= 1;
			grTestTriangle.Vertex3	= 2;
		// Draw here:
			bResult = GradientFill(
				hDCBitmap, 
				testTriangle, 
				3, 
				&grTestTriangle,
				1, 
				GRADIENT_FILL_TRIANGLE
			);
		// ================================================================
		BitBlt(
				hDCForm, 
				0, 
				0, 
				pStruct.rcPaint.right, 
				pStruct.rcPaint.bottom, 
				hDCBitmap,
				0,
				0, 
				SRCCOPY
			);
		SelectObject(hDCBitmap, oldBitmap);
		DeleteObject(backBuffer);
		DeleteObject(hDCBitmap);
		ReleaseDC(hWnd, hDCForm);
	EndPaint(hWnd, &pStruct);
	return 0;
}


LRESULT mainForm_keyPressed(LPVOID Sender, WPARAM wParam, LPARAM lParam)
{
	BYTE anchor = testForm1->getAnchors();
	TCHAR number[20];
 	if ( wParam == 32 )
	{
		if ( ++anchor == 16 ) anchor = 0;
		_itow_s(anchor, number, 20, 10);
		mainForm->MBShow(number, _T("Anchor changed"), MB_OK);
		testForm1->setAnchors(anchor);
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
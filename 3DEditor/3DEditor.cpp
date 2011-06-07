// 3DEditor.cpp: определяет точку входа для приложения.
//

#include "3DEditor.h"

// Глобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Remove this stuff:
FORM				mainForm;
UINT				ufWidth, ufHeight;

BUTTON				btMove, btRotate, btScale,
					tabCreate, tabModify,
					btObjects, btCams, btLights,
					btPan, btCamSphere, btLookAround;

LABEL				lbX, lbY, lbZ;

TEXTBOX				tbX, tbY, tbZ;

LPWINBASE			testLpButton1;

LPVIEWPORT_POOL		testPool;

SCENE3D				testScene;

TARGCAMERA3D		CameraTop, 
					CameraFront, 
					CameraRight, 
					CameraPersp;

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
				1270, 804,
				NULL,
				LoadMenu(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR))
			); 
	mainForm.AssignEventHandler(WM_DESTROY, mainForm_OnDestroy, TRUE);
	mainForm.AssignEventHandler(WM_COMMAND, mainForm_menuClick, TRUE);
	mainForm.AssignEventHandler(WM_PAINT, mainForm_OnPaint, TRUE);
	mainForm.AssignEventHandler(WM_KEYDOWN, mainForm_keyPressed, TRUE);
	mainForm.AssignEventHandler(WM_GETDLGCODE, mainForm_ProcKeys, TRUE);
	mainForm.getClientSize(&ufWidth, &ufHeight);
	mainForm.setColor(FORM_BG_COLOR);

	testScene.setAmbientColor(VIEWPORT_BG_COLOR);

	// Scene assembly here:
	testScene.AddObject(&CameraTop);
	testScene.AddObject(&CameraFront);
	testScene.AddObject(&CameraRight);
	testScene.AddObject(&CameraPersp);
	CameraTop.Translate(.0f, .0f, 200.f);
	CameraTop.Roll(-(FLOAT)M_PI_2);
	CameraFront.Translate(200.0f, .0f, .0f);
	CameraRight.Translate(.0f, 200.0f, .0f);
	CameraPersp.Translate(200.0f, 200.0f, 200.f);

	// Viewports here:
	testPool = new VIEWPORT_POOL(&mainForm, &testScene);
	testPool->addViewport(
					10, 
					BT_TOOL_H + 10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_TOP,
					RM_WIREFRAME
				);
	testPool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					BT_TOOL_H + 10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_FRONT,
					RM_WIREFRAME
				);
	testPool->addViewport(
					10, 
					BT_TOOL_H + 10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_LEFT,
					RM_WIREFRAME
				);
	testPool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					BT_TOOL_H + 10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_PERSPECTIVE,
					RM_SHADEDWF
				);

	TCHAR *name = new TCHAR[256];
		// object toolbar buttons	
	LoadString(hInstance, N_BT_MOVE, name, 256);
	btMove.Create(	BT_MOVE,
					name,
					&mainForm,
					10, 5,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_ROTATE, name, 256);
	btRotate.Create(BT_ROTATE,
					name,
					&mainForm,
					10 + BT_TOOL_W + 5, 5,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_SCALE, name, 256);
	btScale.Create(BT_SCALE,
					name,
					&mainForm,
					10 + BT_TOOL_W * 2 + 5 * 2, 5,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_LB_X, name, 256);
	lbX.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W * 3 - BT_TOOL_H * 2 - 40, 11,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbY.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W*2 - BT_TOOL_H - 40, 11,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbZ.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W - 40, 11,
				LB_V_H, LB_V_W);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbX.Create(	TB_X,
				name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W * 2 - BT_TOOL_H * 2 - 45, 5,
				BT_TOOL_H, BT_TOOL_H);
	tbY.Create(	TB_Y,
				name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W - BT_TOOL_H - 45, 5,
				BT_TOOL_H, BT_TOOL_H);
	tbZ.Create(	TB_Z,
				name,
				&mainForm, 
				VIEWPORT_AREA_W - 45, 5,
				BT_TOOL_H, BT_TOOL_H);
	tbX.setBgFillColor(TEXTBOX_BG_COLOR);
	tbY.setBgFillColor(TEXTBOX_BG_COLOR);
	tbZ.setBgFillColor(TEXTBOX_BG_COLOR);

		// right toolbarr
	LoadString(hInstance, N_TAB_CREATE, name, 256);
	tabCreate.Create(TAB_CREATE,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 12, 10 + BT_TOOL_H / 2,
					TAB_W, TAB_H,
					FALSE);
	LoadString(hInstance, N_TAB_MODIFY, name, 256);
	tabModify.Create(TAB_MODIFY,
					name,
					&mainForm,
					ufWidth - TAB_W - 6, 10 + BT_TOOL_H / 2,
					TAB_W, TAB_H,
					FALSE);

	LoadString(hInstance, N_BT_LIGHTS, name, 256);
	btLights.Create(BT_LIGHTS,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W - 6, 15 + BT_TOOL_H / 2 + TAB_H,					
					SUB_TAB_W, SUB_TAB_H,
					FALSE);
	LoadString(hInstance, N_BT_CAMS, name, 256);
	btCams.Create(	BT_CAMS,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 2 - 9, 15 + BT_TOOL_H / 2 + TAB_H,
					SUB_TAB_W, SUB_TAB_H,
					FALSE);
	LoadString(hInstance, N_BT_OBJECTS, name, 256);
	btObjects.Create(BT_OBJECTS,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 - 13, 15 + BT_TOOL_H / 2 + TAB_H,
					SUB_TAB_W, SUB_TAB_H,
					FALSE);

		// camera toolbar buttons
	LoadString(hInstance, N_BT_PAN, name, 256);
	btPan.Create(BT_PAN,
					name,
					&mainForm,
					VIEWPORT_AREA_W / 2 - BT_TOOL_H * 2 - BT_TOOL_H / 2 - 11, 
					VIEWPORT_AREA_H + BT_TOOL_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_CAMSPHERE, name, 256);
	btCamSphere.Create(BT_PAN,
					name,
					&mainForm,
					VIEWPORT_AREA_W / 2 - BT_TOOL_H / 2 - 6, 
					VIEWPORT_AREA_H + BT_TOOL_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_LOOK_AROUND, name, 256);
	btLookAround.Create(BT_LOOK_AROUND,
					name,
					&mainForm,
					VIEWPORT_AREA_W / 2 + BT_TOOL_H * 2 - 16, 
					VIEWPORT_AREA_H + BT_TOOL_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	

	delete [] name;
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
			break;

		case VK_LEFT:
				CameraPersp.Strafe(-strafeDir);
			break;

		case VK_RIGHT:
				CameraPersp.Strafe(strafeDir);
			break;

		case VK_DOWN:
				CameraPersp.Follow(-flyDir);
			break;

		case VK_UP:
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
				if ( !TRANSLATOR3D::loadSceneScript(&testScene, fName) ) {
					MessageBox((HWND)lParam, _T("Невозможно открыть данный файл"), _T("Ошибка чтения"), MB_ICONERROR | MB_OK );
					break;
				}
				mainForm.Invalidate();
			}
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

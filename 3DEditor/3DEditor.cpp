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

LABEL				lbX, lbY, lbZ,
					lbObjects;

TEXTBOX				tbX, tbY, tbZ;

LISTBOX				listObjects;

LPWINBASE			testLpButton1;

LPVIEWPORT_POOL		Pool;

SCENE3D				Scene;

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

	Scene.setAmbientColor(VIEWPORT_BG_COLOR);
	MICROPHONE3D testMic(60, 60, 140);
	testMic.Fly(-120);
	Scene.AddObject(&testMic);

	// Viewports here:
	Pool = new VIEWPORT_POOL(&mainForm, &Scene);
	Pool->addViewport(
					10, 
					BT_TOOL_H + 10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_TOP,
					RM_WIREFRAME
				);
	Pool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					BT_TOOL_H + 10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_FRONT,
					RM_WIREFRAME
				);
	Pool->addViewport(
					10, 
					BT_TOOL_H + 10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_LEFT,
					RM_WIREFRAME
				);
	Pool->addViewport(
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
				VIEWPORT_AREA_W - LB_V_W * 3 - 122, 10,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbY.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W*2 - 82, 10,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbZ.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W - 40, 10,
				LB_V_H, LB_V_W);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbX.Create(	TB_X,
				name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W * 2 - 127, 9,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbY.Create(	TB_Y,
				name,
				&mainForm, 
				VIEWPORT_AREA_W - LB_V_W - 86, 9,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbZ.Create(	TB_Z,
				name,
				&mainForm, 
				VIEWPORT_AREA_W - 45, 9,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbX.setBgFillColor(TEXT_BG_COLOR);
	tbY.setBgFillColor(TEXT_BG_COLOR);
	tbZ.setBgFillColor(TEXT_BG_COLOR);

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

	LoadString(hInstance, N_LB_OBJECTS, name, 256);
	lbObjects.Create(name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 - 13,
					VIEWPORT_AREA_H + 52 - LIST_OBJ_H,
					BT_TOOL_W, BT_TOOL_H);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	listObjects.Create(LIST_OBJECTS,
						name,
						&mainForm,
						ufWidth - SUB_TAB_W * 3 - 13,
						VIEWPORT_AREA_H + 73 - LIST_OBJ_H,
						LIST_OBJ_W, LIST_OBJ_H);
	listObjects.setBgFillColor(TEXT_BG_COLOR);
						

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

	delete Pool; 
	mainForm.Destroy();
	return iResult;
}

 //Form event handler functions
 //============================================================================
LRESULT mainForm_OnPaint(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	((LPFORM)Sender)->Validate();
	Pool->RenderWorld();
	return 0L;
}

LRESULT mainForm_keyPressed(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	//switch ( wParam )
	//{
	//}
	//mainForm.Invalidate();

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
				if ( !TRANSLATOR3D::saveSceneScript(&Scene, fName) )
					MessageBox((HWND)lParam, _T("Неправильное имя файла"), _T("Ошибка записи"), MB_ICONERROR | MB_OK );
			}
			break;
		case IDM_LOAD:
			if ( OpenFileDialog((HWND)lParam, ofn) ) {
				CopyMemory(fName, ofn.lpstrFile, ofn.nMaxFile);
				if ( !TRANSLATOR3D::loadSceneScript(&Scene, fName) ) {
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

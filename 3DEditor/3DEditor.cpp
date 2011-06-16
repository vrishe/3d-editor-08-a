// 3DEditor.cpp: определ€ет точку входа дл€ приложени€.
//

#include "3DEditor.h"

// √лобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// “екст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// им€ класса главного окна

HINSTANCE			hInstance;

FORM				mainForm;
UINT				ufWidth, ufHeight;

BOOL				modifyMode;

TOOLS				activeTool;

BUTTON				btMove, btRotate, btScale,
					tabCreate, tabModify, delObject,
					btPan, btCamSphere, btLookAround, btZoomView;

BUTTON				btObjects, btCams, btLights;
BUTTON				btMicrophone, btTank, btBox, 
					btCone, btExCone, btHole, btSphere;
BUTTON				btLight;
BUTTON				btCamera;
BUTTON				btMake, btBind, btProjSet;

LABEL				lbX, lbY, lbZ,
					lbObjects,
					lbQuick,
					lbParams[MAX_PARAMS_NUM];

TEXTBOX				tbX, tbY, tbZ,
					tbParams[MAX_PARAMS_NUM];

LISTBOX				listObjects;

SCENE3D				Scene;
LPVIEWPORT_POOL		Pool;
LPOBJECT3D			activeObject;
COLORREF			activeObjectNSColor;
LPVIEWPORT			activeViewport;
POINTS				mouse;

VECTOR3D			Crosshair[] = { 
						VECTOR3D(-5.0f, .0f, .0f), VECTOR3D(5.0f, .0f, .0f),
						VECTOR3D(.0f, -5.0f, .0f), VECTOR3D(.0f, 5.0f, .0f),
						VECTOR3D(.0f, .0f, -5.0f), VECTOR3D(.0f, .0f, 5.0f)
					};

// Win API entry point:
// ===================================

int APIENTRY _tWinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	INT		iResult;
	HACCEL	hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	hInstance = hInst;

	// Get this stuff below out of here!
	mainForm.Create(
				_T("Main form class"),
				(FORM_TYPE)( WS_OVERLAPPED
							| WS_SYSMENU
							| WS_CLIPCHILDREN
							| WS_CLIPSIBLINGS ), 
				WS_EX_APPWINDOW | WS_EX_CONTROLPARENT,
				CW_USEDEFAULT, 0,
				1270, 770,
				NULL,
				LoadMenu(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR))
			); 
	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	mainForm.setText(szTitle);
	mainForm.AssignEventHandler(WM_CLOSE,		mainForm_OnClose,		TRUE);
	mainForm.AssignEventHandler(WM_COMMAND,		mainForm_InterfClick,	TRUE);
	mainForm.AssignEventHandler(WM_MOUSEWHEEL,	viewport_whMouseRotate,	TRUE);
	mainForm.AssignEventHandler(WM_PAINT,		mainForm_OnPaint,		TRUE);
	mainForm.AssignEventHandler(WM_KEYDOWN,		mainForm_keyPressed,	TRUE);
	mainForm.AssignEventHandler(WM_GETDLGCODE,	mainForm_ProcKeys,		TRUE);
	mainForm.getClientSize(&ufWidth, &ufHeight);
	mainForm.setColor(FORM_BG_COLOR);

	Scene.setAmbientColor(VIEWPORT_BG_COLOR);

	// Viewports here:
	Pool = new VIEWPORT_POOL(&mainForm, &Scene);
	Pool->addViewport(
					10, 
					10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_TOP,
					RM_WIREFRAME
				);
	LoadString(
			hInstance, 
			IDS_VIEWPORT_NAME + VIEW_TOP, 
			szTitle, 
			MAX_LOADSTRING
		); 
	Pool->getViewport(0U)->getCamera()->Roll((FLOAT)M_PI);
	Pool->getViewport(0U)->getCamera()->setName(szTitle, MAX_LOADSTRING);

	Pool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_FRONT,
					RM_WIREFRAME
				);
	LoadString(
			hInstance, 
			IDS_VIEWPORT_NAME + VIEW_FRONT, 
			szTitle, 
			MAX_LOADSTRING
		);
	Pool->getViewport(1U)->getCamera()->setName(szTitle, MAX_LOADSTRING);

	Pool->addViewport(
					10, 
					10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_LEFT,
					RM_WIREFRAME
				);
	LoadString(
			hInstance, 
			IDS_VIEWPORT_NAME + VIEW_LEFT, 
			szTitle,
			MAX_LOADSTRING
		);
	Pool->getViewport(2U)->getCamera()->setName(szTitle, MAX_LOADSTRING);

	Pool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_PERSPECTIVE,
					RM_SHADEDWF
				);
	LoadString(
			hInstance, 
			IDS_VIEWPORT_NAME + VIEW_PERSPECTIVE, 
			szTitle, 
			MAX_LOADSTRING
		);
	Pool->getViewport(3U)->getCamera()->setName(szTitle, MAX_LOADSTRING);

	for ( UINT i = 0; i < Pool->getViewportCount(); i++ )
	{
		Pool->getViewport(i)->AssignEventHandler(
										WM_LBUTTONDOWN,
										viewport_lbMouseDown,
										TRUE
									);
		Pool->getViewport(i)->AssignEventHandler(
										WM_MOUSEMOVE,
										viewport_mouseMove,
										TRUE
									);
		Pool->getViewport(i)->AssignEventHandler(
										WM_LBUTTONUP,
										viewport_lbMouseUp,
										TRUE
									);
		Pool->getViewport(i)->AssignEventHandler(
										WM_MOUSEWHEEL,
										viewport_whMouseRotate,
										TRUE
									);
	}

	Draw_MainToolbars();
	RefreshObjectsList();
	RefreshTextBoxes();
	mainForm.Show();

	GetActiveViewport();
	GetActiveObject();

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR));
	iResult = mainForm.DoMSGCycle(hAccelTable);

	delete Pool; 
	mainForm.Destroy();
	return iResult;
}

// Draw interface functions
 //============================================================================
VOID Draw_MainToolbars() {
	TCHAR *name = new TCHAR[256];
		// object toolbar buttons	
	LoadString(hInstance, N_BT_MOVE, name, 256);
	btMove.Create(	BT_MOVE,
					name,
					&mainForm,
					VIEWPORT_AREA_W / 2 + 50,
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_ROTATE, name, 256);
	btRotate.Create(BT_ROTATE,
					name,
					&mainForm,
					VIEWPORT_AREA_W / 2 + BT_TOOL_W + 55, 
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_SCALE, name, 256);
	btScale.Create(BT_SCALE,
					name,
					&mainForm,
					VIEWPORT_AREA_W / 2 + BT_TOOL_W * 2 + 60, 
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_LB_X, name, 256);
	lbX.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W / 2 + BT_TOOL_W * 3 + 75, 
				VIEWPORT_AREA_H + BT_TOOL_H / 2 + 2,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbY.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W / 2 + BT_TOOL_W * 4 + LB_V_W + 70,
				VIEWPORT_AREA_H + BT_TOOL_H / 2 + 2,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbZ.Create(	name,
				&mainForm, 
				VIEWPORT_AREA_W / 2 + BT_TOOL_W * 5 + LB_V_W * 2 + 65,
				VIEWPORT_AREA_H + BT_TOOL_H / 2 + 2,
				LB_V_W, LB_V_H);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbX.Create(	TB_X,
				_T("0.000"),
				&mainForm, 
				VIEWPORT_AREA_W / 2 + BT_TOOL_W * 3 + LB_V_W + 74, 
				VIEWPORT_AREA_H + BT_TOOL_H / 2,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbY.Create(	TB_Y,
				_T("0.000"),
				&mainForm, 
				VIEWPORT_AREA_W / 2 + BT_TOOL_W * 4 + LB_V_W * 2 + 69, 
				VIEWPORT_AREA_H + BT_TOOL_H / 2,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbZ.Create(	TB_Z,
				_T("0.000"),
				&mainForm, 
				VIEWPORT_AREA_W / 2 + BT_TOOL_W * 5 + LB_V_W * 3 + 64, 
				VIEWPORT_AREA_H + BT_TOOL_H / 2,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbX.setBgFillColor(TEXT_BG_COLOR);
	tbY.setBgFillColor(TEXT_BG_COLOR);
	tbZ.setBgFillColor(TEXT_BG_COLOR);

		// right toolbar
	LoadString(hInstance, N_TAB_CREATE, name, 256);
	tabCreate.Create(TAB_CREATE,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 17, 10,
					TAB_W, TAB_H,
					FALSE);
	LoadString(hInstance, N_TAB_MODIFY, name, 256);
	tabModify.Create(TAB_MODIFY,
					name,
					&mainForm,
					ufWidth - TAB_W - 10, 10,
					TAB_W, TAB_H,
					FALSE);

	LoadString(hInstance, N_LB_OBJECTS, name, 256);
	lbObjects.Create(name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 - 13,
					VIEWPORT_AREA_H - LIST_OBJ_H - 7,
					BT_TOOL_W, BT_TOOL_H / 2);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	listObjects.Create(LIST_OBJECTS,
						name,
						&mainForm,
						ufWidth - SUB_TAB_W * 3 - 18,
						VIEWPORT_AREA_H - LIST_OBJ_H + 12,
						LIST_OBJ_W, LIST_OBJ_H);
	listObjects.setBgFillColor(TEXT_BG_COLOR);
	LoadString(hInstance, N_BT_DELETE, name, 256);
	delObject.Create(BT_DELETE,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 - 18 + LIST_OBJ_W - BT_TOOL_W,
					VIEWPORT_AREA_H - LIST_OBJ_H - 13,
					BT_TOOL_W, BT_TOOL_H - 10
				);

		// camera toolbar buttons
	LoadString(hInstance, N_BT_PAN, name, 256);
	btPan.Create(BT_PAN,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 - 13, 
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_CAMSPHERE, name, 256);
	btCamSphere.Create(BT_CAMSPHERE,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 + BT_TOOL_W - 7, 
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_LOOK_AROUND, name, 256);
	btLookAround.Create(BT_LOOK_AROUND,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 + BT_TOOL_W * 2 - 1,
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	LoadString(hInstance, N_BT_ZOOM, name, 256);
	btZoomView.Create(BT_ZOOM,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 + BT_TOOL_W * 3 + 5,
					VIEWPORT_AREA_H + 10,
					BT_TOOL_W, BT_TOOL_H,
					FALSE);
	delete [] name;
}

VOID Draw_InitCreateToolbar() {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_LIGHTS, name, 256);
	btLights.Create(BT_LIGHTS,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W - 11, 15 + TAB_H,					
					SUB_TAB_W, SUB_TAB_H,
					FALSE);
	LoadString(hInstance, N_BT_CAMS, name, 256);
	btCams.Create(	BT_CAMS,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 2 - 14, 15 + TAB_H,
					SUB_TAB_W, SUB_TAB_H,
					FALSE);
	LoadString(hInstance, N_BT_OBJECTS, name, 256);
	btObjects.Create(BT_OBJECTS,
					name,
					&mainForm,
					ufWidth - SUB_TAB_W * 3 - 17, 15 + TAB_H,
					SUB_TAB_W, SUB_TAB_H,
					FALSE);

	delete [] name;
}
//VOID Draw_InitModifyToolbar (HINSTANCE hInstance) {}
VOID Draw_InitObjectsToolbar() {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_TANK, name, 256);
	btTank.Create(
				BT_TANK,
				name,
				&mainForm,
				ufWidth - TAB_W * 2 - 17, 
				20 + TAB_H + SUB_TAB_H,				
				TAB_W, SUB_TAB_H,
				FALSE);
	/*LoadString(hInstance, N_BT_CONE, name, 256);
	btCone.Create(
				BT_CONE,
				name,
				&mainForm,
				ufWidth - TAB_W - 10, 
				20 + TAB_H + SUB_TAB_H,				
				TAB_W, SUB_TAB_H,
				FALSE);
	LoadString(hInstance, N_BT_EX_CONE, name, 256);
	btExCone.Create(
				BT_EX_CONE,
				name,
				&mainForm,
				ufWidth - TAB_W * 2 - 17, 
				22 + TAB_H + SUB_TAB_H * 2,				
				TAB_W, SUB_TAB_H,
				FALSE);
	LoadString(hInstance, N_BT_HOLE, name, 256);
	btHole.Create(
				BT_HOLE,
				name,
				&mainForm,
				ufWidth - TAB_W - 10, 
				22 + TAB_H + SUB_TAB_H * 2,				
				TAB_W, SUB_TAB_H,
				FALSE);
	LoadString(hInstance, N_BT_SPHERE, name, 256);
	btSphere.Create(
				BT_SPHERE,
				name,
				&mainForm,
				ufWidth - TAB_W * 2 - 17, 
				24 + TAB_H + SUB_TAB_H * 3,				
				TAB_W, SUB_TAB_H,
				FALSE);	

	btBox.Disable();
	btCone.Disable();
	btExCone.Disable();
	btHole.Disable();
	btSphere.Disable();*/

	//LoadString(hInstance, N_BT_MIC, name, 256);
	//btMicrophone.Create(BT_MIC,
	//				name,
	//				&mainForm,
	//				ufWidth - TAB_W * 2 - 17, 
	//				35 + TAB_H + SUB_TAB_H * 4,				
	//				TAB_W, SUB_TAB_H,
	//				FALSE);

	/*LoadString(hInstance, N_BT_TANK, name, 256);
	btTank.Create(BT_TANK,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 17, 
					26 + TAB_H + SUB_TAB_H * 4,				
					TAB_W, SUB_TAB_H,
					FALSE);*/

	delete [] name;
}
VOID Draw_InitLightsToolbar() {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_LIGHT, name, 256);
	btLight.Create(BT_LIGHT,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 17, 
					20 + TAB_H + SUB_TAB_H,				
					TAB_W, SUB_TAB_H,
					FALSE);

	delete [] name;
}
VOID Draw_InitCamsToolbar() {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_CAMERA, name, 256);
	btCamera.Create(BT_CAMERA,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 17, 
					20 + TAB_H + SUB_TAB_H,				
					TAB_W, SUB_TAB_H,
					FALSE);

	delete [] name;
}
VOID Draw_InitMicrophoneToolbar() {
	TCHAR *name = new TCHAR[256];
	name[0]		= '\0';

	LoadString(hInstance, N_LB_NAME, name, 256);
	lbParams[0].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						37 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						46, 16);
	if ( modifyMode )
		((LPMICROPHONE3D)activeObject)->getName(name, 256);
	else
		ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[0].Create(	TB_NAME,
						name,
						&mainForm,
						ufWidth - TAB_W * 2 + 36, 
						35 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						LIST_OBJ_W - 44, BT_TOOL_H - 9);

	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getBaseHeight();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[4].Create(	TB_BDIAM,
						(modifyMode ? name : _T("12")),
						&mainForm,
						ufWidth - 165, 
						218,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getBaseRadius() * 2;
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[5].Create(	TB_BHEIGHT,
						(modifyMode ? name : _T("114")),
						&mainForm,
						ufWidth - 165, 
						243,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getButtonWidth();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[6].Create(	TB_BWIDTH,
						(modifyMode ? name : _T("25")),
						&mainForm,
						ufWidth - 165, 
						268,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getUprightRadius() * 2;
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[7].Create(	TB_UDIAM,
						(modifyMode ? name : _T("16")),
						&mainForm,
						ufWidth - 165, 
						293,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getUprightHeight();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[8].Create(	TB_UHEIGHT,
						(modifyMode ? name : _T("95")),
						&mainForm,
						ufWidth - 165, 
						318,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getUprightGap();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[9].Create(	TB_UGAP,
						(modifyMode ? name : _T("3.5")),
						&mainForm,
						ufWidth - 165, 
						343,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getHandleIndent();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[10].Create(TB_HANDIND,
						(modifyMode ? name : _T("5")),
						&mainForm,
						ufWidth - 165, 
						368,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getHeadRadius() * 2;
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[11].Create(TB_HEADDIAM,
						(modifyMode ? name : _T("55")),
						&mainForm,
						ufWidth - 165, 
						393,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getHeadDepth();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[12].Create(TB_HEADDEPTH,
						(modifyMode ? name : _T("21")),
						&mainForm,
						ufWidth - 165, 
						418,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPMICROPHONE3D)activeObject)->getCoreRadius() * 2;
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[13].Create(TB_COREDIAM,
						(modifyMode ? name : _T("42")),
						&mainForm,
						ufWidth - 165, 
						443,
						75, BT_TOOL_H - 9);
	
	if ( !modifyMode )
	{
		LoadString(hInstance, N_TAB_CREATE, name, 256);
	}
	else
	{
		LoadString(hInstance, N_TAB_MODIFY, name, 256);
	}

	btMake.Create(	BT_MAKEMIC,
				name,
				&mainForm,
				ufWidth - 157, 
				470,
				BT_TOOL_W, BT_TOOL_H);

	LoadString(hInstance, N_LB_BHEIGHT, name, 256);
	lbParams[3].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						220,				
						100, 16);
	LoadString(hInstance, N_LB_BDIAM, name, 256);
	lbParams[4].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						245,				
						100, 16);
	LoadString(hInstance, N_LB_BWIDTH, name, 256);
	lbParams[5].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						270,				
						100, 16);
	LoadString(hInstance, N_LB_UDIAM, name, 256);
	lbParams[6].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						295,				
						100, 16);
	LoadString(hInstance, N_LB_UHEIGHT, name, 256);
	lbParams[7].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						320,				
						100, 16);
	LoadString(hInstance, N_LB_UGAP, name, 256);
	lbParams[8].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						345,				
						100, 16);
	LoadString(hInstance, N_LB_HANDIND, name, 256);
	lbParams[9].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						370,				
						100, 16);
	LoadString(hInstance, N_LB_HEADDIAM, name, 256);
	lbParams[10].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						395,				
						100, 16);
	LoadString(hInstance, N_LB_HEADDEPTH, name, 256);
	lbParams[11].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						420,				
						100, 16);
	LoadString(hInstance, N_LB_COREDIAM, name, 256);
	lbParams[12].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						445,				
						100, 16);

	LoadString(hInstance, N_LB_R, name, 256);
	lbParams[14].Create(	name,
				&mainForm, 
				ufWidth - TAB_W * 2  + 20, 
				147,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_G, name, 256);
	lbParams[15].Create(	name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H + 61, 
				147,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_B, name, 256);
	lbParams[16].Create(	name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H *2 + 102, 
				147,
				LB_V_W, LB_V_H);

	if ( modifyMode ) {
		COLORREF tmp = ((LPMICROPHONE3D)activeObject)->getColor();
		swprintf_s(name, 256, _T("%d"), RED(tmp));
	}
	else
		ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[14].Create(	TB_R,
				name,
				&mainForm, 
				ufWidth - TAB_W * 2  + 36, 
				145,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	if ( modifyMode ) {
		COLORREF tmp = ((LPMICROPHONE3D)activeObject)->getColor();
		swprintf_s(name, 256, _T("%d"), GREEN(tmp));
	}
	tbParams[15].Create(	TB_G,
				name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H + 77, 
				145,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	if ( modifyMode ) {
		COLORREF tmp = ((LPMICROPHONE3D)activeObject)->getColor();
		swprintf_s(name, 256, _T("%d"), BLUE(tmp));
	}
	tbParams[16].Create(	TB_B,
				name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H *2 + 118, 
				145,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	delete [] name;
}

VOID Draw_InitTankToolbar() {
	TCHAR *name = new TCHAR[256];
	name[0]		= '\0';

	LoadString(hInstance, N_LB_NAME, name, 256);
	lbParams[0].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						37 + TAB_H + SUB_TAB_H * 2,				
						46, 16);
	if ( modifyMode )
		((LPJAGDPANTHER3D)activeObject)->getName(name, 256);
	else
		ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[0].Create(	TB_NAME,
						name,
						&mainForm,
						ufWidth - TAB_W * 2 + 36, 
						35 + TAB_H + SUB_TAB_H * 2,
						LIST_OBJ_W - 44, BT_TOOL_H - 9);

	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getBodyLength();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[4].Create(	TB_LENGTH,
						(modifyMode ? name : _T("312")),
						&mainForm,
						ufWidth - 165, 
						190,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getBodyWidth();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[5].Create(	TB_WIDTH,
						(modifyMode ? name : _T("156")),
						&mainForm,
						ufWidth - 165, 
						215,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getBodyHeight();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[6].Create(	TB_HEIGHT,
						(modifyMode ? name : _T("100")),
						&mainForm,
						ufWidth - 165, 
						240,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getFTArmSlope();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[7].Create(	TB_FTA,
						(modifyMode ? name : _T("57")),
						&mainForm,
						ufWidth - 165, 
						265,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getFBArmSlope();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[8].Create(	TB_FBA,
						(modifyMode ? name : _T("56")),
						&mainForm,
						ufWidth - 165, 
						290,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getBTArmSlope();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[9].Create(	TB_BTA,
						(modifyMode ? name : _T("13")),
						&mainForm,
						ufWidth - 165, 
						315,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getBBArmSlope();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[10].Create(TB_BBA,
						(modifyMode ? name : _T("24")),
						&mainForm,
						ufWidth - 165, 
						340,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getTopArmSlope();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[11].Create(TB_TA,
						(modifyMode ? name : _T("6.5")),
						&mainForm,
						ufWidth - 165, 
						365,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getSideArmSlope();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[12].Create(TB_SA,
						(modifyMode ? name : _T("30")),
						&mainForm,
						ufWidth - 165, 
						390,
						75, BT_TOOL_H - 9);
	if ( modifyMode ) {
		float tmp = ((LPJAGDPANTHER3D)activeObject)->getCannonLength();
		swprintf_s(name, 256, _T("%.2f"), tmp);
	}
	tbParams[13].Create(TB_CLEN,
						(modifyMode ? name : _T("104")),
						&mainForm,
						ufWidth - 165, 
						415,
						75, BT_TOOL_H - 9);
	
	if ( !modifyMode )
	{
		LoadString(hInstance, N_TAB_CREATE, name, 256);
	}
	else
	{
		LoadString(hInstance, N_TAB_MODIFY, name, 256);
	}

	btMake.Create(	BT_MAKETANK,
				name,
				&mainForm,
				ufWidth - SUB_TAB_W * 3 - 18 + LIST_OBJ_W - BT_TOOL_W, 
				407,
				BT_TOOL_W, BT_TOOL_H);

	LoadString(hInstance, N_LB_LENGTH, name, 256);
	lbParams[3].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						192,				
						100, 16);
	LoadString(hInstance, N_LB_WIDTH, name, 256);
	lbParams[4].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						217,				
						100, 16);
	LoadString(hInstance, N_LB_HEIGHT, name, 256);
	lbParams[5].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						242,				
						100, 16);
	LoadString(hInstance, N_LB_FTA, name, 256);
	lbParams[6].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						267,				
						100, 16);
	LoadString(hInstance, N_LB_FBA, name, 256);
	lbParams[7].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						292,				
						100, 16);
	LoadString(hInstance, N_LB_BTA, name, 256);
	lbParams[8].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						317,				
						100, 16);
	LoadString(hInstance, N_LB_BBA, name, 256);
	lbParams[9].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						342,				
						100, 16);
	LoadString(hInstance, N_LB_TA, name, 256);
	lbParams[10].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						367,				
						100, 16);
	LoadString(hInstance, N_LB_SA, name, 256);
	lbParams[11].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						392,				
						100, 16);
	LoadString(hInstance, N_LB_CLEN, name, 256);
	lbParams[12].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						417,				
						100, 16);

	LoadString(hInstance, N_LB_R, name, 256);
	lbParams[14].Create(	name,
				&mainForm, 
				ufWidth - TAB_W * 2  + 20, 
				147,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_G, name, 256);
	lbParams[15].Create(	name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H + 61, 
				147,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_B, name, 256);
	lbParams[16].Create(	name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H *2 + 102, 
				147,
				LB_V_W, LB_V_H);

	if ( modifyMode ) {
		COLORREF tmp = ((LPJAGDPANTHER3D)activeObject)->getColor();
		swprintf_s(name, 256, _T("%d"), RED(tmp));
	}
	else
		ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[14].Create(	TB_R,
				name,
				&mainForm, 
				ufWidth - TAB_W * 2  + 36, 
				145,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	if ( modifyMode ) {
		COLORREF tmp = ((LPJAGDPANTHER3D)activeObject)->getColor();
		swprintf_s(name, 256, _T("%d"), GREEN(tmp));
	}
	tbParams[15].Create(	TB_G,
				name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H + 77, 
				145,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	if ( modifyMode ) {
		COLORREF tmp = ((LPJAGDPANTHER3D)activeObject)->getColor();
		swprintf_s(name, 256, _T("%d"), BLUE(tmp));
	}
	tbParams[16].Create(	TB_B,
				name,
				&mainForm, 
				ufWidth - TAB_W * 2 + LB_V_H *2 + 118, 
				145,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	delete [] name;
}


VOID Draw_InitLightToolbar() {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_LB_NAME, name, 256);
	lbParams[0].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						37 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						46, 16);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[0].Create(	TB_NAME,
						name,
						&mainForm,
						ufWidth - TAB_W * 2 + 36, 
						35 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						LIST_OBJ_W - 44, BT_TOOL_H - 9);

	LoadString(hInstance, N_LB_R, name, 256);
	lbParams[1].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H * 3 - 122, 
				152,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_G, name, 256);
	lbParams[2].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H*2 - 82, 
				152,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_B, name, 256);
	lbParams[3].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H - 40, 
				152,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_X, name, 256);
	lbParams[4].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H * 3 - 122, 
				175,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbParams[5].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H*2 - 82, 
				175,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbParams[6].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H - 40, 
				175,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_POWER, name, 256);
	lbParams[7].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H - 550, 
				200,
				LB_V_W, LB_V_H);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[1].Create(	TB_R,
				_T("150"),
				&mainForm, 
				ufWidth - LB_V_H * 2 - 127, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[2].Create(	TB_G,
				_T("150"),
				&mainForm, 
				ufWidth - LB_V_H - 86, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[3].Create(	TB_B,
				_T("150"),
				&mainForm, 
				ufWidth - 45, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[4].Create(	TB_TX,
				_T("-1"),
				&mainForm, 
				ufWidth - LB_V_H * 2 - 127, 
				173,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[5].Create(	TB_TY,
				_T("-1"),
				&mainForm, 
				ufWidth - LB_V_H - 86, 
				173,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[6].Create(	TB_TZ,
				_T("-1"),
				&mainForm, 
				ufWidth - 45, 
				173,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[7].Create(	TB_COREDIAM,
				_T("0.5"),
				&mainForm, 
				ufWidth - 45, 
				198,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	if ( modifyMode )
	{
		LPDIFLIGHT3D	light	= (LPDIFLIGHT3D)activeObject;
		VECTOR3D		lPos	= light->getPosition();
		COLORREF		lColor	= light->getColor();
		
		light->getName(name, 256);
		tbParams[0].setText(name);

		_stprintf_s(name, 256, _T("%d"), RED(lColor));
		tbParams[1].setText(name);
		_stprintf_s(name, 256, _T("%d"), GREEN(lColor));
		tbParams[2].setText(name);
		_stprintf_s(name, 256, _T("%d"), BLUE(lColor));
		tbParams[3].setText(name);

		_stprintf_s(name, 256, _T("%.3f"), lPos.x);
		tbParams[4].setText(name);
		_stprintf_s(name, 256, _T("%.3f"), lPos.y);
		tbParams[5].setText(name);		
		_stprintf_s(name, 256, _T("%.3f"), lPos.z);
		tbParams[6].setText(name);

		_stprintf_s(name, 256, _T("%.3f"), light->getPower());
		tbParams[7].setText(name);
		LoadString(hInstance, N_TAB_MODIFY, name, 256);
	}
	else
	{
		LoadString(hInstance, N_TAB_CREATE, name, 256);
	}


	btMake.Create(	BT_MAKELIGHT,
				name,
				&mainForm,
				ufWidth - 80, 
				225,
				BT_TOOL_W, BT_TOOL_H);
	

	delete [] name;
}

VOID Draw_InitCameraToolbar() {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_LB_NAME, name, 256);
	lbParams[0].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						37 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						45, 16);
	LoadString(hInstance, N_LB_TARGET, name, 256);
	lbParams[10].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						152,				
						71, 16);
	LoadString(hInstance, N_LB_POSITION, name, 256);
	lbParams[11].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						175,				
						71, 16);
	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[0].Create(	TB_NAME,
						name,
						&mainForm,
						ufWidth - TAB_W * 2 + 36, 
						35 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						LIST_OBJ_W - 44, BT_TOOL_H - 9);

	LoadString(hInstance, N_LB_X, name, 256);
	lbParams[1].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H * 3 - 122, 
				152,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbParams[2].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H*2 - 82, 
				152,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbParams[3].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H - 40, 
				152,
				LB_V_W, LB_V_H);

	LoadString(hInstance, N_LB_X, name, 256);
	lbParams[4].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H * 3 - 122, 
				175,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbParams[5].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H*2 - 82, 
				175,
				LB_V_W, LB_V_H);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbParams[6].Create(	name,
				&mainForm, 
				ufWidth - LB_V_H - 40, 
				175,
				LB_V_W, LB_V_H);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[1].Create(	TB_TX,
				_T("0.000"),
				&mainForm, 
				ufWidth - LB_V_H * 2 - 127, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[2].Create(	TB_TY,
				_T("0.000"),
				&mainForm, 
				ufWidth - LB_V_H - 86, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[3].Create(	TB_TZ,
				_T("0.000"),
				&mainForm, 
				ufWidth - 45, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	tbParams[4].Create(	TB_R,
				_T("0.000"),
				&mainForm, 
				ufWidth - LB_V_H * 2 - 127, 
				177,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[5].Create(	TB_G,
				_T("0.000"),
				&mainForm, 
				ufWidth - LB_V_H - 86, 
				177,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[6].Create(	TB_B,
				_T("0.000"),
				&mainForm, 
				ufWidth - 45, 
				177,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	if ( modifyMode )
	{
		LPTARGCAMERA3D	cam		= (LPTARGCAMERA3D)activeObject;
		VECTOR3D		vPos	= vPos = cam->getTargetPosition();
		UINT			btID;

		cam->getName(name, 256);
		tbParams[0].setText(name);
		_stprintf_s(name, 256, _T("%.3f"), vPos.x);
		tbParams[1].setText(name);
		_stprintf_s(name, 256, _T("%.3f"), vPos.y);
		tbParams[2].setText(name);
		_stprintf_s(name, 256, _T("%.3f"), vPos.z);
		tbParams[3].setText(name);
		vPos = cam->getPosition();
		_stprintf_s(name, 256, _T("%.3f"), vPos.x);
		tbParams[4].setText(name);
		_stprintf_s(name, 256, _T("%.3f"), vPos.y);
		tbParams[5].setText(name);
		_stprintf_s(name, 256, _T("%.3f"), vPos.z);
		tbParams[6].setText(name);
		
		if ( 
			activeViewport != NULL
			&& activeViewport->getCamera() == cam 
		) {
			LoadString(hInstance, N_BT_RELEASECAM, name, 256);
			btID = BT_RELEASE;
		}
		else
		{
			LoadString(hInstance, N_BT_BINDCAM, name, 256);
			btID = BT_BIND;
		}

		btBind.Create(  btID,
					name,
					&mainForm,
					ufWidth - 85 - BT_TOOL_W,
					210,
					BT_TOOL_W, BT_TOOL_H
				);	
		if ( activeViewport == NULL ) btBind.Disable();

		LoadString(
			hInstance, 
			N_BT_PROJ_CENTRAL - cam->getProjectionType(),
			name, 
			256
		);
		btProjSet.Create( BT_PROJSET,
					name,
					&mainForm,
					ufWidth - 90 - BT_TOOL_W * 2,
					210,
					BT_TOOL_W, BT_TOOL_H
				);

		LoadString(hInstance, N_TAB_MODIFY, name, 256);	
	}
	else
	{
		LoadString(hInstance, N_TAB_CREATE, name, 256);
	}

	btMake.Create(	BT_MAKECAM,
						name,
						&mainForm,
						ufWidth - 80, 
						210,
						BT_TOOL_W, BT_TOOL_H);

	delete [] name;
}

VOID Draw_DestroyRightToolbar () {
	btObjects.Destroy();
	btCams.Destroy();
	btLights.Destroy();


	//btMicrophone.Destroy();
	btTank.Destroy();
	btBox.Destroy();
	btCone.Destroy();
	btExCone.Destroy();
	btHole.Destroy();
	btSphere.Destroy();
	btLight.Destroy();
	btCamera.Destroy();

	btMake.Destroy();
	btBind.Destroy();
	btProjSet.Destroy();

	for ( UINT i = 0; i < MAX_PARAMS_NUM; i++ ) {
		lbParams[i].Destroy();
		tbParams[i].Destroy();
	}
}

VOID RefreshObjectsList() {
	listObjects.clear();
	UINT N = Scene.getObjectClassCount(CLS_MESH);
	for ( UINT i = 0; i < N; i++ ) {
		TCHAR *buf = new TCHAR[256];
		LPMESH3D mesh = (LPMESH3D)Scene.getObject(CLS_MESH, i);
		mesh->getName(buf, 256);
		listObjects.addItem(buf, mesh);
	}
	N = Scene.getObjectClassCount(CLS_CAMERA);
	for ( UINT i = 0; i < N; i++ ) {
		TCHAR *buf = new TCHAR[256];
		LPCAMERA3D cam = (LPCAMERA3D)Scene.getObject(CLS_CAMERA, i);
		cam->getName(buf, 256);
		listObjects.addItem(buf, cam);
	}
	N = Scene.getObjectClassCount(CLS_LIGHT);
	for ( UINT i = 0; i < N; i++ ) {
		TCHAR *buf = new TCHAR[256];
		LPDIFLIGHT3D light = (LPDIFLIGHT3D)Scene.getObject(CLS_LIGHT, i);
		light->getName(buf, 256);
		listObjects.addItem(buf, light);
	}
}

 //Controls functions
 //============================================================================
UINT CreateMic() {
	TCHAR *buf = new TCHAR[256];
	tbParams[4].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bH = (FLOAT)(FLOAT)_ttof(buf);
	if ( bH < 3 ) {
		delete [] buf;
		return 1;
	}

	tbParams[5].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bR = (FLOAT)(FLOAT)_ttof(buf) / 2;
	if ( bR < 1 ) {
		delete [] buf;
		return 2;
	}
	
	tbParams[6].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bW = (FLOAT)(FLOAT)_ttof(buf);
	if ( bW < 1  || bW > 0.75f * bR ) {
		delete [] buf;
		return 3;
	}

	tbParams[12].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hD = (FLOAT)(FLOAT)_ttof(buf);
	if ( hD < 1 || hD > bR ) {
		delete [] buf;
		return 8;
	}

	tbParams[7].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uR = (FLOAT)(FLOAT)_ttof(buf) / 2;
	if ( uR < 1 || uR > hD) {
		delete [] buf;
		return 4;
	}

	tbParams[8].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uH = (FLOAT)(FLOAT)_ttof(buf);
	if ( uH < 1) {
		delete [] buf;
		return 5;
	}

	tbParams[9].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uG = (FLOAT)(FLOAT)_ttof(buf);
	if ( uG < 1 || uG > uH * 0.7f) {
		delete [] buf;
		return 6;
	}

	tbParams[10].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hI = (FLOAT)(FLOAT)_ttof(buf);
	if ( hI < 1 || hI > uH * 0.8f) {
		delete [] buf;
		return 7;
	}

	tbParams[11].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hR = (FLOAT)(FLOAT)_ttof(buf) / 2;

	tbParams[13].getText(buf, 256 * sizeof(TCHAR));
	FLOAT cR = (FLOAT)(FLOAT)_ttof(buf) / 2;
	if ( cR < 0.5 || cR > hR ) {
		delete [] buf;
		return 10;
	}
	if ( hR < 0.5 || hR < cR) {
		delete [] buf;
		return 9;
	}

	tbParams[14].getText(buf, 256 * sizeof(TCHAR));
	UCHAR r = _ttoi(buf);
	tbParams[15].getText(buf, 256 * sizeof(TCHAR));
	UCHAR g = _ttoi(buf);
	tbParams[16].getText(buf, 256 * sizeof(TCHAR));
	UCHAR b = _ttoi(buf);

	if ( r == 0 && g == 0 && b == 0 ) {
		r = 200; g = 200; b = 200;
	}

	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( _tcslen(buf) == 0 ) {
		delete [] buf;
		return 11;
	}
	
 	LPMICROPHONE3D mic = new MICROPHONE3D(r, g, b, bR, bH, bW, uR, uH, uG, hI, hR, hD, cR);
	mic->setName(buf, 256);
	Scene.AddObject(mic);

	RefreshObjectsList();
	GetActiveObject();

	delete[] buf;
	return 0;
}

UINT ModifMic() {
	if ( activeObject == NULL ) return 100;

	TCHAR *buf = new TCHAR[256];
	LPMICROPHONE3D mic = (LPMICROPHONE3D)activeObject;

	tbParams[4].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bH = (FLOAT)_wtof(buf);
	if ( bH < 3 )
		return 1;

	tbParams[5].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bR = (FLOAT)_wtof(buf) / 2;
	if ( bR < 1 )
		return 2;
	
	tbParams[6].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bW = (FLOAT)_wtof(buf);
	if ( bW < 1 || bW > 0.7f * bR )
		return 3;

	tbParams[12].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hD = (FLOAT)_wtof(buf);
	if ( hD < 1 || hD > bR )
		return 8;	

	tbParams[7].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uR = (FLOAT)_wtof(buf) / 2;
	if ( uR < 1 || uR > hD )
		return 4;

	tbParams[8].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uH = (FLOAT)_wtof(buf);
	if ( uH < 1 )
		return 5;

	tbParams[9].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uG = (FLOAT)_wtof(buf);
	if ( uG < 1 || uG > .7f * uH )
		return 6;

	tbParams[10].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hI = (FLOAT)_wtof(buf);
	if ( hI < 1 || hI > uH * 0.8f )
		return 7;
	

	tbParams[11].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hR = (FLOAT)_wtof(buf) / 2;
	if ( hR < 1 )
		return 9;

	tbParams[13].getText(buf, 256 * sizeof(TCHAR));
	FLOAT cR = (FLOAT)_wtof(buf) / 2;
	if ( cR < 1 || cR > hR )
		return 10;

	tbParams[14].getText(buf, 256 * sizeof(TCHAR));
	int r = _wtoi(buf);
	tbParams[15].getText(buf, 256 * sizeof(TCHAR));
	int g = _wtoi(buf);
	tbParams[16].getText(buf, 256 * sizeof(TCHAR));
	int b = _wtoi(buf);

	if ( r == 0 && g == 0 && b == 0 ) {
		r = RED(mic->getColor());
		g = GREEN(mic->getColor());
		b = BLUE(mic->getColor());
	}

	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( _tcslen(buf) == 0 )
		return 11;

	VECTOR3D pos = mic->getPosition();
	VECTOR3D fwd = mic->getForwardLookDirection();
	VECTOR3D uwd = mic->getUpLookDirection();
	VECTOR3D rwd = mic->getRightLookDirection();

	Scene.DeleteObject(mic);
	delete mic;

	mic = new MICROPHONE3D(r, g, b, bR, bH, bW, uR, uH, uG, hI, hR, hD, cR);
	mic->Translate(pos);
	mic->setForwardLookDirection(fwd);
	mic->setUpLookDirection(uwd);
	mic->setRightLookDirection(rwd);
	mic->setName(buf, 256);
	Scene.AddObject(mic);

	INT curSel = listObjects.getCurSel();
	RefreshObjectsList();
	listObjects.setCurSel(curSel);
	GetActiveObject();
	
	delete[] buf;
	return 0;
}

UINT ModifTank() {
	TCHAR *buf = new TCHAR[256];

	tbParams[4].getText(buf, 256);
	FLOAT tL = (FLOAT)(FLOAT)_ttof(buf);
	if ( tL < 100 || tL > 1000 ) {
		delete [] buf;
		return 1;
	}

	tbParams[5].getText(buf, 256);
	FLOAT tW = (FLOAT)(FLOAT)_ttof(buf);
	if ( tW < tL * .5f || tW > tL * .75f ) {
		delete [] buf;
		return 2;
	}
	
	tbParams[6].getText(buf, 256);
	FLOAT tH = (FLOAT)(FLOAT)_ttof(buf);
	if ( tH < tL * .3f  || tH > .4f * tL ) {
		delete [] buf;
		return 3;
	}

	tbParams[7].getText(buf, 256);
	FLOAT ftA = (FLOAT)(FLOAT)_ttof(buf);
	if ( ftA < 45 || ftA > 59) {
		delete [] buf;
		return 4;
	}

	tbParams[8].getText(buf, 256);
	FLOAT fbA = (FLOAT)(FLOAT)_ttof(buf);
	if ( fbA < 45 || fbA > 59) {
		delete [] buf;
		return 5;
	}

	tbParams[9].getText(buf, 256);
	FLOAT btA = (FLOAT)(FLOAT)_ttof(buf);
	if ( btA < 5 || btA > 20) {
		delete [] buf;
		return 6;
	}

	tbParams[10].getText(buf, 256);
	FLOAT bbA = (FLOAT)(FLOAT)_ttof(buf);
	if ( bbA < 5 || bbA > 24) {
		delete [] buf;
		return 7;
	}

	tbParams[11].getText(buf, 256);
	FLOAT tA = (FLOAT)(FLOAT)_ttof(buf);
	if ( tA < 0 || tA > 15) {
		delete [] buf;
		return 8;
	}
	
	tbParams[12].getText(buf, 256);
	FLOAT sA = (FLOAT)(FLOAT)_ttof(buf);
	if ( sA < 10 || sA > 32 ) {
		delete [] buf;
		return 9;
	}

	tbParams[13].getText(buf, 256);
	FLOAT cLen = (FLOAT)(FLOAT)_ttof(buf);
	if ( cLen < tL / 3 || cLen > tL ) {
		delete [] buf;
		return 10;
	}

	tbParams[14].getText(buf, 256);
	UCHAR r = _ttoi(buf);
	tbParams[15].getText(buf, 256);
	UCHAR g = _ttoi(buf);
	tbParams[16].getText(buf, 256);
	UCHAR b = _ttoi(buf);

	if ( r == 0 && g == 0 && b == 0 ) {
		r = 200; g = 200; b = 200;
	}

	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete [] buf;
		return 11;
	}

	LPJAGDPANTHER3D tank = (LPJAGDPANTHER3D)activeObject;
	VECTOR3D pos = tank->getPosition();
	VECTOR3D fwd = tank->getForwardLookDirection();
	VECTOR3D uwd = tank->getUpLookDirection();
	VECTOR3D rwd = tank->getRightLookDirection();
	Scene.DeleteObject(tank);
	delete tank;
	
 	tank = new JAGDPANTHER3D(tL, tW, tH, fbA, ftA, bbA, btA, tA, sA, cLen, r, g, b);
	tank->Translate(pos);
	tank->setForwardLookDirection(fwd);
	tank->setUpLookDirection(uwd);
	tank->setRightLookDirection(rwd);
	tank->setName(buf, 256);
	Scene.AddObject(tank);

	INT curSel = listObjects.getCurSel();
	RefreshObjectsList();
	listObjects.setCurSel(curSel);
	GetActiveObject();

	delete[] buf;
	return 0;
}

UINT CreateTank() {
	TCHAR *buf = new TCHAR[256];

	tbParams[4].getText(buf, 256);
	FLOAT tL = (FLOAT)(FLOAT)_ttof(buf);
	if ( tL < 100 || tL > 1000 ) {
		delete [] buf;
		return 1;
	}

	tbParams[5].getText(buf, 256);
	FLOAT tW = (FLOAT)(FLOAT)_ttof(buf);
	if ( tW < tL * .5f || tW > tL * .75f ) {
		delete [] buf;
		return 2;
	}
	
	tbParams[6].getText(buf, 256);
	FLOAT tH = (FLOAT)(FLOAT)_ttof(buf);
	if ( tH < tL * .3f  || tH > .4f * tL ) {
		delete [] buf;
		return 3;
	}

	tbParams[7].getText(buf, 256);
	FLOAT ftA = (FLOAT)(FLOAT)_ttof(buf);
	if ( ftA < 45 || ftA > 59) {
		delete [] buf;
		return 4;
	}

	tbParams[8].getText(buf, 256);
	FLOAT fbA = (FLOAT)(FLOAT)_ttof(buf);
	if ( fbA < 45 || fbA > 59) {
		delete [] buf;
		return 5;
	}

	tbParams[9].getText(buf, 256);
	FLOAT btA = (FLOAT)(FLOAT)_ttof(buf);
	if ( btA < 5 || btA > 20) {
		delete [] buf;
		return 6;
	}

	tbParams[10].getText(buf, 256);
	FLOAT bbA = (FLOAT)(FLOAT)_ttof(buf);
	if ( bbA < 5 || bbA > 24) {
		delete [] buf;
		return 7;
	}

	tbParams[11].getText(buf, 256);
	FLOAT tA = (FLOAT)(FLOAT)_ttof(buf);
	if ( tA < 0 || tA > 15) {
		delete [] buf;
		return 8;
	}

	
	tbParams[12].getText(buf, 256);
	FLOAT sA = (FLOAT)(FLOAT)_ttof(buf);
	if ( sA < 10 || sA > 32 ) {
		delete [] buf;
		return 9;
	}

	tbParams[13].getText(buf, 256);
	FLOAT cLen = (FLOAT)(FLOAT)_ttof(buf);
	if ( cLen < tL * .3f || cLen > tL ) {
		delete [] buf;
		return 10;
	}

	tbParams[14].getText(buf, 256);
	UCHAR r = _ttoi(buf);
	tbParams[15].getText(buf, 256);
	UCHAR g = _ttoi(buf);
	tbParams[16].getText(buf, 256);
	UCHAR b = _ttoi(buf);

	if ( r == 0 && g == 0 && b == 0 ) {
		r = 200; g = 200; b = 200;
	}

	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete [] buf;
		return 11;
	}

 	LPJAGDPANTHER3D tank = new JAGDPANTHER3D(tL, tW, tH, fbA, ftA, bbA, btA, tA, sA, cLen, r, g, b);
	tank->setName(buf, 256);
	Scene.AddObject(tank);

	RefreshObjectsList();
	GetActiveObject();

	delete[] buf;
	return 0;
}

BOOL CreateBox() {
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256);
	UCHAR r = _ttoi(buf);
	tbParams[2].getText(buf, 256);
	UCHAR g = _ttoi(buf);
	tbParams[3].getText(buf, 256);
	UCHAR b = _ttoi(buf);

	tbParams[4].getText(buf, 256);
	FLOAT x = (FLOAT)_ttof(buf);
	tbParams[5].getText(buf, 256);
	FLOAT y = (FLOAT)_ttof(buf);
	tbParams[6].getText(buf, 256);
	FLOAT z = (FLOAT)_ttof(buf);

	tbParams[7].getText(buf, 256);
	FLOAT p = (FLOAT)_ttof(buf);
	
	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete[] buf;
		return false;
	}
	LPDIFLIGHT3D light = new DIFLIGHT3D(r, g, b);
	light->setName(buf, 256);
	light->LookAt(x, y, z);
	light->setPower(p);
	Scene.AddObject(light);

	RefreshObjectsList();
	GetActiveObject();

	delete[] buf;
	return true;
}

BOOL CreateLight() {
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256);
	UCHAR r = _ttoi(buf);
	tbParams[2].getText(buf, 256);
	UCHAR g = _ttoi(buf);
	tbParams[3].getText(buf, 256);
	UCHAR b = _ttoi(buf);

	tbParams[4].getText(buf, 256);
	FLOAT x = (FLOAT)_ttof(buf);
	tbParams[5].getText(buf, 256);
	FLOAT y = (FLOAT)_ttof(buf);
	tbParams[6].getText(buf, 256);
	FLOAT z = (FLOAT)_ttof(buf);

	tbParams[7].getText(buf, 256);
	FLOAT p = (FLOAT)_ttof(buf);
	
	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete[] buf;
		return false;
	}
	LPDIFLIGHT3D light = new DIFLIGHT3D(r, g, b);
	light->setName(buf, 256);
	light->LookAt(x, y, z);
	light->setPower(p);
	Scene.AddObject(light);

	RefreshObjectsList();
	GetActiveObject();

	delete[] buf;
	return true;
}

BOOL ModifLight() {
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256);
	UCHAR r = _ttoi(buf);
	tbParams[2].getText(buf, 256);
	UCHAR g = _ttoi(buf);
	tbParams[3].getText(buf, 256);
	UCHAR b = _ttoi(buf);

	tbParams[4].getText(buf, 256);
	FLOAT x = (FLOAT)_ttof(buf);
	tbParams[5].getText(buf, 256);
	FLOAT y = (FLOAT)_ttof(buf);
	tbParams[6].getText(buf, 256);
	FLOAT z = (FLOAT)_ttof(buf);

	tbParams[7].getText(buf, 256);
	FLOAT p = (FLOAT)_ttof(buf);
	
	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete[] buf;
		return false;
	}
	LPDIFLIGHT3D light = (LPDIFLIGHT3D)activeObject;
	light->setName(buf, 256);
	light->LookAt(x, y, z);
	light->setColor(r, g, b);
	light->setPower(p);

	INT curSel = listObjects.getCurSel();
	RefreshObjectsList();
	listObjects.setCurSel(curSel);
	GetActiveObject();

	delete[] buf;
	return true;
}

UINT CreateCam() {
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256);
	FLOAT tx = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[2].getText(buf, 256);
	FLOAT ty = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[3].getText(buf, 256);
	FLOAT tz = (FLOAT)(FLOAT)_ttof(buf);

	tbParams[4].getText(buf, 256);
	FLOAT x = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[5].getText(buf, 256);
	FLOAT y = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[6].getText(buf, 256);
	FLOAT z = (FLOAT)(FLOAT)_ttof(buf);
	
	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete[] buf;
		return 1;
	}
	if ( tx == x && ty == y && tz == z ) {
		delete[] buf;
		return 2;
	}

	LPTARGCAMERA3D cam = new TARGCAMERA3D();
	cam->setName(buf, 256);
	cam->Translate(x, y, z);
	cam->TargetTranslate(tx, ty, tz);
	Scene.AddObject(cam);

	if ( activeViewport != NULL ) 
		activeViewport->setCamera(cam);

	RefreshObjectsList();
	GetActiveObject();

	delete[] buf;
	return 0;
}

BOOL ModifCam()
{
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256);
	FLOAT tx = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[2].getText(buf, 256);
	FLOAT ty = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[3].getText(buf, 256);
	FLOAT tz = (FLOAT)(FLOAT)_ttof(buf);

	tbParams[4].getText(buf, 256);
	FLOAT x = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[5].getText(buf, 256);
	FLOAT y = (FLOAT)(FLOAT)_ttof(buf);
	tbParams[6].getText(buf, 256);
	FLOAT z = (FLOAT)(FLOAT)_ttof(buf);
	
	tbParams[0].getText(buf, 256);
	if ( _tcslen(buf) == 0 ) {
		delete[] buf;
		return 1;
	}
	if ( tx == x && ty == y && tz == z ) {
		delete[] buf;
		return 2;
	}
	LPTARGCAMERA3D cam = (LPTARGCAMERA3D)activeObject;
	cam->setName(buf, 256);
	cam->Translate(x, y, z);
	cam->TargetTranslate(tx, ty, tz);

	INT curSel = listObjects.getCurSel();
	RefreshObjectsList();
	listObjects.setCurSel(curSel);
	GetActiveObject();

	delete[] buf;
	return 0;
}

BOOL ToPoint() {
	if ( activeObject == NULL ) return false;

	FLOAT	m_pi_d_180	= (FLOAT)M_PI / 180.0f;
	TCHAR *buf = new TCHAR[256];

	tbX.getText(buf, 256);
	FLOAT x = (FLOAT)_ttof(buf);
	tbY.getText(buf, 256);
	FLOAT y = (FLOAT)_ttof(buf);
	tbZ.getText(buf, 256);
	FLOAT z = (FLOAT)_ttof(buf);

	delete[] buf;

	switch ( activeTool ) {
	case IS_MOVE:
		activeObject->Translate(x, y, z);
		break;
	case IS_ROTATE:
		activeObject->Rotate(y * m_pi_d_180, z * m_pi_d_180, x * m_pi_d_180);
		break;
	case IS_SCALE:
		activeObject->Scale(x, y, z);
		break;
	}

	if ( activeObject->clsID() >= CLS_HULL ) ((LPHULL3D)activeObject)->Transform();
	return true;
}

BOOL Draw_RefreshModifyToolbar() {
	if ( !modifyMode ) return false;
	Draw_DestroyRightToolbar();
	if ( activeObject == NULL ) return false;
	switch ( activeObject->clsID() ) {
	case CLS_MESH:
		Draw_InitTankToolbar();
		break;
	case CLS_LIGHT:
		Draw_InitLightToolbar();
		break;
	case CLS_CAMERA:
		Draw_InitCameraToolbar();
		break;
	}
	return true;
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
	FLOAT	dirH	= .0f,
			dirV	= .0f,
			dirD	= .0f;
	BYTE	keyBrd[256];
	if ( wParam == VK_DELETE ) DeleteActiveObject();

	if ( activeViewport == NULL
		|| 	!GetKeyboardState(keyBrd) ) return 1L;
	if ( wParam == VK_SPACE )
	{
		if ( activeViewport->getRenderMode() < RM_SHADEDWF )
			activeViewport->setRenderMode(
					(RENDER_MODE)(activeViewport->getRenderMode() + 1)
				);
		else
			activeViewport->setRenderMode(RM_WIREFRAME);
		mainForm.Invalidate();
	}

	if ( activeObject == NULL ) return 1L;

	LPTARGCAMERA3D cam = activeViewport->getCamera();
	switch ( activeTool ) {
	case IS_MOVE:
		if ( keyBrd[VK_LEFT] & 0x80 )	dirH -= PAN_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirH += PAN_ASPECT;
		activeObject->Strafe(dirH);

		if ( keyBrd[VK_UP] & 0x80 )		dirV += PAN_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirV -= PAN_ASPECT;
		activeObject->Fly(dirV);

		if ( keyBrd[VK_HOME] & 0x80 )	dirD += PAN_ASPECT;
		if ( keyBrd[VK_END] & 0x80 )	dirD -= PAN_ASPECT;
		activeObject->Follow(dirD);

		if ( activeObject->clsID() >= CLS_HULL ) 
				((LPHULL3D)activeObject)->Transform();
		RefreshTextBoxes();
		break;

	case IS_ROTATE:
		if ( keyBrd[VK_LEFT] & 0x80 )	dirH -= ROTATION_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirH += ROTATION_ASPECT;
		activeObject->Yaw(dirH);

		if ( keyBrd[VK_UP] & 0x80 )		dirV += ROTATION_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirV -= ROTATION_ASPECT;
		activeObject->Pitch(dirV);

		if ( keyBrd[VK_HOME] & 0x80 )	dirD += ROTATION_ASPECT;
		if ( keyBrd[VK_END] & 0x80 )	dirD -= ROTATION_ASPECT;
		activeObject->Roll(dirD);

		if ( activeObject->clsID() >= CLS_HULL ) 
				((LPHULL3D)activeObject)->Transform();
		RefreshTextBoxes();
		break;

	case IS_SCALE:
		dirH = dirV = dirD = 1.0f;
		if ( keyBrd[VK_LEFT] & 0x80 )	dirH /= SCALE_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirH *= SCALE_ASPECT;
		activeObject->ScaleAlong(dirH);

		if ( keyBrd[VK_UP] & 0x80 )		dirV /= SCALE_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirV *= SCALE_ASPECT;
		activeObject->ScaleVAcross(dirV);

		if ( keyBrd[VK_HOME] & 0x80 )	dirD /= SCALE_ASPECT;
		if ( keyBrd[VK_END] & 0x80 )	dirD *= SCALE_ASPECT;
		activeObject->ScaleHAcross(dirD);
		
		if ( activeObject->clsID() >= CLS_HULL ) 
				((LPHULL3D)activeObject)->Transform();
		break;

	case IS_PAN:
		if ( keyBrd[VK_LEFT] & 0x80 )	dirH -= PAN_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirH += PAN_ASPECT;
		cam->Strafe(dirH);

		if ( keyBrd[VK_UP] & 0x80 )		dirV += PAN_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirV -= PAN_ASPECT;
		cam->Fly(dirV);

		if ( keyBrd[VK_HOME] & 0x80 )	dirD += PAN_ASPECT;
		if ( keyBrd[VK_END] & 0x80 )	dirD -= PAN_ASPECT;
		cam->Follow(dirD);

		cam->Transform();
		break;

	case IS_ZOOM:
		dirH = cam->getHFov();
		dirV = cam->getVFov();

		if ( keyBrd[VK_LEFT] & 0x80 )	dirV -= ZOOM_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirV += ZOOM_ASPECT;
		cam->setVFov(dirV);

		if ( keyBrd[VK_UP] & 0x80 )		dirH += ZOOM_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirH -= ZOOM_ASPECT;
		cam->setHFov(dirH);

		cam->Transform();
		break;

	case IS_CAMROTATE:
		if ( keyBrd[VK_LEFT] & 0x80 )	dirH -= PAN_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirH += PAN_ASPECT;
		cam->StrafeLatitude(dirH);

		if ( keyBrd[VK_UP] & 0x80 )		dirV += PAN_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirV -= PAN_ASPECT;
		cam->StrafeLongitude(dirV);

		if ( keyBrd[VK_HOME] & 0x80 )	dirD += PAN_ASPECT;
		if ( keyBrd[VK_END] & 0x80 )	dirD -= PAN_ASPECT;
		cam->FollowLookAxis(dirD);

		cam->Transform();
		break;

	case IS_LOOK:
		if ( keyBrd[VK_LEFT] & 0x80 )	dirH -= PAN_ASPECT;
		if ( keyBrd[VK_RIGHT] & 0x80 )	dirH += PAN_ASPECT;
		cam->TargetStrafe(dirH);

		if ( keyBrd[VK_UP] & 0x80 )		dirV += PAN_ASPECT;
		if ( keyBrd[VK_DOWN] & 0x80 )	dirV -= PAN_ASPECT;
		cam->TargetFly(dirV);

		if ( keyBrd[VK_HOME] & 0x80 )	dirD += PAN_ASPECT;
		if ( keyBrd[VK_END] & 0x80 )	dirD -= PAN_ASPECT;
		cam->TargetFollow(dirD);

		cam->Transform();
		break;
	}

	mainForm.Invalidate();
	return 0L;
}

LRESULT mainForm_InterfClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME	ofn			= { 0 };
	LPOBJECT3D		tempObject;
	UINT			error;
	BOOL			bResult;

	switch (LOWORD(wParam))
	{
		case BT_MOVE:
			if ( activeTool != IS_MOVE )
				activeTool = IS_MOVE;
			else
				ToPoint();
			RefreshTextBoxes();
			mainForm.Invalidate();
			mainForm.setFocus();
			break;

		case BT_ROTATE:
			if ( activeTool != IS_ROTATE )
				activeTool = IS_ROTATE;
			else
				ToPoint();
			RefreshTextBoxes();
			mainForm.Invalidate();
			mainForm.setFocus();
			break;

		case BT_SCALE:
			if ( activeTool != IS_SCALE )
				activeTool = IS_SCALE;
			else
				ToPoint();
			RefreshTextBoxes();
			mainForm.Invalidate();
			mainForm.setFocus();
			break;

		case BT_PAN:
			activeTool = IS_PAN;
			mainForm.setFocus();
			break;

		case BT_CAMSPHERE:
			activeTool = IS_CAMROTATE;
			mainForm.setFocus();
			break;

		case BT_LOOK_AROUND:
			activeTool = IS_LOOK;
			mainForm.setFocus();
			break;

		case BT_ZOOM:
			activeTool = IS_ZOOM;
			mainForm.setFocus();
			break;

		case BT_DELETE:
			DeleteActiveObject();
			break;

		case TAB_CREATE:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			modifyMode = false;
			break;

		case TAB_MODIFY:
			modifyMode = true;
			Draw_RefreshModifyToolbar();
			break;

		case BT_OBJECTS:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			Draw_InitObjectsToolbar();
			break;

		case BT_LIGHTS:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			Draw_InitLightsToolbar();
			break;

		//case BT_MIC:
		//	Draw_DestroyRightToolbar();
		//	Draw_InitCreateToolbar();
		//	Draw_InitObjectsToolbar();
		//	Draw_InitMicrophoneToolbar();
		//	tbParams[0].setFocus();
		//	break;

		case BT_TANK:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			Draw_InitObjectsToolbar();
			Draw_InitTankToolbar();
			tbParams[0].setFocus();
			break;

		case BT_LIGHT:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			Draw_InitLightsToolbar();
			Draw_InitLightToolbar();
			tbParams[0].setFocus();
			break;

		case BT_CAMS:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			Draw_InitCamsToolbar();
			break;

		case BT_CAMERA:
			Draw_DestroyRightToolbar();
			Draw_InitCreateToolbar();
			Draw_InitCamsToolbar();
			Draw_InitCameraToolbar();
			tbParams[0].setFocus();
			break;

		case BT_MAKELIGHT:
			if ( modifyMode )
				bResult = ModifLight();
			else
				bResult = CreateLight();

			if ( !bResult )
				MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );

			mainForm.Invalidate();
			break;

		case BT_MAKECAM:
			if ( modifyMode )
				bResult = ModifCam();
			else
				bResult = CreateCam();

			if ( bResult == 1 )
				MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );
			if ( bResult == 2 )
				MessageBox(NULL, _T("Wrong parameters"), _T("Error"), MB_OK | MB_ICONERROR );

			mainForm.Invalidate();
			break;

		case BT_BIND:
			if ( activeViewport != NULL )
			{
				activeViewport->setCamera((LPTARGCAMERA3D)activeObject);
				Draw_RefreshModifyToolbar();
				mainForm.Invalidate();
			}
			break;

		case BT_RELEASE:
			if ( activeViewport != NULL )
			{
				activeViewport->setCameraByObjectID(DEFAULT_CAMERA_ID);
				Draw_RefreshModifyToolbar();
				mainForm.Invalidate();
			}
			break;

		case BT_PROJSET:
			if ( ((LPTARGCAMERA3D)activeObject)->getProjectionType() < PT_CENTRAL )
				((LPTARGCAMERA3D)activeObject)->setProjectionType(
					(PROJECTION_TYPE)(((LPTARGCAMERA3D)activeObject)->getProjectionType() + 1)
				);
			else
				((LPTARGCAMERA3D)activeObject)->setProjectionType(PT_PARALLEL);
			Draw_RefreshModifyToolbar();
			mainForm.Invalidate();
			break;

		//case BT_MAKEMIC:
		//	if ( modifyMode )
		//		error = ModifMic();
		//	else
		//		error = CreateMic();

		//	switch (error) 
		//	{
		//		case 1:
		//			MessageBox(NULL, _T("To small height."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 2:
		//			MessageBox(NULL, _T("To small radius."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 3:
		//			MessageBox(NULL, _T("Wrong button size."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 4:
		//			MessageBox(NULL, _T("Wrong upright diameter."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 5:
		//			MessageBox(NULL, _T("To small upright."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 6:
		//			MessageBox(NULL, _T("Wrong upright gap size."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 7:
		//			MessageBox(NULL, _T("Wrong handle indent"), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 8:
		//			MessageBox(NULL, _T("Wrong head depth."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 9:
		//			MessageBox(NULL, _T("Wrong head diameter size"), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 10:
		//			MessageBox(NULL, _T("Wrong core diameter size."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//		case 11:
		//			MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );
		//			break;
		//	}

		//	ToPoint();
		//	RefreshTextBoxes();
		//	mainForm.Invalidate();
		//	mainForm.setFocus();
		//	break;

		case BT_MAKETANK:
			if ( modifyMode )
				error = ModifTank();
			else
				error = CreateTank();

			switch (error) 
			{
				case 1:
					MessageBox(NULL, _T("Length must be in range: [100; 1000]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 2:
					MessageBox(NULL, _T("Width must be in range: [.5 * Length; .75 * Length]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 3:
					MessageBox(NULL, _T("Height must be in range: [.3 * Length; .4 * Length]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 4:
					MessageBox(NULL, _T("Frontal top armor sheet slope must be in: [45, 59]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 5:
					MessageBox(NULL, _T("Frontal bottom armor sheet slope must be in: [45, 59]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 6:
					MessageBox(NULL, _T("Back top armor sheet slope must be in: [5, 20]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 7:
					MessageBox(NULL, _T("Back bottom armor sheet slope must be in: [5, 24]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 8:
					MessageBox(NULL, _T("Top armor sheet slope must be in: [5, 15]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 9:
					MessageBox(NULL, _T("Side armor sheets slope must be in: [10, 32]."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 10:
					MessageBox(NULL, _T("Cannon length must be in range: [.3 * Length; Length]"), _T("Error"), MB_OK | MB_ICONERROR );
					break;
				case 11:
					MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );
					break;
			}

			ToPoint();
			RefreshTextBoxes();
			mainForm.Invalidate();
			mainForm.setFocus();
			break;

		case LIST_OBJECTS:
				switch ( HIWORD(wParam) ) 
				{
					case LBN_SELCHANGE:
						tempObject = activeObject;
						GetActiveObject();
						if ( activeObject != tempObject )
							Draw_RefreshModifyToolbar();
						mainForm.setFocus();
						break;
				}
			break;

		case IDM_NEW:
			SceneCleanUp();
			mainForm.Invalidate();
			break;

		case IDM_SAVE:
			listObjects.setCurSel(-1);
			GetActiveObject();
			mainForm.Invalidate();

			ofn.lpstrFile		= new TCHAR[256];
			ofn.lpstrFile[0]	= '\0';
			ofn.nMaxFile		= 256;
			ofn.lpstrFilter		= OPEN_FILE_FILTER;
			ofn.nFilterIndex	= 2;
			ofn.lpstrFileTitle	= NULL;
			ofn.nMaxFileTitle	= 0;
			ofn.lpstrInitialDir = NULL;
			if ( mainForm.SFDShow(&ofn) ) 
			{
				if ( !TRANSLATOR3D::saveSceneScript(&Scene, ofn.lpstrFile) )
					MessageBox(
						(HWND)lParam, 
						_T("Wrong file name"), 
						_T("Saving error"), 
						MB_ICONERROR | MB_OK 
					);
			}
			delete[] ofn.lpstrFile;
			break;

		case IDM_LOAD:
			listObjects.setCurSel(-1);
			GetActiveObject();
			mainForm.Invalidate();

			ofn.lpstrFile		= new TCHAR[256];
			ofn.lpstrFile[0]	= '\0';
			ofn.nMaxFile		= 256;
			ofn.lpstrFilter		= OPEN_FILE_FILTER;
			ofn.nFilterIndex	= 2;
			ofn.lpstrFileTitle	= NULL;
			ofn.nMaxFileTitle	= 0;
			ofn.lpstrInitialDir = NULL;
			if ( mainForm.OFDShow(&ofn) ) 
			{
				SceneCleanUp();
				if ( !TRANSLATOR3D::loadSceneScript(&Scene, ofn.lpstrFile) ) 
					MessageBox(
						(HWND)lParam,
						_T("Can not open this file"), 
						_T("Opening error"), 
						MB_ICONERROR | MB_OK 
					);
				RefreshObjectsList();
				Scene.setAmbientColor(VIEWPORT_BG_COLOR);
				RefreshObjectsList();
				mainForm.Invalidate();
			}
			delete[] ofn.lpstrFile;
			break;

		case IDM_ABOUT:
			mainForm.DBShow(
					MAKEINTRESOURCE(IDD_ABOUTBOX), 
					About_DialogBox_Handler
				);
			break;

		case IDM_EXIT:
			mainForm_OnClose(&mainForm, 0, 0);
			break;

		default: return 1L;
	}
	return 0L;
}

LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam) 
{ return DLGC_WANTALLKEYS; }

LRESULT mainForm_OnClose(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	SceneCleanUp();
	((LPFORM)Sender)->Destroy();
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

// ============================================================================
// Mouse handling funcs:
// ============================================================================
LRESULT viewport_lbMouseDown(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	mouse		= MAKEPOINTS(lParam);
	Pool->setActiveViewport((DWORD)((LPVIEWPORT)Sender)->getID());
	GetActiveViewport();
	mainForm.Invalidate();
	((LPFORM)Sender)->captureMouse();

	return 0L;
}

LRESULT viewport_lbMouseUp(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();

	Draw_RefreshModifyToolbar();
	mainForm.setFocus();

	return 0L;
}

LRESULT viewport_mouseMove(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	if ( wParam != MK_LBUTTON
		|| activeViewport == NULL ) return 1L;

	POINTS	cur = MAKEPOINTS(lParam);
	SHORT	aspectH = cur.x - mouse.x,
			aspectV = cur.y - mouse.y;

	FLOAT	dirH	= 0,
			dirV	= 0;

	dirH -= (FLOAT)aspectH;
	dirV += (FLOAT)aspectV;

	LPTARGCAMERA3D cam = activeViewport->getCamera();
	switch(activeTool)
	{
		case IS_PAN:
			cam->Strafe(dirH);
			cam->Fly(dirV);
			break;

		case IS_CAMROTATE:
			cam->StrafeLatitude(dirH);
			cam->StrafeLongitude(-dirV);
			break;

		case IS_LOOK:
			cam->TargetStrafe(dirH);
			cam->TargetFly(dirV);
			break;
	}

	mainForm.Invalidate();
	mouse = cur;
	return 0L;
}

LRESULT viewport_whMouseRotate(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	if ( LOWORD(wParam) != MK_CONTROL
		|| activeViewport == NULL ) return 1L;

	FLOAT	dirD; 
	FLOAT	sign = WHEEL_SENS 
					* GET_WHEEL_DELTA_WPARAM(wParam) 
					/ WHEEL_DELTA;

	LPTARGCAMERA3D cam = activeViewport->getCamera();
	switch(activeTool)
	{
		case IS_PAN:
			dirD = PAN_ASPECT * sign;
			cam->Follow(dirD);
			break;

		case IS_ZOOM:
			dirD = ZOOM_ASPECT * sign;
			cam->Zoom(dirD);
			break;

		case IS_CAMROTATE:
			dirD = PAN_ASPECT * sign;
			cam->FollowLookAxis(dirD);
			break;

		case IS_LOOK:
			dirD = PAN_ASPECT * sign;
			cam->TargetFollow(dirD);
			break;
	}

	mainForm.Invalidate();
	return 0L;
}

VOID RefreshTextBoxes()
{
	VECTOR3D	vData;
	TCHAR		bufX[10] = _T("\0"),
				bufY[10] = _T("\0"),
				bufZ[10] = _T("\0");

	if ( activeObject != NULL )
	{
		switch( activeTool )
		{
			case IS_MOVE:
				vData = activeObject->getPosition();
				break;
			case IS_ROTATE:
				vData = activeObject->getRotation();
				vData *= 180.0f / (FLOAT)M_PI;
				break;
			case IS_SCALE:
				vData = activeObject->getScale();
				break;
		};
		_stprintf_s(bufX, _T("%.3f"), vData.x);
		_stprintf_s(bufY, _T("%.3f"), vData.y);
		_stprintf_s(bufZ, _T("%.3f"), vData.z);
	}
	tbX.setText(bufX);
	tbY.setText(bufY);
	tbZ.setText(bufZ);
}

VOID GetActiveViewport()
{
	activeViewport = NULL;
	activeViewport = Pool->getActiveViewport();
	if ( activeViewport == NULL ) activeTool = IS_NONE;

}

VOID GetActiveObject()
{
	ActiveObjectColorReset();

	activeObject = NULL;
	listObjects.getItem(
			listObjects.getCurSel(), 
			NULL, 
			0, 
			(LPVOID*)&activeObject
		);
	if ( activeObject != NULL ) 
	{
		ActiveObjectHighlight();

		delObject.Enable();
		tabModify.Enable();
		btMove.Enable();
		btRotate.Enable();
		btScale.Enable();
		tbX.Enable();
		tbY.Enable();
		tbZ.Enable();
		RefreshTextBoxes();
	}
	else
	{
		delObject.Disable();
		tabModify.Disable();
		btMove.Disable();
		btRotate.Disable();
		btScale.Disable();
		tbX.Disable();
		tbY.Disable();
		tbZ.Disable();
	}
}

VOID ActiveObjectHighlight()
{
	if (
		activeObject != NULL
		&& activeObject->clsID() == CLS_MESH
	) {
		activeObjectNSColor = ((LPMESH3D)activeObject)->getColor();
		((LPMESH3D)activeObject)->setColor(0x00FFFFFF);
	}
	mainForm.Invalidate();
}

VOID ActiveObjectColorReset()
{
	if ( 
		activeObject != NULL
		&& activeObject->clsID() == CLS_MESH
	) ((LPMESH3D)activeObject)->setColor(activeObjectNSColor);
}

VOID DeleteActiveObject()
{
	Scene.DeleteObject(activeObject);
	if (activeObject != NULL ) 
	{
		if ( activeObject->clsID() == CLS_CAMERA )
		{
			for ( UINT i = 0; i < Pool->getViewportCount(); i++ )
			{
				if ( Pool->getViewport(i)->getCamera() == activeObject )
					Pool->getViewport(i)->setCameraByObjectID(DEFAULT_CAMERA_ID);
			}
		}
		delete activeObject;
	}
	RefreshObjectsList();
	mainForm.Invalidate();
	GetActiveObject();
	Draw_RefreshModifyToolbar();
}

VOID SceneCleanUp()
{
	while ( listObjects.getItem(0, NULL, 0, (LPVOID*)&activeObject) )
	{
		if ( Scene.DeleteObject(activeObject) ) 
		{
			delete activeObject;
			activeObject = NULL;
			listObjects.delItem(0);
		}
		
	}
}


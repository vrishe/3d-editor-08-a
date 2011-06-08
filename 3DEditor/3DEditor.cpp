// 3DEditor.cpp: определяет точку входа для приложения.
//

#include "3DEditor.h"

// Глобальные переменные:
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Remove this stuff:
FORM				mainForm;
UINT				ufWidth, ufHeight;

TOOLS				activeTool;

BUTTON				btMove, btRotate, btScale,
					tabCreate, tabModify,
					btPan, btCamSphere, btLookAround;

BUTTON				btObjects, btCams, btLights;
BUTTON				btMicrophone;
BUTTON				btLight;
BUTTON				btCamera;
BUTTON				btQMake, btFMake;

LABEL				lbX, lbY, lbZ,
					lbObjects,
					lbQuick,
					lbParams[MAX_PARAMS_NUM];

TEXTBOX				tbX, tbY, tbZ,
					tbParams[MAX_PARAMS_NUM];

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
	mainForm.AssignEventHandler(WM_COMMAND, mainForm_InterfClick, TRUE);
	mainForm.AssignEventHandler(WM_PAINT, mainForm_OnPaint, TRUE);
	mainForm.AssignEventHandler(WM_KEYDOWN, mainForm_keyPressed, TRUE);
	mainForm.AssignEventHandler(WM_GETDLGCODE, mainForm_ProcKeys, TRUE);
	mainForm.getClientSize(&ufWidth, &ufHeight);
	mainForm.setColor(FORM_BG_COLOR);

	Scene.setAmbientColor(VIEWPORT_BG_COLOR);
	MICROPHONE3D testMic(60, 60, 140);
	testMic.Fly(-120);
//	Scene.AddObject(&testMic);

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
	Pool->getViewport(0U)->AssignEventHandler(
									WM_LBUTTONDOWN,
									viewport_lbMouseClick,
									TRUE
								);

	Pool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					BT_TOOL_H + 10,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_FRONT,
					RM_WIREFRAME
				);
	Pool->getViewport(1U)->AssignEventHandler(
									WM_LBUTTONDOWN,
									viewport_lbMouseClick,
									TRUE
								);

	Pool->addViewport(
					10, 
					BT_TOOL_H + 10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_LEFT,
					RM_WIREFRAME
				);
	Pool->getViewport(2U)->AssignEventHandler(
									WM_LBUTTONDOWN,
									viewport_lbMouseClick,
									TRUE
								);

	Pool->addViewport(
					10 + VIEWPORT_AREA_W / 2, 
					BT_TOOL_H + 10 + VIEWPORT_AREA_H / 2,
					VIEWPORT_AREA_W / 2 - 5,
					VIEWPORT_AREA_H / 2 - 5,
					VIEW_PERSPECTIVE,
					RM_SHADEDWF
				);
	Pool->getViewport(3U)->AssignEventHandler(
									WM_LBUTTONDOWN,
									viewport_lbMouseClick,
									TRUE
								);
	Pool->setActiveViewport(3U);
	Draw_MainToolbars(hInstance);
	RefreshObjectsList();

	mainForm.Show();

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DEDITOR));
	iResult = mainForm.DoMSGCycle(hAccelTable);

	delete Pool; 
	mainForm.Destroy();
	return iResult;
}

// Draw interface functions
 //============================================================================
VOID Draw_MainToolbars (HINSTANCE hInstance) {
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
	btCamSphere.Create(BT_CAMSPHERE,
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
}

VOID Draw_InitCreateToolbar (HINSTANCE hInstance) {
	TCHAR *name = new TCHAR[256];

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

	delete [] name;
}
VOID Draw_InitModifyToolbar (HINSTANCE hInstance) {}
VOID Draw_InitObjectsToolbar (HINSTANCE hInstance) {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_MIC, name, 256);
	btMicrophone.Create(BT_MIC,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 12, 
					20 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H,				
					TAB_W, SUB_TAB_H,
					FALSE);

	delete [] name;
}
VOID Draw_InitLightsToolbar (HINSTANCE hInstance) {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_LIGHT, name, 256);
	btLight.Create(BT_LIGHT,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 12, 
					20 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H,				
					TAB_W, SUB_TAB_H,
					FALSE);

	delete [] name;
}
VOID Draw_InitCamsToolbar (HINSTANCE hInstance) {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_BT_CAMERA, name, 256);
	btCamera.Create(BT_CAMERA,
					name,
					&mainForm,
					ufWidth - TAB_W * 2 - 12, 
					20 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H,				
					TAB_W, SUB_TAB_H,
					FALSE);

	delete [] name;
}
VOID Draw_InitMicrophoneToolbar (HINSTANCE hInstance) {
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

	LoadString(hInstance, N_LB_QUICK, name, 256);
	lbQuick.Create(	name,
					&mainForm,
					ufWidth - TAB_W * 2 - 12, 
					60 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
					100, 16);
	LoadString(hInstance, N_LB_HEIGHT, name, 256);
	lbParams[1].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						83 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						46, 16);
	LoadString(hInstance, N_LB_BDIAM, name, 256);
	lbParams[2].Create(	name,
						&mainForm,
						ufWidth - 140, 
						83 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						56, 16);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[2].Create(	TB_HEIGHT,
						name,
						&mainForm,
						ufWidth - 225, 
						80 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						80, BT_TOOL_H - 9);
	tbParams[3].Create(	TB_BDIAM,
						name,
						&mainForm,
						ufWidth - 80, 
						80 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						75, BT_TOOL_H - 9);
	tbParams[4].Create(	TB_BDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						218,
						75, BT_TOOL_H - 9);
	tbParams[5].Create(	TB_BHEIGHT,
						name,
						&mainForm,
						ufWidth - 165, 
						242,
						75, BT_TOOL_H - 9);
	tbParams[6].Create(	TB_BWIDTH,
						name,
						&mainForm,
						ufWidth - 165, 
						268,
						75, BT_TOOL_H - 9);
	tbParams[7].Create(	TB_UDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						293,
						75, BT_TOOL_H - 9);
	tbParams[8].Create(	TB_UHEIGHT,
						name,
						&mainForm,
						ufWidth - 165, 
						318,
						75, BT_TOOL_H - 9);
	tbParams[9].Create(	TB_UGAP,
						name,
						&mainForm,
						ufWidth - 165, 
						343,
						75, BT_TOOL_H - 9);
	tbParams[10].Create(TB_HANDIND,
						name,
						&mainForm,
						ufWidth - 165, 
						368,
						75, BT_TOOL_H - 9);
	tbParams[11].Create(TB_HEADDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						393,
						75, BT_TOOL_H - 9);
	tbParams[12].Create(TB_HEADDEPTH,
						name,
						&mainForm,
						ufWidth - 165, 
						418,
						75, BT_TOOL_H - 9);
	tbParams[13].Create(TB_COREDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						443,
						75, BT_TOOL_H - 9);
	
	LoadString(hInstance, N_TAB_CREATE, name, 256);
	btQMake.Create(	BT_QMAKE,
					name,
					&mainForm,
					ufWidth - 80, 
					195,
					BT_TOOL_W, BT_TOOL_H);
	btFMake.Create(	BT_FMAKE,
					name,
					&mainForm,
					ufWidth - 80, 
					495,
					BT_TOOL_W, BT_TOOL_H);

	LoadString(hInstance, N_LB_BHEIGHT, name, 256);
	lbParams[3].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						220,				
						120, 16);
	LoadString(hInstance, N_LB_BDIAM, name, 256);
	lbParams[4].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						245,				
						120, 16);
	LoadString(hInstance, N_LB_BWIDTH, name, 256);
	lbParams[5].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						270,				
						120, 16);
	LoadString(hInstance, N_LB_UDIAM, name, 256);
	lbParams[6].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						295,				
						120, 16);
	LoadString(hInstance, N_LB_UHEIGHT, name, 256);
	lbParams[7].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						320,				
						120, 16);
	LoadString(hInstance, N_LB_UGAP, name, 256);
	lbParams[8].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						345,				
						120, 16);
	LoadString(hInstance, N_LB_HANDIND, name, 256);
	lbParams[9].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						370,				
						120, 16);
	LoadString(hInstance, N_LB_HEADDIAM, name, 256);
	lbParams[10].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						395,				
						120, 16);
	LoadString(hInstance, N_LB_HEADDEPTH, name, 256);
	lbParams[11].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						420,				
						120, 16);
	LoadString(hInstance, N_LB_COREDIAM, name, 256);
	lbParams[12].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						445,				
						120, 16);

	LoadString(hInstance, N_LB_R, name, 256);
	lbParams[14].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W * 3 - 122, 
				470,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_G, name, 256);
	lbParams[15].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W*2 - 82, 
				470,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_B, name, 256);
	lbParams[16].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 40, 
				470,
				LB_V_H, LB_V_W);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[14].Create(	TB_R,
				name,
				&mainForm, 
				ufWidth - LB_V_W * 2 - 127, 
				468,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[15].Create(	TB_G,
				name,
				&mainForm, 
				ufWidth - LB_V_W - 86, 
				468,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[16].Create(	TB_B,
				name,
				&mainForm, 
				ufWidth - 45, 
				468,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	delete [] name;
}
VOID Draw_InitLightToolbar (HINSTANCE hInstance) {
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
				ufWidth - LB_V_W * 3 - 122, 
				152,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_G, name, 256);
	lbParams[2].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W*2 - 82, 
				152,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_B, name, 256);
	lbParams[3].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 40, 
				152,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_X, name, 256);
	lbParams[4].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W * 3 - 122, 
				175,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbParams[5].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W*2 - 82, 
				175,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbParams[6].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 40, 
				175,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_POWER, name, 256);
	lbParams[7].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 550, 
				200,
				LB_V_H, LB_V_W);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[1].Create(	TB_R,
				name,
				&mainForm, 
				ufWidth - LB_V_W * 2 - 127, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[2].Create(	TB_G,
				name,
				&mainForm, 
				ufWidth - LB_V_W - 86, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[3].Create(	TB_B,
				name,
				&mainForm, 
				ufWidth - 45, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[4].Create(	TB_TX,
				name,
				&mainForm, 
				ufWidth - LB_V_W * 2 - 127, 
				173,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[5].Create(	TB_TY,
				name,
				&mainForm, 
				ufWidth - LB_V_W - 86, 
				173,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[6].Create(	TB_TZ,
				name,
				&mainForm, 
				ufWidth - 45, 
				173,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[7].Create(	TB_COREDIAM,
				name,
				&mainForm, 
				ufWidth - 45, 
				198,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	LoadString(hInstance, N_TAB_CREATE, name, 256);
	btFMake.Create(	BT_MAKELIGHT,
					name,
					&mainForm,
					ufWidth - 80, 
					225,
					BT_TOOL_W, BT_TOOL_H);
	

	delete [] name;
}
VOID Draw_InitCameraToolbar (HINSTANCE hInstance) {
	TCHAR *name = new TCHAR[256];

	LoadString(hInstance, N_LB_NAME, name, 256);
	lbParams[0].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						37 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						30, 16);
	LoadString(hInstance, N_LB_TARGET, name, 256);
	lbParams[10].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 11, 
						152,				
						71, 16);
	LoadString(hInstance, N_LB_POSITION, name, 256);
	lbParams[10].Create(name,
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
				ufWidth - LB_V_W * 3 - 122, 
				152,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbParams[2].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W*2 - 82, 
				152,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbParams[3].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 40, 
				152,
				LB_V_H, LB_V_W);

	LoadString(hInstance, N_LB_X, name, 256);
	lbParams[4].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W * 3 - 122, 
				175,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Y, name, 256);
	lbParams[5].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W*2 - 82, 
				175,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_Z, name, 256);
	lbParams[6].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 40, 
				175,
				LB_V_H, LB_V_W);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[1].Create(	TB_TX,
				name,
				&mainForm, 
				ufWidth - LB_V_W * 2 - 127, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[2].Create(	TB_TY,
				name,
				&mainForm, 
				ufWidth - LB_V_W - 86, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[3].Create(	TB_TZ,
				name,
				&mainForm, 
				ufWidth - 45, 
				150,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	tbParams[4].Create(	TB_R,
				name,
				&mainForm, 
				ufWidth - LB_V_W * 2 - 127, 
				177,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[5].Create(	TB_G,
				name,
				&mainForm, 
				ufWidth - LB_V_W - 86, 
				177,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[6].Create(	TB_B,
				name,
				&mainForm, 
				ufWidth - 45, 
				177,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	LoadString(hInstance, N_TAB_CREATE, name, 256);
	btFMake.Create(	BT_MAKECAM,
					name,
					&mainForm,
					ufWidth - 80, 
					210,
					BT_TOOL_W, BT_TOOL_H);
	

	delete [] name;
}

VOID Draw_ModifyMicrophoneToolbar (HINSTANCE hInstance) {
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

	LoadString(hInstance, N_LB_QUICK, name, 256);
	lbQuick.Create(	name,
					&mainForm,
					ufWidth - TAB_W * 2 - 12, 
					60 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
					100, 16);
	LoadString(hInstance, N_LB_HEIGHT, name, 256);
	lbParams[1].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						83 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						46, 16);
	LoadString(hInstance, N_LB_BDIAM, name, 256);
	lbParams[2].Create(	name,
						&mainForm,
						ufWidth - 140, 
						83 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,				
						56, 16);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[2].Create(	TB_HEIGHT,
						name,
						&mainForm,
						ufWidth - 225, 
						80 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						80, BT_TOOL_H - 9);
	tbParams[3].Create(	TB_BDIAM,
						name,
						&mainForm,
						ufWidth - 80, 
						80 + BT_TOOL_H / 2 + TAB_H + SUB_TAB_H * 2,
						75, BT_TOOL_H - 9);
	tbParams[4].Create(	TB_BDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						218,
						75, BT_TOOL_H - 9);
	tbParams[5].Create(	TB_BHEIGHT,
						name,
						&mainForm,
						ufWidth - 165, 
						242,
						75, BT_TOOL_H - 9);
	tbParams[6].Create(	TB_BWIDTH,
						name,
						&mainForm,
						ufWidth - 165, 
						268,
						75, BT_TOOL_H - 9);
	tbParams[7].Create(	TB_UDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						293,
						75, BT_TOOL_H - 9);
	tbParams[8].Create(	TB_UHEIGHT,
						name,
						&mainForm,
						ufWidth - 165, 
						318,
						75, BT_TOOL_H - 9);
	tbParams[9].Create(	TB_UGAP,
						name,
						&mainForm,
						ufWidth - 165, 
						343,
						75, BT_TOOL_H - 9);
	tbParams[10].Create(TB_HANDIND,
						name,
						&mainForm,
						ufWidth - 165, 
						368,
						75, BT_TOOL_H - 9);
	tbParams[11].Create(TB_HEADDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						393,
						75, BT_TOOL_H - 9);
	tbParams[12].Create(TB_HEADDEPTH,
						name,
						&mainForm,
						ufWidth - 165, 
						418,
						75, BT_TOOL_H - 9);
	tbParams[13].Create(TB_COREDIAM,
						name,
						&mainForm,
						ufWidth - 165, 
						443,
						75, BT_TOOL_H - 9);
	
	LoadString(hInstance, N_TAB_MODIFY, name, 256);
	
	btFMake.Create(	BT_MODIF,
					name,
					&mainForm,
					ufWidth - 80, 
					495,
					BT_TOOL_W, BT_TOOL_H);

	LoadString(hInstance, N_LB_BHEIGHT, name, 256);
	lbParams[3].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						220,				
						120, 16);
	LoadString(hInstance, N_LB_BDIAM, name, 256);
	lbParams[4].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						245,				
						120, 16);
	LoadString(hInstance, N_LB_BWIDTH, name, 256);
	lbParams[5].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						270,				
						120, 16);
	LoadString(hInstance, N_LB_UDIAM, name, 256);
	lbParams[6].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						295,				
						120, 16);
	LoadString(hInstance, N_LB_UHEIGHT, name, 256);
	lbParams[7].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						320,				
						120, 16);
	LoadString(hInstance, N_LB_UGAP, name, 256);
	lbParams[8].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						345,				
						120, 16);
	LoadString(hInstance, N_LB_HANDIND, name, 256);
	lbParams[9].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						370,				
						120, 16);
	LoadString(hInstance, N_LB_HEADDIAM, name, 256);
	lbParams[10].Create(	name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						395,				
						120, 16);
	LoadString(hInstance, N_LB_HEADDEPTH, name, 256);
	lbParams[11].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						420,				
						120, 16);
	LoadString(hInstance, N_LB_COREDIAM, name, 256);
	lbParams[12].Create(name,
						&mainForm,
						ufWidth - TAB_W * 2 - 12, 
						445,				
						120, 16);

	LoadString(hInstance, N_LB_R, name, 256);
	lbParams[14].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W * 3 - 122, 
				470,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_G, name, 256);
	lbParams[15].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W*2 - 82, 
				470,
				LB_V_H, LB_V_W);
	LoadString(hInstance, N_LB_B, name, 256);
	lbParams[16].Create(	name,
				&mainForm, 
				ufWidth - LB_V_W - 40, 
				470,
				LB_V_H, LB_V_W);

	ZeroMemory(name, 256 * sizeof(TCHAR));
	tbParams[14].Create(	TB_R,
				name,
				&mainForm, 
				ufWidth - LB_V_W * 2 - 127, 
				468,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[15].Create(	TB_G,
				name,
				&mainForm, 
				ufWidth - LB_V_W - 86, 
				468,
				BT_TOOL_W - 19, BT_TOOL_H - 9);
	tbParams[16].Create(	TB_B,
				name,
				&mainForm, 
				ufWidth - 45, 
				468,
				BT_TOOL_W - 19, BT_TOOL_H - 9);

	delete [] name;
}

VOID Draw_DestroyRightToolbar () {
	btObjects.Destroy();
	btCams.Destroy();
	btLights.Destroy();

	btMicrophone.Destroy();
	btLight.Destroy();
	btCamera.Destroy();

	lbQuick.Destroy();
	btQMake.Destroy();
	btFMake.Destroy();
	//btModif.
	for ( UINT i = 0; i < MAX_PARAMS_NUM; i++ ) {
		lbParams[i].Destroy();
		tbParams[i].Destroy();
	}
}

VOID RefreshObjectsList () {
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
BOOL CreateMicFast() {
	TCHAR *buf = new TCHAR[256];

	tbParams[2].getText(buf, 256 * sizeof(TCHAR));
	FLOAT h = (FLOAT)_wtof(buf);
	if ( h < 10 ) {
		delete [] buf;
		return false;
	}

	tbParams[3].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bR = (FLOAT)_wtof(buf) / 2;
	if ( bR < 3 ) {
		delete [] buf;
		return false;
	}
	
	tbParams[14].getText(buf, 256 * sizeof(TCHAR));
	UCHAR r = _wtoi(buf);
	tbParams[15].getText(buf, 256 * sizeof(TCHAR));
	UCHAR g = _wtoi(buf);
	tbParams[16].getText(buf, 256 * sizeof(TCHAR));
	UCHAR b = _wtoi(buf);

	if ( r == 0 && g == 0 && b == 0 ) {
		r = 200; g = 200; b = 200;
	}

	
	LPMICROPHONE3D mic = new MICROPHONE3D(r, g, b, bR, h);
	
	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( wcslen(buf) == 0 ) {
		delete [] buf;
		return false;
	}
	mic->setName(buf, 256);

	Scene.AddObject(mic);
	delete [] buf;
	return true;
}

UINT CreateMicFull() {
	TCHAR *buf = new TCHAR[256];
	tbParams[4].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bH = (FLOAT)_wtof(buf);
	if ( bH < 10 ) {
		delete [] buf;
		return 1;
	}

	tbParams[5].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bR = (FLOAT)_wtof(buf) / 2;
	if ( bR < 1 ) {
		delete [] buf;
		return 2;
	}
	
	tbParams[6].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bW = (FLOAT)_wtof(buf);
	if ( bW < 1  || bW > 0.75f * bR ) {
		delete [] buf;
		return 3;
	}

	tbParams[12].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hD = (FLOAT)_wtof(buf);
	if ( hD < 1 || hD > bR ) {
		delete [] buf;
		return 9;
	}

	tbParams[7].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uR = (FLOAT)_wtof(buf) / 2;
	if ( uR < 1 || uR > hD) {
		delete [] buf;
		return 4;
	}

	tbParams[8].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uH = (FLOAT)_wtof(buf);
	if ( uH < 1) {
		delete [] buf;
		return 5;
	}

	tbParams[9].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uG = (FLOAT)_wtof(buf);
	if ( uG < 1) {
		delete [] buf;
		return 6;
	}

	tbParams[10].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hI = (FLOAT)_wtof(buf);
	if ( hI < 1 || hI > uH * 0.8f) {
		delete [] buf;
		return 7;
	}

	tbParams[11].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hR = (FLOAT)_wtof(buf) / 2;
	if ( hR < 1) {
		delete [] buf;
		return 8;
	}

	tbParams[13].getText(buf, 256 * sizeof(TCHAR));
	FLOAT cR = (FLOAT)_wtof(buf) / 2;
	if ( cR < 1 || cR > hR ) {
		delete [] buf;
		return 10;
	}

	tbParams[14].getText(buf, 256 * sizeof(TCHAR));
	UCHAR r = _wtoi(buf);
	tbParams[15].getText(buf, 256 * sizeof(TCHAR));
	UCHAR g = _wtoi(buf);
	tbParams[16].getText(buf, 256 * sizeof(TCHAR));
	UCHAR b = _wtoi(buf);

	if ( r == 0 && g == 0 && b == 0 ) {
		r = 200; g = 200; b = 200;
	}

	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( wcslen(buf) == 0 ) {
		delete [] buf;
		return 11;
	}
	
	delete [] buf;
	LPMICROPHONE3D mic = new MICROPHONE3D(r, g, b, bR, bH, bW, uR, uH, uG, hI, hR, hD, cR);
	mic->setName(buf, 256);
	Scene.AddObject(mic);
	return 0;
}

UINT ModifMicFull() {
	TCHAR *buf = new TCHAR[256];

	UINT i = listObjects.getCurSel();

	if ( i < 0 || i > ( OBJECT3D::Counter - 4 ) )
		return 100;

	LPMICROPHONE3D mic = (LPMICROPHONE3D)Scene.getObject(CLS_MESH, i);

	tbParams[4].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bH = (FLOAT)_wtof(buf);
	if ( bH > 10 )
		mic->setBaseHeight(bH);

	tbParams[5].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bR = (FLOAT)_wtof(buf) / 2;
	if ( mic->getBaseRadius() > 1 )
		mic->setBaseRadius(bR);
	
	tbParams[6].getText(buf, 256 * sizeof(TCHAR));
	FLOAT bW = (FLOAT)_wtof(buf);
	if ( mic->getButtonWidth() > 1 && bW < 0.75f * mic->getBaseRadius() )
		mic->setButtonWidth(bW);

	tbParams[12].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hD = (FLOAT)_wtof(buf);
	if ( mic->getHeadDepth() > 1 && hD < mic->getBaseRadius() ) {
		mic->setHeadDepth(hD);
	}

	tbParams[7].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uR = (FLOAT)_wtof(buf) / 2;
	if ( mic->getUprightRadius() > 1 && uR < mic->getHeadDepth()) {
		mic->setUprightRadius(uR);
	}

	tbParams[8].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uH = (FLOAT)_wtof(buf);
	if ( mic->getUprightHeight() > 1) {
		mic->setUprightHeight(uH);
	}

	tbParams[9].getText(buf, 256 * sizeof(TCHAR));
	FLOAT uG = (FLOAT)_wtof(buf);
	if ( mic->getUprightGap() > 1) {
		mic->setUprightGap(uG);
	}

	tbParams[10].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hI = (FLOAT)_wtof(buf);
	if ( mic->getHandleIndent() > 1 && hI < mic->getUprightHeight() * 0.8f) {
		mic->setHandleIndent(hI);
	}

	tbParams[11].getText(buf, 256 * sizeof(TCHAR));
	FLOAT hR = (FLOAT)_wtof(buf) / 2;
	if ( mic->getHeadRadius() > 1) {
		mic->setHeadRadius(hR);
	}

	tbParams[13].getText(buf, 256 * sizeof(TCHAR));
	FLOAT cR = (FLOAT)_wtof(buf) / 2;
	if ( mic->getCoreRadius() > 1 && cR < mic->getHeadRadius() ) {
		mic->setCoreRadius(cR);
	}

	tbParams[14].getText(buf, 256 * sizeof(TCHAR));
	UCHAR r = _wtoi(buf);
	tbParams[15].getText(buf, 256 * sizeof(TCHAR));
	UCHAR g = _wtoi(buf);
	tbParams[16].getText(buf, 256 * sizeof(TCHAR));
	UCHAR b = _wtoi(buf);

	if ( r != 0 || g != 0 || b != 0 ) {
		mic->setColor(r, g, b);
	}

	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( wcslen(buf) != 0 ) {
		mic->setName(buf, 256);
	}
	
	delete [] buf;
	return 0;
}

BOOL CreateLight() {
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256 * sizeof(TCHAR));
	UCHAR r = _wtoi(buf);
	tbParams[2].getText(buf, 256 * sizeof(TCHAR));
	UCHAR g = _wtoi(buf);
	tbParams[3].getText(buf, 256 * sizeof(TCHAR));
	UCHAR b = _wtoi(buf);

	tbParams[4].getText(buf, 256 * sizeof(TCHAR));
	FLOAT x = (FLOAT)_wtof(buf);
	tbParams[5].getText(buf, 256 * sizeof(TCHAR));
	FLOAT y = (FLOAT)_wtof(buf);
	tbParams[6].getText(buf, 256 * sizeof(TCHAR));
	FLOAT z = (FLOAT)_wtof(buf);

	tbParams[7].getText(buf, 256 * sizeof(TCHAR));
	FLOAT p = (FLOAT)_wtof(buf);
	
	LPDIFLIGHT3D light = new DIFLIGHT3D(r, g, b);
	
	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( wcslen(buf) == 0 ) {
		delete [] buf;
		return false;
	}
	light->setName(buf, 256);
	light->LookAt(x, y, z);
	light->setPower(p);

	Scene.AddObject(light);
	delete [] buf;
	return true;
}

BOOL CreateCam() {
	TCHAR *buf = new TCHAR[256];

	tbParams[1].getText(buf, 256 * sizeof(TCHAR));
	FLOAT tx = (FLOAT)_wtof(buf);
	tbParams[2].getText(buf, 256 * sizeof(TCHAR));
	FLOAT ty = (FLOAT)_wtof(buf);
	tbParams[3].getText(buf, 256 * sizeof(TCHAR));
	FLOAT tz = (FLOAT)_wtof(buf);

	tbParams[4].getText(buf, 256 * sizeof(TCHAR));
	FLOAT x = (FLOAT)_wtof(buf);
	tbParams[5].getText(buf, 256 * sizeof(TCHAR));
	FLOAT y = (FLOAT)_wtof(buf);
	tbParams[6].getText(buf, 256 * sizeof(TCHAR));
	FLOAT z = (FLOAT)_wtof(buf);
	
	LPTARGCAMERA3D cam = new TARGCAMERA3D;
	
	tbParams[0].getText(buf, 256 * sizeof(TCHAR));
	if ( wcslen(buf) == 0 ) {
		delete [] buf;
		return false;
	}
	cam->setName(buf, 256);
	cam->Translate(x, y, z);
	cam->setTargetPoint(tx, ty, tz);

	Scene.AddObject(cam);
	delete [] buf;
	return true;
}

BOOL ToPoint() {
	UINT i = listObjects.getCurSel();
	if ( i < 0 || i > ( OBJECT3D::Counter - 4 ) )
		return false;

	TCHAR *buf = new TCHAR[256];
	LPOBJECT3D obj = NULL;
	listObjects.getItem(i, buf, 256, (LPVOID*)&obj);

	tbX.getText(buf, 256 * sizeof(TCHAR));
	FLOAT x = _wtof(buf);
	tbY.getText(buf, 256 * sizeof(TCHAR));
	FLOAT y = _wtof(buf);
	tbZ.getText(buf, 256 * sizeof(TCHAR));
	FLOAT z = _wtof(buf);

	switch ( activeTool ) {
	case IS_MOVE:
		obj->Translate(x, y, z);
		break;
	case IS_ROTATE:
		//obj->Roll(x * M_PI / 180);
		//obj->Pitch(y * M_PI / 180);
		//obj->Yaw(z * M_PI / 180);
		obj->RotateAxis(&VECTOR3D(1, 0, 0), x * M_PI / 180);
		obj->RotateAxis(&VECTOR3D(0, 1, 0), y * M_PI / 180);
		obj->RotateAxis(&VECTOR3D(0, 0, 1), z * M_PI / 180);
		break;
	case IS_SCALE:
		obj->ScaleByX(x);
		obj->ScaleByY(y);
		obj->ScaleByZ(z);
		break;
	}
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
	FLOAT move = 3.f;
	FLOAT rot  = 0.3f; 
	FLOAT scale = 0.6f;

	BOOL error = false;

	UINT i = listObjects.getCurSel();

	if ( i < 0 || i > ( OBJECT3D::Counter - 4 ) )
		error = true;

	TCHAR *buf = new TCHAR[256];
	LPOBJECT3D obj = NULL;
	listObjects.getItem(i, buf, 256, (LPVOID*)&obj);

	UINT ind = Pool->getActiveViewportIndex();
	switch ( activeTool ) {
	case IS_MOVE:
		switch ( wParam ) {
		case VK_LEFT:
			if ( !error )
				obj->Strafe(-move);
			break;
		case VK_RIGHT:
			if ( !error )
				obj->Strafe(move);
			break;
		case VK_UP:
			if ( !error )
				obj->Fly(move);
			break;
		case VK_DOWN:
			if ( !error )
				obj->Fly(-move);
			break;
		case VK_HOME:
			if ( !error )
				obj->Follow(move);
			break;
		case VK_END:
			if ( !error )
				obj->Follow(-move);
			break;
		}
		break;
	case IS_ROTATE:
		switch ( wParam ) {
		case VK_LEFT:
			if ( !error )
				obj->Yaw(-rot);
			break;
		case VK_RIGHT:
			if ( !error )
				obj->Yaw(rot);
			break;
		case VK_UP:
			if ( !error )
				obj->Pitch(rot);
			break;
		case VK_DOWN:
			if ( !error )
				obj->Pitch(-rot);
			break;
		case VK_HOME:
			if ( !error )
				obj->Roll(rot);
			break;
		case VK_END:
			if ( !error )
				obj->Roll(-rot);
			break;
		}
		break;
	case IS_SCALE:
		switch ( wParam ) {
		case VK_LEFT:
			if ( !error )
				obj->ScaleByX(-scale);
			break;
		case VK_RIGHT:
			if ( !error )
				obj->ScaleByX(scale);
			break;
		case VK_UP:
			if ( !error )
				obj->ScaleByZ(scale);
			break;
		case VK_DOWN:
			if ( !error )
				obj->ScaleByZ(-scale);
			break;
		case VK_HOME:
			if ( !error )
				obj->ScaleByY(scale);
			break;
		case VK_END:
			if ( !error )
				obj->ScaleByY(-scale);
			break;
		}
		break;
	case IS_PAN:
		switch ( wParam ) {
		case VK_LEFT:
			Pool->getViewport(ind)->camDefault.Strafe(-move);
			Pool->getViewport(ind)->camDefault.TargetStrafe(-move);
			break;
		case VK_RIGHT:
			Pool->getViewport(ind)->camDefault.Strafe(move);
			Pool->getViewport(ind)->camDefault.TargetStrafe(move);
			break;
		case VK_UP:
			Pool->getViewport(ind)->camDefault.Fly(move);
			Pool->getViewport(ind)->camDefault.TargetFly(move);
			break;
		case VK_DOWN:
			Pool->getViewport(ind)->camDefault.Fly(-move);
			Pool->getViewport(ind)->camDefault.TargetFly(-move);
			break;
		case VK_HOME:
			Pool->getViewport(ind)->camDefault.Follow(move);
			Pool->getViewport(ind)->camDefault.TargetFollow(move);
			break;
		case VK_END:
			Pool->getViewport(ind)->camDefault.Follow(-move);
			Pool->getViewport(ind)->camDefault.TargetFollow(-move);
			break;
		}
		break;
	case IS_CAMROTATE:
		switch ( wParam ) {
		case VK_LEFT:
			Pool->getViewport(ind)->camDefault.Strafe(-move);
			break;
		case VK_RIGHT:
			Pool->getViewport(ind)->camDefault.Strafe(move);
			break;
		case VK_UP:
			Pool->getViewport(ind)->camDefault.Fly(move);
			break;
		case VK_DOWN:
			Pool->getViewport(ind)->camDefault.Fly(-move);
			break;
		case VK_HOME:
			Pool->getViewport(ind)->camDefault.Follow(move);
			break;
		case VK_END:
			Pool->getViewport(ind)->camDefault.Follow(-move);
			break;
		}
		break;
	case IS_LOOK:
		switch ( wParam ) {
		case VK_LEFT:
			Pool->getViewport(ind)->camDefault.TargetStrafe(-move);
			break;
		case VK_RIGHT:
			Pool->getViewport(ind)->camDefault.TargetStrafe(move);
			break;
		case VK_UP:
			Pool->getViewport(ind)->camDefault.TargetFly(move);
			break;
		case VK_DOWN:
			Pool->getViewport(ind)->camDefault.TargetFly(-move);
			break;
		case VK_HOME:
			Pool->getViewport(ind)->camDefault.TargetFollow(move);
			break;
		case VK_END:
			Pool->getViewport(ind)->camDefault.TargetFollow(-move);
			break;
		}
		break;
	}
	mainForm.Invalidate();

	delete [] buf;
	return 0L;
}

LRESULT mainForm_InterfClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME ofn;
	TCHAR *fName = new TCHAR[256];
	UINT error;

	switch (LOWORD(wParam))
	{
	case BT_MOVE:
		if ( activeTool != IS_MOVE )
			activeTool = IS_MOVE;
		else
			ToPoint();
		mainForm.Invalidate();
		break;
	case BT_ROTATE:
		if ( activeTool != IS_ROTATE )
			activeTool = IS_ROTATE;
		else
			ToPoint();
		mainForm.Invalidate();
		break;
	case BT_SCALE:
		if ( activeTool != IS_SCALE )
			activeTool = IS_SCALE;
		else
			ToPoint();
		mainForm.Invalidate();
		break;
	case BT_PAN:
		activeTool = IS_PAN;
		break;
	case BT_CAMSPHERE:
		activeTool = IS_CAMROTATE;
		break;
	case BT_LOOK_AROUND:
		activeTool = IS_LOOK;
		break;
	case TAB_CREATE:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		break;
	case TAB_MODIFY:
		Draw_DestroyRightToolbar();
		Draw_ModifyMicrophoneToolbar(GetModuleHandle(NULL));
		break;
	case BT_OBJECTS:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		Draw_InitObjectsToolbar(GetModuleHandle(NULL));
		break;
	case BT_LIGHTS:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		Draw_InitLightsToolbar(GetModuleHandle(NULL));
		break;
	case BT_MIC:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		Draw_InitObjectsToolbar(GetModuleHandle(NULL));
		Draw_InitMicrophoneToolbar(GetModuleHandle(NULL));
		break;
	case BT_LIGHT:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		Draw_InitLightsToolbar(GetModuleHandle(NULL));
		Draw_InitLightToolbar(GetModuleHandle(NULL));
		break;
	case BT_CAMS:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		Draw_InitCamsToolbar(GetModuleHandle(NULL));
		break;
	case BT_CAMERA:
		Draw_DestroyRightToolbar();
		Draw_InitCreateToolbar(GetModuleHandle(NULL));
		Draw_InitCamsToolbar(GetModuleHandle(NULL));
		Draw_InitCameraToolbar(GetModuleHandle(NULL));
		break;
	case BT_MAKELIGHT:
		if ( !CreateLight() )
			MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );
		RefreshObjectsList();
		mainForm.Invalidate();
		break;
	case BT_MAKECAM:
		if ( !CreateCam() )
			MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );
		RefreshObjectsList();
		mainForm.Invalidate();
		break;
	case BT_MODIF:
		ModifMicFull();
		mainForm.Invalidate();
		break;
	case BT_QMAKE:
		if ( !CreateMicFast() )
			MessageBox(NULL, _T("To small parameter."), _T("Error"), MB_OK | MB_ICONERROR );
		RefreshObjectsList();
		mainForm.Invalidate();
		break;
	case BT_FMAKE:
		error = CreateMicFull();
		switch (error) {
		case 1:
			MessageBox(NULL, _T("To small height."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 2:
			MessageBox(NULL, _T("To small radius."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 3:
			MessageBox(NULL, _T("Wrong button size."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 4:
			MessageBox(NULL, _T("To small upright diameter."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 5:
			MessageBox(NULL, _T("To small upright."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 6:
			MessageBox(NULL, _T("Wrong upright gap size."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 7:
			MessageBox(NULL, _T("To small handle indent"), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 8:
			MessageBox(NULL, _T("Wrong head diameter size."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 9:
			MessageBox(NULL, _T("To small head depth."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 10:
			MessageBox(NULL, _T("Wrong core diameter size."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		case 11:
			MessageBox(NULL, _T("Enter name."), _T("Error"), MB_OK | MB_ICONERROR );
			break;
		}
		RefreshObjectsList();
		mainForm.Invalidate();
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
			RefreshObjectsList();
			mainForm.Invalidate();
		}
		break;
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

LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam) 
{ return DLGC_WANTALLKEYS; }

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

// ============================================================================
// Focus handling funcs:
// ============================================================================

LRESULT mainForm_onFocusLost(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	Pool->setActiveViewport(NO_ACTIVE_VIEWPORT);
	return 0L;
}

// ============================================================================
// Mouse handling funcs:
// ============================================================================
LRESULT viewport_lbMouseClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	LPVIEWPORT vp = (LPVIEWPORT)Sender;
	Pool->setActiveViewport((DWORD)vp->getID());
	mainForm.setFocus();
	mainForm.Invalidate();

	return 0L;
}
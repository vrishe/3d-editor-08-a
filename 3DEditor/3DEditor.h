#pragma once

// Used to define any internal use stuff.

#include "targetver.h"
#include "resource.h"

#include "External.h"
#include "Internal.h"

#define OPEN_FILE_FILTER _T("All files\0*.*\0\nText\0*.txt\0\n3DEditor\0*.3de\0\0")

#define MAX_LOADSTRING 100
#define MAX_PARAMS_NUM 20

#define VIEWPORT_AREA_H 670
#define VIEWPORT_AREA_W 968

#define FORM_BG_COLOR RGB(120, 120, 126)
#define VIEWPORT_BG_COLOR RGB(90, 90, 95)
#define TEXT_BG_COLOR RGB(155, 155, 155)

#define LIST_OBJ_H	200
#define LIST_OBJ_W	266

#define LB_V_H	10
#define LB_V_W	20

#define BT_TOOL_H 30
#define BT_TOOL_W 60
#define TAB_H 30
#define TAB_W 131
#define SUB_TAB_H 20
#define SUB_TAB_W 87

enum TOOLS {
	IS_NONE			= 0,
	IS_MOVE			= 1,
	IS_ROTATE		= 2,
	IS_SCALE		= 3,
	IS_PAN			= 4,
	IS_CAMROTATE	= 5,
	IS_LOOK			= 6,
	IS_ZOOM			= 7
};

#define PAN_ASPECT		6.0f
#define ROTATION_ASPECT	.1f
#define SCALE_ASPECT	.95f
#define ZOOM_ASPECT		.01f
#define WHEEL_SENS		3.0f

enum MF_LISTBOX {
	LIST_OBJECTS	= 10,
};

enum MF_TEXTBOX {
	TB_X			= 110,
	TB_Y			= 111,
	TB_Z			= 112,
	TB_HEIGHT		= 113,
	TB_BDIAM		= 114,
	TB_BHEIGHT		= 115,
	TB_BWIDTH		= 116,
	TB_UDIAM		= 117,
	TB_UHEIGHT		= 118,
	TB_UGAP			= 119,
	TB_HANDIND		= 120,
	TB_HEADDIAM		= 121,
	TB_HEADDEPTH	= 122,
	TB_COREDIAM		= 123,
	TB_R			= 124,
	TB_G			= 125,
	TB_B			= 126,
	TB_NAME			= 127,
	TB_TX			= 128,
	TB_TY			= 129,
	TB_TZ			= 130
};

enum MF_BUTTON {
	BT_MOVE			= 1000,
	BT_ROTATE		= 1001,
	BT_SCALE		= 1002,
	TAB_CREATE		= 1003,
	TAB_MODIFY		= 1004,
	BT_OBJECTS		= 1005,
	BT_CAMS			= 1006,
	BT_LIGHTS		= 1007,
	BT_PAN			= 1008,
	BT_CAMSPHERE	= 1009,
	BT_LOOK_AROUND	= 1010,
	BT_MIC			= 1011,
	BT_LIGHT		= 1012,
	BT_CAMERA		= 1013,
	BT_MAKEMIC		= 1014,
	BT_MAKELIGHT	= 1015,
	BT_MAKECAM		= 1016,
	BT_ZOOM			= 1017,
	BT_DELETE		= 1018,
	BT_BIND			= 1019,
	BT_RELEASE		= 1020,
	BT_PROJSET		= 1021
};

// Draw interface functions
VOID Draw_MainToolbars();

VOID Draw_InitCreateToolbar();
//VOID Draw_InitModifyToolbar();

VOID Draw_InitObjectsToolbar();
VOID Draw_InitLightsToolbar();
VOID Draw_InitCamsToolbar();

VOID Draw_InitMicrophoneToolbar();
VOID Draw_InitLightToolbar();
VOID Draw_InitCameraToolbar();

BOOL Draw_RefreshModifyToolbar();
VOID Draw_DestroyRightToolbar();

VOID RefreshObjectsList();

VOID RemoveSceneObject();

// Controls functions
UINT CreateMic();
UINT ModifMic();
BOOL CreateLight(); 
BOOL ModifLight();
BOOL CreateCam();
BOOL ModifCam();

// Form event handler functions
LRESULT mainForm_InterfClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_keyPressed(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnClose(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnPaint(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);

LRESULT viewport_lbMouseDown(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT viewport_lbMouseUp(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT viewport_mouseMove(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT viewport_whMouseRotate(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK About_DialogBox_Handler(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Save_DialogBox_Handler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

VOID RefreshTextBoxes();
VOID GetActiveViewport();
VOID GetActiveObject();
VOID ActiveObjectHighlight();
VOID ActiveObjectColorReset();
VOID DeleteActiveObject();
VOID SceneCleanUp();
// TODO: define any new event handler prototype here
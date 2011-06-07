#pragma once

// Used to define any internal use stuff.

#include "targetver.h"
#include "resource.h"

#include "External.h"
#include "Internal.h"

#define MAX_LOADSTRING 100

#define MAX_PARAMS_NUM 20

#define VIEWPORT_AREA_H 670
#define VIEWPORT_AREA_W 968

#define FORM_BG_COLOR RGB(120, 120, 126)
#define VIEWPORT_BG_COLOR 90, 90, 95
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
	IS_MOVE			= 0,
	IS_ROTATE		= 1,
	IS_SCALE		= 2,
	IS_PAN			= 3,
	IS_CAMROTATE	= 4,
	IS_LOOK			= 5
};

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
	TB_COREDIAM		= 123
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
	BT_QMAKE		= 1012,
	BT_FMAKE		= 1013
};

// Draw interface functions
VOID Draw_MainToolbars (HINSTANCE);
VOID Draw_InitCreateToolbar (HINSTANCE);
VOID Draw_InitModifyToolbar (HINSTANCE);
VOID Draw_InitObjectsToolbar (HINSTANCE);
VOID Draw_InitMicrophoneToolbar (HINSTANCE);

VOID Draw_DestroyRightToolbar ();

 //Controls functions
void ClearActiveTool();


 //Form event handler functions
LRESULT mainForm_InterfClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_keyPressed(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnDestroy(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnPaint(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About_DialogBox_Handler(HWND, UINT, WPARAM, LPARAM);
LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Save_DialogBox_Handler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL OpenFileDialog(HWND hWnd, OPENFILENAME& ofn);
BOOL SaveFileDialog(HWND hWnd, OPENFILENAME& ofn);
// TODO: define any new event handler prototype here
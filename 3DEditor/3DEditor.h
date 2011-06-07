#pragma once

// Used to define any internal use stuff.

#include "targetver.h"
#include "resource.h"

#include "External.h"
#include "Internal.h"

#define MAX_LOADSTRING 100

#define VIEWPORT_AREA_H  670
#define VIEWPORT_AREA_W 968

#define FORM_BG_COLOR RGB(128, 125, 125)
#define VIEWPORT_BG_COLOR 100, 100, 100
#define TEXTBOX_BG_COLOR RGB(119, 119, 119)

#define LB_V_H	10
#define LB_V_W	20

#define BT_TOOL_H 30
#define BT_TOOL_W 60
#define TAB_H 30
#define TAB_W 131
#define SUB_TAB_H 20
#define SUB_TAB_W 87

enum MF_TEXTBOX {
	TB_X		= 110,
	TB_Y		= 111,
	TB_Z		= 112,
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
	BT_LOOK_AROUND	= 1010
};

LRESULT mainForm_menuClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_keyPressed(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnDestroy(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnPaint(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About_DialogBox_Handler(HWND, UINT, WPARAM, LPARAM);
LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Save_DialogBox_Handler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL OpenFileDialog(HWND hWnd, OPENFILENAME& ofn);
BOOL SaveFileDialog(HWND hWnd, OPENFILENAME& ofn);
// TODO: define any new event handler prototype here
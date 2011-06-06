#pragma once

// Used to define any internal use stuff.

#include "targetver.h"
#include "resource.h"

#include "External.h"
#include "Internal.h"

#define MAX_LOADSTRING 100

LRESULT mainForm_menuClick(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_keyPressed(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnDestroy(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnPaint(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About_DialogBox_Handler(HWND, UINT, WPARAM, LPARAM);
LRESULT mainForm_ProcKeys(LPOBJECT Sender, WPARAM wParam, LPARAM lParam);
// TODO: define any new event handler prototype here
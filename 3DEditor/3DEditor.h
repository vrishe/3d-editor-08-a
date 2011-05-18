#pragma once

// Used to define any internal use stuff.

#include "targetver.h"
#include "resource.h"

#include "External.h"
#include "Internal.h"

#define MAX_LOADSTRING 100

LRESULT mainForm_menuClick(LPVOID Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_keyPressed(LPVOID Sender, WPARAM wParam, LPARAM lParam);
LRESULT mainForm_OnDestroy(LPVOID Sender, WPARAM wParam, LPARAM lParam);
LRESULT testForm_OnPaint(LPVOID Sender, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About_DialogBox_Handler(HWND, UINT, WPARAM, LPARAM);
// TODO: define any new event handler prototype here
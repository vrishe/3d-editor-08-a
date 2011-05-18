#pragma once

// Here are placed necessary includes/defines
// which needed to resolve project compatibility.

#include <WinSDKVer.h>

// NOTE: to change target platform you should
//		 define _WIN32_WINNT macro properly.
//		 DO NOT MOVE THIS DEFINITION ELSEWHERE!
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#include <SDKDDKVer.h>

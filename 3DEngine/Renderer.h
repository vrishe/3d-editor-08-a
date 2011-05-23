#pragma once

#include <tchar.h>

#include "Abstractions.h"
#include "..\Controls\Controls.h"
#include "..\3DEngine\Primitives.h"


// Custom error codes
#define E_POOL_NOT_INITIALIZED		0x10000000
#define E_EVENT_CREATION_FAILED		0x10000005
#define E_THREAD_CREATION_FAILED	0x10000006

#define E_EVENT_NOT_EXIST			0x1000000A
#define E_VIEWPORT_NOT_EXIST		0x1000000B
#define E_NO_VIEWPORTS_EXIST		0x1000000C
#define E_THREAD_WAIT_TIMEOUT		0x1000000D
#define E_MAX_COUNT_REACHED			0x1000000E

#define SHUTDOWN_ON_DEMAND			0

typedef HANDLE	THREAD, *LPTHREAD, 
				EVENT, *LPEVENT;

// Necessary Viewport class definitions
enum RENDER_MODE {
	RM_WIREFRAME	= 0x01,
	RM_SHADED		= 0x02
};
#define RM_WFSHADED RM_WIREFRAME | RM_SHADED

#define MAX_VIEWPORT_COUNT	6
#define MAX_RENDER_TIMEOUT	1000

typedef struct tagVIEWPORT_INFO {
	INT				PosX;
	INT				PosY;
	UINT			Width;
	UINT			Height;
	UINT			Camera;
	RENDER_MODE		rMode;
} VIEWPORT_INFO, *LPVIEWPORT_INFO;

typedef struct tagTHREAD_CONTROLS {
	EVENT doRender;
	EVENT shutDown;
	EVENT renderComplete;
} THREAD_CONTORLS, *LPTHREAD_CONTROLS;

typedef struct tagVIEWPORT : public VIEWPORT_INFO {
	LPSCENE3D		*lppScene;
	HDC				*lpHdc;
	DWORD			ID;

	THREAD			thOwner;
	THREAD_CONTORLS thControls;
} VIEWPORT, *LPVIEWPORT;

typedef vector<LPVIEWPORT> VIEWPORTS_LIST;
typedef vector<EVENT> EVENTS_LIST;

class clsRenderPool {
private:
	HDC	hDC;
	LPSCENE3D		Scene;
	VIEWPORTS_LIST	Viewports;
	EVENTS_LIST		vpsRenderComplete;

	EVENT renderSignal;
	static DWORD WINAPI Render(LPVOID renderInfo);
	UINT findViewport(DWORD vpID);
public:
	clsRenderPool();
	clsRenderPool(LPSCENE3D lpScene);
	//clsRenderPool(LPSCENE3D, LPVIEWPORT_INFO vpInfo, UINT nToAdd, LPUINT nOfAdded);
	~clsRenderPool();

	BOOL assignScene(LPSCENE3D lpScene);
	UINT addViewport(
				INT			vpPosX,
				INT			vpPosY,
				UINT		vpWidth,
				UINT		vpHeight,
				UINT		vpCamera,
				RENDER_MODE vpRMode
			);
	BOOL delViewport(UINT vpIndex);
	BOOL delViewport(DWORD vpID);
	LPVIEWPORT getViewport(UINT vpIndex);
	LPVIEWPORT getViewport(DWORD vpID);

	DWORD RenderWorld(HDC outputContext);
};
typedef clsRenderPool RENDER_POOL, *LPRENDER_POOL;
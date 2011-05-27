#pragma once

#include <tchar.h>

#include "Camera.h"
#include "..\Controls\Controls.h"


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
#define THREAD_WAIT_TIMEOUT	5000
#define VIEWPORT_CLASS_NAME	_T("RenderPool Viewport Class")

typedef struct tagTHREAD_CONTROLS {
	EVENT doRender;
	EVENT shutDown;
} THREAD_CONTROLS, *LPTHREAD_CONTROLS;

class clsViewport : public clsForm {
private:
	UINT			cameraObjectID;
	RENDER_MODE		rMode;
	LPSCENE3D		Scene;

public:
	clsViewport();
	clsViewport(
		LPSCENE3D lpScene,
		UINT uCameraObjectID		= 0, 
		RENDER_MODE renderMode		= RM_WIREFRAME
	);
	virtual ~clsViewport();

	DWORD SetUp(
				LPFORM vpOwner,
				INT	vpPosX,
				INT vpPosY,
				UINT vpWidth,
				UINT vpHeight
			);

	LPSCENE3D	getScene();	
	UINT		getCameraObjectID();
	RENDER_MODE	getRenderMode();

	BOOL		setScene(LPSCENE3D lpSceneHost);
	BOOL		setCameraObjectID(UINT uCameraObjectID);
	VOID		setRenderMode(RENDER_MODE renderMode);

	BOOL		Render();
};
typedef clsViewport VIEWPORT, *LPVIEWPORT;

typedef struct tagTHREAD_DATA {
	LPVIEWPORT		Viewport;

	THREAD			Thread;
	THREAD_CONTROLS threadControls;
} THREAD_DATA, *LPTHREAD_DATA;

typedef vector<LPTHREAD_DATA> LPVIEWPORTS_LIST;
class clsRenderPool {
private:
	LPFORM				Owner;
	LPSCENE3D			Scene;
	LPVIEWPORTS_LIST	Viewports;

	EVENT				renderEvent;

	static DWORD WINAPI Render(LPVOID renderInfo);
	UINT				findViewport(DWORD vpID);
public:
	clsRenderPool(LPFORM Owner);
	clsRenderPool(LPFORM Owner, LPSCENE3D lpScene);
	//clsRenderPool(LPSCENE3D, LPVIEWPORT_INFO vpInfo, UINT nToAdd, LPUINT nOfAdded);
	~clsRenderPool();

	BOOL assignScene(LPSCENE3D lpScene);
	DWORD addViewport(
				INT			vpPosX,
				INT			vpPosY,
				UINT		vpWidth,
				UINT		vpHeight,
				UINT		vpCameraObjectID,
				RENDER_MODE vpRMode
			);
	BOOL delViewport(UINT vpIndex);
	BOOL delViewport(DWORD vpID);

	LPVIEWPORT getViewport(UINT vpIndex);
	LPVIEWPORT getViewport(DWORD vpID);
	UINT getViewportCount();

	BOOL RenderWorld();
};
typedef clsRenderPool RENDER_POOL, *LPRENDER_POOL;
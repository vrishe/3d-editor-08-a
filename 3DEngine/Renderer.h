#pragma once

#include <tchar.h>
#include <vector>
#include <algorithm>

#include "Camera.h"
#include "Lighters.h"
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
	RM_SHADED		= 0x02,
	RM_SHADEDWF		= 0x03
};

#define MAX_VIEWPORT_COUNT	6
#define THREAD_WAIT_TIMEOUT	5000
#define VIEWPORT_CLASS_NAME	_T("RenderPool Viewport Class")

typedef struct tagTHREAD_CONTROLS {
	EVENT doRender;
	EVENT shutDown;
	EVENT jobDone;
} THREAD_CONTROLS, *LPTHREAD_CONTROLS;

// first- index of a polygon in mesh, second - index of a mesh in scene
typedef vector < pair <DIRECTPOLY3D, UINT> > SCENEPOLY;
typedef vector < VECTOR3D > SCENEVERT;

#define DEFAULT_CAMERA_ID	UINT_MAX

class clsViewport : public clsForm {
private:
	UINT			ID;
	UINT			cameraObjectID;
	RENDER_MODE		rMode;
	LPSCENE3D		Scene;

public:
	CAMERA3D		camDefault;

	clsViewport();
	clsViewport(
		LPSCENE3D lpScene,
		UINT uCameraObjectID		= DEFAULT_CAMERA_ID, 
		RENDER_MODE renderMode		= RM_WIREFRAME
	);
	virtual ~clsViewport();
	UINT	getID();

	DWORD SetUp(
				UINT	vpID,
				LPFORM	vpOwner,
				INT		vpPosX,
				INT		vpPosY,
				UINT	vpWidth,
				UINT	vpHeight
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

#define VIEW_DISTANCE_DEFAULT	100
enum VIEW_TYPE {
		VIEW_LEFT			= 1,
		VIEW_RIGHT			= 2,
		VIEW_FRONT			= 3,
		VIEW_BACK			= 4,
		VIEW_TOP			= 5,
		VIEW_BOTTOM			= 6,
		VIEW_PERSPECTIVE	= 7
};
VOID SetViewportDefaultView(LPVIEWPORT vp, VIEW_TYPE vt);

#ifndef FRAME_STROKE_COLORREF
#define FRAME_STROKE_COLORREF RGB(0, 0, 0)
#endif //FRAME_STROKE_COLORREF

#define NO_ACTIVE_VIEWPORT UINT_MAX

typedef struct tagTHREAD_DATA {
	LPVIEWPORT		Viewport;
	BOOL			isActive;

	THREAD			Thread;
	THREAD_CONTROLS threadControls;
} THREAD_DATA, *LPTHREAD_DATA;

typedef vector<LPTHREAD_DATA> LPVIEWPORTS_LIST;
typedef vector<EVENT> LPEVENTS_LIST;
class clsViewportPool {
private:
	LPFORM				Owner;
	LPSCENE3D			Scene;
	LPVIEWPORTS_LIST	Viewports;

	LPEVENTS_LIST		vpStates;
	EVENT				renderEvent;

	static DWORD WINAPI Render(LPVOID renderInfo);
	UINT				findViewport(DWORD vpID);
public:
	clsViewportPool(LPFORM Owner);
	clsViewportPool(LPFORM Owner, LPSCENE3D lpScene);
	~clsViewportPool();

	BOOL		assignScene(LPSCENE3D lpScene);
	DWORD		addViewport(
					INT			vpPosX,
					INT			vpPosY,
					UINT		vpWidth,
					UINT		vpHeight,
					VIEW_TYPE	vpVType,
					RENDER_MODE vpRMode
				);
	BOOL		delViewport(UINT vpIndex);
	BOOL		delViewport(DWORD vpID);

	LPVIEWPORT	getViewport(UINT vpIndex);
	LPVIEWPORT	getViewport(DWORD vpID);
	UINT		getActiveViewportIndex();
	UINT		getViewportCount();
	
	VOID		setActiveViewport(UINT vpActiveIndex);
	VOID		setActiveViewport(DWORD vpActiveID);

	DWORD		RenderWorld();
};
typedef clsViewportPool VIEWPORT_POOL, *LPVIEWPORT_POOL;
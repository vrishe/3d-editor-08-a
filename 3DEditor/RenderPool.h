
#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <ddraw.h>

#define MAX_VIEWPORT_COUNT		6
#define MAX_VIEWPORT_NAME_LEN	15

#define WAIT_TIMEOUT_LIMIT		2000

// Custom error codes
#define ERROR_POOL_NOT_INITIALIZED		0x10000000
#define ERROR_D2D1_INIT_FAILED			0x10000001
#define ERROR_D2D1_DCRT_CREATE_FAILED	0x10000002
#define ERROR_POOL_HEAP_INIT_FAILED		0x10000003
#define ERROR_HEAP_ALLOC_FAILED			0x10000004
#define ERROR_EVENT_CREATION_FAILED		0x10000005
#define ERROR_THREAD_CREATION_FAILED	0x10000006

#define ERROR_D2D1_FACTORY_NOT_EXIST	0x10000007
#define ERROR_POOL_HEAP_NOT_EXIST		0x10000008
#define ERROR_D2D1_DCRT_NOT_EXIST		0x10000009
#define ERROR_EVENT_NOT_EXIST			0x1000000A
#define ERROR_VIEWPORT_NOT_EXIST		0x1000000B
#define ERROR_NO_VIEWPORTS_EXIST		0x1000000C
#define ERROR_THREAD_WAIT_TIMEOUT		0x1000000D
#define ERROR_MAX_COUNT_EXCEEDED		0x1000000E

#define SHUTDOWN_ON_DEMAND				0

typedef ID2D1Factory*			lpID2D1Factory;
typedef ID2D1DCRenderTarget*	lpID2D1Viewport;
typedef D2D1_RENDER_TARGET_PROPERTIES ID2D1ViewportProps, *lpID2D1ViewportProps;
typedef HANDLE					THREAD, *LPTHREAD, EVENT, *LPEVENT;

DWORD WINAPI Render (LPVOID vpObjPtr);

enum RENDER_MODE {
	RM_WIREFRAME	= 0x01,
	RM_SHADED		= 0x02,
	RM_WFSHADED		= 0x03,
	RM_FLATSHADED	= 0x04,
	RM_WFFLATSHADED = 0x05
};

class Camera3D {
private:
public:
	Camera3D();
	virtual ~Camera3D();
};

typedef Camera3D CAMERA3D, *LPCAMERA3D;

typedef struct ViewportOpt {
	LPTSTR			Name;
	RECT			Rect;
	LPCAMERA3D		Camera;
	RENDER_MODE		RenderMode;
} VIEWPORT_INFO, *LPVIEWPORT_INFO;

template<class Interface> VOID SafeRelease(Interface **intObj);

typedef class CDirect3D9 {

};

typedef class Viewport {
private:
	HANDLE					heap;

	LPTSTR					Name;
	RECT					Rect;
	LPCAMERA3D				Camera;
	RENDER_MODE				RenderMode;

	HDC						*pHdc;
	lpID2D1Factory			Factory;
	lpID2D1Viewport			Context;
	EVENT					ControlEvents[3];
	THREAD					OwnerThread;

public:

	Viewport();
	~Viewport();

	DWORD Initialize(
			VIEWPORT_INFO vpInfo, 
			HDC *pVpHdc, 
			EVENT globalRenderEvent, 
			LPEVENT renderStateEvent,
			LPTHREAD ownerThread
		);
	VOID Release();
	BOOL ManageContext();

	VOID setName(LPCTSTR vpName = NULL);
	VOID setCamera(LPCAMERA3D vpCam);
	VOID setRenderMode(RENDER_MODE vpRrMode);
	VOID setRect(RECT vpRect);

	static DWORD WINAPI Render(LPVOID vpObjPtr);

	void* operator new(size_t size, HANDLE objHeap);
	void operator delete(void* objPtr, HANDLE objHeap);
	void operator delete(void* objPtr);
} VIEWPORT, *LPVIEWPORT;

typedef class RenderPool {
private:
	BOOL			isPoolInitialized;
	LPVIEWPORT*		Viewports;
	LPTHREAD		RenderThreads;
	LPEVENT			renderResultEvents;
	
	HWND			hWndTied;
	HDC				hWndHdc;
	RECT			poolRect;
	
	UINT			vpCount;
	EVENT			poolRenderEvent;

	HANDLE			renderPoolHeap;

	DWORD viewportMemRealloc(SIZE_T nOfBlocks);
	BOOL checkViewportsOverlap(LPRECT rc); //
	UINT findViewport(LPCTSTR vpName); //
public:
	RenderPool();
	~RenderPool();

	BOOL Initialize(HWND hWnd, LPRECT poolBounds = NULL);
	VOID Release();

	BOOL AddViewport(const LPVIEWPORT_INFO vpInfo);
	BOOL AddViewport(
		LPCTSTR vpName			= NULL,
		LPRECT vpBounds			= NULL, 
		LPCAMERA3D vpCam		= NULL, 
		RENDER_MODE vpRdMode	= RM_WIREFRAME
		); 
	BOOL AddViewports(const LPVIEWPORT_INFO vpInfo, UINT vpCount); //

	BOOL RemoveViewport(UINT vpNumber);
	BOOL RemoveViewport(LPCTSTR vpName); //

	BOOL setViewportInfo(LPVIEWPORT_INFO vpInfo, UINT vpNumber); //
	BOOL setViewportName(LPCTSTR vpName, UINT vpNumber); //
	BOOL setViewportRect(LPRECT vpRect, UINT vpNumber); //
	BOOL setViewportCamera(LPCAMERA3D vpCam, UINT vpNumber); //
	BOOL setViewportRenderMode(RENDER_MODE vpRdMode, UINT vpNumber); //

	const LPVIEWPORT getViewport(UINT vwNumber);
	const LPVIEWPORT getViewport(LPCTSTR vpName); //
	UINT getViewportCount();

	BOOL isInitialized();

	DWORD RenderWorld();
} RENDER_POOL, *LPRENDER_POOL;
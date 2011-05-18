
#include "RenderPool.h"

template<class Interface>
VOID SafeRelease(Interface **intObj)
{
	if ( intObj		!= NULL
		&& *intObj	!= NULL )
	{
		(*intObj)->Release();
		(*intObj) = NULL;
	}
}


// Viewport container class implementation
// ============================================================================
Viewport::Viewport()
{
	Name		= NULL;
	Context		= NULL;
	Factory		= NULL;
	Camera		= NULL;
	pHdc		= NULL;
	RenderMode	= RM_WIREFRAME;
	SetRectEmpty(&Rect);
	ZeroMemory(ControlEvents, sizeof(ControlEvents) / sizeof(EVENT));
}

Viewport::~Viewport() { Release(); }

DWORD Viewport::Initialize(
		VIEWPORT_INFO vpInfo, 
		HDC *pVpHdc, 
		EVENT globalRenderEvent,
		LPEVENT renderStateEvent,
		LPTHREAD ownerThread
) {
	UINT	bAllocSize;

	try
	{
		ControlEvents[1]	= CreateEvent(NULL, FALSE, FALSE, NULL);
		ControlEvents[2]	= CreateEvent(NULL, FALSE, FALSE, NULL);
		if ( ControlEvents[1]	== NULL
			|| ControlEvents[2] == NULL ) throw (DWORD)ERROR_EVENT_CREATION_FAILED;

		if ( globalRenderEvent		== NULL
			|| renderStateEvent		== NULL
			|| ownerThread			== NULL
			|| pVpHdc				== NULL 
			|| IsRectEmpty(&vpInfo.Rect) ) throw (DWORD)ERROR_BAD_ARGUMENTS;
		ControlEvents[0]	= globalRenderEvent;
		CopyRect(&Rect, &vpInfo.Rect);
		pHdc		= pVpHdc;
		Camera		= vpInfo.Camera;
		RenderMode	= vpInfo.RenderMode;
		if ( Camera == NULL
			&& ( RenderMode != RM_FLATSHADED
			|| RenderMode != RM_WFFLATSHADED ) ) RenderMode = RM_WIREFRAME;
		if ( vpInfo.Name != NULL )
		{
			bAllocSize = (wcslen(vpInfo.Name) + 1) * sizeof(TCHAR);
			Name = (LPTSTR)HeapAlloc(heap, 0, bAllocSize);
			if ( Name == NULL ) throw(ERROR_HEAP_ALLOC_FAILED);
			CopyMemory(Name, vpInfo.Name, bAllocSize);
		}

		OwnerThread = CreateThread(
						NULL, 
						0, 
						Render, 
						this,
						NULL,
						NULL
					);
		if ( OwnerThread == NULL ) throw (DWORD)ERROR_THREAD_CREATION_FAILED;
		*ownerThread		= OwnerThread;
		*renderStateEvent	= ControlEvents[2];
	}
	catch ( DWORD errc )
	{
		Release();
		return errc;
	}

	return ERROR_SUCCESS;
}

VOID Viewport::Release()
{
	UINT eSize = sizeof(ControlEvents) / sizeof(EVENT);

	SetEvent(ControlEvents[1]);
	WaitForSingleObject(OwnerThread, INFINITE);
	for ( UINT i = 1; i < eSize; i++ )
		if ( CloseHandle(ControlEvents[i]) ) ControlEvents[i] = NULL;
	if ( CloseHandle(OwnerThread) ) OwnerThread = NULL;
	SafeRelease(&Context);
	SafeRelease(&Factory);
	if ( Name != NULL ) HeapFree(heap, 0, Name);
}

BOOL Viewport::ManageContext()
{
	HRESULT hr;
	ID2D1ViewportProps vpProps;

	SafeRelease(&Context);
	vpProps = D2D1::RenderTargetProperties(
						D2D1_RENDER_TARGET_TYPE_DEFAULT,
						D2D1::PixelFormat(
								DXGI_FORMAT_B8G8R8A8_UNORM, 
								D2D1_ALPHA_MODE_IGNORE
							)
					);
	hr = Factory->CreateDCRenderTarget(&vpProps, &Context);
	if ( FAILED(hr) ) Context = NULL;

	return SUCCEEDED(hr);
}

DWORD WINAPI Viewport::Render (LPVOID vpObjPtr)
{
	HRESULT		hr;
	BOOL		alive;
	LPVIEWPORT	vp		= (LPVIEWPORT)vpObjPtr;

	try
	{	
		hr = D2D1CreateFactory (
						D2D1_FACTORY_TYPE_SINGLE_THREADED,
						&vp->Factory
					);
		if ( FAILED(hr) )	throw (DWORD)ERROR_D2D1_INIT_FAILED;
		SetEvent(vp->ControlEvents[2]);
		alive = vp->ManageContext();
		if ( !alive )		throw (DWORD)ERROR_D2D1_DCRT_CREATE_FAILED;

		while ( alive )
		{
			switch ( WaitForMultipleObjects(2, vp->ControlEvents, FALSE, INFINITE) )
			{
				case WAIT_OBJECT_0: 
					vp->Context->BindDC(*vp->pHdc, &vp->Rect);
					vp->Context->BeginDraw();
					vp->Context->Clear();
						// TODO: Place render code here
					hr = vp->Context->EndDraw();
					SetEvent(vp->ControlEvents[2]);
					if ( hr == D2DERR_RECREATE_TARGET )
					{
						if ( !vp->ManageContext() )
							throw (DWORD)ERROR_D2D1_DCRT_CREATE_FAILED;
					}
					break;

				case WAIT_OBJECT_0 + 1:
					SafeRelease(&vp->Context);
					SafeRelease(&vp->Factory);
					alive = FALSE;
			}
		}
	}
	catch ( DWORD errc )
	{
		return errc;
	}
	return SHUTDOWN_ON_DEMAND;
}

void* Viewport::operator new(size_t size, HANDLE objHeap)
{
	LPVOID objPtr;
	if ( objHeap != NULL )
	{
		objPtr = HeapAlloc(objHeap, 0, size);
		if ( objPtr != NULL )
			((LPVIEWPORT)objPtr)->heap = objHeap;
		return objPtr;
	}
	return NULL;
}

void Viewport::operator delete(void* objPtr, HANDLE objHeap)
{
	HeapFree(objHeap, 0, objPtr);
}

void Viewport::operator delete(void* objPtr)
{
	HeapFree(((LPVIEWPORT)objPtr)->heap, 0, objPtr);
}

// RenderPool class implementation
// ============================================================================

RenderPool::RenderPool()
{
	renderPoolHeap		= NULL;
	Viewports			= NULL;
	RenderThreads		= NULL;
	renderResultEvents	= NULL;
	hWndTied			= NULL;
	poolRenderEvent		= NULL;
	vpCount				= 0;
	SetRectEmpty(&poolRect);
}

RenderPool::~RenderPool() 
{
	Release();
}

DWORD RenderPool::viewportMemRealloc(SIZE_T nOfBlocks)
{
	LPVIEWPORT*	newVpPtr = NULL;
	LPTHREAD	newTdPtr = NULL;
	LPEVENT		newEtPtr = NULL;
	SIZE_T		cpLen;

	try
	{
		if ( renderPoolHeap == NULL )	throw (DWORD)ERROR_POOL_HEAP_NOT_EXIST;
		if ( nOfBlocks == vpCount )		throw (DWORD)ERROR_BAD_ARGUMENTS;

		if ( nOfBlocks != 0 )
		{
			newVpPtr = (LPVIEWPORT*)HeapAlloc(renderPoolHeap, 
				0, nOfBlocks * sizeof(LPVIEWPORT)); 
			newTdPtr = (LPTHREAD)HeapAlloc(renderPoolHeap, 
				0, nOfBlocks * sizeof(THREAD));
			newEtPtr = (LPEVENT)HeapAlloc(renderPoolHeap,
				0, nOfBlocks * sizeof(EVENT));

			if ( newVpPtr	== NULL 
				|| newTdPtr	== NULL	
				|| newEtPtr	== NULL ) throw (DWORD)ERROR_HEAP_ALLOC_FAILED;
		}
		
		cpLen = vpCount < nOfBlocks ? vpCount : nOfBlocks;
		if ( cpLen > 0 )
		{
			CopyMemory(newVpPtr, Viewports, cpLen * sizeof(LPVIEWPORT));
			CopyMemory(newTdPtr, RenderThreads, cpLen * sizeof(THREAD));
			CopyMemory(newEtPtr, renderResultEvents, cpLen * sizeof(EVENT));
		}

		HeapFree(renderPoolHeap, 0, Viewports);
		HeapFree(renderPoolHeap, 0, RenderThreads);
		HeapFree(renderPoolHeap, 0, renderResultEvents);
		Viewports				= newVpPtr;
		RenderThreads			= newTdPtr;
		renderResultEvents		= newEtPtr;
	}
	catch ( DWORD errc )
	{
		return errc;
	}

	return ERROR_SUCCESS;
}

BOOL RenderPool::Initialize(HWND hWnd, LPRECT poolBounds)
{
	try 
	{
		if ( isPoolInitialized ) Release();

		renderPoolHeap	= HeapCreate (
							HEAP_CREATE_ENABLE_EXECUTE, 
							0, 0
						  );
		if ( renderPoolHeap == NULL ) throw (DWORD)ERROR_POOL_HEAP_INIT_FAILED;

		poolRenderEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
		if ( poolRenderEvent == NULL ) throw (DWORD)ERROR_EVENT_CREATION_FAILED;
		
		hWndTied = hWnd;
		if ( poolBounds == NULL ) 
			GetClientRect(hWndTied, &poolRect);
		else
			CopyRect(&poolRect, poolBounds);
		if ( IsRectEmpty(&poolRect) 
			|| !IsWindow(hWndTied) ) throw (DWORD)ERROR_BAD_ARGUMENTS;
	}
	catch ( DWORD errc )
	{
		SetLastError(errc);
		Release();
		return isPoolInitialized = FALSE;
	}

	return isPoolInitialized = TRUE;	
}

VOID RenderPool::Release()
{
	isPoolInitialized = FALSE;
	while ( vpCount != 0 )
	{
		vpCount--;
		delete Viewports[vpCount];
	}
	if ( poolRenderEvent != NULL ) 
	{
		CloseHandle(poolRenderEvent);
		poolRenderEvent = NULL;
	}

	if ( renderPoolHeap != NULL ) 
	{
		HeapDestroy(renderPoolHeap);
		renderPoolHeap		= NULL;
		Viewports			= NULL;
		RenderThreads		= NULL;
		renderResultEvents	= NULL;
	}

	SetRectEmpty(&poolRect);
	hWndTied = NULL;
}

BOOL RenderPool::AddViewport(LPVIEWPORT_INFO vpInfo)
{
	DWORD errCode;

	try 
	{
		if ( !isPoolInitialized )				throw (DWORD)ERROR_POOL_NOT_INITIALIZED;
		if ( vpCount == MAX_VIEWPORT_COUNT )	throw (DWORD)ERROR_MAX_COUNT_EXCEEDED;

		errCode = viewportMemRealloc(vpCount + 1);
		if ( errCode != ERROR_SUCCESS )		throw errCode;

		Viewports[vpCount] = new(renderPoolHeap) Viewport();
		if ( Viewports[vpCount] == NULL )	throw (DWORD)ERROR_HEAP_ALLOC_FAILED;

		IntersectRect(&vpInfo->Rect, &poolRect, &vpInfo->Rect);
		errCode = Viewports[vpCount]->Initialize(
											*vpInfo, 
											&hWndHdc, 
											poolRenderEvent, 
											&renderResultEvents[vpCount],
											&RenderThreads[vpCount]
										);
		if ( errCode != ERROR_SUCCESS ) throw errCode;
		errCode = WaitForSingleObject(
								renderResultEvents[vpCount], 
								WAIT_TIMEOUT_LIMIT
							);
		if ( errCode == WAIT_TIMEOUT )	throw (DWORD)ERROR_THREAD_WAIT_TIMEOUT;
		++vpCount;
	}
	catch ( DWORD errc )
	{
		SetLastError(errc);
		return FALSE;
	}
	return  TRUE;
}

BOOL RenderPool::AddViewport(
	LPCTSTR vpName,
	LPRECT vpBounds, 
	LPCAMERA3D vpCam, 
	RENDER_MODE vpRdMode
) {
	VIEWPORT_INFO vpInfo;

	if ( vpBounds == NULL ) 
		GetClientRect(hWndTied, &vpInfo.Rect);
	else
		vpInfo.Rect = *vpBounds;

	vpInfo.Name			= (LPTSTR)vpName;
	vpInfo.Camera		= vpCam;
	vpInfo.RenderMode	= vpRdMode;

	return AddViewport(&vpInfo);
}

BOOL RenderPool::RemoveViewport(UINT vpNumber)
{
	DWORD errCode;

	try
	{
		if ( !isPoolInitialized )	throw (DWORD)ERROR_POOL_NOT_INITIALIZED;
		if ( vpNumber >= vpCount )	throw (DWORD)ERROR_VIEWPORT_NOT_EXIST;

		delete Viewports[vpNumber];
		
		for ( UINT i = vpNumber + 1; i < vpCount; i++ )
		{
			Viewports[i - 1]			= Viewports[i];
			RenderThreads[i - 1]		= RenderThreads[i];
			renderResultEvents[i - 1]	= renderResultEvents[i];
		}

		errCode = viewportMemRealloc(vpCount - 1);
		if ( errCode != ERROR_SUCCESS )		throw errCode;
		vpCount--;
	}
	catch ( DWORD errc )
	{
		SetLastError(errc);
		return FALSE;
	}
	return TRUE;
}

const LPVIEWPORT RenderPool::getViewport(UINT vpNumber) 
{ 
	if ( vpNumber < vpCount )
		return Viewports[vpNumber];
	else
		return NULL;
}

UINT RenderPool::getViewportCount() { return vpCount; }

DWORD RenderPool::RenderWorld()
{
	DWORD	errCode,
			exitCode;
	try
	{
		if ( !isPoolInitialized )	throw (DWORD)ERROR_POOL_NOT_INITIALIZED;
		if ( vpCount == 0 )			throw (DWORD)ERROR_NO_VIEWPORTS_EXIST;
		
		hWndHdc = GetDC(hWndTied);
		SetEvent(poolRenderEvent);
		errCode = WaitForMultipleObjects(
									vpCount,
									renderResultEvents, 
									TRUE,
									WAIT_TIMEOUT_LIMIT
								);
		ResetEvent(poolRenderEvent);
		ReleaseDC(hWndTied, hWndHdc);
		if ( errCode == WAIT_TIMEOUT ) throw (DWORD)ERROR_THREAD_WAIT_TIMEOUT;
	}
	catch ( DWORD errc )
	{
		if  ( errc = ERROR_THREAD_WAIT_TIMEOUT ) 
		{
			for ( UINT i = 0; i < vpCount; )
			{
				GetExitCodeThread(RenderThreads[i], &exitCode);
				if ( exitCode != STILL_ACTIVE )
					RemoveViewport(i);
				else
					i++;
			}
		}
		return errc;
	}
	return ERROR_SUCCESS;
}

BOOL RenderPool::isInitialized() { return isPoolInitialized; }


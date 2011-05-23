
#include "Renderer.h"

// ============================================================================
// clsViewport implementation
LRESULT BkGndSaver(LPVOID Sender, WPARAM wParam, LPARAM lParam) { return 1L; }

clsViewport::clsViewport()
{
	cameraObjectID		= 0;
	rMode				= RM_WIREFRAME;
	lppScene			= NULL;

	AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
}

clsViewport::clsViewport(
	LPSCENE3D *lppSceneHost, 
	UINT uCameraObjectID, 
	RENDER_MODE renderMode
) {
	lppScene	= NULL;
	rMode		= RM_WIREFRAME;
	if ( lppSceneHost		!= NULL 
		&& *lppSceneHost	!= NULL 
		&& (*lppSceneHost)->getObject(uCameraObjectID)->clsID() == CLS_CAMERA
	) {
		lppScene	= lppSceneHost;
		cameraObjectID = uCameraObjectID;
	}
	rMode				= renderMode;

	AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
}

clsViewport::~clsViewport() { }

DWORD clsViewport::SetUp(
				LPFORM vpOwner,
				INT	vpPosX,
				INT vpPosY,
				UINT vpWidth,
				UINT vpHeight
) {
	if ( lppScene == NULL ) return E_FAILED;
	return 	clsForm::Create(	
				VIEWPORT_CLASS_NAME,
				(FORM_TYPE)(CHILD_FORM 
						| WS_CLIPSIBLINGS
						| WS_BORDER),
				0,
				vpPosX,
				vpPosY,
				vpWidth,
				vpHeight,
				vpOwner
			);
}

LPSCENE3D clsViewport::getLpScene()			{ return *lppScene; }
UINT clsViewport::getCameraObjectID()		{ return cameraObjectID; }
RENDER_MODE clsViewport::getRenderMode()	{ return rMode; }

VOID clsViewport::setSceneHost(LPSCENE3D *lppSceneHost)
{
	if ( lppSceneHost		!= NULL 
		&& *lppSceneHost	!= NULL 
		&& (*lppSceneHost)->getObjectClassCount(CLS_CAMERA) != 0
	) lppScene = lppSceneHost;
}

VOID clsViewport::setCameraObjectID(UINT uCameraObjectID)
{
	if ( lppScene		!= NULL 
		&& *lppScene	!= NULL 
		&& (*lppScene )->getObject(uCameraObjectID)->clsID() == CLS_CAMERA
	) cameraObjectID = uCameraObjectID;
}

VOID clsViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }

// ============================================================================
// clsRenderPool Implementation
DWORD WINAPI clsRenderPool::Render(LPVOID renderInfo)
{
	DWORD		dwWaitResult;
	BOOL		bAlive;

	LPVIEWPORT	vp				= (LPVIEWPORT)renderInfo;
	HDC			hVpDC,
				hMemDC;
	HBITMAP		hBMP,
				hBMPOld;
	UINT		vpWidth,
				vpHeight;
	
	// For test purposes only
	TRIVERTEX			testTriangle[3];
	GRADIENT_TRIANGLE	grTestTriangle;

	do {
		dwWaitResult = WaitForMultipleObjects(
						2,
						(LPEVENT)&vp->threadControls,
						FALSE,
						INFINITE
					);
		bAlive = dwWaitResult != WAIT_OBJECT_0 + 1;
		vp->getClientDC(&hVpDC);
		vp->getClientSize(&vpWidth, &vpHeight);
		hMemDC	= CreateCompatibleDC(hVpDC);
		hBMP	= CreateCompatibleBitmap(
									hVpDC,
									vpWidth,
									vpHeight
								);
		hBMPOld = (HBITMAP)SelectObject(hMemDC, hBMP);

		// Let's set triangle's vertices
		testTriangle[0].x		= 10;
		testTriangle[0].y		= 10;
		testTriangle[0].Red		= 0xFFFF;
		testTriangle[0].Green	= 0x0000;
		testTriangle[0].Blue	= 0x0000;
		testTriangle[0].Alpha	= 0x0000;

		testTriangle[1].x		= 940;
		testTriangle[1].y		= 450;
		testTriangle[1].Red		= 0x0000;
		testTriangle[1].Green	= 0xFFFF;
		testTriangle[1].Blue	= 0x0000;
		testTriangle[1].Alpha	= 0x0000;

		testTriangle[2].x		= 320;
		testTriangle[2].y		= 660;
		testTriangle[2].Red		= 0x0000;
		testTriangle[2].Green	= 0x0000;
		testTriangle[2].Blue	= 0xFFFF;
		testTriangle[2].Alpha	= 0x0000;

		grTestTriangle.Vertex1	= 0;
		grTestTriangle.Vertex2	= 1;
		grTestTriangle.Vertex3	= 2;

		// Won't change
		GradientFill(
			hMemDC, 
			testTriangle, 
			3, 
			&grTestTriangle,
			1, 
			GRADIENT_FILL_TRIANGLE
		);

		BitBlt(
			hVpDC,
			0,
			0,
			vpWidth,
			vpHeight,
			hMemDC,
			0,
			0,
			SRCCOPY
		);
		//vp->Validate();

		SelectObject(hMemDC, hBMPOld);
		DeleteObject(hBMP);
		DeleteDC(hMemDC);
		vp->dropDC(&hVpDC);
		ResetEvent(vp->threadControls.doRender);
	} while ( bAlive );

	return SHUTDOWN_ON_DEMAND;
}

UINT clsRenderPool::findViewport(DWORD vpID)
{
	UINT vpCount = Viewports.size();
	for ( UINT i = 0; i < vpCount; i++ )
	{
		if ( GetThreadId(Viewports[i]->Thread) == vpID ) return i;
	}
	return MAX_VIEWPORT_COUNT;
}

clsRenderPool::clsRenderPool(LPFORM lpOwner) 
	: Owner(lpOwner), Scene(NULL) 
{
	renderEvent = CreateEvent(0, TRUE, FALSE, NULL);
}
clsRenderPool::clsRenderPool(LPFORM lpOwner, LPSCENE3D lpScene) 
	: Owner(lpOwner), Scene(NULL)
{
	renderEvent = CreateEvent(0, TRUE, FALSE, NULL);
	assignScene(lpScene);
}
clsRenderPool::~clsRenderPool() 
{ 
	if ( renderEvent != NULL )	CloseHandle(renderEvent);
}

BOOL clsRenderPool::assignScene(LPSCENE3D lpScene)
{
	return (Scene = lpScene) != NULL;
}

DWORD clsRenderPool::addViewport(
						INT			vpPosX,
						INT			vpPosY,
						UINT		vpWidth,
						UINT		vpHeight,
						UINT		vpCameraObjectID,
						RENDER_MODE vpRMode
) {
	LPVIEWPORT	vpInfo;
	DWORD		vpID		= 0,
				dwResult;				
	BOOL		bResult;
	
	bResult 
		= Scene											!= NULL
		&& Owner										!= NULL
		&& renderEvent									!= NULL
		&& Viewports.size()								!= MAX_VIEWPORT_COUNT
		&& Scene->getObject(vpCameraObjectID)->clsID() 	== CLS_CAMERA;
	if ( bResult )
	{
		vpInfo = new VIEWPORT(&Scene, vpCameraObjectID, vpRMode);
		dwResult = vpInfo->SetUp(
							Owner,
							vpPosX,
							vpPosY,
							vpWidth,
							vpHeight
						);
		if ( bResult = SUCCEEDED(dwResult) )
		{
			vpInfo->Thread		= CreateThread(
										0,
										0,
										Render,
										vpInfo,
										CREATE_SUSPENDED,
										&vpID
									);
			bResult = vpInfo->Thread != NULL;
			if ( bResult )
			{
				vpInfo->threadControls.doRender = renderEvent;
				vpInfo->threadControls.shutDown	= CreateEvent(0, FALSE, FALSE, NULL);
				bResult = vpInfo->threadControls.shutDown != NULL;
				if ( bResult ) 
				{
					Viewports.push_back(vpInfo);
					ResumeThread(vpInfo->Thread);
					vpInfo->Show();
				}
			}
		}
		if ( !bResult ) delete vpInfo;
	}
	return vpID;
}

BOOL clsRenderPool::delViewport(UINT vpIndex)
{
	BOOL bResult = vpIndex < Viewports.size();
	if ( bResult ) 
	{
		SetEvent(Viewports[vpIndex]->threadControls.shutDown);
		WaitForSingleObject(Viewports[vpIndex]->Thread, THREAD_WAIT_TIMEOUT);
		CloseHandle(Viewports[vpIndex]->Thread);
		CloseHandle(Viewports[vpIndex]->threadControls.shutDown);
		delete Viewports[vpIndex];
		Viewports.erase(Viewports.begin() + vpIndex);
	}
	return bResult;
}

BOOL clsRenderPool::delViewport(DWORD vpID)
{
	return delViewport(findViewport(vpID));
}

LPVIEWPORT clsRenderPool::getViewport(UINT vpIndex)
{
	if ( vpIndex < Viewports.size() ) return Viewports[vpIndex];
	return NULL;
}

LPVIEWPORT clsRenderPool::getViewport(DWORD vpID)
{
	return Viewports[findViewport(vpID)];
}

BOOL clsRenderPool::RenderWorld() { return SetEvent(renderEvent); }

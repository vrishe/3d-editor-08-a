
#include "Renderer.h"

// ============================================================================
// clsViewport implementation
LRESULT BkGndSaver(LPVOID Sender, WPARAM wParam, LPARAM lParam) { return 1L; }

clsViewport::clsViewport()
{
	cameraObjectID	= 0;
	rMode			= RM_WIREFRAME;
	Scene			= NULL;

	AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
}

clsViewport::clsViewport(
	LPSCENE3D lpSceneHost, 
	UINT uCameraObjectID, 
	RENDER_MODE renderMode
) {
	cameraObjectID	= 0;
	Scene	= NULL;
	rMode	= RM_WIREFRAME;

	AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
	
	setScene(lpSceneHost);
	setCameraObjectID(uCameraObjectID);
	setRenderMode(renderMode);
}

clsViewport::~clsViewport() { }

DWORD clsViewport::SetUp(
				LPFORM vpOwner,
				INT	vpPosX,
				INT vpPosY,
				UINT vpWidth,
				UINT vpHeight
) {
	if ( Scene == NULL ) return E_DOES_NOT_EXIST;
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

LPSCENE3D clsViewport::getScene()			{ return Scene; }
UINT clsViewport::getCameraObjectID()		{ return cameraObjectID; }
RENDER_MODE clsViewport::getRenderMode()	{ return rMode; }

BOOL clsViewport::setScene(LPSCENE3D lpSceneHost)
{
	BOOL bResult 
		= lpSceneHost	!= NULL 
		&& lpSceneHost->getObjectClassCount(CLS_CAMERA) != 0;
	if ( bResult ) { 
		Scene = lpSceneHost;
		if ( Scene->getObject(cameraObjectID) == NULL )
			cameraObjectID = Scene->getObject(CLS_CAMERA, 0)->objID();
	}
	return bResult;
}

BOOL clsViewport::setCameraObjectID(UINT uCameraObjectID)
{	
	BOOL bResult 
		= Scene		!= NULL 
		&& Scene->getObject(uCameraObjectID)->clsID() == CLS_CAMERA;
	if ( bResult ) cameraObjectID = uCameraObjectID;
	return bResult;
}

VOID clsViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }

BOOL clsViewport::Render() 
{
	BOOL				bResult			= Scene != NULL;
	HDC					hVpDC,
						hMemDC;
	HBRUSH				hBrCurrent,
						hBrOld;
	HBITMAP				hBMP,
						hBMPOld;
	RECT				clientRect;
	INT					centerX,
						centerY;

	UINT				sceneObjCount,
						objVertCount,
						objEdgeCount,
						objPolyCount;

	HANDLE				procHeap		= GetProcessHeap();

	LPCAMERA3D			camToRender;
	LPMESH3D			objToRender;
	LPVECTOR3D			objVertBuffer;
	LPEDGE3D			objEdgeBuffer;
	LPPOLY3D			objPolyBuffer;
	COLOR3D				objColor;
	POINT				vert2DDrawBuffer[2];

	MATRIX3D			viewMatrix,
						worldMatrix(true);
	VECTOR3D			camPos;

	// Approaching viewport canvas for drawing
	GetClientRect(hWnd, &clientRect);
	centerX		= (INT)ceil((FLOAT)(clientRect.right / 2));
	centerY		= (INT)ceil((FLOAT)(clientRect.bottom / 2));
	hVpDC		= GetDC(hWnd);
	hMemDC		= CreateCompatibleDC(hVpDC);
	hBMP		= CreateCompatibleBitmap(
								hVpDC,
								clientRect.right,
								clientRect.bottom
							);
	hBMPOld		= (HBITMAP)SelectObject(hMemDC, hBMP);

	// Filling Viewport with scene ambient color
	hBrCurrent	= CreateSolidBrush(Scene->getAmbientColorRef());
	hBrOld		= (HBRUSH)SelectObject(hMemDC, hBrCurrent);
	FillRect(hMemDC, &clientRect, hBrCurrent);
	SelectObject(hMemDC, hBrOld);
	DeleteObject(hBrCurrent);

	if ( bResult ) 
	{
		sceneObjCount	= Scene->getObjectClassCount(CLS_MESH);
		camToRender		= (LPCAMERA3D)Scene->getObject(cameraObjectID);
		camPos			= camToRender->getPosition();
		camToRender->GetViewMatrix(&viewMatrix);

		for ( UINT i = 0; i < sceneObjCount; i++ )
		{
			objToRender		= (LPMESH3D)Scene->getObject(CLS_MESH, i);	
			objColor		= objToRender->getColor();
			objVertCount	= objToRender->getVCount();
			objPolyCount	= objToRender->getPCount();	
			objEdgeCount	= objToRender->getECount();
			objEdgeBuffer	= objToRender->getEdgesRaw();
			objPolyBuffer	= objToRender->getPolygonsRaw();
			objVertBuffer	= (LPVECTOR3D)HeapAlloc(
											procHeap, 
											HEAP_ZERO_MEMORY, 
											sizeof(VECTOR3D) * objVertCount
										);
			CopyMemory(objVertBuffer, objToRender->getVerticesRaw(), sizeof(VECTOR3D) * objVertCount);
			for ( UINT j = 0; j < objVertCount; j++ )
			{
				*(objVertBuffer + j) -= camPos;
				Matrix3DTransformCoord(
							&viewMatrix,
							(LPVECTOR3D)(objVertBuffer + j),
							(LPVECTOR3D)(objVertBuffer + j)
						);
				*(objVertBuffer + j) += camPos;
			}
			
			// Transformation calculations here:
			for ( UINT j = 0; j < objEdgeCount; j++ ) 
			{

				vert2DDrawBuffer[0].x 
					= (LONG)objVertBuffer[objEdgeBuffer[j].first].x
					+ centerX;
				vert2DDrawBuffer[0].y 
					= (LONG)objVertBuffer[objEdgeBuffer[j].first].y
					+ centerY;

				vert2DDrawBuffer[1].x 
					= (LONG)objVertBuffer[objEdgeBuffer[j].second].x
					+ centerX;
				vert2DDrawBuffer[1].y 
					= (LONG)objVertBuffer[objEdgeBuffer[j].second].y
					+ centerY;

				Polyline( hMemDC, vert2DDrawBuffer, 2 );
			}

			HeapFree(procHeap, NULL, objVertBuffer);
		}
	}

	BitBlt(
		hVpDC,
		0,
		0,
		clientRect.right,
		clientRect.bottom,
		hMemDC,
		0,
		0,
		SRCCOPY
	);


	SelectObject(hMemDC, hBMPOld);
	DeleteObject(hBMP);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hVpDC);

	return bResult;
}

// ============================================================================
// clsRenderPool Implementation
DWORD WINAPI clsRenderPool::Render(LPVOID renderInfo)
{
	DWORD		dwWaitResult;
	BOOL		bAlive;

	LPTHREAD_DATA	vp			= (LPTHREAD_DATA)renderInfo;
	
	do {
		dwWaitResult = WaitForMultipleObjects(
						2,
						(LPEVENT)&vp->threadControls,
						FALSE,
						INFINITE
					);
		bAlive = dwWaitResult != WAIT_OBJECT_0 + 1;
		vp->Viewport->Render();
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
	BOOL bResult
		= lpScene != NULL
		&& lpScene->getObjectClassCount(CLS_CAMERA);
	if ( bResult ) Scene = lpScene;
	return bResult;
}

DWORD clsRenderPool::addViewport(
						INT			vpPosX,
						INT			vpPosY,
						UINT		vpWidth,
						UINT		vpHeight,
						UINT		vpCameraObjectID,
						RENDER_MODE vpRMode
) {
	LPTHREAD_DATA	thData;
	LPOBJECT3D		camFound;
	DWORD			vpID		= 0,
					dwResult;				
	BOOL			bResult;
	
	bResult 
		= Scene												!= NULL
		&& Owner											!= NULL
		&& renderEvent										!= NULL
		&& Viewports.size()									!= MAX_VIEWPORT_COUNT
		&& (camFound = Scene->getObject(vpCameraObjectID)) 	!= NULL
		&& camFound->clsID()								== CLS_CAMERA;
	if ( bResult )
	{
		thData				= new THREAD_DATA;
		thData->Viewport	= new VIEWPORT(Scene, vpCameraObjectID, vpRMode);
		dwResult = thData->Viewport->SetUp(
										Owner,
										vpPosX,
										vpPosY,
										vpWidth,
										vpHeight
									);
		if ( bResult = SUCCEEDED(dwResult) )
		{
			thData->Thread	= CreateThread(
									0,
									0,
									Render,
									thData,
									CREATE_SUSPENDED,
									&vpID
								);
			bResult = thData->Thread != NULL;
			if ( bResult )
			{
				thData->threadControls.doRender = renderEvent;
				thData->threadControls.shutDown	= CreateEvent(0, FALSE, FALSE, NULL);
				bResult = thData->threadControls.shutDown != NULL;
				if ( bResult ) 
				{
					Viewports.push_back(thData);
					ResumeThread(thData->Thread);
					thData->Viewport->Show();
				}
			}
		}
		if ( !bResult ) 
		{
			delete thData->Viewport;
			delete thData;
		}
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
		delete Viewports[vpIndex]->Viewport;
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
	if ( vpIndex < Viewports.size() ) return Viewports[vpIndex]->Viewport;
	return NULL;
}

LPVIEWPORT clsRenderPool::getViewport(DWORD vpID)
{
	return Viewports[findViewport(vpID)]->Viewport;
}

BOOL clsRenderPool::RenderWorld() { return SetEvent(renderEvent); }

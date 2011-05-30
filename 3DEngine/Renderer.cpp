
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

bool ZBufSort(pair <DIRECTPOLY3D, UINT> a, pair <DIRECTPOLY3D, UINT> b) {
	float s1  = (a.first.first.z + a.first.second.z + a.first.third.z) / 3;
	float s2  = (b.first.first.z + b.first.second.z + b.first.third.z) / 3;
	if ( s1 < s2 )
		return true;
	return false;
}

BOOL clsViewport::Render() 
{
	BOOL				bResult			= Scene != NULL;
	HANDLE				procHeap		= GetProcessHeap();

	HDC					hVpDC,
						hMemDC;

	HBRUSH				hBrCurrent,
						hBrOld,
					   *hBrObjects;
	HBITMAP				hBMP,
						hBMPOld;

	RECT				clientRect;
	FLOAT				centerX,
						centerY;

	UINT				sceneObjCount,
						sceneVertCount,
						scenePolyCount,
						objVertCount,
						objEdgeCount,
						objPolyCount;

	LPCAMERA3D			camToRender;
	LPMESH3D			objToRender;
	LPVECTOR3D			objVertBuffer;
	LPEDGE3D			objEdgeBuffer;
	LPPOLY3D			objPolyBuffer;
	COLOR3D				objColor;
	SCENEPOLY			scenePolyBuffer;

	MATRIX3D			cameraMatrix,
						projectionMatrix,
						viewportMatrix;
	FLOAT				divisor,
						nearClip,
						farClip;

	VECTOR3D			camPos;
	POINT				vert2DDrawBuffer[3];

	// Approaching viewport canvas for drawing
	GetClientRect(hWnd, &clientRect);
	centerX		= ceil((FLOAT)(clientRect.right));
	centerY		= ceil((FLOAT)(clientRect.bottom));
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
		const UINT VEC3DSIZE = sizeof(VECTOR3D);
		sceneObjCount	= Scene->getObjectClassCount(CLS_MESH);
		scenePolyCount	= Scene->getPolygonsCount();
		sceneVertCount	= Scene->getVerticesCount();

		// prepearing camera
		camToRender		= (LPCAMERA3D)Scene->getObject(cameraObjectID);
		nearClip		= camToRender->getNearCP();
		farClip			= camToRender->getFarCP();
		
		camToRender->GetViewMatrix(&cameraMatrix);

		viewportMatrix.SetIdentity();
		viewportMatrix._22 = -1.0f;
		viewportMatrix._41 = (FLOAT)clientRect.right / 2;
		viewportMatrix._42 = (FLOAT)clientRect.bottom / 2;

		if ( camToRender->getProjectionType() == PT_CENTRAL )
			camToRender->GetPerspectiveMatrix(&projectionMatrix);
		else 
			camToRender->GetParallelMatrix(&projectionMatrix);

		for ( UINT i = 0; i < sceneObjCount; i++ )
		{
			objToRender		= (LPMESH3D)Scene->getObject(CLS_MESH, i);	
			objColor		= objToRender->getColor();
			objVertCount	= objToRender->getVerticesCount();
			objPolyCount	= objToRender->getPolygonsCount();	
			objEdgeCount	= objToRender->getEdgesCount();
			objEdgeBuffer	= objToRender->getEdgesRaw();
			objPolyBuffer	= objToRender->getPolygonsRaw();
			objVertBuffer	= (LPVECTOR3D)HeapAlloc(
											procHeap, 
											HEAP_ZERO_MEMORY, 
											VEC3DSIZE * objVertCount
										);

			objToRender->getVerticesTransformed(objVertBuffer);

			// camera projection transformations
			for ( UINT j = 0; j < objVertCount; j++ ) 
			{
				Matrix3DTransformCoord(
						&cameraMatrix,
						objVertBuffer + j,
						objVertBuffer + j
					);

				divisor = objVertBuffer[j].z + (farClip - nearClip) / 2;
				Matrix3DTransformCoord(
								&projectionMatrix,
								objVertBuffer + j,
								objVertBuffer + j
							);
				if ( camToRender->getProjectionType() == PT_CENTRAL )
					objVertBuffer[j] /= divisor;
				else
					objVertBuffer[j].z /= divisor;

				Matrix3DTransformCoord(
						&viewportMatrix,
						objVertBuffer + j,
						objVertBuffer + j
					);
			}


			if ( rMode != RM_WIREFRAME ) {
				// filling a part of scene buffer
				for (UINT j = 0; j < objPolyCount; j++) {
					DIRECTPOLY3D tmp;
					tmp.first	= objVertBuffer[ objPolyBuffer[j].first ];
					tmp.second	= objVertBuffer[ objPolyBuffer[j].second ];
					tmp.third	= objVertBuffer[ objPolyBuffer[j].third ];
					scenePolyBuffer.push_back(pair<DIRECTPOLY3D,int>(tmp,i));
				}
			}
			else {
				for ( UINT j = 0; j < objEdgeCount; j++ ) { // Yeah baby, that's the stuff!
					if ( objVertBuffer[objEdgeBuffer[j].first].z >= 0
					  && objVertBuffer[objEdgeBuffer[j].first].z <= 1
					  && objVertBuffer[objEdgeBuffer[j].second].z >= 0
					  && objVertBuffer[objEdgeBuffer[j].second].z <= 1 
					  && objVertBuffer[objEdgeBuffer[j].first].x >= 0
					  && objVertBuffer[objEdgeBuffer[j].first].x <= clientRect.right
					  && objVertBuffer[objEdgeBuffer[j].first].y >= 0
					  && objVertBuffer[objEdgeBuffer[j].first].y <= clientRect.bottom
					  && objVertBuffer[objEdgeBuffer[j].second].x >= 0
					  && objVertBuffer[objEdgeBuffer[j].second].x <= clientRect.right
					  && objVertBuffer[objEdgeBuffer[j].second].y >= 0
					  && objVertBuffer[objEdgeBuffer[j].second].y <= clientRect.bottom)
					{ 
						vert2DDrawBuffer[0].x 
							= (LONG)objVertBuffer[objEdgeBuffer[j].first].x;
						vert2DDrawBuffer[0].y 
							= (LONG)objVertBuffer[objEdgeBuffer[j].first].y;
				
						vert2DDrawBuffer[1].x 
							= (LONG)objVertBuffer[objEdgeBuffer[j].second].x;
						vert2DDrawBuffer[1].y 
							= (LONG)objVertBuffer[objEdgeBuffer[j].second].y;

						Polyline( hMemDC, vert2DDrawBuffer, 2 );
					}
				}
			}

			HeapFree(procHeap, NULL, objVertBuffer);
		}

		if ( rMode != RM_WIREFRAME ) 
		{
			sort(scenePolyBuffer.begin(), scenePolyBuffer.end(), ZBufSort);

			HPEN hPenCur = CreatePen(PS_SOLID, 1, RGB(0,0,0));
			HPEN hPenOld = (HPEN)SelectObject(hMemDC, hPenCur);
			hBrObjects	= new HBRUSH[sceneObjCount];
			for (UINT i = 0; i < sceneObjCount; i++ )
				hBrObjects[i] = CreateSolidBrush(
						((LPMESH3D)Scene->getObject(CLS_MESH, i))->getColorRef()
					);

			for (UINT i = 0; i < scenePolyCount; i++ ) { // AAaaaaand..... Here we go again! 
				if ( scenePolyBuffer[i].first.first.z > 0
				  && scenePolyBuffer[i].first.first.z <= 1
				  && scenePolyBuffer[i].first.second.z >= 0
				  && scenePolyBuffer[i].first.second.z <= 1
				  && scenePolyBuffer[i].first.third.z >= 0
				  && scenePolyBuffer[i].first.third.z <= 1 

				  && scenePolyBuffer[i].first.first.x >= 0
				  && scenePolyBuffer[i].first.first.x <= clientRect.right
				  && scenePolyBuffer[i].first.second.x >= 0
			   	  && scenePolyBuffer[i].first.second.x <= clientRect.right
				  && scenePolyBuffer[i].first.third.x >= 0
			   	  && scenePolyBuffer[i].first.third.x <= clientRect.right
				  
				  && scenePolyBuffer[i].first.first.y >= 0
			   	  && scenePolyBuffer[i].first.first.y <= clientRect.bottom
				  && scenePolyBuffer[i].first.second.y >= 0
			   	  && scenePolyBuffer[i].first.second.y <= clientRect.bottom
				  && scenePolyBuffer[i].first.third.y >= 0
			   	  && scenePolyBuffer[i].first.third.y <= clientRect.bottom)
				{ 
					VECTOR3D normal(scenePolyBuffer[i].first.Normal(1));
					float cosA = Vector3DMultS(&normal, &VECTOR3D(0,0,1)) / Vector3DLength(&normal);
					if (cosA >= -1 && cosA <= 0) 
					{
						hBrOld		= (HBRUSH)SelectObject(hMemDC, hBrObjects[scenePolyBuffer[i].second]);
						vert2DDrawBuffer[0].x = (LONG)scenePolyBuffer[i].first.first.x;
						vert2DDrawBuffer[0].y = (LONG)scenePolyBuffer[i].first.first.y;

						vert2DDrawBuffer[1].x = (LONG)scenePolyBuffer[i].first.second.x;
						vert2DDrawBuffer[1].y = (LONG)scenePolyBuffer[i].first.second.y;

						vert2DDrawBuffer[2].x = (LONG)scenePolyBuffer[i].first.third.x;
						vert2DDrawBuffer[2].y = (LONG)scenePolyBuffer[i].first.third.y;

						Polygon( hMemDC, vert2DDrawBuffer, 3 );
						SelectObject(hMemDC, hBrOld);
					}
				}
			}

			for (UINT i = 0; i < sceneObjCount; i++ ) DeleteObject(hBrObjects[i]);		
			delete[] hBrObjects;
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
		SetEvent(vp->threadControls.jobDone);
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
				thData->threadControls.jobDone	= CreateEvent(0, FALSE, FALSE, NULL);
				bResult = thData->threadControls.shutDown != NULL
						&& thData->threadControls.jobDone != NULL;
				if ( bResult ) 
				{
					Viewports.push_back(thData);
					vpStates.push_back(thData->threadControls.jobDone);
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
		CloseHandle(Viewports[vpIndex]->threadControls.jobDone);
		delete Viewports[vpIndex]->Viewport;
		delete Viewports[vpIndex];
		Viewports.erase(Viewports.begin() + vpIndex);
		vpStates.erase(vpStates.begin() + vpIndex);
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

DWORD clsRenderPool::RenderWorld() 
{ 
	SetEvent(renderEvent);
	return WaitForMultipleObjects(
				vpStates.size(),
				vpStates.data(),
				TRUE,
				THREAD_WAIT_TIMEOUT
			);
}


#include "Renderer.h"

// ============================================================================
// clsViewport implementation
LRESULT BkGndSaver(LPVOID Sender, WPARAM wParam, LPARAM lParam) { return 1L; }

clsViewport::clsViewport(){
	cameraObjectID	= DEFAULT_CAMERA_ID;
	rMode			= RM_WIREFRAME;
	Scene			= NULL;

	AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
}

clsViewport::clsViewport(
	LPSCENE3D lpSceneHost, 
	UINT uCameraObjectID, 
	RENDER_MODE renderMode) 
{
	cameraObjectID	= DEFAULT_CAMERA_ID;
	rMode			= RM_WIREFRAME;
	Scene			= NULL;

	AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
	
	setScene(lpSceneHost);
	setCameraObjectID(uCameraObjectID);
	setRenderMode(renderMode);
}

clsViewport::~clsViewport() { }

UINT clsViewport::getID() { return ID; } 

DWORD clsViewport::SetUp(
				UINT	vpID,
				LPFORM	vpOwner,
				INT		vpPosX,
				INT		vpPosY,
				UINT	vpWidth,
				UINT	vpHeight) 
{
	if ( Scene == NULL ) return E_DOES_NOT_EXIST;
	ID	= vpID;
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

BOOL clsViewport::setScene(LPSCENE3D lpSceneHost) {
	BOOL bResult = lpSceneHost	!= NULL;
	if ( bResult ) Scene = lpSceneHost;
	return bResult;
}

BOOL clsViewport::setCameraObjectID(UINT uCameraObjectID) {	
	BOOL bResult = TRUE;

	cameraObjectID = DEFAULT_CAMERA_ID;
	if ( uCameraObjectID != DEFAULT_CAMERA_ID )
	{
			bResult = Scene != NULL
				&& Scene->getObject(uCameraObjectID)->clsID() == CLS_CAMERA;
			if ( bResult ) cameraObjectID = uCameraObjectID;		
	}
	return bResult;
}

VOID clsViewport::setRenderMode(RENDER_MODE renderMode) { rMode = renderMode; }

bool ZBufSort(pair <DIRECTPOLY3D, UINT> a, pair <DIRECTPOLY3D, UINT> b) {
	float s1  = (a.first.first.z + a.first.second.z + a.first.third.z) / 3;
	float s2  = (b.first.first.z + b.first.second.z + b.first.third.z) / 3;
	if ( s1 > s2 )
		return true;
	return false;
}

BOOL clsViewport::Render() {
	BOOL				bResult			= Scene != NULL;
	HANDLE				procHeap		= GetProcessHeap();

	HDC					hVpDC,
						hMemDC;

	HPEN				hPenCurrent, 
						hPenOld;
	HBRUSH				hBrCurrent,
						hBrOld;
	HBITMAP				hBMP,
						hBMPOld;

	RECT				clientRect;
	UINT				sceneObjCount,
						sceneLightCount,
						scenePolyCount,
						sceneLightedPolyCount,
						objVertCount,
						objEdgeCount,
						objPolyCount;

	FLOAT				centerX,
						centerY,
						divisor,
						nearClip,
						farClip;
	VECTOR3D			camPos;

	LPCAMERA3D			camToRender;
	LPDIFLIGHT3D		lightToRender;
	LPMESH3D			objToRender;
	LPVECTOR3D			objVertBuffer;
	LPEDGE3D			objEdgeBuffer;
	LPPOLY3D			objPolyBuffer;
	COLOR3D				objColor;

	MATRIX3D			cameraMatrix,
						projectionMatrix,
						viewportMatrix;
	
	SCENEPOLY			scenePolyBuffer;
	LPCOLOR3D			scenePolyColorBuffer;
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
	hBrCurrent	= CreateSolidBrush(Scene->getAmbientColor());
	hBrOld		= (HBRUSH)SelectObject(hMemDC, hBrCurrent);
	FillRect(hMemDC, &clientRect, hBrCurrent);
	SelectObject(hMemDC, hBrOld);
	DeleteObject(hBrCurrent);

	if ( bResult ) 
	{
		sceneObjCount	= Scene->getObjectClassCount(CLS_MESH);

		if ( rMode != RM_WIREFRAME ) {
			scenePolyCount			= Scene->getPolygonsCount();
			sceneLightCount			= Scene->getObjectClassCount(CLS_LIGHT);
			scenePolyColorBuffer	= new COLOR3D[scenePolyCount];
			sceneLightedPolyCount	= 0;
		}

		// prepearing camera
		camToRender		= cameraObjectID == DEFAULT_CAMERA_ID ? 
							&camDefault
							: (LPCAMERA3D)Scene->getObject(cameraObjectID);
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
			objToRender				= (LPMESH3D)Scene->getObject(CLS_MESH, i);	
			objColor				= objToRender->getColor();
			objVertCount			= objToRender->getVerticesCount();
			objVertBuffer			= (LPVECTOR3D)HeapAlloc(
											procHeap, 
											HEAP_ZERO_MEMORY, 
											sizeof(VECTOR3D) * objVertCount
										);

			objToRender->getVerticesTransformed(objVertBuffer);

			// calculate lightning here
			if ( rMode != RM_WIREFRAME ) {
				objPolyBuffer	= objToRender->getPolygonsRaw();
				objPolyCount	= objToRender->getPolygonsCount();
				UINT lightTo	= sceneLightedPolyCount + objPolyCount; // number of polygons to light

				for (UINT j = sceneLightedPolyCount; j < lightTo; j++) {
					VECTOR3D normal(objPolyBuffer[j - sceneLightedPolyCount].Normal(objVertBuffer, 2));
					Vector3DNormalize(&normal, &normal);
					if ( !sceneLightCount ) {
						scenePolyColorBuffer[j] = objToRender->getColor();
						continue;
					}
					else
						scenePolyColorBuffer[j] = COLOR3D(0,0,0);

					for (UINT k = 0; k < sceneLightCount; k++) 	{
						lightToRender = (LPDIFLIGHT3D)Scene->getObject(CLS_LIGHT, k);
						FLOAT power = lightToRender->getPower();
						COLOR3D lightColor	= lightToRender->getColor();
						if ( power == 0 || lightColor == BLACK)
							continue;
						FLOAT ratio = Vector3DMultS(&normal, &lightToRender->getForwardLookDirection());
						if (ratio < -EPSILON)
							ratio = power - ratio;
						else
							if (ratio < EPSILON)
								ratio = max(power / 3.3f, (FLOAT)DARK_SIDE);
							else
								ratio = (FLOAT)DARK_SIDE;

						COLOR3D newColor	= objToRender->getColor();								
						UINT red	= (UINT)(min((newColor.Red + lightColor.Red)/2, 255)     * ratio);
						UINT green	= (UINT)(min((newColor.Green + lightColor.Green)/2, 255) * ratio);
						UINT blue	= (UINT)(min((newColor.Blue + lightColor.Blue)/2, 255)   * ratio);

						if (scenePolyColorBuffer[j] != BLACK) {
							newColor.Red	= (UINT)(min((red	+ scenePolyColorBuffer[j].Red)  , 255));
							newColor.Green	= (UINT)(min((green + scenePolyColorBuffer[j].Green), 255));
							newColor.Blue	= (UINT)(min((blue	+ scenePolyColorBuffer[j].Blue) , 255));
						}
						else {
							newColor.Red	= ( red > 255	? 255 : red );
							newColor.Green	= ( green > 255	? 255 : green );
							newColor.Blue	= ( blue > 255	? 255 : blue );
						}
						scenePolyColorBuffer[j] = newColor;
					}
				}
			}

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


			if ( rMode != RM_WIREFRAME ) 
			{
				// filling a part of scene buffer
				for (UINT j = 0; j < objPolyCount; j++) {
					VECTOR3D normal(objPolyBuffer[j].Normal(objVertBuffer, 1));
					float cosCam = Vector3DMultS(&normal, &VECTOR3D(0,0,1)) 
													/ Vector3DLength(&normal);
					if (cosCam >= -1 && cosCam < 0) {
						DIRECTPOLY3D tmp;
						tmp.first	= objVertBuffer[ objPolyBuffer[j].first ];
						tmp.second	= objVertBuffer[ objPolyBuffer[j].second ];
						tmp.third	= objVertBuffer[ objPolyBuffer[j].third ];
						tmp.colorRef = RGB(scenePolyColorBuffer[j + sceneLightedPolyCount].Red, 
										   scenePolyColorBuffer[j + sceneLightedPolyCount].Green,
										   scenePolyColorBuffer[j + sceneLightedPolyCount].Blue);
						scenePolyBuffer.push_back(pair<DIRECTPOLY3D,int>(tmp,i));
					}
				}
				sceneLightedPolyCount += objPolyCount;
			}
			else 
			{
				objEdgeCount	= objToRender->getEdgesCount();
				objEdgeBuffer	= objToRender->getEdgesRaw();

				hPenCurrent		= CreatePen(PS_SOLID, 1, objToRender->getColorRef());
				hPenOld			= (HPEN)SelectObject(hMemDC, hPenCurrent);
				for ( UINT j = 0; j < objEdgeCount; j++ ) 
				{
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
						&& objVertBuffer[objEdgeBuffer[j].second].y <= clientRect.bottom
					) { 
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
				SelectObject(hMemDC, hPenOld);
				DeleteObject(hPenCurrent);
			}

			HeapFree(procHeap, NULL, objVertBuffer);
		}

		if ( rMode != RM_WIREFRAME ) 
		{
			sort(scenePolyBuffer.begin(), scenePolyBuffer.end(), ZBufSort);

			scenePolyCount	= scenePolyBuffer.size();
			for (UINT i = 0; i < scenePolyCount; i++ ) 
			{
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
			   		&& scenePolyBuffer[i].first.third.y <= clientRect.bottom
				) { 
					objToRender	= (LPMESH3D)Scene->getObject(
												CLS_MESH, 
												scenePolyBuffer[i].second
											);

					hBrCurrent = CreateSolidBrush(scenePolyBuffer[i].first.colorRef);
					if ( rMode == RM_SHADEDWF ) 
						hPenCurrent = CreatePen(PS_SOLID, 1, objToRender->getColorRef());
					else
						hPenCurrent = CreatePen(PS_SOLID, 1, scenePolyBuffer[i].first.colorRef);
					
					hPenOld = (HPEN)SelectObject(hMemDC, hPenCurrent);
					hBrOld	= (HBRUSH)SelectObject(hMemDC, hBrCurrent);

					vert2DDrawBuffer[0].x = (LONG)scenePolyBuffer[i].first.first.x;
					vert2DDrawBuffer[0].y = (LONG)scenePolyBuffer[i].first.first.y;

					vert2DDrawBuffer[1].x = (LONG)scenePolyBuffer[i].first.second.x;
					vert2DDrawBuffer[1].y = (LONG)scenePolyBuffer[i].first.second.y;

					vert2DDrawBuffer[2].x = (LONG)scenePolyBuffer[i].first.third.x;
					vert2DDrawBuffer[2].y = (LONG)scenePolyBuffer[i].first.third.y;

					Polygon( hMemDC, vert2DDrawBuffer, 3 );

					SelectObject(hMemDC, hBrOld);
					SelectObject(hMemDC, hPenOld);
					DeleteObject(hBrCurrent);
					DeleteObject(hPenCurrent);
				}
			}
			delete[] scenePolyColorBuffer;
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

VOID SetViewportDefaultView(LPVIEWPORT vp, VIEW_TYPE vt)
{
	VECTOR3D	defCamPos;
	FLOAT		perspCoords = VIEW_DISTANCE_DEFAULT;

	switch ( vt )
	{
		case VIEW_LEFT:
			defCamPos.y = perspCoords;
			break;

		case VIEW_RIGHT:

			defCamPos.y = -perspCoords;
			break;

		case VIEW_FRONT:

			defCamPos.x = perspCoords;
			break;	

		case VIEW_BACK:

			defCamPos.x = -perspCoords;
			break;	

		case VIEW_TOP:
			defCamPos.z = perspCoords;
			vp->camDefault.Roll((FLOAT)M_PI);
			break;

		case VIEW_BOTTOM:

			defCamPos.z = -perspCoords;
			break;

		case VIEW_PERSPECTIVE:
			perspCoords = sqrt((perspCoords * perspCoords) / 3);
			defCamPos.x = perspCoords;
			defCamPos.y = perspCoords;
			defCamPos.z = perspCoords;
			vp->camDefault.setProjectionType(PT_CENTRAL);
			break;
	}

	vp->camDefault.Translate(defCamPos);
}

// ============================================================================
// clsViewportPool Implementation
DWORD WINAPI clsViewportPool::Render(LPVOID renderInfo)
{
	HDC			hDC;
	UINT		vpWidth,
				vpHeight;

	DWORD		dwWaitResult;
	BOOL		bAlive;

	LPTHREAD_DATA	vp			= (LPTHREAD_DATA)renderInfo;
	LPTSTR			vpName		= new TCHAR[MAX_OBJECT_NAME_LEN];

	HBRUSH			hBrOld;

	HFONT			hFontOld,
					hFontCur = CreateFont(
									-FRAME_FONT_SIZE,
									0, 0, 0,
									FW_THIN,
									0, 0, 0,
									DEFAULT_CHARSET,
									OUT_DEFAULT_PRECIS,
									CLIP_DEFAULT_PRECIS,
									CLEARTYPE_QUALITY,
									VARIABLE_PITCH,
									FRAME_FONT_FAMILY
								);

	HPEN			hPenOld,
					hPenCur = CreatePen(
									PS_INSIDEFRAME, 
									FRAME_STROKE_WIDTH, 
									FRAME_STROKE_COLORREF
								);
	do {
		dwWaitResult = WaitForMultipleObjects(
						2,
						(LPEVENT)&vp->threadControls,
						FALSE,
						INFINITE
					);
		bAlive = dwWaitResult != WAIT_OBJECT_0 + 1;

		vp->Viewport->Render();

		vp->Viewport->camDefault.getName(vpName, MAX_OBJECT_NAME_LEN);
		vp->Viewport->getDC(&hDC);
		if ( vp->isActive ) 
		{
			vp->Viewport->getSize(&vpWidth, &vpHeight);
			hPenOld = (HPEN)SelectObject(hDC, hPenCur);
			hBrOld	= (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
			Rectangle(hDC, 0, 0, vpWidth, vpHeight);
			SelectObject(hDC, hPenOld);
			SelectObject(hDC, hBrOld);
		}
		hFontOld = (HFONT)SelectObject(hDC, hFontCur); 
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, FRAME_FONT_COLOR);
		TextOut(
			hDC, 
			FRAME_STROKE_WIDTH, 
			FRAME_STROKE_WIDTH, 
			vpName, 
			_tcslen(vpName)
		);
		SelectObject(hDC, hFontOld);
		vp->Viewport->dropDC(&hDC);

		ResetEvent(vp->threadControls.doRender);
		SetEvent(vp->threadControls.jobDone);
	} while ( bAlive );

	DeleteObject(hPenCur);	
	DeleteObject(hFontCur);
	delete[] vpName;
	return SHUTDOWN_ON_DEMAND;
}

UINT clsViewportPool::findViewport(DWORD vpID)
{
	UINT vpCount = Viewports.size();
	for ( UINT i = 0; i < vpCount; i++ )
	{
		if ( Viewports[i]->Viewport->getID() == vpID ) return i;
	}
	return MAX_VIEWPORT_COUNT;
}

clsViewportPool::clsViewportPool(LPFORM lpOwner) 
	: Owner(lpOwner), Scene(NULL) 
{
	renderEvent		= CreateEvent(0, TRUE, FALSE, NULL);
}
clsViewportPool::clsViewportPool(LPFORM lpOwner, LPSCENE3D lpScene) 
	: Owner(lpOwner), Scene(NULL)
{
	renderEvent		= CreateEvent(0, TRUE, FALSE, NULL);
	assignScene(lpScene);
}
clsViewportPool::~clsViewportPool() 
{ 
	if ( renderEvent	!= NULL )	CloseHandle(renderEvent);
}

BOOL clsViewportPool::assignScene(LPSCENE3D lpScene)
{
	BOOL bResult = lpScene != NULL;
	if ( bResult ) Scene = lpScene;
	return bResult;
}

DWORD clsViewportPool::addViewport(
					INT			vpPosX,
					INT			vpPosY,
					UINT		vpWidth,
					UINT		vpHeight,				
					VIEW_TYPE	vpVType,
					RENDER_MODE vpRMode
) {
	LPTHREAD_DATA	thData		= {NULL};
	DWORD			vpID		= 0,
					dwResult;				
	
	try {
		if ( Scene == NULL || Owner == NULL )			throw E_FAILED;
		if ( renderEvent == NULL )						throw E_EVENT_CREATION_FAILED;
		if ( Viewports.size() == MAX_VIEWPORT_COUNT )	throw E_MAX_COUNT_REACHED; 

		thData				= new THREAD_DATA;
		thData->Viewport	= new VIEWPORT(Scene, DEFAULT_CAMERA_ID, vpRMode);

		thData->Thread	= CreateThread(
									0,
									0,
									Render,
									thData,
									CREATE_SUSPENDED,
									&vpID
								);
		if ( thData->Thread == NULL ) throw E_THREAD_CREATION_FAILED;

		dwResult = thData->Viewport->SetUp(
									vpID,
									Owner,
									vpPosX,
									vpPosY,
									vpWidth,
									vpHeight
								);
		if ( FAILED(dwResult) ) throw dwResult;

		thData->threadControls.doRender = renderEvent;
		thData->threadControls.shutDown	= CreateEvent(0, FALSE, FALSE, NULL);
		thData->threadControls.jobDone	= CreateEvent(0, FALSE, FALSE, NULL);
		if (
			thData->threadControls.shutDown		== NULL
			|| thData->threadControls.jobDone	== NULL
		) throw E_EVENT_CREATION_FAILED;

		thData->isActive = FALSE;
		Viewports.push_back(thData);
		vpStates.push_back(thData->threadControls.jobDone);
		SetViewportDefaultView(thData->Viewport, vpVType);
		ResumeThread(thData->Thread);
		thData->Viewport->Show();
	}
	catch ( DWORD errCode )
	{
		if ( thData->Viewport != NULL )	delete thData->Viewport;
		if ( thData != NULL )			delete thData;
		return errCode;
	}
	return S_DONE;
}

BOOL clsViewportPool::delViewport(UINT vpIndex)
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

BOOL clsViewportPool::delViewport(DWORD vpID)
{
	return delViewport(findViewport(vpID));
}

LPVIEWPORT clsViewportPool::getViewport(UINT vpIndex)
{
	if ( vpIndex < Viewports.size() ) return Viewports[vpIndex]->Viewport;
	return NULL;
}

LPVIEWPORT clsViewportPool::getViewport(DWORD vpID)
{
	UINT vpIndex = findViewport(vpID);
	if ( vpIndex < Viewports.size() ) return Viewports[vpIndex]->Viewport;
	return NULL;
}

LPVIEWPORT clsViewportPool::getActiveViewport()
{
	UINT vpCount = Viewports.size();

	for(UINT i = 0; i < vpCount; i++)
		if ( Viewports[i]->isActive ) return Viewports[i]->Viewport;

	return NULL;
}

UINT clsViewportPool::getActiveViewportIndex()
{
	UINT vpCount = Viewports.size();

	for(UINT i = 0; i < vpCount; i++)
		if ( Viewports[i]->isActive ) return i;

	return vpCount;
}

UINT clsViewportPool::getViewportCount() { return Viewports.size(); }

VOID clsViewportPool::setActiveViewport(UINT vpActiveIndex)
{
	UINT vpCount = Viewports.size();
	BOOL bResult = vpActiveIndex < vpCount;

	for(UINT i = 0; i < vpCount; i++)
			Viewports[i]->isActive = FALSE;

	if ( bResult ) 
		Viewports[vpActiveIndex]->isActive = bResult;
}

VOID clsViewportPool::setActiveViewport(DWORD vpActiveID)
{
	setActiveViewport(findViewport(vpActiveID));
}

DWORD clsViewportPool::RenderWorld() 
{ 
	SetEvent(renderEvent);
	return WaitForMultipleObjects(
				vpStates.size(),
				vpStates.data(),
				TRUE,
				THREAD_WAIT_TIMEOUT
			);
}

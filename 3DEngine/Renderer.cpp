
#include "Renderer.h"

// ============================================================================
// clsViewport implementation
LRESULT BkGndSaver(LPVOID Sender, WPARAM wParam, LPARAM lParam) { return 1L; }

clsViewport::clsViewport() {
  rMode     = RM_WIREFRAME;
  Scene     = NULL;

  camOutput   = &camDefault;
  AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);
}

clsViewport::clsViewport(
  LPSCENE3D lpSceneHost, 
  UINT uCameraObjectID, 
  RENDER_MODE renderMode
) {
  rMode     = RM_WIREFRAME;
  Scene     = NULL;

  camOutput   = &camDefault;
  AssignEventHandler(WM_ERASEBKGND, BkGndSaver, TRUE);

  setScene(lpSceneHost);
  setRenderMode(renderMode);
}

clsViewport::~clsViewport() { }

UINT clsViewport::getID() { return ID; } 

DWORD clsViewport::SetUp(
        UINT  vpID,
        LPFORM  vpOwner,
        INT   vpPosX,
        INT   vpPosY,
        UINT  vpWidth,
        UINT  vpHeight) 
{
  if ( Scene == NULL ) return E_DOES_NOT_EXIST;
  ID  = vpID;
  return  clsForm::Create(
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

LPSCENE3D clsViewport::getScene()     { return Scene; }
UINT clsViewport::getCameraObjectID()
{
  if ( 
    Scene != NULL 
    && camOutput != NULL 
  ) return camOutput == &camDefault ? 
      DEFAULT_CAMERA_ID : camOutput->objID();

  return camDefault.objID();
}
LPTARGCAMERA3D clsViewport::getCamera() { return camOutput; }
RENDER_MODE clsViewport::getRenderMode()  { return rMode; }

BOOL clsViewport::setScene(LPSCENE3D lpSceneHost) {
  BOOL bResult = lpSceneHost  != NULL;
  if ( bResult ) Scene = lpSceneHost;
  return bResult;
}

BOOL clsViewport::setCameraByObjectID(UINT uCameraObjectID) 
{
  LPOBJECT3D  scObj;
  BOOL bResult = Scene != NULL;

  if ( bResult )
  {
    camOutput = &camDefault;
    if ( uCameraObjectID != DEFAULT_CAMERA_ID )
    {
      scObj = Scene->getObject(uCameraObjectID);
      bResult = scObj->clsID() == CLS_CAMERA;
      if ( bResult ) camOutput = (LPTARGCAMERA3D)scObj;
    }
  }
  return bResult;
}

BOOL clsViewport::setCamera(const LPTARGCAMERA3D cam)
{
  BOOL bResult = TRUE;

  if ( bResult ) 
  {
    camOutput = cam;
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
  BOOL        bResult     = Scene != NULL 
                    && camOutput != NULL;
  HANDLE        procHeap    = GetProcessHeap();

  HDC         hVpDC,
            hMemDC;

  HPEN        hPenCurrent, 
            hPenOld;
  HBRUSH        hBrCurrent,
            hBrOld;
  HBITMAP       hBMP,
            hBMPOld;

  RECT        clientRect;
  UINT        sceneObjCount,
            sceneLightCount,
            scenePolyCount,
            sceneLightedPolyCount,
            objVertCount,
            objEdgeCount,
            objPolyCount;

  LPDIFLIGHT3D    lightToRender;
  LPMESH3D      objToRender;
  LPVECTOR3D      objVertBuffer;
  LPEDGE3D      objEdgeBuffer;
  LPPOLY3D      objPolyBuffer;

  MATRIX3D      cameraMatrix,
            projectionMatrix,
            viewportMatrix;

  SCENEPOLY     scenePolyBuffer;
  LPCOLORREF      scenePolyColorBuffer;
  POINT       vert2DDrawBuffer[3];

  // Approaching viewport canvas for drawing
  GetClientRect(hWnd, &clientRect);
  hVpDC   = GetDC(hWnd);
  hMemDC    = CreateCompatibleDC(hVpDC);
  hBMP    = CreateCompatibleBitmap(
                hVpDC,
                clientRect.right,
                clientRect.bottom
              );
  hBMPOld   = (HBITMAP)SelectObject(hMemDC, hBMP);

  // Filling Viewport with scene ambient color
  hBrCurrent  = CreateSolidBrush(Scene->getAmbientColor());
  hBrOld    = (HBRUSH)SelectObject(hMemDC, hBrCurrent);
  FillRect(hMemDC, &clientRect, hBrCurrent);
  SelectObject(hMemDC, hBrOld);
  DeleteObject(hBrCurrent);

  if ( bResult ) 
  {
    sceneObjCount = Scene->getObjectClassCount(CLS_MESH);

    if ( rMode != RM_WIREFRAME ) {
      scenePolyCount      = Scene->getPolygonsCount();
      sceneLightCount     = Scene->getObjectClassCount(CLS_LIGHT);
      scenePolyColorBuffer  = new COLORREF[scenePolyCount];
      sceneLightedPolyCount = 0;
    }

    camOutput->GetViewMatrix(cameraMatrix);
    camOutput->GetProjectionMatrix(projectionMatrix);

    viewportMatrix.SetIdentity();
    viewportMatrix._22 = -1.0f;
    viewportMatrix._41 = (FLOAT)clientRect.right / 2;
    viewportMatrix._42 = (FLOAT)clientRect.bottom / 2;

    // Drawing objects
    for ( UINT i = 0; i < sceneObjCount; i++ ) 
    {
      objToRender       = (LPMESH3D)Scene->getObject(CLS_MESH, i);
      objVertCount      = objToRender->getVerticesCount();
      objVertBuffer     = (LPVECTOR3D)HeapAlloc(
                      procHeap, 
                      HEAP_ZERO_MEMORY, 
                      sizeof(VECTOR3D) * objVertCount
                    );

      objToRender->getVerticesTransformed(objVertBuffer);

      // calculate lightning here
      if ( rMode != RM_WIREFRAME ) {
        objPolyBuffer = objToRender->getPolygonsRaw();
        objPolyCount  = objToRender->getPolygonsCount();
        UINT lightTo  = sceneLightedPolyCount + objPolyCount; // number of polygons to light

        for (UINT j = sceneLightedPolyCount; j < lightTo; j++) {
          VECTOR3D normal(objPolyBuffer[j - sceneLightedPolyCount].Normal(objVertBuffer, 2));
          Vector3DNormalize(normal, normal);
          if ( !sceneLightCount ) {
            scenePolyColorBuffer[j] = objToRender->getColor();
            continue;
          }
          else
            scenePolyColorBuffer[j] = 0;

          for (UINT k = 0; k < sceneLightCount; k++)  {
            lightToRender = (LPDIFLIGHT3D)Scene->getObject(CLS_LIGHT, k);
            FLOAT power = lightToRender->getPower();
            COLORREF lightColor = lightToRender->getColor();
            if ( power == 0 || lightColor == BLACK)
              continue;
            FLOAT ratio = Vector3DMultS(normal, lightToRender->getForwardLookDirection());
            if (ratio < -EPSILON)
              ratio = power - ratio;
            else
              if (ratio < EPSILON)
                ratio = max(power / 3.3f, (FLOAT)DARK_SIDE);
              else
                ratio = (FLOAT)DARK_SIDE;

            COLORREF newColor = objToRender->getColor();
            UINT red  = (UINT)(min((RED(newColor) + RED(lightColor))/2, 255)     * ratio);
            UINT green  = (UINT)(min((GREEN(newColor) + GREEN(lightColor))/2, 255) * ratio);
            UINT blue = (UINT)(min((BLUE(newColor) + BLUE(lightColor))/2, 255)   * ratio);

            if (scenePolyColorBuffer[j] != BLACK) {
              newColor = RGB(
                (BYTE)min((red  + RED(scenePolyColorBuffer[j])), 255),
                (BYTE)min((green + GREEN(scenePolyColorBuffer[j])), 255),
                (BYTE)min((blue + BLUE(scenePolyColorBuffer[j])), 255)
              );
            }
            else {
              newColor = RGB( 
                red > 255 ? 255 : red,
                green > 255 ? 255 : green,
                blue > 255  ? 255 : blue 
              );
            }
            scenePolyColorBuffer[j] = newColor;
          }
        }
      }

      // camera projection transformations
      for ( UINT j = 0; j < objVertCount; j++ ) 
      {
        Matrix3DTransformCoord(
            cameraMatrix,
            objVertBuffer[j],
            objVertBuffer[j]
          );

        float z = objVertBuffer[j].z;

        Matrix3DTransformCoord(
                projectionMatrix,
                objVertBuffer[j],
                objVertBuffer[j]
              );
        if ( projectionMatrix._44 < .0f ) {
          objVertBuffer[j] /= objVertBuffer[j].z 
                  + projectionMatrix._34;
          if ( z > camOutput->getFarCP() || z < camOutput->getNearCP() )
            objVertBuffer[j].z = 2;
        }
        else {
          objVertBuffer[j].z /= objVertBuffer[j].z 
                  + projectionMatrix._34;
          if ( z > camOutput->getFarCP() || z < camOutput->getNearCP() )
            objVertBuffer[j].z = 2;
        }

        Matrix3DTransformCoord(
            viewportMatrix,
            objVertBuffer[j],
            objVertBuffer[j]
          );
      }


      if ( rMode != RM_WIREFRAME ) 
      {
        // filling a part of scene buffer
        for (UINT j = 0; j < objPolyCount; j++) {
          VECTOR3D normal(objPolyBuffer[j].Normal(objVertBuffer, 1));
          float cosCam = Vector3DMultS(normal, VECTOR3D(0,0,1)) 
                          / Vector3DLength(normal);
          if (cosCam >= -1 && cosCam < 0) {
            DIRECTPOLY3D tmp;
            tmp.first = objVertBuffer[ objPolyBuffer[j].first ];
            tmp.second  = objVertBuffer[ objPolyBuffer[j].second ];
            tmp.third = objVertBuffer[ objPolyBuffer[j].third ];
            tmp.colorRef = scenePolyColorBuffer[j + sceneLightedPolyCount];
            scenePolyBuffer.push_back(pair<DIRECTPOLY3D,int>(tmp,i));
          }
        }
        sceneLightedPolyCount += objPolyCount;
      }
      else 
      {
        objEdgeCount  = objToRender->getEdgesCount();
        objEdgeBuffer = objToRender->getEdgesRaw();

        hPenCurrent   = CreatePen(PS_SOLID, 1, objToRender->getColor());
        hPenOld     = (HPEN)SelectObject(hMemDC, hPenCurrent);
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

      scenePolyCount  = scenePolyBuffer.size();
      for (UINT i = 0; i < scenePolyCount; i++ ) 
      {
        if ( scenePolyBuffer[i].first.first.z > 0
          && scenePolyBuffer[i].first.first.z < 1
          && scenePolyBuffer[i].first.second.z > 0
          && scenePolyBuffer[i].first.second.z < 1
          && scenePolyBuffer[i].first.third.z > 0
          && scenePolyBuffer[i].first.third.z < 1 

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
          objToRender = (LPMESH3D)Scene->getObject(
                        CLS_MESH, 
                        scenePolyBuffer[i].second
                      );

          hBrCurrent = CreateSolidBrush(scenePolyBuffer[i].first.colorRef);
          if ( rMode == RM_SHADEDWF ) 
            hPenCurrent = CreatePen(PS_SOLID, 1, objToRender->getColor());
          else
            hPenCurrent = CreatePen(PS_SOLID, 1, scenePolyBuffer[i].first.colorRef);

          hPenOld = (HPEN)SelectObject(hMemDC, hPenCurrent);
          hBrOld  = (HBRUSH)SelectObject(hMemDC, hBrCurrent);

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
  VECTOR3D    defCamPos;
  FLOAT     perspCoords = VIEW_DISTANCE_DEFAULT;

  if ( vp != NULL )
  {
    LPTARGCAMERA3D cam = vp->getCamera();
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
        break;

      case VIEW_BOTTOM:

        defCamPos.z = -perspCoords;
        break;

      case VIEW_PERSPECTIVE:
        perspCoords = sqrt((perspCoords * perspCoords) / 3);
        defCamPos.x = perspCoords;
        defCamPos.y = perspCoords;
        defCamPos.z = perspCoords;
        cam->setProjectionType(PT_CENTRAL);
        break;
    }

    cam->Translate(defCamPos);
    cam->TargetTranslate(.0f, .0f, .0f);
  }
}

// ============================================================================
// clsViewportPool Implementation
DWORD WINAPI clsViewportPool::Render(LPVOID renderInfo)
{
  clock_t   time;
  HDC     hDC;
  UINT    vpWidth,
        vpHeight;

  DWORD   dwWaitResult;
  BOOL    bAlive;

  LPTHREAD_DATA vp      = (LPTHREAD_DATA)renderInfo;
  LPTSTR      vpName    = new TCHAR[MAX_OBJECT_NAME_LEN];

  HBRUSH      hBrOld;

  HFONT     hFontOld,
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

  HPEN      hPenOld,
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

    time = clock();

    bAlive = dwWaitResult != WAIT_OBJECT_0 + 1;

    vp->Viewport->Render();

    vp->Viewport->getCamera()->getName(vpName, MAX_OBJECT_NAME_LEN);
    vp->Viewport->getDC(&hDC);
    if ( vp->isActive ) 
    {
      vp->Viewport->getSize(&vpWidth, &vpHeight);
      hPenOld = (HPEN)SelectObject(hDC, hPenCur);
      hBrOld  = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
      Rectangle(hDC, 1, 1, vpWidth - 1, vpHeight - 1);
      SelectObject(hDC, hPenOld);
      SelectObject(hDC, hBrOld);
    }
    hFontOld = (HFONT)SelectObject(hDC, hFontCur); 
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, FRAME_FONT_COLOR);
    TextOut(
      hDC, 
      FRAME_STROKE_WIDTH + 2, 
      FRAME_STROKE_WIDTH, 
      vpName, 
      _tcslen(vpName)
    );
    SelectObject(hDC, hFontOld);
    vp->Viewport->dropDC(&hDC);

    time = CLOCKS_PER_FRAME - clock() + time;
    if ( time > 0 ) Sleep(time);

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
  renderEvent   = CreateEvent(0, TRUE, FALSE, NULL);
}
clsViewportPool::clsViewportPool(LPFORM lpOwner, LPSCENE3D lpScene) 
  : Owner(lpOwner), Scene(NULL)
{
  renderEvent   = CreateEvent(0, TRUE, FALSE, NULL);
  assignScene(lpScene);
}
clsViewportPool::~clsViewportPool() 
{ 
  if ( renderEvent  != NULL ) CloseHandle(renderEvent);
}

BOOL clsViewportPool::assignScene(LPSCENE3D lpScene)
{
  BOOL bResult = lpScene != NULL;
  if ( bResult ) Scene = lpScene;
  return bResult;
}

DWORD clsViewportPool::addViewport(
          INT     vpPosX,
          INT     vpPosY,
          UINT    vpWidth,
          UINT    vpHeight,
          VIEW_TYPE vpVType,
          RENDER_MODE vpRMode
) {
  LPTHREAD_DATA thData    = {NULL};
  DWORD     vpID    = 0,
          dwResult;

  try {
    if ( Scene == NULL || Owner == NULL )     throw E_FAILED;
    if ( renderEvent == NULL )            throw E_EVENT_CREATION_FAILED;
    if ( Viewports.size() == MAX_VIEWPORT_COUNT ) throw E_MAX_COUNT_REACHED; 

    thData        = new THREAD_DATA;
    thData->Viewport  = new VIEWPORT(Scene, DEFAULT_CAMERA_ID, vpRMode);

    thData->Thread  = CreateThread(
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
    thData->threadControls.shutDown = CreateEvent(0, FALSE, FALSE, NULL);
    thData->threadControls.jobDone  = CreateEvent(0, FALSE, FALSE, NULL);
    if (
      thData->threadControls.shutDown   == NULL
      || thData->threadControls.jobDone == NULL
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
    if ( thData->Viewport != NULL ) delete thData->Viewport;
    if ( thData != NULL )     delete thData;
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

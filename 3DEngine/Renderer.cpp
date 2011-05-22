
#include "Renderer.h"

// ============================================================================
// clsRenderPool Implementation
DWORD WINAPI clsRenderPool::Render(LPVOID renderInfo)
{
	LPVIEWPORT	vp			= (LPVIEWPORT)renderInfo;
	BOOL		bAlive		= TRUE;
	DWORD		dwWaitResult;
	HDC			hDCForm;
	HBITMAP		hBmp;
	
	TRIVERTEX			testTriangle[3];
	GRADIENT_TRIANGLE	grTestTriangle;

	do {
		dwWaitResult = WaitForMultipleObjects(
							2,
							(LPEVENT)&vp->thControls,
							FALSE,
							INFINITE
							);
		bAlive = dwWaitResult != WAIT_OBJECT_0 + 1;

		hDCForm = CreateCompatibleDC(*vp->lpHdc);
		hBmp	= CreateCompatibleBitmap(
									*vp->lpHdc,
									vp->Width,
									vp->Height
								);
		SelectObject(hDCForm, hBmp);

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
		// Draw here:
		//((LPFORM)Sender)->MBShow(_T("Before"), NULL, MB_OK);
		GradientFill(
			hDCForm, 
			testTriangle, 
			3, 
			&grTestTriangle,
			1, 
			GRADIENT_FILL_TRIANGLE
		);
		//((LPFORM)Sender)->MBShow(_T("After"), NULL, MB_OK);
		// ================================================================
		BitBlt(
			*vp->lpHdc,
			vp->PosX, 
			vp->PosY,
			vp->Width,
			vp->Height,
			hDCForm,
			0,
			0,
			SRCCOPY
		);
		DeleteObject(hBmp);
		DeleteObject(hDCForm);
		SetEvent(vp->thControls.renderComplete);
		ResetEvent(vp->thControls.doRender);
	} while ( bAlive );

	return SHUTDOWN_ON_DEMAND;
}

UINT clsRenderPool::findViewport(DWORD vpID)
{
	UINT vpCount = Viewports.size();
	for ( UINT i = 0; i < vpCount; i++ )
	{
		if ( Viewports[i]->ID == vpID ) return i;
	}
	return MAX_VIEWPORT_COUNT;
}

clsRenderPool::clsRenderPool() : Scene(NULL), hDC(NULL)
{
	renderSignal = CreateEvent(0, TRUE, FALSE, NULL);
}
clsRenderPool::clsRenderPool(LPSCENE3D lpScene) : hDC(NULL)
{
	renderSignal = CreateEvent(0, TRUE, FALSE, NULL);
	assignScene(lpScene);
}
clsRenderPool::~clsRenderPool() 
{ 
	if (renderSignal != NULL) CloseHandle(renderSignal);
}

BOOL clsRenderPool::assignScene(LPSCENE3D lpScene)
{
	return (Scene = lpScene) != NULL;
}

UINT clsRenderPool::addViewport(
						INT			vpPosX,
						INT			vpPosY,
						UINT		vpWidth,
						UINT		vpHeight,
						UINT		vpCamera,
						RENDER_MODE vpRMode
) {
	LPVIEWPORT	vpInfo	= new VIEWPORT;
	DWORD		vpID	= 0;
	BOOL		bResult;
	
	bResult 
		= Scene										!= NULL
		&& renderSignal									!= NULL
		&& Viewports.size()							!= MAX_VIEWPORT_COUNT
		&& Scene->getObject(CLS_CAMERA, vpCamera)	!= NULL;
	if ( bResult )
	{
		vpInfo->PosX		= vpPosX;
		vpInfo->PosY		= vpPosY;
		vpInfo->Width		= vpWidth;
		vpInfo->Height		= vpHeight;
		vpInfo->lppScene	= &Scene;
		vpInfo->Camera		= vpCamera;
		vpInfo->rMode		= vpRMode;
		vpInfo->lpHdc		= &hDC;
		vpInfo->thOwner		= CreateThread(
									0,
									0,
									Render,
									vpInfo,
									NULL,
									&vpID
								);
		bResult = vpInfo->thOwner != NULL;
		if ( bResult )
		{
			vpInfo->thControls.doRender			= renderSignal;
			vpInfo->thControls.shutDown			= CreateEvent(0, FALSE, FALSE, NULL);
			vpInfo->thControls.renderComplete	= CreateEvent(0, FALSE, FALSE, NULL);
			bResult 
				= vpInfo->thControls.shutDown			!= NULL
				&& vpInfo->thControls.renderComplete	!= NULL;
			if ( bResult )
			{
				//ResumeThread(vpInfo->thOwner);
				vpInfo->ID = vpID;
				vpsRenderComplete.push_back(vpInfo->thControls.renderComplete);
				Viewports.push_back(vpInfo);
			}
		}
		if ( !bResult )	
		{
			vpID	= 0;
			delete vpInfo;
		}
	}
	return vpID;
}

BOOL clsRenderPool::delViewport(UINT vpIndex)
{
	BOOL bResult = vpIndex < Viewports.size();
	if ( bResult ) 
	{
		delete Viewports[vpIndex];
		Viewports.erase(Viewports.begin() + vpIndex);
		vpsRenderComplete.erase(vpsRenderComplete.begin() + vpIndex);
	}
	return bResult;
}

BOOL clsRenderPool::delViewport(DWORD vpID)
{
	return delViewport( findViewport(vpID) );
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

DWORD clsRenderPool::RenderWorld(HDC outputContext)
{
	hDC = outputContext;
	SetEvent(renderSignal);
	return WaitForMultipleObjects(
				vpsRenderComplete.size(),
				vpsRenderComplete.data(),
				TRUE,
				MAX_RENDER_TIMEOUT
			);
}

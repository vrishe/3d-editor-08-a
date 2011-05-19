
#include "Forms.h"

namespace Forms
{
	ATOM ClassRegister(
			LPCTSTR clsName, 
			UINT clsStyle, 
			INT clsXtraBytes, 
			INT frmXtraBytes,
			HICON hIcon,
			HICON hIconSm,
			HCURSOR hCur, 
			HBRUSH bgrColor
	) {
		WNDCLASSEX wndClsEx;
		ZeroMemory(&wndClsEx, sizeof(WNDCLASSEX));
		wndClsEx.cbSize			= sizeof(WNDCLASSEX);
		wndClsEx.style			= clsStyle;
		wndClsEx.lpfnWndProc	= DefWindowProc;
		wndClsEx.cbClsExtra		= clsXtraBytes;
		wndClsEx.cbWndExtra		= frmXtraBytes;
		wndClsEx.hIcon			= hIcon;
		wndClsEx.hIconSm		= hIconSm;
		wndClsEx.hCursor		= hCur;
		wndClsEx.hbrBackground	= bgrColor;
		wndClsEx.lpszClassName	= clsName;

		return RegisterClassEx(&wndClsEx);
	}

// ============================================================================
// clsControl class implementation
// ============================================================================

	// Abusive code here: 
	HWND clsControl::gethWnd()
	{
		return hWnd;
	}

	// ************************

	HINSTANCE clsControl::hInst = NULL;

	LRESULT CALLBACK clsControl::CtrlProc ( 
		HWND hWnd, 
		UINT Event, 
		WPARAM wParam, 
		LPARAM lParam 
	) {
		EVENT_FUNC_MAP::const_iterator finder;
		clsControl *ctrlHandled = (clsForm*)GetWindowLongPtr(hWnd, GWL_USERDATA),
				*ctrlChildHandled;

		UINT	uChildrenCount = ctrlHandled->ChildrenList.size();
		HDWP	hDeferWndStruct;
		LPRECT	rcCtrlRects;
		RECT	rcOffset;

		switch ( Event )
		{
			case WM_NCCALCSIZE:
				if ( (BOOL)wParam )
				{
					rcCtrlRects		= ((LPNCCALCSIZE_PARAMS)lParam)->rgrc;
					if ( ctrlHandled->Parent == NULL )
					{
						rcOffset.left	= rcCtrlRects[1].left 
											- rcCtrlRects[0].left;
						rcOffset.top	= rcCtrlRects[1].top 
											- rcCtrlRects[0].top;
						rcOffset.right	= rcCtrlRects[0].right 
											- rcCtrlRects[1].right;
						rcOffset.bottom	= rcCtrlRects[0].bottom 
											- rcCtrlRects[1].bottom;
						ctrlHandled->Offset = rcOffset;
					}
					else
					{
						rcOffset = ctrlHandled->Parent->Offset;
						if ( (ctrlHandled->Parent->Anchors 
							& ANCR_LEFT) == 0 ) rcOffset.left = 0;
						if ( (ctrlHandled->Parent->Anchors 
							& ANCR_TOP) == 0 ) rcOffset.top = 0;
						if ( (ctrlHandled->Parent->Anchors 
							& ANCR_RIGHT) == 0 ) rcOffset.right = 0;
						if ( (ctrlHandled->Parent->Anchors 
							& ANCR_BOTTOM) == 0 ) rcOffset.bottom = 0;
					}
					
					hDeferWndStruct = BeginDeferWindowPos(uChildrenCount);
					for ( UINT i = 0; 
						i < uChildrenCount
						&& hDeferWndStruct; 
						i++ )
					{
						ctrlChildHandled = ctrlHandled->ChildrenList[i];

						if ( (ctrlChildHandled->Anchors 
								& (ANCR_LEFT | ANCR_RIGHT)) == 0 )
						{
							ctrlChildHandled->Position.x
								+= (rcCtrlRects[0].right - rcCtrlRects[0].left) / 2
								- (rcCtrlRects[1].right - rcCtrlRects[1].left) / 2;
						}
						else
						{
							if ( (ctrlChildHandled->Anchors 
								& ANCR_LEFT) == 0 )
							{
								ctrlChildHandled->Position.x 
									+= rcOffset.left + rcOffset.right;
							}
							else
							{
								if ( (ctrlChildHandled->Anchors 
										& ANCR_RIGHT) != 0 )
								{
									ctrlChildHandled->Width 
										+= rcOffset.left + rcOffset.right;
								}
							}
						}
						if ( (ctrlChildHandled->Anchors 
								& (ANCR_TOP | ANCR_BOTTOM)) == 0 )
						{
							ctrlChildHandled->Position.y
								+= (rcCtrlRects[0].bottom - rcCtrlRects[0].top) / 2
								- (rcCtrlRects[1].bottom - rcCtrlRects[1].top) / 2;
						}
						else
						{
							if ( (ctrlChildHandled->Anchors 
								& ANCR_TOP) == 0 )
							{
								ctrlChildHandled->Position.y 
									+= rcOffset.top + rcOffset.bottom;
							}
							else
							{
								if ( (ctrlChildHandled->Anchors 
										& ANCR_BOTTOM) != 0 )
								{
									ctrlChildHandled->Height 
										+= rcOffset.top + rcOffset.bottom;
								}
							}
						}
								
						hDeferWndStruct = DeferWindowPos(
											hDeferWndStruct,
											ctrlChildHandled->hWnd,
											NULL,
											ctrlChildHandled->Position.x,
											ctrlChildHandled->Position.y,
											ctrlChildHandled->Width,
											ctrlChildHandled->Height,
											SWP_NOZORDER | SWP_NOREDRAW
										);
					}
					if ( hDeferWndStruct != NULL )
						EndDeferWindowPos(hDeferWndStruct);
				}
				break;

			case WM_WINDOWPOSCHANGED:
				if ( ctrlHandled->Parent == NULL )
				{
					ctrlHandled->Position.x = ((LPWINDOWPOS)lParam)->x;
					ctrlHandled->Position.y = ((LPWINDOWPOS)lParam)->y;
					ctrlHandled->Width		= ((LPWINDOWPOS)lParam)->cx;
					ctrlHandled->Height		= ((LPWINDOWPOS)lParam)->cy;
				}
				break;
		}

		finder = ctrlHandled->EventHandlers.find(Event);
		if ( finder == ctrlHandled->EventHandlers.end() )
			return DefWindowProc(hWnd, Event, wParam, lParam);

		return finder->second(ctrlHandled, wParam, lParam);	
	}

	UINT clsControl::findFirstChild(clsControl *lpCtrlChild)
	{
		UINT	vCount	= ChildrenList.size(),
				i		= 0;

		while ( i < vCount )
		{
			if ( ChildrenList[i] == lpCtrlChild )
				return i;
			else
				i++;
		}
		ChildrenList.shrink_to_fit();
		return i;
	}

	BOOL clsControl::removeFirstChildFound(clsControl *lpCtrlChild)
	{
		UINT	vCount	= ChildrenList.size(),
				i		= findFirstChild(lpCtrlChild);

		if ( i != vCount ) 
		{
			ChildrenList.erase(ChildrenList.begin() + i);
			return TRUE;
		}
		return FALSE;
	}

	BOOL clsControl::manageWindowState(INT nCmdShow)
	{
		ShowWindow(hWnd, nCmdShow);
		return UpdateWindow(hWnd);
	}

	clsControl::clsControl( ATOM wCls ) {
		if ( hInst == NULL ) hInst = GetModuleHandle(NULL);
		wndCls = wCls;

		hWnd		= NULL;
		Parent		= NULL;
		Anchors		= NULL;

		Position.x	= 0;
		Position.y	= 0;
		Width		= 0;
		Height		= 0;
	}

	clsControl::~clsControl() { Destroy(); }

	DWORD clsControl::Create(
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			RECT		ctrlDim,
			clsControl*	ctrlParent
	) {
		RECT rcRealDimentions;
		if ( hInst == NULL )	return E_FAIL;
		if ( wndCls == NULL )	return E_DOES_NOT_EXIST;
		if ( hWnd != NULL )		return S_ALREADY_EXISTS;
		if ( ctrlParent			!= NULL
			&& ctrlParent->hWnd	== NULL	) 
				return E_BAD_ARGUMENTS;

		hWnd = CreateWindowEx(
					ctrlStyleEx, 
					(LPCTSTR)wndCls, 
					NULL, 
					ctrlStyle, 
					ctrlDim.left, 
					ctrlDim.top, 
					ctrlDim.right - ctrlDim.left, 
					ctrlDim.bottom - ctrlDim.top, 
					ctrlParent != NULL ? ctrlParent->hWnd : NULL, 
					NULL, 
					hInst, 
					NULL 
				); 
		if ( hWnd == NULL ) return E_FAIL;
	// Setting all necessary data of controling class
		Parent = ctrlParent;
		Anchors = ANCR_TOP | ANCR_LEFT;
		if ( Parent != NULL ) Parent->ChildrenList.push_back(this);
		getBoundaries(&rcRealDimentions);
		Position.x	= rcRealDimentions.left;
		Position.y	= rcRealDimentions.top;
		Width		= rcRealDimentions.right 
						- rcRealDimentions.left;
		Height		= rcRealDimentions.bottom 
						- rcRealDimentions.top;
	// Sublassing process part:
		SetLastError(0);
		SetWindowLongPtr(hWnd, GWL_WNDPROC, PtrToLong(CtrlProc));
		SetWindowLongPtr(hWnd, GWL_USERDATA, PtrToLong(this));
		if ( GetLastError() != 0 ) 	
		{
			Destroy();
			return E_SUBCLASSING_FAILED;
		}
		return S_OK;
	}

	DWORD clsControl::Create(
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			POINT		ctrlPos,
			UINT		ctrlWidth,
			UINT		ctrlHeight,
			clsControl*	ctrlParent
	) {
		RECT ctrlDim;
		return SetRect(
				&ctrlDim, 
				ctrlPos.x, 
				ctrlPos.y, 
				ctrlPos.x + ctrlWidth, 
				ctrlPos.y + ctrlHeight
			)
				&& Create(
						ctrlStyle,
						ctrlStyleEx,
						ctrlDim,
						ctrlParent
					);
	}

	DWORD clsControl::Create(
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			INT			ctrlPosX,
			INT			ctrlPosY,
			UINT		ctrlWidth,
			UINT		ctrlHeight,
			clsControl*	ctrlParent
	) {
		RECT ctrlDim;
		return SetRect(
				&ctrlDim, 
				ctrlPosX, 
				ctrlPosY, 
				ctrlPosX + ctrlWidth, 
				ctrlPosY + ctrlHeight
			)
				&& Create(
						ctrlStyle,
						ctrlStyleEx,
						ctrlDim,
						ctrlParent
					);
	}

	DWORD clsControl::Destroy()
	{
		if ( hWnd == NULL ) return E_DOES_NOT_EXIST;
		while ( ChildrenList.size() != 0 )
		{
			ChildrenList[0]->Destroy();
			ChildrenList.erase(ChildrenList.begin());
		}
		DestroyWindow(hWnd);
		EventHandlers.clear();

		hWnd		= NULL;
		Parent		= NULL;
		Anchors		= NULL;

		Position.x	= 0;
		Position.y	= 0;
		Width		= 0;
		Height		= 0;
		return S_OK;
	}

	DWORD clsControl::AssignEventHandler( UINT Event, EVENT_FUNC Handler, BOOL Replace )
	{
		EVENT_FUNC_MAP::iterator finder = EventHandlers.find(Event); 
		ELEMENT NewEventHandler;
		if ( Handler == NULL 
			&& !Replace ) return E_FAIL;
		if ( finder != EventHandlers.end() ) 
		{
			if ( Replace )
			{
				finder->second = Handler;
				return S_REPLACED;
			}
			return S_ALREADY_EXISTS;
		}

		NewEventHandler.first	= Event;
		NewEventHandler.second	= Handler;
		EventHandlers.insert ( NewEventHandler );
		return S_ASSIGNED;

		return S_OK;
	}

	VOID clsControl::ResetEventHandlers() { EventHandlers.clear(); }

	BOOL clsControl::Show(BOOL bRecursive)		
	{
		BOOL bResult = manageWindowState(SW_SHOWNORMAL); ;
		for ( UINT i = 0; 
			i < ChildrenList.size() 
				&& bRecursive 
				&& bResult; 
			i++ )
				bResult = ChildrenList[i]->Show(bRecursive);
		return bResult;
	}

	BOOL clsControl::Hide()		{ return manageWindowState(SW_HIDE); }

	BOOL clsControl::Enable()		{ return EnableWindow(hWnd, TRUE); }

	BOOL clsControl::Disable()		{ return EnableWindow(hWnd, FALSE); }

	BOOL clsControl::MoveTo(POINT ptDest) 
	{ 
		BOOL bResult = SetWindowPos(
							hWnd, 
							NULL,
							ptDest.x,
							ptDest.y,
							0,
							0,
							SWP_NOSIZE | SWP_NOZORDER
						);
		if ( bResult ) Position = ptDest;
		return bResult;
	}

	BOOL clsControl::MoveTo(INT ptDestX, INT ptDestY)
	{
		POINT ptDest;
		ptDest.x = ptDestX;
		ptDest.y = ptDestY;
		return MoveTo(ptDest);
	}

	BOOL clsControl::SizeTo(UINT cWidth, UINT cHeight)
	{
		BOOL bResult = SetWindowPos(
							hWnd, 
							NULL,
							0,
							0,
							cWidth,
							cHeight,
							SWP_NOMOVE | SWP_NOZORDER
						);
		if ( bResult )
		{
			Width	= cWidth;
			Height	= cHeight;
		}
		return bResult;
	}

	BOOL clsControl::MoveSizeTo(RECT ctrlSizePos)
	{
		INT		ctrlPosX	= ctrlSizePos.left < ctrlSizePos.right ? 
									ctrlSizePos.left : ctrlSizePos.right,
				ctrlPosY	= ctrlSizePos.top < ctrlSizePos.bottom ? 
									ctrlSizePos.top : ctrlSizePos.bottom;
		UINT	ctrlWidth	= abs(ctrlSizePos.right - ctrlSizePos.left),
				ctrlHeight	= abs(ctrlSizePos.bottom - ctrlSizePos.top);
		return SetWindowPos(
					hWnd, 
					NULL,
					ctrlPosX,
					ctrlPosY,
					ctrlWidth,
					ctrlHeight,
					SWP_NOZORDER
				); 
	}

	BOOL clsControl::MoveSizeTo(POINT ptDest, UINT cWidth, UINT cHeight)
	{
		RECT ctrlSizePos;

		return SetRect(
				&ctrlSizePos, 
				ptDest.x, 
				ptDest.y, 
				ptDest.x + cWidth, 
				ptDest.y + cHeight
			)
			&& MoveSizeTo(ctrlSizePos);
	}

	BOOL clsControl::MoveSizeTo(INT ptDestX, INT ptDestY, UINT cWidth, UINT cHeight)
	{
		RECT ctrlSizePos;

		return SetRect(
				&ctrlSizePos, 
				ptDestX, 
				ptDestY, 
				ptDestX + cWidth, 
				ptDestY + cHeight
			)
			&& MoveSizeTo(ctrlSizePos);
	}

	BOOL clsControl::setText(LPCTSTR ctrlText)
	{
		return SetWindowText(hWnd, ctrlText);
	}

	DWORD clsControl::setParent(clsControl *lpCtrlsParent)
	{
		DWORD ctrlStyle = 0;
		HWND hWndParent = NULL;

		if ( hWnd == NULL ) return E_DOES_NOT_EXIST;
		if (	lpCtrlsParent		!= NULL 
			&&	lpCtrlsParent->hWnd == NULL ) 
				return E_BAD_ARGUMENTS;

		if (	lpCtrlsParent		!= NULL 
			&&	lpCtrlsParent->hWnd != NULL )
				hWndParent = lpCtrlsParent->hWnd;

		if ( Parent == lpCtrlsParent ) return S_ALREADY_EXISTS;

		if ( Parent != NULL ) 
			Parent->removeFirstChildFound(this);

		if ( hWndParent != NULL )
		{
			ctrlStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
			ctrlStyle = ctrlStyle & ~WS_POPUP | WS_CHILD;
			SetWindowLongPtr(hWnd, GWL_STYLE, ctrlStyle);
		}
		if ( SetParent(hWnd, hWndParent) == NULL ) return E_FAIL;
		if ( hWndParent == NULL )
		{
			ctrlStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
			ctrlStyle = ctrlStyle & ~WS_CHILD | WS_POPUP;
			SetWindowLongPtr(hWnd, GWL_STYLE, ctrlStyle);			
		}
		SetWindowPos(
				hWnd, 
				NULL, 
				0, 0, 0, 0,  
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW
			);
		Parent = lpCtrlsParent;
		Parent->ChildrenList.push_back(this);
		Parent->ChildrenList.shrink_to_fit();
				
		return S_OK;
	}

	VOID clsControl::setAnchors(BYTE ctrlAnchors) { Anchors = ctrlAnchors; } 


	clsControl *clsControl::getParent() { return Parent; }

	UINT clsControl::getText(LPTSTR ctrlText, UINT nLengthToCopy)
	{
		return GetWindowText(hWnd, ctrlText, nLengthToCopy);
	}

	BYTE clsControl::getAnchors() { return Anchors; }

	VOID clsControl::getBoundaries(LPRECT rcDims)
	{
		if ( rcDims != NULL )
		{
			SetRectEmpty(rcDims);
			GetWindowRect(hWnd, rcDims);
			if ( Parent != NULL )
				MapWindowPoints(NULL, Parent->hWnd, (LPPOINT)rcDims, 2);
		}
	}

	VOID clsControl::getPos(LPINT pWhereX, LPINT pWhereY)
	{
		RECT rcDims;
		getBoundaries(&rcDims);	
		if ( pWhereX != NULL ) { *pWhereX = rcDims.left; }
		if ( pWhereY != NULL ) { *pWhereY = rcDims.top; }
	}

	VOID clsControl::getPos(LPPOINT ptWhere)
	{
		RECT rcDims;
		getBoundaries(&rcDims);	
		if ( ptWhere != NULL )
		{
			ptWhere->x = rcDims.left;
			ptWhere->y = rcDims.right;
		}
	}

	VOID clsControl::getSize(LPUINT pWidth, LPUINT pHeight)
	{
		RECT rcDims;
		getBoundaries(&rcDims);	

		if ( pWidth != NULL ) { *pWidth = rcDims.right - rcDims.left; }
		if ( pHeight != NULL ) { *pHeight = rcDims.bottom - rcDims.top; }
	}

	UINT clsControl::getWidth()
	{
		UINT cWidth;
		getSize(&cWidth, NULL);
		return cWidth;
	}

	UINT clsControl::getHeight()
	{
		UINT cHeight;
		getSize(NULL, &cHeight);
		return cHeight;
	}

	INT clsControl::getWidthMnemonic() { return Width; }

	INT clsControl::getHeightMnemonic() { return Height; }

	BOOL clsControl::isVisible()	{ return IsWindowVisible(hWnd); }

	BOOL clsControl::isEnabled()	{ return IsWindowEnabled(hWnd); }

	BOOL clsControl::isActive()	{ return GetActiveWindow() == hWnd; }

	BOOL clsControl::isParent(clsControl *lpCtrlChild) 
	{ 
		return ChildrenList.size() != findFirstChild(lpCtrlChild); 
	} 

	BOOL clsControl::isChild(clsControl *lpCtrlParent) { return Parent == lpCtrlParent; }

// ============================================================================
// clsForm class implementation
// ============================================================================

	clsForm::clsForm(
			LPCTSTR clsName,
			HBRUSH bgrColor,
			HICON hIcon,
			HICON hIconSm,
			HCURSOR hCur,
			DWORD clsStyle,
			INT clsXtraBytes,
			INT frmXtraBytes
	) : clsControl( ClassRegister(
							clsName,
							clsStyle,
							clsXtraBytes,
							frmXtraBytes,
							hIcon,
							hIconSm,
							hCur,
							bgrColor 
						)
	) { frmClsAutoUnreg = TRUE; }

	clsForm::clsForm(ATOM frmCls) : clsControl(frmCls) { frmClsAutoUnreg = FALSE; }

	clsForm::~clsForm() 
	{	
		if ( frmClsAutoUnreg ) UnregisterClass((LPCTSTR)wndCls, hInst); 
	}

	DWORD clsForm::Create(
				FORM_TYPE	frmType,
				DWORD		frmStyleEx,
				RECT		frmDim,
				clsForm*		frmParent,
				HMENU		frmMenu
	) {
		DWORD dwResult = clsControl::Create(
									frmType,
									frmStyleEx,
									frmDim,
									frmParent
								);	
		if ( SUCCEEDED(dwResult) ) setMenu(frmMenu);
		return dwResult;
	}

	DWORD clsForm::Create(
				FORM_TYPE	frmType,
				DWORD		frmStyleEx,
				POINT		frmPos,
				UINT		frmWidth,
				UINT		frmHeight,
				clsForm*		frmParent,
				HMENU		frmMenu
	) {
		RECT frmDim;
		return SetRect(
				&frmDim, 
				frmPos.x, 
				frmPos.y, 
				frmPos.x + frmWidth, 
				frmPos.y + frmHeight
			)
				&& Create(
					frmType,
					frmStyleEx,
					frmDim,
					frmParent,
					frmMenu
				);		
	}

	DWORD clsForm::Create(
				FORM_TYPE	frmType,
				DWORD		frmStyleEx,
				INT			frmPosX,
				INT			frmPosY,
				UINT		frmWidth,
				UINT		frmHeight,
				clsForm*		frmParent,
				HMENU		frmMenu
	) {
		RECT frmDim;
		return SetRect(
				&frmDim, 
				frmPosX, 
				frmPosY, 
				frmPosX + frmWidth, 
				frmPosY + frmHeight
			)
				&& Create(
					frmType,
					frmStyleEx,
					frmDim,
					frmParent,
					frmMenu
				);	
	}

	BOOL clsForm::Validate(LPRECT pValidRect) 
	{ 
		return ValidateRect(hWnd, pValidRect); 
	}

	BOOL clsForm::Invalidate(LPRECT pInvalidRect, BOOL bErase) 
	{ 
		return InvalidateRect(hWnd, pInvalidRect, bErase); 
	} 

	BOOL clsForm::Maximize() { return ShowWindow(hWnd, SW_SHOWMAXIMIZED); }

	BOOL clsForm::Minimize() { return ShowWindow(hWnd, SW_SHOWMINIMIZED); }

	BOOL clsForm::Restore()  { return ShowWindow(hWnd, SW_SHOWNORMAL); }

	INT_PTR clsForm::DBShow(LPCTSTR dbTemplate, DLGPROC dbProcedure)
	{
		return DialogBox(hInst, dbTemplate, hWnd, dbProcedure);
	}

	INT_PTR clsForm::MBShow(LPCTSTR mbText, LPCTSTR mbCaption, UINT mbType)
	{
		return MessageBox(hWnd, mbText, mbCaption, mbType);
	}

	BOOL clsForm::setMenu(HMENU frmMenu)
	{
		DWORD wndStyle;
		HMENU hMenuPrev;
		wndStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		if ( (wndStyle & WS_CHILD) != 0 ) return FALSE;
		hMenuPrev = GetMenu(hWnd);
		if ( !SetMenu(hWnd, frmMenu) ) return FALSE;
		if ( hMenuPrev != NULL 
			&& !DestroyMenu(hMenuPrev) ) return FALSE;
		return DrawMenuBar(hWnd);
	}

	//HMENU clsForm::getMenu() { return GetWindowLongPtr(...) }
	VOID clsForm::getFormClientSize(LPUINT fcWidth, LPUINT fcHeight)
	{
		RECT clientRect;
		BOOL bResult = GetClientRect(hWnd, &clientRect);
		if ( fcWidth != NULL ) *fcWidth = bResult ? clientRect.right : 0U;
		if ( fcHeight != NULL) *fcHeight = bResult ? clientRect.bottom : 0U;
	}

	BOOL clsForm::isMaximized() { return IsZoomed(hWnd); }

	BOOL clsForm::isMinimized() { return IsIconic(hWnd); }

	BOOL clsForm::isNormal() { return !(isMaximized() || isMinimized()); }
};


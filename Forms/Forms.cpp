
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
// Control class implementation
// ============================================================================

	// Abusive code here: 
	HWND Control::gethWnd()
	{
		return hWnd;
	}

	// ************************

	HINSTANCE Control::hInst = NULL;

	LRESULT CALLBACK Control::CtrlProc ( 
		HWND hWnd, 
		UINT Event, 
		WPARAM wParam, 
		LPARAM lParam 
	) {
		EVENT_FUNC_MAP::const_iterator finder;
		Control *ctrlHandled = (Form *)GetWindowLongPtr(hWnd, GWL_USERDATA),
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

	UINT Control::findFirstChild(Control *lpCtrlChild)
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

	BOOL Control::removeFirstChildFound(Control *lpCtrlChild)
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

	BOOL Control::manageWindowState(INT nCmdShow)
	{
		ShowWindow(hWnd, nCmdShow);
		return UpdateWindow(hWnd);
	}

	Control::Control( ATOM wCls ) {
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

	Control::~Control() { Destroy(); }

	DWORD Control::Create(
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			RECT		ctrlDim,
			Control*	ctrlParent
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

	DWORD Control::Create(
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			POINT		ctrlPos,
			UINT		ctrlWidth,
			UINT		ctrlHeight,
			Control*	ctrlParent
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

	DWORD Control::Create(
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			INT			ctrlPosX,
			INT			ctrlPosY,
			UINT		ctrlWidth,
			UINT		ctrlHeight,
			Control*	ctrlParent
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

	DWORD Control::Destroy()
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

	DWORD Control::AssignEventHandler( UINT Event, EVENT_FUNC Handler, BOOL Replace )
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

	VOID Control::ResetEventHandlers() { EventHandlers.clear(); }

	BOOL Control::Show(BOOL bRecursive)		
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

	BOOL Control::Hide()		{ return manageWindowState(SW_HIDE); }

	BOOL Control::Enable()		{ return EnableWindow(hWnd, TRUE); }

	BOOL Control::Disable()		{ return EnableWindow(hWnd, FALSE); }

	BOOL Control::MoveTo(POINT ptDest) 
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

	BOOL Control::MoveTo(INT ptDestX, INT ptDestY)
	{
		POINT ptDest;
		ptDest.x = ptDestX;
		ptDest.y = ptDestY;
		return MoveTo(ptDest);
	}

	BOOL Control::SizeTo(UINT cWidth, UINT cHeight)
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

	BOOL Control::MoveSizeTo(RECT ctrlSizePos)
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

	BOOL Control::MoveSizeTo(POINT ptDest, UINT cWidth, UINT cHeight)
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

	BOOL Control::MoveSizeTo(INT ptDestX, INT ptDestY, UINT cWidth, UINT cHeight)
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

	BOOL Control::setText(LPCTSTR ctrlText)
	{
		return SetWindowText(hWnd, ctrlText);
	}

	DWORD Control::setParent(Control *lpCtrlsParent)
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

	VOID Control::setAnchors(BYTE ctrlAnchors) { Anchors = ctrlAnchors; } 


	Control *Control::getParent() { return Parent; }

	UINT Control::getText(LPTSTR ctrlText, UINT nLengthToCopy)
	{
		return GetWindowText(hWnd, ctrlText, nLengthToCopy);
	}

	BYTE Control::getAnchors() { return Anchors; }

	VOID Control::getBoundaries(LPRECT rcDims)
	{
		if ( rcDims != NULL )
		{
			SetRectEmpty(rcDims);
			GetWindowRect(hWnd, rcDims);
			if ( Parent != NULL )
				MapWindowPoints(NULL, Parent->hWnd, (LPPOINT)rcDims, 2);
		}
	}

	VOID Control::getPos(LPINT pWhereX, LPINT pWhereY)
	{
		RECT rcDims;
		getBoundaries(&rcDims);	
		if ( pWhereX != NULL ) { *pWhereX = rcDims.left; }
		if ( pWhereY != NULL ) { *pWhereY = rcDims.top; }
	}

	VOID Control::getPos(LPPOINT ptWhere)
	{
		RECT rcDims;
		getBoundaries(&rcDims);	
		if ( ptWhere != NULL )
		{
			ptWhere->x = rcDims.left;
			ptWhere->y = rcDims.right;
		}
	}

	VOID Control::getSize(LPUINT pWidth, LPUINT pHeight)
	{
		RECT rcDims;
		getBoundaries(&rcDims);	

		if ( pWidth != NULL ) { *pWidth = rcDims.right - rcDims.left; }
		if ( pHeight != NULL ) { *pHeight = rcDims.bottom - rcDims.top; }
	}

	UINT Control::getWidth()
	{
		UINT cWidth;
		getSize(&cWidth, NULL);
		return cWidth;
	}

	UINT Control::getHeight()
	{
		UINT cHeight;
		getSize(NULL, &cHeight);
		return cHeight;
	}

	INT Control::getWidthMnemonic() { return Width; }

	INT Control::getHeightMnemonic() { return Height; }

	BOOL Control::isVisible()	{ return IsWindowVisible(hWnd); }

	BOOL Control::isEnabled()	{ return IsWindowEnabled(hWnd); }

	BOOL Control::isActive()	{ return GetActiveWindow() == hWnd; }

	BOOL Control::isParent(Control *lpCtrlChild) 
	{ 
		return ChildrenList.size() != findFirstChild(lpCtrlChild); 
	} 

	BOOL Control::isChild(Control *lpCtrlParent) { return Parent == lpCtrlParent; }

// ============================================================================
// Form class implementation
// ============================================================================

	Form::Form(
			LPCTSTR clsName,
			HBRUSH bgrColor,
			HICON hIcon,
			HICON hIconSm,
			HCURSOR hCur,
			DWORD clsStyle,
			INT clsXtraBytes,
			INT frmXtraBytes
	) : Control( ClassRegister(
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

	Form::Form(ATOM frmCls) : Control(frmCls) { frmClsAutoUnreg = FALSE; }

	Form::~Form() 
	{	
		if ( frmClsAutoUnreg ) UnregisterClass((LPCTSTR)wndCls, hInst); 
	}

	DWORD Form::Create(
				FORM_TYPE	frmType,
				DWORD		frmStyleEx,
				RECT		frmDim,
				Form*		frmParent,
				HMENU		frmMenu
	) {
		DWORD dwResult = Control::Create(
									frmType,
									frmStyleEx,
									frmDim,
									frmParent
								);	
		if ( SUCCEEDED(dwResult) ) setMenu(frmMenu);
		return dwResult;
	}

	DWORD Form::Create(
				FORM_TYPE	frmType,
				DWORD		frmStyleEx,
				POINT		frmPos,
				UINT		frmWidth,
				UINT		frmHeight,
				Form*		frmParent,
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

	DWORD Form::Create(
				FORM_TYPE	frmType,
				DWORD		frmStyleEx,
				INT			frmPosX,
				INT			frmPosY,
				UINT		frmWidth,
				UINT		frmHeight,
				Form*		frmParent,
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

	BOOL Form::Validate(LPRECT pValidRect) 
	{ 
		return ValidateRect(hWnd, pValidRect); 
	}

	BOOL Form::Invalidate(LPRECT pInvalidRect, BOOL bErase) 
	{ 
		return InvalidateRect(hWnd, pInvalidRect, bErase); 
	} 

	BOOL Form::Maximize() { return ShowWindow(hWnd, SW_SHOWMAXIMIZED); }

	BOOL Form::Minimize() { return ShowWindow(hWnd, SW_SHOWMINIMIZED); }

	BOOL Form::Restore()  { return ShowWindow(hWnd, SW_SHOWNORMAL); }

	INT_PTR Form::DBShow(LPCTSTR dbTemplate, DLGPROC dbProcedure)
	{
		return DialogBox(hInst, dbTemplate, hWnd, dbProcedure);
	}

	INT_PTR Form::MBShow(LPCTSTR mbText, LPCTSTR mbCaption, UINT mbType)
	{
		return MessageBox(hWnd, mbText, mbCaption, mbType);
	}

	BOOL Form::setMenu(HMENU frmMenu)
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

	//HMENU Form::getMenu() { return GetWindowLongPtr(...) }
	VOID Form::getFormClientSize(LPUINT fcWidth, LPUINT fcHeight)
	{
		RECT clientRect;
		BOOL bResult = GetClientRect(hWnd, &clientRect);
		if ( fcWidth != NULL ) *fcWidth = bResult ? clientRect.right : 0U;
		if ( fcHeight != NULL) *fcHeight = bResult ? clientRect.bottom : 0U;
	}

	BOOL Form::isMaximized() { return IsZoomed(hWnd); }

	BOOL Form::isMinimized() { return IsIconic(hWnd); }

	BOOL Form::isNormal() { return !(isMaximized() || isMinimized()); }
};


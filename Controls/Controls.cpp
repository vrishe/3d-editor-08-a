
#include "Controls.h"

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

BOOL SetWindowProps(HWND hWnd, INT index, LONG lProp, LPLONG lPropOld)
{
	BOOL bResult;
	DWORD prevProp;
	SetLastError(0);
	bResult = (prevProp = SetWindowLongPtr(hWnd, index, lProp))	!= 0
			|| GetLastError()									== 0;
	if ( bResult ) 
	{
		if ( IsWindowVisible(hWnd) )
			bResult = SetWindowPos(
							hWnd, 
							NULL, 
							0, 0, 0, 0, 
							SWP_NOMOVE 
							| SWP_NOSIZE 
							| SWP_NOZORDER 
							| SWP_FRAMECHANGED
						);
		if ( lPropOld != NULL ) *lPropOld = prevProp;
	}
	return bResult;
}

// ============================================================================
// clsWinBase class implementation
// ============================================================================
HINSTANCE clsWinBase::hInst = GetModuleHandle(NULL);

LRESULT CALLBACK clsWinBase::CtrlProc ( 
	HWND hWnd, 
	UINT Event, 
	WPARAM wParam, 
	LPARAM lParam 
) {
	EVENT_FUNC_MAP::const_iterator finder;
	clsWinBase *wbHandled = (clsForm*)GetWindowLongPtr(hWnd, GWL_USERDATA),
			*wbChildHandled;

	UINT	uChildrenCount = wbHandled->ChildrenList.size();
	HDWP	hDeferWndStruct;
	LPRECT	rcCtrlRects;
	RECT	rcOffset;

	switch ( Event )
	{
		case WM_NCCALCSIZE:
			if ( (BOOL)wParam )
			{
				rcCtrlRects		= ((LPNCCALCSIZE_PARAMS)lParam)->rgrc;
				if ( wbHandled->Parent == NULL )
				{
					rcOffset.left	= rcCtrlRects[1].left 
										- rcCtrlRects[0].left;
					rcOffset.top	= rcCtrlRects[1].top 
										- rcCtrlRects[0].top;
					rcOffset.right	= rcCtrlRects[0].right 
										- rcCtrlRects[1].right;
					rcOffset.bottom	= rcCtrlRects[0].bottom 
										- rcCtrlRects[1].bottom;
					wbHandled->Offset = rcOffset;
				}
				else
				{
					rcOffset = wbHandled->Parent->Offset;
					if ( (wbHandled->Parent->Anchors 
						& ANCR_LEFT) == 0 ) rcOffset.left = 0;
					if ( (wbHandled->Parent->Anchors 
						& ANCR_TOP) == 0 ) rcOffset.top = 0;
					if ( (wbHandled->Parent->Anchors 
						& ANCR_RIGHT) == 0 ) rcOffset.right = 0;
					if ( (wbHandled->Parent->Anchors 
						& ANCR_BOTTOM) == 0 ) rcOffset.bottom = 0;
				}
					
				hDeferWndStruct = BeginDeferWindowPos(uChildrenCount);
				for ( UINT i = 0; 
					i < uChildrenCount
					&& hDeferWndStruct; 
					i++ )
				{
					wbChildHandled = wbHandled->ChildrenList[i];

					if ( (wbChildHandled->Anchors 
							& (ANCR_LEFT | ANCR_RIGHT)) == 0 )
					{
						wbChildHandled->Position.x
							+= (rcCtrlRects[0].right - rcCtrlRects[0].left) / 2
							- (rcCtrlRects[1].right - rcCtrlRects[1].left) / 2;
					}
					else
					{
						if ( (wbChildHandled->Anchors 
							& ANCR_LEFT) == 0 )
						{
							wbChildHandled->Position.x 
								+= rcOffset.left + rcOffset.right;
						}
						else
						{
							if ( (wbChildHandled->Anchors 
									& ANCR_RIGHT) != 0 )
							{
								wbChildHandled->Width 
									+= rcOffset.left + rcOffset.right;
							}
						}
					}
					if ( (wbChildHandled->Anchors 
							& (ANCR_TOP | ANCR_BOTTOM)) == 0 )
					{
						wbChildHandled->Position.y
							+= (rcCtrlRects[0].bottom - rcCtrlRects[0].top) / 2
							- (rcCtrlRects[1].bottom - rcCtrlRects[1].top) / 2;
					}
					else
					{
						if ( (wbChildHandled->Anchors 
							& ANCR_TOP) == 0 )
						{
							wbChildHandled->Position.y 
								+= rcOffset.top + rcOffset.bottom;
						}
						else
						{
							if ( (wbChildHandled->Anchors 
									& ANCR_BOTTOM) != 0 )
							{
								wbChildHandled->Height 
									+= rcOffset.top + rcOffset.bottom;
							}
						}
					}
								
					hDeferWndStruct = DeferWindowPos(
										hDeferWndStruct,
										wbChildHandled->hWnd,
										NULL,
										wbChildHandled->Position.x,
										wbChildHandled->Position.y,
										wbChildHandled->Width,
										wbChildHandled->Height,
										SWP_NOZORDER | SWP_NOREDRAW
									);
				}
				if ( hDeferWndStruct != NULL )
					EndDeferWindowPos(hDeferWndStruct);
			}
			break;

		case WM_WINDOWPOSCHANGED:
			if ( wbHandled->Parent == NULL )
			{
				wbHandled->Position.x = ((LPWINDOWPOS)lParam)->x;
				wbHandled->Position.y = ((LPWINDOWPOS)lParam)->y;
				wbHandled->Width		= ((LPWINDOWPOS)lParam)->cx;
				wbHandled->Height		= ((LPWINDOWPOS)lParam)->cy;
			}
			break;

		case WM_DESTROY:
			wbHandled->hWnd = NULL;
			break;
	}

	finder = wbHandled->EventHandlers.find(Event);
	if ( finder == wbHandled->EventHandlers.end() )
		return wbHandled->defCtrlProc(hWnd, Event, wParam, lParam);

	return finder->second(wbHandled, wParam, lParam);	
}

VOID clsWinBase::restoreControlFocus()
{
	UINT cCount = ChildrenList.size(),
		 cIndex = findFirstChildIndex(1);
	if ( cIndex != cCount ) ChildrenList[cIndex]->setFocus();
}

VOID clsWinBase::tabOrderRecalculate()
{
	UINT tabOrderMax			= getChildrenTabOrderMax();
	clsWinBase	*childCurrent	= NULL,
				*childNext		= NULL;

	for ( UINT i = 1, j; i < tabOrderMax;)
	{
		j = 0;
		do
			childNext = getChildByTabOrder(i+(++j));
		while ( j <= tabOrderMax && childNext == NULL);

		if ( childNext != NULL )
		{
			childCurrent	= getChildByTabOrder(i);
			if ( childCurrent == NULL )
			{
				childNext->tabOrder = i;
			}
			else
			{
				childNext->tabOrder = childCurrent->tabOrder + 1;
				SetWindowPos(
						childNext->hWnd,
						childCurrent->hWnd,
						0, 0, 0, 0,
						SWP_NOSIZE | SWP_NOMOVE
					);
				i++;
			}
		}
		else
		{
			i+=j;
		}
	}
}

BOOL clsWinBase::removeFirstChildFound(clsWinBase *lpCtrlChild)
{
	UINT	cCount	= ChildrenList.size(),
			i		= findFirstChildIndex(lpCtrlChild);

	if ( i != cCount ) 
	{
		ChildrenList.erase(ChildrenList.begin() + i);
		return TRUE;
	}
	return FALSE;
}

BOOL clsWinBase::manageWindowState(INT nCmdShow)
{
	ShowWindow(hWnd, nCmdShow);
	return UpdateWindow(hWnd);
}

clsWinBase::clsWinBase() {
	hWnd		= NULL;

	Parent		= NULL;
	Anchors		= NULL;

	Position.x	= 0;
	Position.y	= 0;
	Width		= 0;
	Height		= 0;
	SetRectEmpty(&Offset);
}

clsWinBase::~clsWinBase() { Destroy(); }

DWORD clsWinBase::Create(
		LPCTSTR		wbClsName,
		DWORD		wbStyle,
		DWORD		wbStyleEx,
		INT			wbPosX,
		INT			wbPosY,
		UINT		wbWidth,
		UINT		wbHeight,
		clsWinBase*	wbParent
) {
	RECT rcRealDimentions;
	WNDCLASSEX wndClsFound;
	if ( hWnd != NULL )		return S_ALREADY_EXISTS;
	if ( hInst == NULL )	return E_HINSTANCE_IS_NULL;
	if ( !(GetClassInfoEx(hInst, wbClsName, &wndClsFound)
			|| GetClassInfoEx(NULL, wbClsName, &wndClsFound)) )
							return E_CLASS_NOT_EXIST;
	if ( wbParent			!= NULL
		&& wbParent->hWnd	== NULL	) 
							return E_BAD_ARGUMENTS;
	
	hWnd = CreateWindowEx(
				wbStyleEx, 
				wbClsName, 
				NULL, 
				wbStyle, 
				wbPosX, 
				wbPosY, 
				wbWidth, 
				wbHeight, 
				wbParent != NULL ? wbParent->hWnd : NULL, 
				NULL, 
				hInst, 
				NULL 
			); 
	if ( hWnd == NULL ) return E_HWND_CREATION_FAILED;
// Setting all necessary data of controling class
	Parent		= wbParent;
	Anchors		= ANCR_TOP | ANCR_LEFT;
	tabOrder	= 0;
	if ( Parent != NULL ) 
	{
		Parent->ChildrenList.push_back(this);
		if ( (wbStyle & WS_TABSTOP) != 0 )
			tabOrder = Parent->getChildrenTabOrderMax() + 1;
	}
	getBoundaries(&rcRealDimentions);
	Position.x	= rcRealDimentions.left;
	Position.y	= rcRealDimentions.top;
	Width		= rcRealDimentions.right 
					- rcRealDimentions.left;
	Height		= rcRealDimentions.bottom 
					- rcRealDimentions.top;
// Sublassing process part:
	if (
		!SetWindowProps(hWnd, GWL_WNDPROC, PtrToLong(CtrlProc), (LPLONG)&defCtrlProc)
	||	!SetWindowProps(hWnd, GWL_USERDATA, PtrToLong(this))								
	) {
		Destroy();
		return E_SUBCLASSING_FAILED;
	}
	return S_DONE;
}

DWORD clsWinBase::Create(
		LPCTSTR		wbClsName,
		DWORD		wbStyle,
		DWORD		wbStyleEx,
		RECT		wbDim,
		clsWinBase*	wbParent
) {
	return Create(
				wbClsName,
				wbStyle,
				wbStyleEx,
				wbDim.left,
				wbDim.right,
				wbDim.right - wbDim.left,
				wbDim.bottom - wbDim.top,
				wbParent
			);	
}

DWORD clsWinBase::Create(
		LPCTSTR		wbClsName,
		DWORD		wbStyle,
		DWORD		wbStyleEx,
		POINT		wbPos,
		UINT		wbWidth,
		UINT		wbHeight,
		clsWinBase*	wbParent
) {
	return Create(
				wbClsName,
				wbStyle,
				wbStyleEx,
				wbPos.x,
				wbPos.y,
				wbWidth,
				wbHeight,
				wbParent
			);
}

VOID clsWinBase::Destroy()
{
	UINT ancestorIndex;
	if ( Parent != NULL )
	{
		ancestorIndex = Parent->findFirstChildIndex(this);	
		if ( ancestorIndex < Parent->ChildrenList.size() )
			Parent->ChildrenList.erase(Parent->ChildrenList.begin() 
													+ ancestorIndex);
	}
	while ( ChildrenList.size() != 0 )	ChildrenList[0]->Destroy();
	DestroyWindow(hWnd);
	EventHandlers.clear();

	Parent		= NULL;
	Anchors		= NULL;

	Position.x	= 0;
	Position.y	= 0;
	Width		= 0;
	Height		= 0;
}

DWORD clsWinBase::AssignEventHandler( UINT Event, EVENT_FUNC Handler, BOOL Replace )
{
	EVENT_FUNC_MAP::iterator finder = EventHandlers.find(Event); 
	ELEMENT NewEventHandler;
	if ( Handler == NULL 
		&& !Replace ) return E_BAD_ARGUMENTS;
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

	return S_DONE;
}

EVENT_FUNC clsWinBase::ObtainEventHandler(UINT Event)
{
	EVENT_FUNC_MAP::iterator finder = EventHandlers.find(Event);
	if ( finder != EventHandlers.end() ) return finder->second;
	return NULL;
}

EVENT_FUNC clsWinBase::operator[](UINT Event)
{
	return ObtainEventHandler(Event);
}

VOID clsWinBase::ResetEventHandlers() { EventHandlers.clear(); }

UINT clsWinBase::findFirstChildIndex(clsWinBase *lpCtrlChild) const
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
	return i;
}

UINT clsWinBase::findFirstChildIndex(UINT fTabOrder) const
{
	UINT	cCount	= ChildrenList.size(),
			i		= 0;

	while ( i < cCount )
	{
		if ( ChildrenList[i]->tabOrder == fTabOrder )
			return i;
		else
			i++;
	}
	return i;
}

BOOL clsWinBase::Validate() 
{ 
	return ValidateRect(hWnd, NULL); 
}

BOOL clsWinBase::Invalidate(BOOL bErase) 
{ 
	return InvalidateRect(hWnd, NULL, bErase); 
} 

BOOL clsWinBase::Show(BOOL bRecursive)		
{
	UINT cCount		= ChildrenList.size();
	BOOL bResult	= manageWindowState(SW_SHOWNORMAL);
	for ( UINT i = 0; 
		i < cCount 
			&& bRecursive 
			&& bResult; 
		i++ )
			bResult = ChildrenList[i]->Show(bRecursive);
	return bResult;
}

BOOL clsWinBase::Hide()		{ return manageWindowState(SW_HIDE); }

BOOL clsWinBase::Enable()	{ return EnableWindow(hWnd, TRUE); }

BOOL clsWinBase::Disable()	{ return EnableWindow(hWnd, FALSE); }

BOOL clsWinBase::MoveTo(POINT ptDest) 
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

BOOL clsWinBase::MoveTo(INT ptDestX, INT ptDestY)
{
	POINT ptDest;
	ptDest.x = ptDestX;
	ptDest.y = ptDestY;
	return MoveTo(ptDest);
}

BOOL clsWinBase::SizeTo(UINT cWidth, UINT cHeight)
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

BOOL clsWinBase::MoveSizeTo(RECT wbSizePos)
{
	INT		wbPosX	= wbSizePos.left < wbSizePos.right ? 
								wbSizePos.left : wbSizePos.right,
			wbPosY	= wbSizePos.top < wbSizePos.bottom ? 
								wbSizePos.top : wbSizePos.bottom;
	UINT	wbWidth	= abs(wbSizePos.right - wbSizePos.left),
			wbHeight	= abs(wbSizePos.bottom - wbSizePos.top);
	return SetWindowPos(
				hWnd, 
				NULL,
				wbPosX,
				wbPosY,
				wbWidth,
				wbHeight,
				SWP_NOZORDER
			); 
}

BOOL clsWinBase::MoveSizeTo(POINT ptDest, UINT cWidth, UINT cHeight)
{
	RECT wbSizePos;

	return SetRect(
			&wbSizePos, 
			ptDest.x, 
			ptDest.y, 
			ptDest.x + cWidth, 
			ptDest.y + cHeight
		)
		&& MoveSizeTo(wbSizePos);
}

BOOL clsWinBase::MoveSizeTo(INT ptDestX, INT ptDestY, UINT cWidth, UINT cHeight)
{
	RECT wbSizePos;

	return SetRect(
			&wbSizePos, 
			ptDestX, 
			ptDestY, 
			ptDestX + cWidth, 
			ptDestY + cHeight
		)
		&& MoveSizeTo(wbSizePos);
}

BOOL clsWinBase::setText(LPCTSTR wbText)
{
	return SetWindowText(hWnd, wbText) && UpdateWindow(hWnd);
}

BOOL clsWinBase::setStyle(DWORD wbStyle)
{
	BOOL	bResult		= hWnd != NULL;
	if ( bResult )
		bResult = SetWindowProps(hWnd, GWL_STYLE, wbStyle);
	return bResult;
}

BOOL clsWinBase::setStyleEx(DWORD wbStyleEx)
{
	BOOL	bResult		= hWnd != NULL;

	if ( bResult )
	{
		SetLastError(0);
		bResult = SetWindowLongPtr(hWnd, GWL_EXSTYLE, wbStyleEx) != 0 
				|| GetLastError() == 0;
		if ( bResult ) bResult = SetWindowPos(
										hWnd, 
										NULL, 
										0, 0, 0, 0, 
										SWP_NOMOVE 
										| SWP_NOSIZE 
										| SWP_NOZORDER 
										| SWP_FRAMECHANGED
									);
	}
	return bResult;
}

DWORD clsWinBase::setParent(clsWinBase *lpCtrlsParent)
{
	DWORD wbStyle = 0;
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
		wbStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		wbStyle = wbStyle & ~WS_POPUP | WS_CHILD;
		SetWindowLongPtr(hWnd, GWL_STYLE, wbStyle);
	}
	if ( SetParent(hWnd, hWndParent) == NULL ) return E_FAIL;
	if ( hWndParent == NULL )
	{
		wbStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		wbStyle = wbStyle & ~WS_CHILD | WS_POPUP;
		SetWindowLongPtr(hWnd, GWL_STYLE, wbStyle);			
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

VOID clsWinBase::setAnchors(BYTE wbAnchors) { Anchors = wbAnchors; } 

BOOL clsWinBase::setTabOrder(UINT tOrder)
{
	clsWinBase *finder;
	UINT tabOrderMax;
	BOOL bResult		= Parent != NULL;

	if ( bResult )
	{
		tabOrderMax = Parent->getChildrenTabOrderMax();
		bResult 	= tabOrderMax > 0
					&& tabOrder > 0
					&& tabOrder != tOrder 
					&& tOrder > 0
					&& tOrder <= tabOrderMax;
		if ( bResult )
		{
			for ( UINT i = tabOrder - 1; i >= tOrder; i--)
			{
				finder = Parent->getChildByTabOrder(i);
				if ( finder != NULL ) finder->tabOrder++;
			}

			tabOrder = tOrder == tabOrderMax ? tOrder + 1 : tOrder;
			Parent->tabOrderRecalculate();
		}
	}
	return bResult;
}

VOID clsWinBase::setFocus() { if (hWnd != NULL) SetFocus(hWnd); }

clsWinBase* clsWinBase::getParent() { return Parent; }

clsWinBase* clsWinBase::getChildFocused() 
{
	HWND		hFocus	= GetFocus();
	clsWinBase *finder	= NULL;

	UINT		i		= 0,
				cCount	= ChildrenList.size();
	if ( hFocus != NULL )
	{
		while ( i < cCount && finder == NULL )
		{
			if ( ChildrenList[i]->tabOrder	!= 0
				&& ChildrenList[i]->hWnd	== hFocus )
				finder = ChildrenList[i];
			i++;
		}
	}
	return finder;
}

clsWinBase* clsWinBase::getChildByTabOrder(UINT tOrder)
{
	UINT cIndex			= findFirstChildIndex(tOrder);
	clsWinBase *finder	= NULL;

	if ( cIndex < ChildrenList.size() ) 
		finder = ChildrenList[cIndex];
	return finder;
}

UINT clsWinBase::getChildrenCount() const { return ChildrenList.size(); } 

UINT clsWinBase::getChildrenTabOrderMax() const
{
	UINT	i			= 0, 
			cCount		= ChildrenList.size(),
			tabOrderMax = 0;

	while ( i < cCount )
	{
		if ( ChildrenList[i]->tabOrder > tabOrderMax ) 
			tabOrderMax = ChildrenList[i]->tabOrder;
		i++;
	}

	return tabOrderMax;
}

UINT clsWinBase::getTabOrder() const { return tabOrder; }

UINT clsWinBase::getText(LPTSTR wbText, UINT nLengthToCopy) const
{
	return GetWindowText(hWnd, wbText, nLengthToCopy);
}

DWORD clsWinBase::getStyle() const 
{
	return hWnd != NULL ? GetWindowLongPtr(hWnd, GWL_STYLE) : 0;
}

DWORD clsWinBase::getStyleEx() const 
{
	return hWnd != NULL ? GetWindowLongPtr(hWnd, GWL_EXSTYLE) : 0;
}


BYTE clsWinBase::getAnchors() const { return Anchors; }

VOID clsWinBase::getBoundaries(LPRECT rcDims) const
{
	if ( rcDims != NULL )
	{
		SetRectEmpty(rcDims);
		GetWindowRect(hWnd, rcDims);
		if ( Parent != NULL )
			MapWindowPoints(NULL, Parent->hWnd, (LPPOINT)rcDims, 2);
	}
}

VOID clsWinBase::getPos(LPINT pWhereX, LPINT pWhereY) const
{
	RECT rcDims;
	getBoundaries(&rcDims);	
	if ( pWhereX != NULL ) { *pWhereX = rcDims.left; }
	if ( pWhereY != NULL ) { *pWhereY = rcDims.top; }
}

VOID clsWinBase::getPos(LPPOINT ptWhere) const
{
	RECT rcDims;
	getBoundaries(&rcDims);	
	if ( ptWhere != NULL )
	{
		ptWhere->x = rcDims.left;
		ptWhere->y = rcDims.right;
	}
}

VOID clsWinBase::getSize(LPUINT pWidth, LPUINT pHeight) const
{
	RECT rcDims;
	getBoundaries(&rcDims);	

	if ( pWidth != NULL ) { *pWidth = rcDims.right - rcDims.left; }
	if ( pHeight != NULL ) { *pHeight = rcDims.bottom - rcDims.top; }
}

UINT clsWinBase::getWidth() const
{
	UINT cWidth;
	getSize(&cWidth, NULL);
	return cWidth;
}

UINT clsWinBase::getHeight() const
{
	UINT cHeight;
	getSize(NULL, &cHeight);
	return cHeight;
}

INT clsWinBase::getWidthMnemonic() const { return Width; }

INT clsWinBase::getHeightMnemonic() const { return Height; }

VOID clsWinBase::getDC(HDC *hDC) 
{ 
	if ( hDC != NULL ) *hDC = GetWindowDC(hWnd); 
}

HWND clsWinBase::getHwnd() { return hWnd; }

BOOL clsWinBase::dropDC(HDC *hDC) 
{ 
	BOOL bResult = hDC != NULL;
	if ( bResult ) 
	{
		bResult = ReleaseDC(hWnd, *hDC);
		*hDC = NULL;
	}
	return  bResult; 
}

BOOL clsWinBase::isVisible() const	{ return IsWindowVisible(hWnd); }

BOOL clsWinBase::isEnabled() const	{ return IsWindowEnabled(hWnd); }

BOOL clsWinBase::isActive()	const { return GetActiveWindow() == hWnd; }

BOOL clsWinBase::isParent(clsWinBase *lpCtrlChild) const
{ 
	return ChildrenList.size() != findFirstChildIndex(lpCtrlChild); 
} 

BOOL clsWinBase::isChild(clsWinBase *lpCtrlParent) const { return Parent == lpCtrlParent; }

// ============================================================================
// clsForm class implementation
// ============================================================================
BOOL clsForm::applyBkBrush(HBRUSH hBrToSet)
{
	HDC		hDC;
	BOOL	bResult = hWnd != NULL;
	if ( bResult )
	{
		bResult = SetClassLongPtr(
							hWnd, 
							GCLP_HBRBACKGROUND, 
							PtrToLong(hBrToSet)
						) != 0;
		getClientDC(&hDC);
		SendMessage(hWnd, WM_ERASEBKGND, (WPARAM)hDC, 0); 
		dropDC(&hDC);
	}
	return bResult;
}

VOID clsForm::releaseBkBrush()
{
	HBRUSH hBr;
	if ( hBrCleanup )
	{
		hBr = (HBRUSH)GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
		SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)BRUSH_DEFAULT);
		DeleteObject(hBr);
		hBrCleanup = FALSE;
	}
}

clsForm::clsForm() 
	: clsWinBase()
{
	frmClsAutoUnreg	= FALSE;
	cycleIsRunning	= FALSE;
	hBrCleanup		= FALSE;
}

clsForm::~clsForm() { }

DWORD clsForm::Create(
			LPCTSTR		frmClsName,
			FORM_TYPE	frmType,
			DWORD		frmStyleEx,
			INT			frmPosX,
			INT			frmPosY,
			UINT		frmWidth,
			UINT		frmHeight,
			clsForm*	frmParent,
			HMENU		frmMenu
) {
	DWORD dwResult;
	WNDCLASSEX	wndClsFound;

	if ( !(GetClassInfoEx(hInst, frmClsName, &wndClsFound)
			|| GetClassInfoEx(NULL, frmClsName, &wndClsFound)) )
	{
		ClassRegister (
					frmClsName,
					CS_VREDRAW | CS_HREDRAW,
					0, 
					0,
					NULL,
					NULL,
					LoadCursor(NULL, IDC_ARROW),
					BRUSH_DEFAULT
				);
		frmClsAutoUnreg	= TRUE;
	}
	dwResult = clsWinBase::Create(
						frmClsName,
						frmType,
						frmStyleEx,
						frmPosX,
						frmPosY,
						frmWidth,
						frmHeight,
						frmParent
					);	
	if ( FAILED(dwResult) ) return dwResult;
	dwResult = AssignEventHandler(
						WM_CTLCOLORSTATIC, 
						clsControl::defRedrawHandler, 
						TRUE
					);
	dwResult = AssignEventHandler(
						WM_CTLCOLOREDIT, 
						clsControl::defRedrawHandler, 
						TRUE
					);
	dwResult = AssignEventHandler(
						WM_CTLCOLORBTN, 
						clsControl::defRedrawHandler, 
						TRUE
					);
	dwResult = AssignEventHandler(
						WM_CTLCOLORLISTBOX, 
						clsControl::defRedrawHandler, 
						TRUE
					);
	if ( FAILED(dwResult) ) return E_FAILED;
	setMenu(frmMenu);
	return S_DONE;	
}

DWORD clsForm::Create(
			LPCTSTR		frmClsName,
			FORM_TYPE	frmType,
			DWORD		frmStyleEx,
			POINT		frmPos,
			UINT		frmWidth,
			UINT		frmHeight,
			clsForm*	frmParent,
			HMENU		frmMenu
) {
	return Create(
				frmClsName,
				frmType,
				frmStyleEx,
				frmPos.x,
				frmPos.y,
				frmWidth,
				frmHeight,
				frmParent,
				frmMenu
			);		
}

DWORD clsForm::Create(
			LPCTSTR		frmClsName,
			FORM_TYPE	frmType,
			DWORD		frmStyleEx,
			RECT		frmDim,
			clsForm*	frmParent,
			HMENU		frmMenu
) {
	return Create(
				frmClsName,
				frmType,
				frmStyleEx,
				frmDim.left,
				frmDim.right,
				frmDim.right - frmDim.left,
				frmDim.bottom - frmDim.top,
				frmParent,
				frmMenu
			);
}

VOID clsForm::Destroy()
{
	releaseBkBrush();
	if ( frmClsAutoUnreg )
	{
		frmClsAutoUnreg = FALSE;
		UnregisterClass(
			(LPCTSTR)GetClassLongPtr(hWnd, GCW_ATOM), 
			hInst
		);
	}

	clsWinBase::Destroy();
}

BOOL clsForm::Maximize() { return ShowWindow(hWnd, SW_SHOWMAXIMIZED); }

BOOL clsForm::Minimize() { return ShowWindow(hWnd, SW_SHOWMINIMIZED); }

BOOL clsForm::Restore()  { return ShowWindow(hWnd, SW_SHOWNORMAL); }

INT_PTR clsForm::DBShow(LPCTSTR dbTemplate, DLGPROC dbProcedure) const
{
	return DialogBox(hInst, dbTemplate, hWnd, dbProcedure);
}

INT_PTR clsForm::MBShow(LPCTSTR mbText, LPCTSTR mbCaption, UINT mbType) const
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

BOOL clsForm::setBrush(HBRUSH hBrBg)
{
	releaseBkBrush();
	return applyBkBrush(hBrBg);
}

BOOL clsForm::setColor(COLORREF bgColor)
{
	HBRUSH newHBr	= CreateSolidBrush(bgColor);
	BOOL bResult	= newHBr != NULL;

	if ( bResult )
	{
		bResult		= applyBkBrush(newHBr);
		hBrCleanup	= bResult;
	}
	return bResult;
}

HBRUSH clsForm::getBrush()
{
	return (HBRUSH)GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
}

COLORREF clsForm::getColor()
{
	LOGBRUSH logBrush = {0};

	GetObject(
		getBrush(),
		sizeof(LOGBRUSH),
		&logBrush
	);
	return logBrush.lbColor;
}

VOID clsForm::getClientSize(LPUINT fcWidth, LPUINT fcHeight) const
{
	RECT clientRect;
	BOOL bResult = GetClientRect(hWnd, &clientRect);
	if ( fcWidth != NULL ) *fcWidth = bResult ? clientRect.right : 0U;
	if ( fcHeight != NULL) *fcHeight = bResult ? clientRect.bottom : 0U;
}

VOID clsForm::getClientWidth(LPUINT fcWidth) const
{
	getClientSize(fcWidth, NULL);
}

VOID clsForm::getClientHeight(LPUINT fcHeight) const
{
	getClientSize(NULL, fcHeight);
}

VOID clsForm::getClientDC(HDC *hDC) 
{ 
	if ( hDC != NULL ) *hDC = GetDC(hWnd); 
}


BOOL clsForm::isMaximized() const { return IsZoomed(hWnd); }

BOOL clsForm::isMinimized() const { return IsIconic(hWnd); }

BOOL clsForm::isNormal() const { return !(isMaximized() || isMinimized()); }

INT clsForm::DoMSGCycle(HACCEL hAccelTable)
{
	INT iResult = -1;
	MSG msg;
	if ( !cycleIsRunning )
	{
		cycleIsRunning = TRUE;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) 
				&& !IsDialogMessage(hWnd, &msg)
			) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		cycleIsRunning = FALSE;
		iResult = (INT)msg.wParam;
	}
	return iResult;
}

// ============================================================================
// clsControl class implementation
// ============================================================================
LRESULT clsControl::defRedrawHandler(LPOBJECT Sender, WPARAM wParam, LPARAM lParam)
{
	clsControl	*ctlText	= (clsControl*)GetWindowLongPtr(
												(HWND)lParam, 
												GWL_USERDATA
											);

	SetTextColor((HDC)wParam, ctlText->textColorRef);
	SetBkMode((HDC)wParam, TRANSPARENT);	
	return (LRESULT)ctlText->bgFillBrush;
}

VOID clsControl::freeBgBrush() 
{
	if ( 
		bCustomBrush
		&& bgFillBrush != NULL 
	) {
		DeleteObject(bgFillBrush);
		bCustomBrush	= FALSE;
	}
	bgFillBrush	= NULL;
}

clsControl::clsControl()
{
	textColorRef	= RGB(0, 0, 0);
	bgFillBrush		= NULL;
	bCustomBrush	= FALSE;
}

VOID clsControl::Destroy()
{
	clsWinBase::Destroy();
	freeBgBrush();
}

BOOL clsControl::setTextColor(COLORREF txtColorRef)
{
	textColorRef = txtColorRef;
	return Invalidate(TRUE);
}

BOOL clsControl::setBgFillBrush(HBRUSH hBr)
{
	freeBgBrush();
	if ( hBr == NULL )
		bgFillBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	else
		bgFillBrush = hBr;
	return Invalidate(TRUE);
}

BOOL clsControl::setBgFillColor(COLORREF bkColorRef)
{
	bCustomBrush = TRUE;
	return setBgFillBrush(CreateSolidBrush(bkColorRef));
}

BOOL clsControl::setID(UINT wbID)
{
	return SetWindowProps(hWnd, GWLP_ID, wbID);
}


COLORREF clsControl::getTextColor() const 
{
	return textColorRef;
}

BOOL clsControl::getBkColor(LPCOLORREF cRefOut) const 
{
	LOGBRUSH	brLog;
	BOOL		bResult;

	bResult = bgFillBrush != GetStockObject(NULL_BRUSH)
			&& GetObject(bgFillBrush, sizeof(LOGBRUSH), &brLog) != 0;
	if ( cRefOut != NULL ) *cRefOut = bResult ? brLog.lbColor : 0; 
	return bResult;
}

UINT clsControl::getID() const { return GetWindowLongPtr(hWnd, GWLP_ID); }

// ============================================================================
// clsButton class implementation
// ============================================================================
DWORD clsButton::Create(
			UINT	ID,
			LPCTSTR btnText,
			LPFORM	btnParent,
			INT		btnPosX,
			INT		btnPosY,
			UINT	btnWidth,
			UINT	btnHeight,
			BOOL	setDefault			
) {
	if ( btnParent == NULL ) return E_BAD_ARGUMENTS;
	DWORD	dwResult = clsWinBase::Create(
							WC_BUTTON,
							WS_CHILD | WS_TABSTOP
							| (setDefault ? BS_DEFPUSHBUTTON 
											: BS_PUSHBUTTON),
							NULL,
							btnPosX,
							btnPosY,
							btnWidth,
							btnHeight,
							btnParent
						);
	if ( FAILED(dwResult) )	return dwResult; 
	if ( 
		!setID(ID)
		|| !setText(btnText) 
	) {		
		Destroy();
		return E_FAILED;
	}
	setBgFillBrush(btnParent->getBrush());
	Show();
	return S_DONE;
}

DWORD clsButton::Create(
				UINT	ID,
				LPCTSTR btnText,
				LPFORM	btnParent,
				POINT	btnPos,
				UINT	btnWidth,
				UINT	btnHeight,
				BOOL	setDefault		
) {
	return Create(
			ID,
			btnText,
			btnParent,
			btnPos.x,
			btnPos.y,
			btnWidth,
			btnHeight,
			setDefault
		);
}

DWORD clsButton::Create(
				UINT	ID,
				LPCTSTR btnText,
				LPFORM	btnParent,
				RECT	btnDim,
				BOOL	setDefault		
) {
	return Create(
			ID,
			btnText,
			btnParent,
			btnDim.left,
			btnDim.top,
			btnDim.right - btnDim.left,
			btnDim.bottom - btnDim.top,
			setDefault
		);
}

// ============================================================================
// clsLabel class implementation
// ============================================================================
DWORD clsLabel::Create(
				LPCTSTR  lText,
				LPFORM	 lParent,
				INT		 lPosX,
				INT		 lPosY,
				UINT	 lWidth,
				UINT	 lHeight
) {
	if ( lParent == NULL ) return E_BAD_ARGUMENTS;
	DWORD	dwResult = clsControl::Create(
							WC_STATIC,
							WS_CHILD,
							NULL,
							lPosX,
							lPosY,
							lWidth,
							lHeight,
							lParent
						);
	if ( FAILED(dwResult) ) return dwResult; 
	if ( !setText(lText) ) 
	{
		Destroy();
		return E_FAILED;
	}
	setBgFillBrush(lParent->getBrush());
	Show();
	return S_DONE;
}

DWORD clsLabel::Create(
				LPCTSTR  lText,
				LPFORM	 lParent,
				POINT	 lPos,
				UINT	 lWidth,
				UINT	 lHeight
) {
	return Create(
			lText,
			lParent,
			lPos.x,
			lPos.y,
			lWidth,
			lHeight
		);
}

DWORD clsLabel::Create(
				LPCTSTR  lText,
				LPFORM	 lParent,
				RECT	 lDim
) {
	return Create(
			lText,
			lParent,
			lDim.left,
			lDim.top,
			lDim.right - lDim.left,
			lDim.bottom - lDim.top
		);
}

// ============================================================================
// clsTextBox class implementation
// ============================================================================
DWORD clsTextBox::Create(
				UINT		ID,
				LPCTSTR		tbText,
				LPFORM		tbParent,
				INT			tbPosX,
				INT			tbPosY,
				UINT		tbWidth,
				UINT		tbHeight,
				EDIT_TYPE	tbType
) {
	DWORD tbStyle = WS_CHILD 
					| WS_TABSTOP 
					| WS_BORDER
					| ES_AUTOHSCROLL;
	if ( tbParent == NULL ) return E_BAD_ARGUMENTS;
	switch (tbType)
	{
		case MULTILINE:
			tbStyle |= ES_MULTILINE | ES_AUTOVSCROLL;
			break;

		case PASSWORD:
			tbStyle |= ES_PASSWORD;
			break;

		case NUMERIC:
			tbStyle |= ES_NUMBER;
			break;
	}
	DWORD	dwResult = clsControl::Create(
							WC_EDIT,
							tbStyle,
							NULL,
							tbPosX,
							tbPosY,
							tbWidth,
							tbHeight,
							tbParent
						);
	if ( FAILED(dwResult) ) return dwResult; 
	if ( 
		!setID(ID)
		|| !setText(tbText) 
	) {
		Destroy();
		return E_FAILED;
	}
	Show();
	return S_DONE;
}

DWORD clsTextBox::Create(
				UINT		ID,
				LPCTSTR		tbText,
				LPFORM		tbParent,
				POINT		tbPos,
				UINT		tbWidth,
				UINT		tbHeight,
				EDIT_TYPE	tbType	
) {
	return Create(
			ID,
			tbText,
			tbParent,
			tbPos.x,
			tbPos.y,
			tbWidth,
			tbHeight,
			tbType
		);
}

DWORD clsTextBox::Create(
				UINT		ID,
				LPCTSTR		tbText,
				LPFORM		tbParent,
				RECT		tbDim,
				EDIT_TYPE	tbType
) {
	return Create(
			ID,
			tbText,
			tbParent,
			tbDim.left,
			tbDim.top,
			tbDim.right - tbDim.left,
			tbDim.bottom - tbDim.top,
			tbType
		);
}


// ============================================================================
// clsListBox class implementation
// ============================================================================
DWORD clsListBox::Create(
				UINT		ID,
				LPCTSTR		lbText,
				LPFORM		lbParent,
				INT			lbPosX,
				INT			lbPosY,
				UINT		lbWidth,
				UINT		lbHeight
) {
	DWORD tbStyle = WS_CHILD 
					| WS_TABSTOP 
					| WS_BORDER 
					| WS_VSCROLL
					| LBS_USETABSTOPS;
	if ( lbParent == NULL ) return E_BAD_ARGUMENTS;
	DWORD	dwResult = clsControl::Create(
							WC_LISTBOX,
							tbStyle,
							NULL,
							lbPosX,
							lbPosY,
							lbWidth,
							lbHeight,
							lbParent
						);
	if ( FAILED(dwResult) ) return dwResult; 
	if ( 
		!setID(ID)
		|| !setText(lbText) 
	) {
		Destroy();
		return E_FAILED;
	}
	Show();
	return S_DONE;
}

DWORD clsListBox::Create(
				UINT		ID,
				LPCTSTR		lbText,
				LPFORM		lbParent,
				POINT		lbPos,
				UINT		lbWidth,
				UINT		lbHeight
) {
	return Create(
			ID,
			lbText,
			lbParent,
			lbPos.x,
			lbPos.y,
			lbWidth,
			lbHeight
		);
}

DWORD clsListBox::Create(
				UINT		ID,
				LPCTSTR		lbText,
				LPFORM		lbParent,
				RECT		lbDim
) {
	return Create(
			ID,
			lbText,
			lbParent,
			lbDim.left,
			lbDim.top,
			lbDim.right - lbDim.left,
			lbDim.bottom - lbDim.top
		);
}

INT clsListBox::addItem(LPCTSTR ItemString, LPVOID ItemData)
{
	INT newStringIndex = SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)ItemString);

	if ( newStringIndex >= 0 )
		SendMessage(hWnd, LB_SETITEMDATA, newStringIndex, (LPARAM)ItemData); 

	return newStringIndex;
}

INT clsListBox::delItem(INT ItemIndex)
{
	return SendMessage(hWnd, LB_DELETESTRING, ItemIndex, 0);
}

VOID clsListBox::clear()
{
	SendMessage(hWnd, LB_RESETCONTENT, 0, 0);
}


BOOL clsListBox::getItem(
			INT		ItemIndex,  
			LPTSTR	ItemString, 
			INT		buffSize,
			LPVOID	*ItemData
) const {
	INT		ItemStringLength;
	BOOL	bResult				= ItemIndex >= 0;
	
	if ( bResult )
	{
		ItemStringLength	= SendMessage(hWnd, LB_GETTEXTLEN, ItemIndex, 0);
		if ( (bResult = ItemStringLength != LB_ERR) )
		{
			if ( 
				(bResult = ItemStringLength < buffSize) && 
				ItemString != NULL 
			)	SendMessage(
						hWnd, 
						LB_GETTEXT,
						ItemIndex, 
						(LPARAM)ItemString
					);
			if ( ItemData != NULL )
				*ItemData = (LPVOID)SendMessage(
											hWnd, 
											LB_GETITEMDATA, 
											ItemIndex, 
											0
										);
		}
	}
	return bResult;
}

INT clsListBox::getItemCount() const
{
	return SendMessage(hWnd, LB_GETCOUNT, 0, 0);
}

INT clsListBox::getCurSel() const
{
	return SendMessage(hWnd, LB_GETCURSEL, 0, 0);
}

VOID clsListBox::setCurSel(INT ItemIndex) 
{
	SendMessage(hWnd, LB_SETCURSEL, ItemIndex, 0);
}

BOOL clsListBox::isSelItem(INT ItemIndex) const
{
	return SendMessage(hWnd, LB_GETSEL, ItemIndex, 0) > 0; 
}


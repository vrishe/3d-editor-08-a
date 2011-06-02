// Forms.lib static library header. This library doesn't pretend to be just and only the one, 
// which must be used to manage WinAPI windows. But it could be helpful significally
// in the case of a newbie one use.
// Compile with: /EHsc (Because of STL)
// ==========================================================================================
//

#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <tchar.h>
#include <map>
#include <vector>

using namespace std;
using std::map;
using std::vector;

// Common library error codes
#define E_FAILED				0x80000000
#define E_DOES_NOT_EXIST		0x80000001
#define E_HINSTANCE_IS_NULL		0x80000002
#define E_CLASS_NOT_EXIST		0x80000003
#define E_BAD_ARGUMENTS			0x80000004
#define E_HWND_CREATION_FAILED	0x80000005
#define E_SUBCLASSING_FAILED	0x80000006

#define S_DONE					0x00000000
#define S_REPLACED				0x00000001
#define S_ALREADY_EXISTS		0x00000002

// Use this stuff if you feel need to register your own window class
ATOM ClassRegister (
			LPCTSTR clsName, 
			UINT clsStyle, 
			INT clsXtraBytes, 
			INT frmXtraBytes,
			HICON hIcon,
			HICON hIconSm,
			HCURSOR hCur, 
			HBRUSH bgrColor
		);

// Necessary clsWinBase class type definitions
enum ANCHOR {
	ANCR_NONE			= 0x00,
	ANCR_LEFT			= 0x01,
	ANCR_TOP			= 0x02,
	ANCR_RIGHT			= 0x04,
	ANCR_BOTTOM			= 0x08
};
#define ANCR_TOPLEFT		ANCR_TOP | ANCR_LEFT
#define ANCR_TOPRIGHT		ANCR_TOP | ANCR_RIGHT
#define ANCR_BOTTOMLEFT		ANCR_BOTTOM | ANCR_LEFT
#define ANCR_BOTTOMRIGHT	ANCR_BOTTOM | ANCR_RIGHT
#define ANCR_LEFTRIGHT		ANCR_LEFT | ANCR_RIGHT
#define	ANCR_BOTTOMTOP		ANCR_BOTTOM | ANCR_TOP
#define ANCR_ALL			ANCR_LEFTRIGHT | ANCR_BOTTOMTOP

typedef LPVOID	LPOBJECT;
typedef LRESULT(*EVENT_FUNC)(LPOBJECT, WPARAM, LPARAM);	// Typical event handler looks like this.
typedef map<UINT, EVENT_FUNC> EVENT_FUNC_MAP;			// Short map definition.
typedef pair<UINT, EVENT_FUNC> ELEMENT;					// Short map pair element definition

// This is a superclass which gives a ground to all
// derived classes, which implement alternative 
// functionality.
class clsWinBase {
private:
	clsWinBase				*Parent;
	vector<clsWinBase *>	ChildrenList;
	
// Default window callback. Don't care about it!
	static LRESULT CALLBACK CtrlProc ( 
					HWND hWnd, 
					UINT Event, 
					WPARAM wParam, 
					LPARAM lParam 
				);
	WNDPROC			defCtrlProc;
	EVENT_FUNC_MAP	EventHandlers;	// STL Map which maps function with an event
	
// Do you really think you should know about
// these methods more than their prototypes? 
	UINT findFirstChild(clsWinBase *lpCtrlChild) const;
	BOOL removeFirstChildFound(clsWinBase *lpCtrlChild);
	BOOL manageWindowState(INT nCmdShow);

protected:
	static HINSTANCE	hInst;		// Hinstance of an application that uses this class.
	LPTSTR				clsName;	// A class name that defines a window class which contol belongs to.
	HWND				hWnd;		// A Window handler, specified to this control.

// Some data field needed for stretching
	BYTE				Anchors;
	RECT				Offset;
	POINT				Position;
	INT					Width;
	INT					Height;
public:
// Construct/Destruct
	clsWinBase();
	virtual ~clsWinBase();

// Create/Destroy
	virtual DWORD Create(
			LPCTSTR		wbClsName,
			DWORD		wbStyle,
			DWORD		wbStyleEx,
			RECT		wbDim,
			clsWinBase*	wbParent
		);
	virtual DWORD Create(
			LPCTSTR		wbClsName,
			DWORD		wbStyle,
			DWORD		wbStyleEx,
			POINT		wbPos,
			UINT		wbWidth,
			UINT		wbHeight,
			clsWinBase*	wbParent
		);
	virtual DWORD Create(
			LPCTSTR		wbClsName,
			DWORD		wbStyle,
			DWORD		wbStyleEx,
			INT			wbPosX,
			INT			wbPosY,
			UINT		wbWidth,
			UINT		wbHeight,
			clsWinBase*	wbParent
	);
	virtual VOID Destroy();

// Interface needed for alternative event handler 
// functions assignation.
	DWORD	AssignEventHandler ( 
			UINT Event, 
			EVENT_FUNC Handler, 
			BOOL Replace 
		);
	EVENT_FUNC ObtainEventHandler(UINT Event);
	EVENT_FUNC operator[](UINT Event);
	VOID	ResetEventHandlers();

// Control state management.
	BOOL	Show(BOOL bRecursive = TRUE);
	BOOL	Hide();
	BOOL	Enable();
	BOOL	Disable();

// Positional transformations. 
// All coordinates supposed to be in client area relation. 
// Client area of desktop == screen coordinates.
// All "-To" methods change window position/size
// regardless of it's Anchors field predefinition.
	BOOL	MoveTo(POINT ptDest);
	BOOL	MoveTo(INT ptDestX, INT ptDestY);
	BOOL	SizeTo(UINT cWidth, UINT cHeight);

	BOOL	MoveSizeTo(RECT wbSizePos);
	BOOL	MoveSizeTo(POINT ptDest, UINT cWidth, UINT cHeight);
	BOOL	MoveSizeTo(INT ptDestX, INT ptDestY, UINT cWidth, UINT cHeight);

// Setters.
	BOOL	setText(LPCTSTR wbText);
	DWORD	setParent(clsWinBase *lpCtrlParent);						
	VOID	setAnchors(BYTE wbAnchors);

// Getters.
	UINT		getText(LPTSTR wbText, UINT nLengthToCopy) const;
	clsWinBase*	getParent();
	BYTE		getAnchors() const;
	VOID		getBoundaries(LPRECT rcDims) const;
	VOID		getPos(LPINT pWhereX, LPINT pWhereY) const;
	VOID		getPos(LPPOINT ptWhere) const;
	VOID		getSize(LPUINT pWidth, LPUINT pHeight) const;
	UINT		getWidth() const;
	UINT		getHeight() const;
	INT			getWidthMnemonic() const;
	INT			getHeightMnemonic() const;
	VOID		getDC(HDC *hDC);

	BOOL		dropDC(HDC *hDC);
// Checkers.
	BOOL isVisible() const;
	BOOL isEnabled() const;
	BOOL isActive() const;
	BOOL isParent(clsWinBase *lpCtrlChild) const;
	BOOL isChild(clsWinBase *lpCtrlParent) const;
};
typedef clsWinBase WINBASE, *LPWINBASE;
typedef const clsWinBase *LPCWINBASE;

//class Menu {
//private:
//	HMENU nMenu;

//public:
//	Menu();
//	~Menu();

//	HRESULT Create();
//	HRESULT Destroy();

//};


// Necessary clsForm class structs/definitions/enums
#define BRUSH_DEFAULT	(HBRUSH)(COLOR_WINDOW + 1)
#define CSTYLE_DEFAULT	

enum FORM_TYPE {
	APP_FORM	= WS_OVERLAPPEDWINDOW,
	CHILD_FORM	= WS_CHILDWINDOW,
	POPUP_FORM	= WS_POPUPWINDOW
};

// This is a class designed for easy form management. 
// Forget about WNDPROC because of window subclassing. 
// Use the event-assigned handler functions!
class clsForm : public clsWinBase	{
private:
	BOOL	frmClsAutoUnreg;

public:
// Construct/Destruct.
	clsForm();
	virtual ~clsForm();

// Create/Destroy.
	virtual DWORD Create(
			LPCTSTR		frmClassName,
			FORM_TYPE	frmType,
			DWORD		frmStyleEx,
			RECT		frmDim,
			clsForm*	frmParent	= NULL,
			HMENU		frmMenu		= NULL
		);
	virtual DWORD Create(
			LPCTSTR		frmClassName,
			FORM_TYPE	frmType,
			DWORD		frmStyleEx,
			POINT		frmPos,
			UINT		frmWidth,
			UINT		frmHeight,
			clsForm*	frmParent	= NULL,
			HMENU		frmMenu		= NULL
		);
	virtual DWORD Create(
			LPCTSTR		frmClassName,
			FORM_TYPE	frmType,
			DWORD		frmStyleEx,
			INT			frmPosX,
			INT			frmPosY,
			UINT		frmWidth,
			UINT		frmHeight,
			clsForm*	frmParent	= NULL,
			HMENU		frmMenu		= NULL
		);
	virtual VOID Destroy();

// Form state management
	BOOL	Validate(LPRECT pValidRect = NULL);
	BOOL	Invalidate(LPRECT pInvalidRect = NULL, BOOL bErase = FALSE);
	BOOL	Maximize();
	BOOL	Minimize();
	BOOL	Restore();
	INT_PTR DBShow(LPCTSTR dbTemplate, DLGPROC dbProcedure) const;
	INT_PTR MBShow(LPCTSTR mbText, LPCTSTR mbCaption, UINT mbType) const;

// Setters.
	BOOL	setMenu(HMENU frmMenu);

// Getters.
	//HMENU	getMenu();
	VOID	getClientSize(LPUINT fcWidth, LPUINT fcHeight) const; 
	VOID	getClientWidth(LPUINT fcWidth) const;
	VOID	getClientHeight(LPUINT fcHeight) const;
	VOID	getClientDC(HDC *hDC);

// Checkers.
	BOOL	isMaximized() const;
	BOOL	isMinimized() const;
	BOOL	isNormal() const;
};
typedef clsForm FORM, *LPFORM; 
typedef const clsForm *LPCFORM;

class clsControl : public clsWinBase {
private:
	UINT tabOrder;

public:
	virtual DWORD Create(
			LPCTSTR		ctrlClsName,
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			RECT		ctrlDim,
			clsWinBase*	ctrlParent
		);
	virtual DWORD Create(
			LPCTSTR		ctrlClsName,
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			POINT		ctrlPos,
			UINT		ctrlWidth,
			UINT		ctrlHeight,
			clsWinBase*	ctrlParent
		);
	virtual DWORD Create(
			LPCTSTR		ctrlClsName,
			DWORD		ctrlStyle,
			DWORD		ctrlStyleEx,
			INT			ctrlPosX,
			INT			ctrlPosY,
			UINT		ctrlWidth,
			UINT		ctrlHeight,
			clsWinBase*	ctrlParent
		);

	VOID setTabOrder(UINT tbOrder);


	UINT getTabOrder() const;
};

class clsButton : public clsControl {
private:
	UINT	ID;

public:
	virtual DWORD Create(
				UINT	btnId,
				LPCTSTR btnText,
				LPFORM	btnParent,
				RECT	btnDim,
				BOOL	setDefault	= FALSE
			);

	virtual DWORD Create(
				UINT	btnId,
				LPCTSTR btnText,
				LPFORM	btnParent,
				POINT	btnPos,
				UINT	btnWidth,
				UINT	btnHeight,
				BOOL	setDefault	=	FALSE			
			);

	virtual DWORD Create(
				UINT	btnId,
				LPCTSTR btnText,
				LPFORM	btnParent,
				INT		btnPosX,
				INT		btnPosY,
				UINT	btnWidth,
				UINT	btnHeight,
				BOOL	setDefault	=	FALSE			
			);

	UINT getID() const;
};
typedef clsButton BUTTON, *LPBUTTON;
typedef const clsButton *LPCBUTTON;




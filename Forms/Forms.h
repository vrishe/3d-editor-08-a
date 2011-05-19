// Forms.lib static library header. This library doesn't pretend to be just and only the one, 
// which must be used to manage WinAPI windows. But it could be helpful significally
// in the case of a newbie one use.
// Compile with: /EHsc (Because of STL)
// ==========================================================================================
//

#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <map>
#include <vector>

namespace Forms
{
	using namespace std;
	using std::map;
	using std::vector;
	// Common library error codes
	#define E_DOES_NOT_EXIST		0x80000001
	#define E_BAD_ARGUMENTS			0x80000002
	#define E_SUBCLASSING_FAILED	0x80000003

	#define S_ASSIGNED				0x10000000
	#define S_REPLACED				0x10000001
	#define S_ALREADY_EXISTS		0x10000002

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

	// Necessary clsControl class type definitions
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

	typedef LRESULT(*EVENT_FUNC)(LPVOID, WPARAM, LPARAM);	// Typical event handler looks like this.
	typedef map<UINT, EVENT_FUNC> EVENT_FUNC_MAP;			// Short map definition.
	typedef pair<UINT, EVENT_FUNC> ELEMENT;					// Short map pair element definition
	
	// This is a superclass which gives a ground to all
	// derived classes, which implement alternative 
	// functionality.
	class clsControl {
	private:
		clsControl				*Parent;
		vector<clsControl *>	ChildrenList;

	
	// Default window callback. Don't care about it!
		static LRESULT CALLBACK CtrlProc ( 
						HWND hWnd, 
						UINT Event, 
						WPARAM wParam, 
						LPARAM lParam 
					);
		EVENT_FUNC_MAP	EventHandlers;	// STL Map which maps function with an event
	
	// Do you really think you should know about
	// these methods more than their prototypes? 
		UINT findFirstChild(clsControl *lpCtrlChild);
		BOOL removeFirstChildFound(clsControl *lpCtrlChild);
		BOOL manageWindowState(INT nCmdShow);

	protected:
		static HINSTANCE	hInst;	// Hinstance of an application that uses this class.
		ATOM				wndCls; // An ATOM defines a window class which contol belongs to.
		HWND				hWnd;	// A Window handler, specified to this control.

	// Some data field needed for stretching
		BYTE				Anchors;
		RECT				Offset;
		POINT				Position;
		INT					Width;
		INT					Height;
	public:
	// Construct/Destruct
		clsControl(ATOM wCls);
		virtual ~clsControl();

	// Create/Destroy
		virtual DWORD Create(
					DWORD		ctrlStyle,
					DWORD		ctrlStyleEx,
					RECT		ctrlDim,
					clsControl*	ctrlParent
				);
		virtual DWORD Create(
					DWORD		ctrlStyle,
					DWORD		ctrlStyleEx,
					POINT		ctrlPos,
					UINT		ctrlWidth,
					UINT		ctrlHeight,
					clsControl*	ctrlParent
				);
		virtual DWORD Create(
					DWORD		ctrlStyle,
					DWORD		ctrlStyleEx,
					INT			ctrlPosX,
					INT			ctrlPosY,
					UINT		ctrlWidth,
					UINT		ctrlHeight,
					clsControl*	ctrlParent
				);

		virtual DWORD Destroy();

	// Interface needed for alternative event handler 
	// functions assignation.
		DWORD	AssignEventHandler ( 
				UINT Event, 
				EVENT_FUNC Handler, 
				BOOL Replace 
			);
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

		BOOL	MoveSizeTo(RECT ctrlSizePos);
		BOOL	MoveSizeTo(POINT ptDest, UINT cWidth, UINT cHeight);
		BOOL	MoveSizeTo(INT ptDestX, INT ptDestY, UINT cWidth, UINT cHeight);

	// Setters.
		BOOL	setText(LPCTSTR ctrlText);
		DWORD	setParent(clsControl *lpCtrlParent);						
		VOID	setAnchors(BYTE ctrlAnchors);

	// Getters.
		UINT		getText(LPTSTR ctrlText, UINT nLengthToCopy);
		clsControl*	getParent();
		BYTE		getAnchors();
		VOID		getBoundaries(LPRECT rcDims);
		VOID		getPos(LPINT pWhereX, LPINT pWhereY);
		VOID		getPos(LPPOINT ptWhere);
		VOID		getSize(LPUINT pWidth, LPUINT pHeight);
		UINT		getWidth();
		UINT		getHeight();
		INT			getWidthMnemonic();
		INT			getHeightMnemonic();
		HWND		gethWnd(); // Do you want to kill me for it? =) Should be removed as quickly as possible!!!
	// Checkers.
		BOOL isVisible();
		BOOL isEnabled();
		BOOL isActive();
		BOOL isParent(clsControl *lpCtrlChild);
		BOOL isChild(clsControl *lpCtrlParent);
	};
	typedef clsControl CONTROL, *LPCONTROL;

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
	enum FORM_TYPE {
		APP_FORM	= WS_OVERLAPPEDWINDOW,
		CHILD_FORM	= WS_CHILDWINDOW,
		POPUP_FORM	= WS_POPUPWINDOW
	};

	// This is a class designed for easy form management. 
	// Forget about WNDPROC because of window subclassing. 
	// Use the event-assigned handler functions!
	class clsForm : public clsControl	{
	private:
		BOOL	frmClsAutoUnreg;

	public:
	// Construct/Destruct.
		clsForm(
			LPCTSTR clsName,
			HBRUSH bgrColor		= (HBRUSH)(COLOR_WINDOW + 1),
			HICON hIcon			= NULL,
			HICON hIconSm		= NULL,
			HCURSOR hCur		= NULL,
			DWORD clsStyle		= CS_VREDRAW | CS_HREDRAW,
			INT clsXtraBytes	= 0,
			INT frmXtraBytes	= 0
		);
		clsForm(ATOM frmCls);
		virtual ~clsForm();

	// Create/Destroy.
		virtual DWORD Create(
					FORM_TYPE	frmType,
					DWORD		frmStyleEx,
					RECT		frmDim,
					clsForm*		frmParent	= NULL,
					HMENU		frmMenu		= NULL
				);
		virtual DWORD Create(
					FORM_TYPE	frmType,
					DWORD		frmStyleEx,
					POINT		frmPos,
					UINT		frmWidth,
					UINT		frmHeight,
					clsForm*		frmParent	= NULL,
					HMENU		frmMenu		= NULL
				);
		virtual DWORD Create(
					FORM_TYPE	frmType,
					DWORD		frmStyleEx,
					INT			frmPosX,
					INT			frmPosY,
					UINT		frmWidth,
					UINT		frmHeight,
					clsForm*		frmParent	= NULL,
					HMENU		frmMenu		= NULL
				);

	// Form state management
		BOOL	Validate(LPRECT pValidRect = NULL);
		BOOL	Invalidate(LPRECT pInvalidRect = NULL, BOOL bErase = FALSE);
		BOOL	Maximize();
		BOOL	Minimize();
		BOOL	Restore();
		INT_PTR DBShow(LPCTSTR dbTemplate, DLGPROC dbProcedure);
		INT_PTR MBShow(LPCTSTR mbText, LPCTSTR mbCaption, UINT mbType);

	// Setters.
		BOOL	setMenu(HMENU frmMenu);

	// Getters.
		HMENU	getMenu(); //
		VOID	getFormClientSize(LPUINT fcWidth, LPUINT fcHeight); 

	// Checkers.
		BOOL	isMaximized();
		BOOL	isMinimized();
		BOOL	isNormal();
	};

	// This is for those who trudges of CAPITAL LETTER TYPE DEFINITIONS.
	typedef clsForm FORM, *LPFORM; 
};


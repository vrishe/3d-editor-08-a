// Forms.lib static library header. This library doesn't pretend to be just and only the one, 
// which must be used to manage WinAPI windows. But it could be helpful significally
// in the case of a newbie one use.
// Compile with: /EHsc (Because of STL)
// ==========================================================================================
//

#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <CommCtrl.h>
#include <Commdlg.h>
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
#define E_BRUSH_CREATION_FAILED 0x80000007

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
BOOL SetWindowProps(HWND hWnd, INT index, LONG lProp, LPLONG lPropOld = NULL);

// Necessary clsWinBase class type definitions
enum ANCHOR {
	ANCR_NONE				= 0x00,
	ANCR_LEFT				= 0x01,
	ANCR_RIGHT				= 0x02,
	ANCR_LEFTRIGHT			= 0x03,
	ANCR_TOP				= 0x04,
	ANCR_TOPLEFT			= 0x05,
	ANCR_TOPRIGHT			= 0x06,
	ANCR_TOPLEFTRIGHT		= 0x07,
	ANCR_BOTTOM				= 0x08,
	ANCR_BOTTOMLEFT			= 0x09,
	ANCR_BOTTOMRIGHT		= 0x0A,
	ANCR_BOTTOMLEFTRIGHT	= 0x0B,
	ANCR_BOTTOMTOP			= 0x0C,
	ANCR_BOTTOMTOPLEFT		= 0x0D,
	ANCR_BOTTOMTOPRIGHT		= 0x0E,
	ANCR_ALL				= 0x0F
};

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
	UINT					tabOrder;
	
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
	VOID	restoreControlFocus();
	VOID	tabOrderRecalculate();
	BOOL	removeFirstChildFound(clsWinBase *lpCtrlChild);
	BOOL	manageWindowState(INT nCmdShow);

protected:
	static HINSTANCE	hInst;		// Hinstance of an application that uses this class.
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

	UINT findFirstChildIndex(clsWinBase *lpCtrlChild)		const;
	UINT findFirstChildIndex(UINT fTabOrder)				const;

	// Control state management.
	BOOL	Validate();
	BOOL	Invalidate(BOOL bErase = FALSE);
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
	BOOL	setStyle(DWORD wbStyle);
	BOOL	setStyleEx(DWORD wbStylEx);
	DWORD	setParent(clsWinBase *lpCtrlParent);						
	VOID	setAnchors(BYTE wbAnchors);
	BOOL	setTabOrder(UINT tOrder);
	VOID	setFocus();

// Getters.
	clsWinBase*	getParent();
	clsWinBase* getChildFocused();
	clsWinBase* getChildByTabOrder(UINT tOrder);
	UINT		getChildrenCount()							const;
	UINT		getChildrenTabOrderMax()					const;
	UINT		getTabOrder()								const;
	UINT		getText(LPTSTR wbText, UINT nLengthToCopy)	const;
	DWORD		getStyle()									const;
	DWORD		getStyleEx()								const;
	BYTE		getAnchors()								const;
	VOID		getBoundaries(LPRECT rcDims)				const;
	VOID		getPos(LPINT pWhereX, LPINT pWhereY)		const;
	VOID		getPos(LPPOINT ptWhere)						const;
	VOID		getSize(LPUINT pWidth, LPUINT pHeight)		const;
	UINT		getWidth()									const;
	UINT		getHeight()									const;
	INT			getWidthMnemonic()							const;
	INT			getHeightMnemonic()							const;
	VOID		getDC(HDC *hDC);
	HWND		getHwnd();

	BOOL		dropDC(HDC *hDC);
// Checkers.
	BOOL isVisible()										const;
	BOOL isEnabled()										const;
	BOOL isActive()											const;
	BOOL isParent(clsWinBase *lpCtrlChild)					const;
	BOOL isChild(clsWinBase *lpCtrlParent)					const;
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
	BOOL	cycleIsRunning;
	BOOL	frmClsAutoUnreg;
	BOOL	hBrCleanup;

	BOOL	applyBkBrush(HBRUSH hBrToSet);	
	VOID	releaseBkBrush();

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
	BOOL	Maximize();
	BOOL	Minimize();
	BOOL	Restore();
	INT_PTR DBShow(LPCTSTR dbTemplate, DLGPROC dbProcedure)			const;
	INT_PTR MBShow(LPCTSTR mbText, LPCTSTR mbCaption, UINT mbType)	const;
	BOOL	OFDShow(LPOPENFILENAME ofn);
	BOOL	SFDShow(LPOPENFILENAME ofn);

// Setters.
	BOOL	setMenu(HMENU frmMenu);
	BOOL	setBrush(HBRUSH hBrBg);
	BOOL	setColor(COLORREF bgColor);

// Getters.
	//HMENU	getMenu();
	HBRUSH 		getBrush();
	COLORREF	getColor();
	VOID		getClientSize(LPUINT fcWidth, LPUINT fcHeight)	const; 
	VOID		getClientWidth(LPUINT fcWidth)					const;
	VOID		getClientHeight(LPUINT fcHeight)				const;
	VOID		getClientDC(HDC *hDC);

// Checkers.
	BOOL	isMaximized()	const;
	BOOL	isMinimized()	const;
	BOOL	isNormal()		const;

	INT DoMSGCycle(HACCEL hAccelTable);
};
typedef clsForm FORM, *LPFORM; 
typedef const clsForm *LPCFORM;

// Necessary clsTextControl class structs/definitions/enums
class clsControl : public clsWinBase {
private:
	HBRUSH		bgFillBrush;
	COLORREF	textColorRef;

	BOOL		bCustomBrush;
	VOID		freeBgBrush();

public:
	clsControl();
	virtual VOID Destroy();
	
	BOOL setTextColor(COLORREF txtColorRef);
	BOOL setBgFillBrush(HBRUSH hBr);
	BOOL setBgFillColor(COLORREF bkColorRef);
	BOOL setID(UINT wbID);

	COLORREF	getTextColor()					const;
	BOOL		getBkColor(LPCOLORREF cRefOut)	const;
	UINT		getID()							const;


	static LRESULT defRedrawHandler(
							LPOBJECT Sender, 
							WPARAM wParam,
							LPARAM lParam
						);
};
typedef clsControl CONTROL, *LPCONTROL;
typedef const clsControl *LPCCONTROL;

// Necessary clsButton class structs/definitions/enums
class clsButton : public clsControl {
public:
	virtual DWORD Create(
				UINT	ID,
				LPCTSTR btnText,
				LPFORM	btnParent,
				RECT	btnDim,
				BOOL	setDefault	= FALSE
			);

	virtual DWORD Create(
				UINT	ID,
				LPCTSTR btnText,
				LPFORM	btnParent,
				POINT	btnPos,
				UINT	btnWidth,
				UINT	btnHeight,
				BOOL	setDefault	=	FALSE			
			);

	virtual DWORD Create(
				UINT	ID,
				LPCTSTR btnText,
				LPFORM	btnParent,
				INT		btnPosX,
				INT		btnPosY,
				UINT	btnWidth,
				UINT	btnHeight,
				BOOL	setDefault	=	FALSE			
			);
};
typedef clsButton BUTTON, *LPBUTTON;
typedef const clsButton *LPCBUTTON;


// Necessary clsLabel class structs/definitions/enums
class clsLabel : public clsControl {
public:
	virtual DWORD Create(
				LPCTSTR  lText,
				LPFORM	 lParent,
				RECT	 lDim
			);

	virtual DWORD Create(
				LPCTSTR  lText,
				LPFORM	 lParent,
				POINT	 lPos,
				UINT	 lWidth,
				UINT	 lHeight
			);

	virtual DWORD Create(
				LPCTSTR  lText,
				LPFORM	 lParent,
				INT		 lPosX,
				INT		 lPosY,
				UINT	 lWidth,
				UINT	 lHeight
			);
};
typedef clsLabel LABEL, *LPLABEL;
typedef const clsLabel *LPCLABEL; 

// Necessary clsTextBox class structs/definitions/enums
enum EDIT_TYPE {
	SINGLELINE	= 0,
	MULTILINE	= 1,
	PASSWORD	= 2,
	NUMERIC		= 3
};

class clsTextBox : public clsControl {
public:
	virtual DWORD Create(
				UINT		ID,
				LPCTSTR		tbText,
				LPFORM		tbParent,
				RECT		tbDim,
				EDIT_TYPE	tbType = SINGLELINE
			);

	virtual DWORD Create(
				UINT		ID,
				LPCTSTR		tbText,
				LPFORM		tbParent,
				POINT		tbPos,
				UINT		tbWidth,
				UINT		tbHeight,
				EDIT_TYPE	tbType = SINGLELINE
			);

	virtual DWORD Create(
				UINT		ID,
				LPCTSTR		tbText,
				LPFORM		tbParent,
				INT			tbPosX,
				INT			tbPosY,
				UINT		tbWidth,
				UINT		tbHeight,
				EDIT_TYPE	tbType = SINGLELINE
			);
};
typedef clsTextBox TEXTBOX, *LPTEXTBOX;
typedef const clsTextBox *LPCTEXTBOX; 


// Necessary clsListBox class structs/definitions/enums
class clsListBox : public clsControl {
public:
	virtual DWORD Create(
				UINT		ID,
				LPCTSTR		lbText,
				LPFORM		lbParent,
				RECT		lbDim
			);

	virtual DWORD Create(
				UINT		ID,
				LPCTSTR		lbText,
				LPFORM		lbParent,
				POINT		lbPos,
				UINT		lbWidth,
				UINT		lbHeight
			);

	virtual DWORD Create(
				UINT		ID,
				LPCTSTR		lbText,
				LPFORM		lbParent,
				INT			lbPosX,
				INT			lbPosY,
				UINT		lbWidth,
				UINT		lbHeight
			);

	INT	addItem(LPCTSTR ItemString, LPVOID ItemData);
	INT delItem(INT ItemIndex);
	VOID clear();

	BOOL getItem(
			INT		ItemIndex, 
			LPTSTR	ItemString, 
			INT		ItemStringLen,
			LPVOID	*ItemData
		)							const; 
	INT	getItemCount()				const;
	INT getCurSel()					const;

	VOID setCurSel(INT ItemIndex);

	BOOL isSelItem(INT ItemIndex)	const;
};
typedef clsListBox LISTBOX, *LPLISTBOX;
typedef const clsListBox *LPCLISTBOX;


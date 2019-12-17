#pragma once

#define WM_SELF_MSG_SHIFT		(8)
#define WM_SELF_MASK		 	(0xaa)

#define Get_Self_Msg_Mask(msg)	((msg) >> WM_SELF_MSG_SHIFT)
#define If_Self_Msg(msg)		((Get_Self_Msg_Mask(msg) == WM_SELF_MASK) ? 1 : 0)

#define WM_SELF_KEY_ENTER_DOWN  ((WM_SELF_MASK << WM_SELF_MSG_SHIFT) | 0xff)
#define WM_SELF_KEY_UP_PRESS	((WM_SELF_MASK << WM_SELF_MSG_SHIFT) | 0xfe)
#define WM_SELF_KEY_DOWN_PRESS	((WM_SELF_MASK << WM_SELF_MSG_SHIFT) | 0xfd)

enum
{
	MAIN_ID = 0,

	STATIC_LAB_MULTICAST_IP_ID,
	EDIT_IMAGE_SELECT_ID,
	STATIC_DEBUG_WINDOW_ID,

};

#define COMTROL_INTERVAL_X					10
#define COMTROL_INTERVAL_Y					20

#define COMMON_CONTROL_WIDTH				120
#define COMMON_CONTROL_HEIGH				20

#define MAIN_WINDOW_WIDTH					1200
#define MAIN_WINDOW_HEIGH					450

#define EDIT_IMAGE_SELECT_X					(30)
#define EDIT_IMAGE_SELECT_Y					(20)
#define EDIT_IMAGE_SELECT_WIDTH				(MAIN_WINDOW_WIDTH / 2 - EDIT_IMAGE_SELECT_X)
#define EDIT_IMAGE_SELECT_HEIGH				(MAIN_WINDOW_HEIGH - EDIT_IMAGE_SELECT_Y * 4 * 3)
#define	EDIT_IMAGE_SELECT_STYLE				(WS_CHILD | WS_VISIBLE | WS_BORDER | BS_RIGHT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL | WS_HSCROLL)
/*
 * ES_AUTOHSCROLL（自动增加水平滚动条） 当用户在行尾键入一个字符时，正文将自动向右滚动10个字符，当用户按回车键时，正文总是滚向左边。
 * ES_AUTOVSCROLL 当用户在最后一个可见行按回车键时，正文向上滚动一页。
 * ES_MULTILINE 　指定编辑控件能否进行多行文本编辑，若多行编辑器不指定ES_AUTOHSCROLL风格，则会自动换行，用户每按一次Enter，在当前光标处输入一个回车换行符(0Dh,0Ah)，文本增加一行。若不指定ES_AUTOVSCROLL，则多行编辑器会在窗口中正文装满时发出警告声响。
*/

#define STATIC_LAB_MULTICAST_IP_X			(EDIT_IMAGE_SELECT_X + EDIT_IMAGE_SELECT_WIDTH + COMTROL_INTERVAL_X * 3)
#define STATIC_LAB_MULTICAST_IP_Y			(EDIT_IMAGE_SELECT_Y)
#define STATIC_LAB_MULTICAST_IP_WIDTH		(MAIN_WINDOW_WIDTH - STATIC_LAB_MULTICAST_IP_X - EDIT_IMAGE_SELECT_X)
#define STATIC_LAB_MUMTICAST_IP_HEIGH		(EDIT_IMAGE_SELECT_HEIGH)
#define	STATIC_LAB_MULTICAST_IP_STYLE		(WS_CHILD | WS_VISIBLE  | BS_FLAT | ES_MULTILINE)// | ES_AUTOHSCROLL | ES_AUTOVSCROLL)// | WS_VSCROLL | WS_HSCROLL)

#define STATIC_DEBUG_WINDOW_X				(EDIT_IMAGE_SELECT_X)
#define STATIC_DEBUG_WINDOW_Y				(EDIT_IMAGE_SELECT_Y + EDIT_IMAGE_SELECT_HEIGH + COMTROL_INTERVAL_Y)
#define STATIC_DEBUG_WINDOW_WIDTH			(EDIT_IMAGE_SELECT_WIDTH)
#define STATIC_DEBUG_WINDOW_HEIGH			(100)
#define	STATIC_DEBUG_WINDOW_STYLE			(WS_CHILD | WS_VISIBLE  | BS_FLAT | ES_MULTILINE)

typedef struct myControl
{
	HWND hWnd;

	LPCWSTR dwExStyle;
	LPCWSTR lpClassName;
	DWORD dwStyle;
	int x;
	int y;
	int nWidth;
	int nHeigh;
	HWND hWndParent;
	HMENU hMenu;
	HINSTANCE hInstance;
	LPVOID lpParam;

	int (*InitHandler)(struct myControl* control);
	int (*MessagHeandler)(struct myControl* control, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int (*ControlSetValue)(struct myControl *control, int type, void *arg, int len);
	int (*ControlSetEnable)(struct myControl *control, int en);

}MyControl_t;

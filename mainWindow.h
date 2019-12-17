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
 * ES_AUTOHSCROLL���Զ�����ˮƽ�������� ���û�����β����һ���ַ�ʱ�����Ľ��Զ����ҹ���10���ַ������û����س���ʱ���������ǹ�����ߡ�
 * ES_AUTOVSCROLL ���û������һ���ɼ��а��س���ʱ���������Ϲ���һҳ��
 * ES_MULTILINE ��ָ���༭�ؼ��ܷ���ж����ı��༭�������б༭����ָ��ES_AUTOHSCROLL�������Զ����У��û�ÿ��һ��Enter���ڵ�ǰ��괦����һ���س����з�(0Dh,0Ah)���ı�����һ�С�����ָ��ES_AUTOVSCROLL������б༭�����ڴ���������װ��ʱ�����������졣
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

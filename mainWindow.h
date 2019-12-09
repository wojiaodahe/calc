#pragma once


enum
{
	MAIN_ID = 0,

	STATIC_LAB_MULTICAST_IP_ID,
	EDIT_IMAGE_SELECT_ID,
	BUTTON_IMAGE_SELECT_ID,

};

#define COMTROL_INTERVAL_X					10
#define COMTROL_INTERVAL_Y					20

#define COMMON_CONTROL_WIDTH				120
#define COMMON_CONTROL_HEIGH				20

#define MAIN_WINDOW_WIDTH					950
#define MAIN_WINDOW_HEIGH					450

#define EDIT_IMAGE_SELECT_X					(30)
#define EDIT_IMAGE_SELECT_Y					(50)
#define EDIT_IMAGE_SELECT_WIDTH				(COMMON_CONTROL_WIDTH * 4)
#define EDIT_IMAGE_SELECT_HEIGH				(COMMON_CONTROL_HEIGH * 5)
#define	EDIT_IMAGE_SELECT_STYLE				(WS_CHILD | WS_VISIBLE | WS_BORDER | BS_RIGHT)

#define BUTTON_IMAGE_SELECT_X				(EDIT_IMAGE_SELECT_X +  EDIT_IMAGE_SELECT_WIDTH + COMTROL_INTERVAL_X)
#define BUTTON_IMAGE_SELECT_Y				(EDIT_IMAGE_SELECT_Y)
#define BUTTON_IMAGE_SELECT_WIDTH			(COMMON_CONTROL_WIDTH / 2)
#define BUTTON_IMAGE_SELECT_HEIGH			(EDIT_IMAGE_SELECT_HEIGH)
#define BUTTON_IMAGE_SELECT_STYLE			(WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT)

#define STATIC_LAB_MULTICAST_IP_X			(EDIT_IMAGE_SELECT_X)
#define STATIC_LAB_MULTICAST_IP_Y			(EDIT_IMAGE_SELECT_Y + EDIT_IMAGE_SELECT_HEIGH + COMTROL_INTERVAL_Y)
#define STATIC_LAB_MULTICAST_IP_WIDTH		(COMMON_CONTROL_WIDTH * 3)
#define STATIC_LAB_MUMTICAST_IP_HEIGH		(COMMON_CONTROL_HEIGH * 2)
#define	STATIC_LAB_MULTICAST_IP_STYLE		(WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT)


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

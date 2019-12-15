#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

// C RunTime Header Files
#include <math.h>
#include <objbase.h>
#include <Commdlg.h>
#include <stdio.h>
#include <WinUser.h>
#include <Iphlpapi.h>

#include "mainWindow.h"

extern int calc(char* str, char * result);
extern int GetNetCardInfo(void (*CallBack)(void* obj, void* arg), void* obj);
TCHAR* CharToTchar(TCHAR* dest, const char* src, int strLen, int tcharLen);
char* TcharToChar(char* dest, TCHAR* src, int len, int tcharLen);
MyControl_t* GetControlUseId(int ctlId);
void ControlSetValue(int ctlId, int valueType, void* value, int len);


int HandlerSetEn(MyControl_t *ctl, int en)
{
	Edit_Enable(ctl->hWnd, en);

	return 0;
}


int  HandlerImageSelectButton(struct myControl* control, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

int HandlerEditImageSelectPreInit(struct myControl* control)
{
	SendMessage(control->hWnd, EM_SETLIMITTEXT, (1024 * 1024 * 32), 0);

	SetFocus(control->hWnd);

	return 0;
}

int HandlerImageFileVersionPreInit(struct myControl* control)
{
	Edit_SetReadOnly(control->hWnd, 1);

	return 0;
}

void GetMulticastIp(TCHAR *ip)
{
	MyControl_t* control;

	control = GetControlUseId(EDIT_IMAGE_SELECT_ID);
	if (!control)
		return;

	GetWindowText(control->hWnd, ip, 512);
}

int HandlerMulticastIpPreInit(struct myControl *control)
{

	LPCWSTR defaultIp = (LPCWSTR)L"224.0.0.119";

	SetWindowText(control->hWnd, defaultIp);

	return 0;
}

int HandlerEditSetValue(struct myControl* control, int type, void* arg, int len)
{
	SetWindowText(control->hWnd, (LPCWSTR)arg);
	return 0;
}

MyControl_t Controls[] =
{
	{NULL, TEXT("Win32Demo"), TEXT("Calculator"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGH, NULL, (HMENU)MAIN_ID, 0, NULL, NULL},

	{
		NULL, TEXT("edit"), TEXT(""),
		STATIC_LAB_MULTICAST_IP_STYLE,
		STATIC_LAB_MULTICAST_IP_X,
		STATIC_LAB_MULTICAST_IP_Y,
		STATIC_LAB_MULTICAST_IP_WIDTH,
		STATIC_LAB_MUMTICAST_IP_HEIGH, NULL, (HMENU)STATIC_LAB_MULTICAST_IP_ID, 0, NULL, 
		HandlerEditImageSelectPreInit,
		NULL,
		HandlerEditSetValue,
		NULL
	},

	{
		NULL, TEXT("edit"), TEXT(""),
		EDIT_IMAGE_SELECT_STYLE,
		EDIT_IMAGE_SELECT_X,
		EDIT_IMAGE_SELECT_Y,
		EDIT_IMAGE_SELECT_WIDTH,
		EDIT_IMAGE_SELECT_HEIGH, NULL, (HMENU)EDIT_IMAGE_SELECT_ID, 0, NULL, 
		HandlerEditImageSelectPreInit,
		NULL,
		HandlerEditSetValue,
		NULL
	},
#if 0
	{
		NULL, TEXT("button"), TEXT("C"),
		BUTTON_IMAGE_SELECT_STYLE,
		BUTTON_IMAGE_SELECT_X,
		BUTTON_IMAGE_SELECT_Y,
		BUTTON_IMAGE_SELECT_WIDTH,
		BUTTON_IMAGE_SELECT_HEIGH, NULL, (HMENU)BUTTON_IMAGE_SELECT_ID, 0, NULL,
		NULL, 
		HandlerImageSelectButton,
		NULL,
		HandlerSetEn
	},
#endif
};

int GetCountOfControls(void)
{
	return sizeof(Controls) / sizeof(Controls[0]);
}

MyControl_t* GetControlUseId(int ctlId)
{
	int i;

	for (i = 0; i < GetCountOfControls(); i++)
	{
		if (Controls[i].hMenu == (HMENU)ctlId)
			return &Controls[i];
	}

	return NULL;
}

void ControlSetValue(int ctlId, int valueType, void* value, int len)
{
	int i;
	
	for (i = 0; i < GetCountOfControls(); i++)
	{
		if (Controls[i].hMenu == (HMENU)ctlId && Controls[i].ControlSetValue)
		{
			Controls[i].ControlSetValue(&Controls[i], valueType, value, len);
			break;
		}
	}
}

void ControlSetEnable(HMENU ctlId)
{
	int i;

	for (i = 0; i < GetCountOfControls(); i++)
	{
		if (Controls[i].hMenu == ctlId && Controls[i].ControlSetEnable)
			Controls[i].ControlSetEnable(&Controls[i], 1);
	}
}

void ControlSetDisable(HMENU CtlId)
{
	int i;

	for (i = 0; i < GetCountOfControls(); i++)
	{

		if (Controls[i].hMenu == CtlId && Controls[i].ControlSetEnable)
		{
			Controls[i].ControlSetEnable(&Controls[i], 0);
			break;
		}
	}
}

#define MAX_STRING_LEN	(1024 * 8)

static char  resultStr[MAX_STRING_LEN];
static char  expStr[MAX_STRING_LEN];
static TCHAR expTchar[MAX_STRING_LEN];
static char History[MAX_STRING_LEN];

int EnterKeyHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND focus;

	static int result = 0;
	static int lineCount;
	MyControl_t* editControl;
	MyControl_t* resultControl;

	focus = GetFocus();

	editControl = GetControlUseId(EDIT_IMAGE_SELECT_ID);
	if (!editControl)
		return -1;

	resultControl = GetControlUseId(STATIC_LAB_MULTICAST_IP_ID);
	if (!resultControl)
		return -1;

	lineCount = Edit_GetLineCount(editControl->hWnd);
	Edit_GetLine(editControl->hWnd, lineCount - 1, (LPARAM)expTchar, 512);

	memset(resultStr, 0, sizeof (resultStr));
	TcharToChar(expStr, expTchar, MAX_STRING_LEN, MAX_STRING_LEN);

	result = calc(expStr, resultStr);
	memset(expTchar, 0, sizeof (expTchar));
	strcat(History, resultStr);
	CharToTchar(expTchar, History, strlen(History), MAX_STRING_LEN);

	ControlSetValue(STATIC_LAB_MULTICAST_IP_ID, 0, expTchar, 0);
	Edit_Scroll(resultControl->hWnd, 2000, 0);

	return 0;
}

void HandleSelfMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR tmp[512];

	switch (message)
	{
	case WM_SELF_KEY_ENTER_DOWN:
		EnterKeyHandler(hWnd, message, wParam, lParam);
		break;
	}
}

void HandleCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	int wmId;
	int wmEvent;

	wmId = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	
	if (If_Self_Msg(message))
	{
		HandleSelfMsg(hWnd,  message, wParam, lParam);
		return;
	}
	else
	{
		for (i = 0; i < GetCountOfControls(); i++)
		{
			if (Controls[i].hMenu == (HMENU)wmId && Controls[i].MessagHeandler)
			{
				Controls[i].MessagHeandler(&Controls[i], hWnd, message, wParam, lParam);
				return;
			}
		}
	}

	DefWindowProc(hWnd, message, wParam, lParam);
}

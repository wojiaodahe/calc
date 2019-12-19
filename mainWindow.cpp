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

#include "history.h"
#include "mainWindow.h"
#include "calc.h"

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
		NULL, TEXT("static"), TEXT(""),
		STATIC_DEBUG_WINDOW_STYLE,
		STATIC_DEBUG_WINDOW_X,
		STATIC_DEBUG_WINDOW_Y,
		STATIC_DEBUG_WINDOW_WIDTH,
		STATIC_DEBUG_WINDOW_HEIGH, NULL, (HMENU)STATIC_DEBUG_WINDOW_ID, 0, NULL,
		HandlerEditImageSelectPreInit,
		NULL,
		HandlerEditSetValue,
		NULL
	},
#endif
};

void debugPrint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MyControl_t* control;
	TCHAR buff[128];

	wsprintf(buff, TEXT("msg: %lx wParam: %lx lParam: %lx"), message, wParam, lParam);

	control = GetControlUseId(STATIC_DEBUG_WINDOW_ID);
	if (!control)
		return;

	SetWindowText(control->hWnd, buff);
}

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



void SetLastLineText(LPCWSTR str)
{
	unsigned int lineIndex;
	unsigned int lineEnd;
	MyControl_t* editControl;

	editControl = GetControlUseId(EDIT_IMAGE_SELECT_ID);
	if (!editControl)
		return;

	SendMessage(editControl->hWnd, EM_SETSEL, -2, -1);//光标设置到最后一行
	lineIndex = SendMessage(editControl->hWnd, EM_LINEINDEX, -1, 0);//最后光标所在行行首的位置
	lineEnd = SendMessage(editControl->hWnd, EM_LINELENGTH, -1, 0);//最后光标所在行长度
	lineEnd = lineIndex + lineEnd;//获取最后一行行末位置

	SendMessage(editControl->hWnd, EM_SETSEL, lineIndex, lineEnd); //选定最后一行
	SendMessage(editControl->hWnd, WM_CUT, 0, 0);//剪切掉

	SendMessage(editControl->hWnd, EM_REPLACESEL, 0, (LPARAM)str);//追加在最后一行上面
}

static char saveFlag = 0;
static char lastLine[MAX_STRING_LEN] = { 0 };

void lastLineSaveLock(void)
{
	saveFlag = 1;
}

void lastLineSaveUnlock(void)
{
	saveFlag = 0;
}

char lastLineGetlock(void)
{
	return saveFlag;
}


int checkValidLine(char *str)
{

	/* 这看似很SB的功能是因为一行里啥都没写 在获取到这一行之后 进行TCHAR转char的时候还是能转出一些玩意儿来,
	 * 这些东西就下面比较的这些玩意儿, 不了解TCHAR, 也不想了解了*/
	if (str[0] == -30 && str[1] == -128 && str[2] == -128 && str[3] == 0)
		return 0;

	return 1;
}

void saveLastLine(void)
{
	TCHAR last[MAX_STRING_LEN];
	int lineCount;
	MyControl_t* editControl;

	if (!lastLineGetlock())
	{
		editControl = GetControlUseId(EDIT_IMAGE_SELECT_ID);
		if (!editControl)
			return;

		lineCount = Edit_GetLineCount(editControl->hWnd);
		Edit_GetLine(editControl->hWnd, lineCount - 1, (LPARAM)last, MAX_STRING_LEN);

		TcharToChar(lastLine, last, MAX_STRING_LEN, MAX_STRING_LEN);
		
		/* 这看似多余的功能是因为一行里啥都没写 在获取到这一行之后 进行TCHAR转char的时候还是能转出一些东西来, 
		 * 这些东西就是 checkValidLine() 里面的那些东西, 不了解TCHAR, 也不想了解了*/
		if (!checkValidLine(lastLine))
			memset(lastLine, 0, sizeof (lastLine));

		lastLineSaveLock();
	}
}

void recoverLastLine(void)
{
	TCHAR last[MAX_STRING_LEN] = {0};
	
	if (strlen(lastLine))
		CharToTchar(last, lastLine, strlen(lastLine), MAX_STRING_LEN);
	
	SetLastLineText(last);
}

void upKeyHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int lineCount;
	char* str;
	static TCHAR Tchar[MAX_STRING_LEN];

	str = getHistoryPrev();
	if (!str)
		return;
	
	saveLastLine();

	memset(Tchar, 0, sizeof (Tchar));
	CharToTchar(Tchar, str, strlen(str), MAX_STRING_LEN);
	SetLastLineText(Tchar);

	//保存当前最后一行
	//获取历史
	//显示历史在最后一行
}

void downKeyHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char* str;
	static TCHAR Tchar[MAX_STRING_LEN];
	
	str = getHistoryNext();
	if (!str)
	{
		recoverLastLine();
		lastLineSaveUnlock();
		return;
	}

	memset(Tchar, 0, sizeof(Tchar));
	CharToTchar(Tchar, str, strlen(str), MAX_STRING_LEN);
	SetLastLineText(Tchar);
}

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
	lastLineSaveUnlock();

	editControl = GetControlUseId(EDIT_IMAGE_SELECT_ID);
	if (!editControl)
		return -1;

	resultControl = GetControlUseId(STATIC_LAB_MULTICAST_IP_ID);
	if (!resultControl)
		return -1;

	memset(resultStr, 0, sizeof(resultStr));
	memset(expTchar, 0, sizeof(expTchar));

	lineCount = Edit_GetLineCount(editControl->hWnd);
	Edit_GetLine(editControl->hWnd, lineCount - 1, (LPARAM)expTchar, MAX_STRING_LEN);

	TcharToChar(expStr, expTchar, MAX_STRING_LEN, MAX_STRING_LEN);

	if (!checkValidLine(expStr))
		return -1;

	result = calc(expStr, resultStr);
	memset(expTchar, 0, sizeof(expTchar));
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
	case WM_SELF_KEY_UP_PRESS:
		upKeyHandler(hWnd, message, wParam, lParam);
		break;
	case WM_SELF_KEY_DOWN_PRESS:
		downKeyHandler(hWnd, message, wParam, lParam);
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

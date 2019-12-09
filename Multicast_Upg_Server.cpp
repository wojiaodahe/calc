﻿#include <windows.h>
#include <CommCtrl.h>

// C RunTime Header Files
#include <math.h>
#include <objbase.h>

#include "mainWindow.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitColtrols(void);
extern int GetCountOfControls(void);
extern void HandleCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static HINSTANCE hInst;
static TCHAR szClassName[] = TEXT("Win32Demo");  //窗口类名


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow
)
{
	HWND     hwnd;  //窗口句柄
	MSG      msg;  //消息
	WNDCLASS wndclass;  //窗口类

	hInst = hInstance;

	/**********第①步：注册窗口类**********/
	//为窗口类的各个字段赋值
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  //窗口风格
	wndclass.lpfnWndProc = WndProc;  //窗口过程
	wndclass.cbClsExtra = 0;  //暂时不需要理解
	wndclass.cbWndExtra = 0;  //暂时不需要理解
	wndclass.hInstance = hInstance;  //当前窗口句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //窗口图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  //鼠标样式
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);;  //窗口背景画刷（白色）
	wndclass.lpszMenuName = NULL;  //窗口菜单
	wndclass.lpszClassName = szClassName;  //窗口类名
	//注册窗口
	RegisterClass(&wndclass);

	InitColtrols();

	/**********第③步：消息循环**********/
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);  //翻译消息
		DispatchMessage(&msg);  //分派消息
	}

	return msg.wParam;
}

HWND CreateControl(MyControl_t *ctl)
{
	ctl->hWnd = CreateWindow(
				ctl->dwExStyle, 
				ctl->lpClassName,
				ctl->dwStyle,
				ctl->x /*X坐标*/, ctl->y /*Y坐标*/, ctl->nWidth /*宽度*/, ctl->nHeigh/*高度*/,
				ctl->hWndParent, ctl->hMenu /*控件唯一标识符*/, ctl->hInstance, ctl->lpParam);

	if (ctl->InitHandler)
		ctl->InitHandler(ctl);

	return ctl->hWnd;
}
extern MyControl_t Controls[];
void InitColtrols(void)
{
	int i;

	for (i = 0; i < GetCountOfControls(); i++)
	{ 
		Controls[i].hInstance	= hInst;
		Controls[i].hWndParent	= Controls[MAIN_ID].hWnd;
		Controls[i].lpParam		= &Controls[i];

		CreateControl(&Controls[i]);
		
		//显示窗口
		ShowWindow(Controls[i].hWnd, 1);

		//更新（绘制）窗口
		UpdateWindow(Controls[i].hWnd);
	}
}

//窗口过程
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
) {
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HFONT hFont;
	static HWND hBtn;

	switch (message) 
	{
	case  WM_CREATE:
		//创建逻辑字体
		hFont = CreateFont(
			-15/*高度*/, -7.5/*宽度*/, 0, 0, 400 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET,  //使用默认字符集
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
			DEFAULT_QUALITY,  //默认输出质量
			FF_DONTCARE,  //不指定字体族*/
			TEXT("微软雅黑")  //字体名
		);

		//SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		HandleCommand(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:
		//请做好善后工作
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
} 
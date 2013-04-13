#include <windows.h>
#include "processEnumWin.h"
#include <stdio.h>
#include <conio.h>
#include <tlhelp32.h> // 声明快照函数的头文件
int WINAPI WinMain(HINSTANCE hInstance,                  // 入口函数
				   HINSTANCE,
				   LPSTR           lpCmdLine,
				   int                   nCmdShow )
{
	MSG msg;
	if (!InitApplication(hInstance))       // 应用初始化
		return FALSE;

	if (!InitInstance(hInstance,nCmdShow)) // 实例初始化
		return FALSE;

	
	while (GetMessage(&msg, NULL, 0, 0))   // 消息循环
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

BOOL InitApplication(HINSTANCE hInstance)   // 应用初始化
{
    WNDCLASS  wc;  // Data structure of the window class

    wc.style            = CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc      = (WNDPROC)MainWndProc;  // Name of the Window Function 
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = TEXT("My1stWClass");  // Name of the window class

    return RegisterClass(&wc);
}

//***************************************************************************************

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)  // 实例初始化
{
    HWND hWnd = CreateWindow(TEXT("My1stWClass"),     // Name of the window class
                                                         TEXT("My First Window"),  // Title of the window
                                                         WS_OVERLAPPEDWINDOW,
                                                         CW_USEDEFAULT,
                                                         CW_USEDEFAULT,
                                                         CW_USEDEFAULT,
                                                         CW_USEDEFAULT,
                                                         NULL,
                                                         NULL,
                                                         hInstance,
                                                         NULL                                        );
    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

// 窗口过程函数
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	
	PAINTSTRUCT ps;
	HDC hdc;
	PROCESSENTRY32 pe32;

	// 将进程信息存到缓冲区
	LPTSTR buffer=new TCHAR[1024];
	int xLocation = 0;
	int yLocation = 0;
	// 给系统内的所有进程拍快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// 在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32); 
	if(hProcessSnap == INVALID_HANDLE_VALUE){       
		//printf(" CreateToolhelp32Snapshot调用失败！ \n");     
		return -1;    
	}
	// 遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	LPTSTR title = TEXT("进程ID    模块名");
	switch (message) {

	case WM_PAINT:  // 窗口客户区得刷新

		hdc = BeginPaint (hWnd, &ps);
		
		for (int i = 0; i < 3; ++i)
		{
			TextOut(hdc,20+i*350,10,title,lstrlen(title));
		}
		while(bMore){
			wsprintf(buffer,L"%05u    %s",pe32.th32ProcessID,pe32.szExeFile);
			if( xLocation < 1000){ //每行输出3个进程信息
				TextOut(hdc,20+xLocation,30+yLocation,buffer,lstrlen(buffer));
				xLocation += 350;
			}
			else{
				yLocation += 20;
				TextOut(hdc,20,30+yLocation,buffer,lstrlen(buffer));
				xLocation = 350;
			}
			bMore = Process32Next(hProcessSnap, &pe32);
			
		}
		EndPaint (hWnd, &ps);
		CloseHandle(hProcessSnap); // 清除snapshot对象
		getch();
		return 0;

	case WM_DESTROY: // 窗口关闭

		PostQuitMessage(0);

		return 0;

	default:  // 缺省消息的处理

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

}
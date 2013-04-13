#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

TCHAR szAppName[] = TEXT("lab2") ;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   PSTR szCmdLine, int iCmdShow)
{
	HACCEL   hAccel ;
	HWND     hwnd ;
	MSG      msg ;
	int		 numberOfAccelerator;
	ACCEL	 arrayAccel[ 20 ];
	WNDCLASS wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICON1)) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = (LPCTSTR)IDR_MENU1 ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
		return 0 ;

	hwnd = CreateWindow (szAppName, TEXT ("lab2"),
						 WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT, CW_USEDEFAULT,
						 CW_USEDEFAULT, CW_USEDEFAULT,
						 NULL, NULL, hInstance, NULL) ;

	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	
	// 为file菜单创建动态加速键
	numberOfAccelerator = CopyAcceleratorTable(hAccel, NULL, 0);
	CopyAcceleratorTable( hAccel, arrayAccel, numberOfAccelerator );
	arrayAccel [ numberOfAccelerator ].cmd	 = IDM_APP_EXIT;
	arrayAccel [ numberOfAccelerator ].fVirt = FCONTROL | FSHIFT | FVIRTKEY;
	arrayAccel [ numberOfAccelerator ].key   = VK_DELETE ;
	++numberOfAccelerator;
	DestroyAcceleratorTable(hAccel);
	hAccel = CreateAcceleratorTable( arrayAccel, numberOfAccelerator );

	while (GetMessage (&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator (hwnd, hAccel, &msg))
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
	}
	DestroyAcceleratorTable(hAccel);
	return msg.wParam ;
}




LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCTSTR iconStr[] = { TEXT("当前使用的图标是：图标1"), 
						  TEXT("当前使用的图标是：图标2"),
						  TEXT("当前使用的图标是：图标3") }; 
	LPCTSTR show[] = { TEXT("显示1"),TEXT("显示2"),TEXT("显示3"),TEXT("显示4") };
	static int iconSet[] = { IDI_ICON1, IDI_ICON2, IDI_ICON3 };
	PAINTSTRUCT ps;
	HDC hdc;
	static HINSTANCE hInstance;
	HMENU hSubMenu ; //File菜单

	//获得当前的图标，以便在icon3时灰化四个show，
	//同时可以避免选择已选定的图标时仍弹出对话框
	static DWORD iconMenuId ; 
	int messageId = LOWORD( wParam ) ;
	//messageId代替后面的command消息中的loword(wParam),保证加速键运行正常

	HMENU      hMenu ;

	switch (message)
	{
	//调用了create就不要再在initmenu调用相同的建立菜单，否则会导致子菜单不可用
	case WM_CREATE : 
		hInstance = ((LPCREATESTRUCT) lParam)->hInstance; // ！！！获得应用程序的实例句柄
		hMenu = GetMenu(hwnd);
		// 动态加入file菜单
		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu,MF_STRING,IDM_APP_EXIT,TEXT("&Exit\tCtrl+Shift+Delete"));
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_POPUP , (UINT)hSubMenu,TEXT("&File"));
		iconMenuId = ID_ICON_ONE;
		CheckMenuRadioItem(hMenu, ID_ICON_ONE,  ID_ICON_THREE, 
			ID_ICON_ONE, MF_BYCOMMAND);  //默认选择图标1
		return 0 ;

	case WM_PAINT:
		hMenu = GetMenu (hwnd) ;
		hdc = BeginPaint(hwnd, &ps);
		if( iconMenuId != ID_ICON_THREE )//当图标不是3时方可显示四个show
		{
			for( messageId = ID_SHOW_ONE; messageId <= ID_SHOW_FOUR; messageId++ )
			{
				if( GetMenuState(hMenu,messageId,MF_BYCOMMAND) & MF_CHECKED )
					TextOut(hdc, (messageId - ID_SHOW_ONE)*60+150,200,
						show[messageId - ID_SHOW_ONE],lstrlen(show[messageId - ID_SHOW_ONE]));
			}
		}

		for (messageId = ID_ICON_ONE; messageId <= ID_ICON_THREE; messageId++ )
		{
			if( GetMenuState(hMenu, messageId, MF_BYCOMMAND) & MF_CHECKED ){
				TextOut(hdc,100, 100, iconStr[messageId - ID_ICON_ONE], 
					lstrlen(iconStr[messageId - ID_ICON_ONE]));
			}
		}
		
		EndPaint (hwnd, &ps);
		return 0;

	case WM_COMMAND:
		hMenu = GetMenu (hwnd) ;

		switch (messageId)
		{
		case IDM_APP_EXIT:
			SendMessage (hwnd, WM_CLOSE, 0, 0) ;
			return 0 ;
			
		case ID_ICON_ONE:  //将要显示的内容用数组表示可以方便地合并这几个case，
		case ID_ICON_TWO:  //只需传递messageId参数即可
		case ID_ICON_THREE:
			
			if( (messageId != iconMenuId) && (MessageBox(hwnd,TEXT("确定要修改吗？"),
				TEXT("Confirmation"),MB_YESNO|MB_ICONQUESTION) == IDYES)){

					CheckMenuRadioItem(hMenu, ID_ICON_ONE,  ID_ICON_THREE, 
						messageId, MF_BYCOMMAND);
					SetClassLong(hwnd,GCL_HICON,(long)LoadIcon(hInstance,
						MAKEINTRESOURCE( iconSet[messageId - ID_ICON_ONE])));
					iconMenuId = messageId ; // which made icon3 do not show 1,2,3,4
					if( messageId == ID_ICON_THREE )
					{
						EnableMenuItem(hMenu,ID_SHOW_ONE,MF_GRAYED);
						EnableMenuItem(hMenu,ID_SHOW_TWO,MF_GRAYED);
						EnableMenuItem(hMenu,ID_SHOW_THREE,MF_GRAYED);
						EnableMenuItem(hMenu,ID_SHOW_FOUR,MF_GRAYED);
					}
					else
					{
						EnableMenuItem(hMenu,ID_SHOW_ONE,MF_ENABLED);
						EnableMenuItem(hMenu,ID_SHOW_TWO,MF_ENABLED);
						EnableMenuItem(hMenu,ID_SHOW_THREE,MF_ENABLED);
						EnableMenuItem(hMenu,ID_SHOW_FOUR,MF_ENABLED);
					}
					InvalidateRect (hwnd, NULL, TRUE);
			}
			return 0;

		case ID_SHOW_ONE:
		case ID_SHOW_TWO:
		case ID_SHOW_THREE:
		case ID_SHOW_FOUR:
			// 使得四个show可以选择显示与否
			if (GetMenuState(hMenu,messageId,MF_BYCOMMAND) & MF_CHECKED)
				CheckMenuItem(hMenu,messageId,MF_UNCHECKED);
			else
				CheckMenuItem(hMenu,messageId,MF_CHECKED);
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case IDM_APP_ABOUT:
			MessageBox(hwnd,
			 TEXT("第二次上机练习\n图标、菜单、加速键、消息框\n\n学号：10112130261\n姓名：杨利民"),
			 TEXT("lab2"),MB_ICONINFORMATION);
			return 0;
		}
	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

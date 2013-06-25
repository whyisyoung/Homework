#include <windows.h>
#include <commdlg.h>
#include "resource.h"

// bug : 程序运行一段时间后菜单出现黑框，甚至无法再选择菜单。

LRESULT CALLBACK       WndProc( HWND, UINT, WPARAM, LPARAM);

BOOL	CALLBACK     ModeDlgProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK ModelessDlgProc( HWND, UINT, WPARAM, LPARAM );

TCHAR szAppName[] = TEXT("lab3");
HWND hDlgModeless = 0;

BOOL bmodeCheck[]	  = { FALSE, FALSE, FALSE };// 模式对话框 复选位图是否checked
BOOL bModelessCheck[] = { TRUE, FALSE, FALSE }; // 无模式对话框 单选位图是否checked,默认选中1

TCHAR szBuffer[20];// 窗口名字

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR szCmdLine, int iCmdShow)
{
	HACCEL   hAccel ;
	HWND     hwnd ;
	MSG		 msg;
	WNDCLASS wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON1) );
	wndclass.hCursor       = LoadCursor ( NULL, IDC_ARROW ) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wndclass.lpszMenuName  = (LPCTSTR)IDR_MENU;
	wndclass.lpszClassName = szAppName ;
	
	if( !RegisterClass( &wndclass ) )
		return 0;
	
	LoadString( hInstance, IDS_APPNAME_ZH, szBuffer, 20);
	hwnd = CreateWindow ( szAppName, szBuffer,
						  WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  NULL, NULL, hInstance, NULL );
	ShowWindow( hwnd, iCmdShow );
	UpdateWindow( hwnd );

	hAccel = LoadAccelerators( hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	while (GetMessage( &msg, NULL, 0, 0))
	{
		if( ( !IsWindow(hDlgModeless) || 
			  !IsDialogMessage(hDlgModeless,&msg) ) && 
			( !TranslateAccelerator(hwnd, hAccel, &msg)) )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HMENU hMenu;
	static HINSTANCE hInstance;
	int messageId = LOWORD( wParam );
	PAINTSTRUCT ps;
	HDC hdc, hdcMem;
	HCURSOR hCursor[3];
	static int cursorSelector = 0;
	static int stringSelector = 0;
	OPENFILENAME ofn;
	char szFile[ 260 ] = "" ;//获取文件名


	// 文字说明当前使用的光标
	TCHAR cursorStr[3][40];
	LoadString( hInstance, IDS_ARROW_ZH+stringSelector, cursorStr[0], 40);
	LoadString( hInstance, IDS_CROSS_ZH+stringSelector, cursorStr[1], 40);
	LoadString( hInstance, IDS_DEFINE_ZH+stringSelector, cursorStr[2], 40);
	hCursor[0] = LoadCursor( NULL, IDC_ARROW );
	hCursor[1] = LoadCursor( NULL, IDC_CROSS );
	hCursor[2] = LoadCursor( hInstance, MAKEINTRESOURCE(IDC_MYCURSOR) );

	static HBITMAP hBitmap[3] ;

	hBitmap [0] = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_BITMAP1) );
	hBitmap [1] = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_BITMAP2) );
	hBitmap [2] = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_BITMAP3) );

	BITMAP bitmap;
	switch (message)
	{
	case WM_CREATE:
		hMenu = GetMenu( hwnd );
		hInstance = ((LPCREATESTRUCT) lParam) -> hInstance;
		CheckMenuRadioItem( hMenu, ID_CURSOR_ARROW, ID_CURSOR_DEFINE, 
			ID_CURSOR_ARROW, MF_BYCOMMAND );
		CheckMenuRadioItem( hMenu, ID_LANG_ZH, ID_LANG_EN, ID_LANG_ZH,MF_BYCOMMAND );
		return 0;

	case WM_COMMAND:
		hMenu = GetMenu( hwnd );
		
		switch ( messageId )
		{
		case ID_EXIT:
			SendMessage( hwnd, WM_CLOSE, 0, 0);
			return 0;

		case ID_DIALOG_MODE:
			DialogBox( hInstance, MAKEINTRESOURCE(IDD_DIALOGMODE), hwnd, (DLGPROC)ModeDlgProc );
			InvalidateRect(hwnd,NULL,TRUE);
			return 0;

		case ID_DIALOG_MODELESS:
			if(!IsWindow(hDlgModeless)){
				hDlgModeless = CreateDialog( hInstance, 
							MAKEINTRESOURCE(IDD_DIALOGMODELESS), hwnd, (DLGPROC)ModelessDlgProc);
				ShowWindow( hDlgModeless, SW_SHOW );
			}
			return 0;

		case ID_DIALOG_FILE:
			ZeroMemory( &ofn, sizeof( OPENFILENAME ));
			ofn.lStructSize     = sizeof( OPENFILENAME );
			ofn.hwndOwner       = hwnd;
			ofn.lpstrFile       = (LPWSTR) szFile;
			ofn.nMaxFile        = sizeof(szFile);
			ofn.lpstrFilter     = TEXT( "All Files(*.*)\0*.*\0Python Source(*.py)\0*.PY\0C++ Files(*.cpp)\0*.CPP\0" );
			ofn.nFilterIndex    = 1;
			ofn.lpstrFileTitle  = NULL;
			ofn.nMaxFileTitle   = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags           = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
			if( GetOpenFileName(&ofn) )
				MessageBox( hwnd, (LPWSTR)szFile, TEXT("文件名"), MB_OK );
			return 0;

		case ID_CURSOR_ARROW:
		case ID_CURSOR_CROSS:
		case ID_CURSOR_DEFINE:
			cursorSelector = messageId - (int)ID_CURSOR_ARROW;
			CheckMenuRadioItem( hMenu, ID_CURSOR_ARROW, ID_CURSOR_DEFINE, 
				messageId, MF_BYCOMMAND );
			//SetCursor( hCursor[ cursorSelector ] ); 为什么SetCursor不行！！！！ WTF!
			SetClassLong( hwnd, GCL_HCURSOR, (long)hCursor[ cursorSelector ]);
			InvalidateRect( hwnd, NULL, TRUE );
			return 0;

		case ID_LANG_ZH:
		case ID_LANG_EN:
			stringSelector = messageId - ID_LANG_ZH;
			CheckMenuRadioItem( hMenu, ID_LANG_ZH, ID_LANG_EN,
					messageId, MF_BYCOMMAND );

			// 动态改变菜单
			hMenu = LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU+stringSelector));
			SetMenu( hwnd, hMenu );
			CheckMenuRadioItem( hMenu, ID_CURSOR_ARROW, ID_CURSOR_DEFINE, 
					ID_CURSOR_ARROW+cursorSelector, MF_BYCOMMAND );
			LoadString( hInstance, IDS_APPNAME_ZH+stringSelector, szBuffer, 20 );
			SetWindowText( hwnd, szBuffer );// 切换窗口中英文名字
			InvalidateRect( hwnd, NULL, TRUE );
			return 0;

		case ID_ABOUT:
			MessageBox( hwnd, 
				TEXT("第三次上机练习\n对话框、光标、字符串、位图\n\n学号：***\n姓名：***"),
				TEXT("lab3"), MB_ICONINFORMATION );
			return 0;
		}
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hMenu = GetMenu( hwnd );
		hdc = BeginPaint( hwnd, &ps );

		// 模式对话框  复选位图
		for( int id=0; id<=2; id++)
		{
			if ( bmodeCheck[ id ] )
			{
				GetObject( hBitmap[ id ] ,sizeof(BITMAP), &bitmap);
				hdcMem = CreateCompatibleDC( hdc );
				SelectObject( hdcMem, hBitmap[ id ]);
				BitBlt( hdc, 100,100+id*80, 48,48, hdcMem, 0,0,SRCCOPY );
				DeleteDC( hdcMem );
			}
		}


		//无模式对话框 单选位图
		for ( int id=0; id<=2; id++ )
		{
			if ( bModelessCheck[ id ])
			{
				GetObject( hBitmap[ id ], sizeof(BITMAP), &bitmap );
				hdcMem = CreateCompatibleDC( hdc );
				SelectObject( hdcMem, hBitmap[ id ]);
				BitBlt( hdc, 400,50,48,48, hdcMem, 0,0, SRCCOPY );
				DeleteDC( hdcMem );
			}
		}

		// 显示当前使用的图标信息
		for ( messageId = ID_CURSOR_ARROW; messageId <= ID_CURSOR_DEFINE; messageId++ )
		{
			if( GetMenuState( hMenu,messageId, MF_BYCOMMAND) & MF_CHECKED ){
				TextOut( hdc, 50, 50, cursorStr[ messageId - ID_CURSOR_ARROW ],
					lstrlen( cursorStr[ messageId - ID_CURSOR_ARROW ]) );
			}
		}

		DeleteDC( hdcMem );
		EndPaint ( hwnd, &ps );
		return 0;
	}

	return DefWindowProc ( hwnd, message, wParam, lParam );
}


BOOL CALLBACK ModeDlgProc( HWND hDlgMode, UINT message, WPARAM wParam, LPARAM lParam )
{
	int modeMessageId =  LOWORD(wParam);
	switch (message)
	{
	case WM_INITDIALOG:
		// 确保再次打开模式对话框时保留上次check的状态
		for (int i=IDC_CHECK1; i<=IDC_CHECK3; i++ )
		{
			if ( bmodeCheck[ i-IDC_CHECK1 ] )
				CheckDlgButton( hDlgMode, i, BST_CHECKED );
		}
		return TRUE;

	case WM_COMMAND:
		switch (modeMessageId)
		{
		case IDOK:
			InvalidateRect( GetParent( hDlgMode ), NULL, TRUE );
			EndDialog(hDlgMode,1);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlgMode,0);
			return TRUE;

		case IDC_CHECK1:
		case IDC_CHECK2:
		case IDC_CHECK3:
			if( bmodeCheck[ modeMessageId-IDC_CHECK1 ] )
				bmodeCheck[ modeMessageId-IDC_CHECK1 ] = FALSE;
			else
				bmodeCheck[ modeMessageId-IDC_CHECK1 ] = TRUE;
			return TRUE;
		}
	default:
		return FALSE;
	}
}

BOOL CALLBACK ModelessDlgProc( HWND hDlgModeless, UINT message, WPARAM wParam, LPARAM lParam )
{
	int modelessMessageId = LOWORD(wParam);
	switch (message)
	{
	case WM_INITDIALOG:
		for (int i=IDC_RADIO1; i<=IDC_RADIO3; i++ )
		{
			if ( bModelessCheck[ i-IDC_RADIO1 ] )
				CheckDlgButton( hDlgModeless, i, BST_CHECKED );
		}
		return TRUE;

	case WM_COMMAND:
		switch(modelessMessageId)
		{
		case IDOK:
			InvalidateRect( GetParent( hDlgModeless ), NULL, TRUE );
			return TRUE;

		case IDCANCEL:
			//此处需发送close消息否则点击取消后无法再打开无模式对话框
			SendMessage( hDlgModeless, WM_CLOSE, 0, 0 );
			EndDialog(hDlgModeless,1);
			return TRUE;
			
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
			CheckRadioButton( hDlgModeless, IDC_RADIO1, IDC_RADIO3, modelessMessageId );
			bModelessCheck [ modelessMessageId - IDC_RADIO1 ] = TRUE;
			for ( int id=0; id<=2; id++)
			{
				if( id != (modelessMessageId-IDC_RADIO1))
					bModelessCheck[ id ] = FALSE;
			}
			return TRUE;
		}

	case  WM_CLOSE:
		DestroyWindow( hDlgModeless );
		hDlgModeless = 0;
		break;
	default:
		return FALSE;
	}
}

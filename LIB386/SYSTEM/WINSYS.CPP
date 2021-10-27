//----------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\winsys.h>

//----------------------------------------------------------------------------
#define	STYLE_FULLSCREEN	WS_EX_APPWINDOW
#define	STYLE_WINDOW		WS_OVERLAPPEDWINDOW

//----------------------------------------------------------------------------
WINDOW_PROC	SystemWindowProc		;

//----------------------------------------------------------------------------
static long FAR PASCAL WrapWindowProc(	HWND hWnd, UINT message,
			   		WPARAM wParam, LPARAM lParam)	;

//----------------------------------------------------------------------------
S32		AppActive	= FALSE		;
S32		AppLive		= TRUE		;

//----------------------------------------------------------------------------
U32		AutoPlayMessage	= 0		;

//----------------------------------------------------------------------------
VOID_FUNC       *PauseMiles	= NULL		;
VOID_FUNC       *ResumeMiles	= NULL		;

//----------------------------------------------------------------------------
VOID_FUNC	*ManageProc	= NULL		;

//----------------------------------------------------------------------------
WINDOW_PROC	*WindowProc = SystemWindowProc	;

//----------------------------------------------------------------------------
HANDLE		hSemaphoreNoReenter		;

//----------------------------------------------------------------------------
HWND		hWnd				;

//----------------------------------------------------------------------------
static void TerminateWindow()
{
	AppLive = FALSE	;
	SendMessage(hWnd, WM_CLOSE, 0, 0);
	ManageEvents()	;
}

//----------------------------------------------------------------------------
void	InitWindow( HANDLE hinstance, int nCmdShow, char *title)
{
   	WNDCLASSEX	wc		;
	char		semname[_MAX_PATH];


	strcpy(semname, title);
	strcat(semname, "AdelineProgramSemaphore");

    	// Create or open a named semaphore.
        hSemaphoreNoReenter = CreateSemaphore(
				NULL,         // Security attributes
		         	0,            // Initial count
		                1,            // Maximum count
		                semname);     // Semaphore name

	// Close handle, and return NULL if existing semaphore opened.
        if (hSemaphoreNoReenter != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
        {
        	CloseHandle(hSemaphoreNoReenter);
                hWnd = FindWindow( "ADELINE", title ) ;
                if (hWnd)
                {
			SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		exit(0);
	}

   	//
   	// set up and register window class
   	//
	wc.cbSize	= sizeof(wc)			;
   	wc.lpszClassName= "ADELINE"			;
   	wc.style	= 0				;
   	wc.cbClsExtra	= 0				;
   	wc.cbWndExtra	= 0				;
   	wc.hInstance	= hinstance			;
//   	wc.hIcon	= LoadIcon( hinstance, MAKEINTRESOURCE(0) );
   	wc.hIcon	= LoadIcon( hinstance, "ICON" );
   	wc.hIconSm 	= NULL				;
/*
   	wc.hIconSm 	= LoadImage( hinstance,
//   				MAKEINTRESOURCE(0),
				"ICON",
   				IMAGE_ICON,
		        	GetSystemMetrics(SM_CXSMICON),
                		GetSystemMetrics(SM_CYSMICON),
                        	LR_DEFAULTCOLOR)	;
*/
   	wc.hCursor	= NULL				;
   	wc.hbrBackground= NULL				;
   	wc.lpszMenuName	= NULL				;
   	wc.lpfnWndProc	= WrapWindowProc		;

   	if(!RegisterClassEx( &wc ))
   	{
		exit(1)	;
   	}

   	/*
   	 * create a window
   	 */

   	hWnd = CreateWindowEx(
   	   	 WS_EX_APPWINDOW,
   	   	 "ADELINE",
   	   	 title,
   	   	 WS_POPUP,
   	   	 0, 0,
   	   	 GetSystemMetrics( SM_CXSCREEN ),
   	   	 GetSystemMetrics( SM_CYSCREEN ),
   	   	 NULL,
   	   	 NULL,
   	   	 hinstance,
   	   	 NULL );

   	if( !hWnd )
   	{
		exit(1);
   	}

	atexit(TerminateWindow)		;

	AutoPlayMessage = RegisterWindowMessage(TEXT("QueryCancelAutoPlay"));

   	ShowWindow( hWnd, nCmdShow )	;
   	UpdateWindow( hWnd )		;
}

//----------------------------------------------------------------------------
static S32 SystemWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD	dwstyle	;

	if(message == AutoPlayMessage)
	{
		return 1 		;
	}

	switch (message)
	{
	case WM_CLOSE:
		if(AppLive)
		{
			return 1	;
		}
		DestroyWindow(hWnd)	;
		break			;

	case WM_DESTROY:
	       	if(hSemaphoreNoReenter)
	       	{
	       		CloseHandle(hSemaphoreNoReenter);
	       		hSemaphoreNoReenter = NULL	;
	       	}
		PostQuitMessage( 0 )	;
		break			;

	case WM_SETFOCUS:
		dwstyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		dwstyle &= ~STYLE_WINDOW	;
		dwstyle |= STYLE_FULLSCREEN	;
		SetWindowLong(hWnd, GWL_EXSTYLE, dwstyle);

		AppActive = TRUE	;
		break			;

	case WM_KILLFOCUS:
		dwstyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		dwstyle &= ~STYLE_FULLSCREEN	;
		dwstyle |= STYLE_WINDOW		;
		SetWindowLong(hWnd, GWL_EXSTYLE, dwstyle);

		AppActive = FALSE	;
		break			;

	case WM_SETCURSOR:
		SetCursor(NULL)		;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam)	;
}

//----------------------------------------------------------------------------
static long FAR PASCAL WrapWindowProc(	HWND hWnd, UINT message,
			   		WPARAM wParam, LPARAM lParam)
{
	return	WindowProc(hWnd, message, wParam, lParam)	;
}

//----------------------------------------------------------------------------
void	ManageEvents()
{
	U32	milespaused = FALSE	;
	MSG	msg			;

	if(ManageProc)
	{
		ManageProc()	;
	}

	do
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				exit(0)		;
				return		;
			}
			TranslateMessage(&msg)	;
			DispatchMessage(&msg)	;
		}

		if(!AppActive)
		{
			if(PauseMiles&&(!milespaused))
			{
				milespaused	= TRUE	;
				PauseMiles()		;
			}
			WaitMessage()	;
		}

	} while(!AppActive)	;

	if(milespaused&&ResumeMiles)
	{
		ResumeMiles()	;
	}
}

//----------------------------------------------------------------------------

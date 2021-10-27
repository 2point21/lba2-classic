//----------------------------------------------------------------------------
#include	<system\adeline.h>
#include	<system\winsys.h>
#include	<system\keybwin.h>

//----------------------------------------------------------------------------
S32	AsciiMode = FALSE	;

//----------------------------------------------------------------------------
S32	Key			;

//----------------------------------------------------------------------------
U8	TabKeys[256+16*8]	;

//----------------------------------------------------------------------------
WINDOW_PROC_PTR	OldKeybWindowProc;

//----------------------------------------------------------------------------
#define	BUFFER_SIZE	20

//----------------------------------------------------------------------------
	HANDLE	AsciiMutex		;

static	S32	BufferKey[BUFFER_SIZE]	;
static	S32	StartKey = 0		;
static	S32	EndKey = 0		;

//----------------------------------------------------------------------------
void	ManageKeyboard()
{
	ManageEvents()	;
	GetKeyboardState(TabKeys);
}

//----------------------------------------------------------------------------
S32	KeybWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CHAR:
		if(AsciiMode)
		{
			U32	nextendkey	;

			WaitForSingleObject(AsciiMutex, INFINITE);

			nextendkey = EndKey+1	;
			if (nextendkey == BUFFER_SIZE)
			{
				nextendkey = 0 ;
			}

			if(nextendkey != StartKey)
			{
				char	dst[2], src[2]	;

				src[0] = (TCHAR)wParam	;
				src[1] = 0		;

				CharToOem(src, dst)	;

				BufferKey[EndKey] = dst[0] ;
				EndKey = nextendkey	   ;
			}

		        ReleaseMutex(AsciiMutex);
		}
		break	;

	case WM_KEYDOWN:
		if(!(lParam&0x40000000))
		{
			Key = wParam ;
		}
		goto endkey	;

	case WM_KEYUP:
		Key = 0		;
		goto endkey	;

        case WM_SYSKEYDOWN:
        case WM_SYSCHAR:
endkey:
	        {
			U32	lRet	;
			U32	dw	;
			// Fool DefWindowProc into thinking we do NOT have
			// a system menu.  Otherwise it will try to
			// pop up its own.
			dw = GetWindowLong( hWnd, GWL_STYLE ) 		;
			SetWindowLong( hWnd, GWL_STYLE, dw &~WS_SYSMENU );
			lRet = OldKeybWindowProc(hWnd, message, wParam, lParam)	;
			SetWindowLong( hWnd, GWL_STYLE, dw )		;
			return lRet 					;
	        }

	}

	return	OldKeybWindowProc(hWnd, message, wParam, lParam) ;
}

//----------------------------------------------------------------------------
S32	GetAscii()
{
	U32	ret = 0	;


	ManageEvents()	;

	WaitForSingleObject(AsciiMutex, INFINITE);

	if(EndKey != StartKey)
	{
		ret = BufferKey[StartKey++];
		if(StartKey == BUFFER_SIZE)
		{
			StartKey = 0 ;
		}
	}

        ReleaseMutex(AsciiMutex);

	return ret ;
}

//----------------------------------------------------------------------------
void	ClearAsciiBuffer()
{
	WaitForSingleObject(AsciiMutex, INFINITE);

	StartKey = EndKey = 0	;

        ReleaseMutex(AsciiMutex);
}

//----------------------------------------------------------------------------

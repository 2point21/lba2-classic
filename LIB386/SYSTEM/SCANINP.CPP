#include <system\adeline.h>

//***************************************************************************
U32     MakeMask(U32 val);

#pragma aux MakeMask =          \
"       bsf     ecx, eax"       \
"       mov     eax, 1"         \
"       shl     eax, cl"        \
        parm caller     [eax]   \
        modify          [ecx]

//***************************************************************************
U32     ScanInput(U32 **ptrinput, U32 *maskinput)
{
        U32     mask, mask0, mask1      ;
        U32     i                       ;
        U32     *ptr                    ;

	// Reset Keyboard (security)
	for(i=0; i<4; i++)      TabKeys[i] = 0 ;
	Key = MyKey = 0 ;

	for(;;)
	{
		MyKey = Key;
		if( MyKey == K_ESC ) return FALSE ;

		// Scan keyboard for an input
		ptr = (U32 *)TabKeys    ;
		for(i=4; i>0; i--, ptr++)
		{
			mask = *ptr             ;
			if(mask) goto exit      ;
		}

	}
exit:
	MyKey = Key;
	if( MyKey == K_ESC ) return FALSE ;

	// result
	*ptrinput       = ptr           ;
	*maskinput      = MakeMask(mask);

	// Ok
	return TRUE ;
}

//***************************************************************************
	U32	Input		;

//***************************************************************************
static	S32	NbKeys		;
static	U32	*InputKeys	;
static	U32	*InputMasks	;

//***************************************************************************
void	DefineInput(S32	nbkeymasks, U32 *keys, U32 *inputmasks)
{
	NbInputKeys 	= nbkeys	;
	KeyMasks	= keymasks	;
	InputMasks	= inputmasks	;
}

//***************************************************************************
void    GetInput()
{
	S32     i	;

	// reset Input
	Input = 0 	;

        // Rebuild Input
        for(i=0; i<NbKeys; i++)
        {
                if(CheckKey(InputKeys[i]))	Input |= InputMasks[i]	;
        }
}

//***************************************************************************

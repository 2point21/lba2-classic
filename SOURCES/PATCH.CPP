#include 	"c_extern.h"

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		   ÛßßßÛ ÛßßßÛ ßßÛßß Ûßßßß Û   Û Ûßßßß ÛÛßßß
		   ÛÛßßß ÛÛßßÛ   ÛÛ  ÛÛ    ÛÛßßÛ ÛÛßß  ßßßßÛ
		   ßß    ßß  ß   ßß  ßßßßß ßß  ß ßßßßß ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

extern	U8	*PtrCompil 	;	// COMPIL.CPP
extern	U8	*PtrSce 	;	// DISKFUNC.CPP

#ifdef	COMPILATOR
S32	MaxPatches = 0 ;
T_PATCH	ListPatches[MAX_PATCHES] ;
#else
T_PATCH *ListPatches ;
#endif

S32	NbPatches = 0 ;

#ifdef	COMPILATOR
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	AddPatch( S16 size, S16 offset )
{
	T_PATCH *ptrpatch ;

	if( NbPatches>=MAX_PATCHES )
	{
		Message( "Warning: ListPatches trop petit ...", TRUE ) ;
		return ;
	}

	ptrpatch = &ListPatches[NbPatches++] ;

	ptrpatch->Size   = size		;
	ptrpatch->Offset = (S16)((PtrSce-StartPtrSce)+(PtrCompil-(U8*)Screen)+offset) ;
}
#endif	// COMPILATOR

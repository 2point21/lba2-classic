#include 	"c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀█ █▀▀▀█ ▀▀█▀▀ █▀▀▀▀ █   █ █▀▀▀▀ ██▀▀▀
		   ██▀▀▀ ██▀▀█   ██  ██    ██▀▀█ ██▀▀  ▀▀▀▀█
		   ▀▀    ▀▀  ▀   ▀▀  ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

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
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
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

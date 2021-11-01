#include 	"c_extern.h"

S32	ValideCube		;

S32	ValidePos     = TRUE 	;
S32	LastValidePos = TRUE 	;

T_OBJET	ValideHero ;
U32	SizeOfBufferValidePos	;
U8	*BufferValidePos	;

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	SaveValidePos( void )
{
	T_OBJET	memoobj ;
	S32	timerrefhr = TimerRefHR ;
	T_OBJET	*ptrobj = &ListObjet[NUM_PERSO] ;

	if( ptrobj->LifePoint<=0 )	return ;

	ValideCube = NumCube  ;

	memcpy( &memoobj, ptrobj, sizeof(T_OBJET) ) ;
	memcpy( ptrobj, &ValideHero, sizeof(T_OBJET) ) ;

	PtrSave = BufferValidePos ;

	SaveContexte( timerrefhr ) ;

	SizeOfBufferValidePos = (U32)(PtrSave-BufferValidePos) ;

	memcpy( ptrobj, &memoobj, sizeof(T_OBJET) ) ;

#ifdef	DEBUG_TOOLS
	if( SizeOfBufferValidePos>=SIZE_BUFFER_VALIDE_POS )
	{
		Message( "Warning: BufferValidePos trop petit (ça a dû patché) !", TRUE ) ;
	}
#endif
}

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
void	RestartValidePos( void )
{
	S32	flagload ;
	S32	savetimerrefhr ;

	PtrSave = BufferValidePos ;

	flagload = LoadContexte( &savetimerrefhr ) ;

	if( !flagload )	InitLoadedGame( ) ;
	else		LoadFile3dObjects() ;

	NewCube = -1 ;

	ChoicePalette() ;
	CameraCenter( 0 ) ;
	RestoreTimer() ;
	SetTimerHR( savetimerrefhr ) ;
	SaveTimer() ;
}

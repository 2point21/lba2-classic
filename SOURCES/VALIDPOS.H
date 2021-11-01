#ifndef	VALIDPOS_H
#define	VALIDPOS_H

#define	SIZE_BUFFER_VALIDE_POS	50000L	// a reajuster

extern	S32	ValideCube		;

extern	S32	ValidePos		;
extern	S32	LastValidePos		;

extern	T_OBJET	ValideHero ;
extern	U32	SizeOfBufferValidePos	;
extern	U8	*BufferValidePos	;

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	SaveValidePos( void ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
extern	void	RestartValidePos( void ) ;
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#endif	// VALIDPOS_H

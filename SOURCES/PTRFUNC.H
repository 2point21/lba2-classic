#ifndef	PTRFUNC_H
#define	PTRFUNC_H

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//
//		     Definition des types de pointeurs de fonction
//
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	void	(Func_ReajustPos)( U8 col ) ;

#pragma	aux	Arg_ReajustPos	"*"	\
		parm caller []		\
		modify [eax ecx edx]

#pragma	aux	(Arg_ReajustPos)	Func_ReajustPos

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	void	(Func_GetShadow)( S32, S32, S32 ) ;

#pragma	aux	Arg_GetShadow	"*"	\
		parm caller []		\
		modify [eax ecx edx]

#pragma	aux	(Arg_GetShadow)		Func_GetShadow


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	void	(Func_DoAnim)( U8 ) ;

#pragma	aux	Arg_DoAnim	"*"	\
		parm caller []		\
		modify [eax ecx edx]

#pragma	aux	(Arg_DoAnim)		Func_DoAnim


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	U8	(Func_WorldColBrick)( S32, S32, S32 ) ;

#pragma	aux	Arg_WorldColBrick	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_WorldColBrick)		Func_WorldColBrick


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	U8	(Func_WorldColBrickVisible)( S32, S32, S32, S32, S32, S32 ) ;

#pragma	aux	Arg_WorldColBrickVisible	"*"	\
		parm caller []				\
		modify [eax ecx edx]

#pragma	aux	(Arg_WorldColBrickVisible)	Func_WorldColBrickVisible


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	void	(Func_ReajustPosExtra)( T_EXTRA *, U8, S16 * ) ;

#pragma	aux	Arg_ReajustPosExtra	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_ReajustPosExtra)	Func_ReajustPosExtra


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	S32	(Func_TreeInsert)( S16 numtype, S32 posx, S32 posy, S32 posz,
				   S32 txmin, S32 tymin, S32 tzmin,
				   S32 txmax, S32 tymax, S32 tzmax ) ;

#pragma	aux	Arg_TreeInsert		"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_TreeInsert)	Func_TreeInsert


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
typedef	void	(Func_Init3DView)( void ) ;

#pragma	aux	Arg_Init3DView		"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_Init3DView)	Func_Init3DView


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
#ifdef	LBA_EDITOR
typedef	void	(Func_InitGrille)( char * ) ;
#else	// LBA_EDITOR
typedef	void	(Func_InitGrille)( U32 ) ;
#endif	// LBA_EDITOR

#pragma	aux	Arg_InitGrille		"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_InitGrille)	Func_InitGrille


//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
#ifndef	LBA_EDITOR
typedef	U8	(Func_FullWorldColBrick)(S32,S32,S32,S32,S32,S32) ;

#pragma	aux	Arg_FullWorldColBrick	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_FullWorldColBrick)	Func_FullWorldColBrick
#endif	// LBA_EDITOR

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#endif	// PTRFUNC_H

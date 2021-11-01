#ifndef	PTRFUNC_H
#define	PTRFUNC_H

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
//
//		     Definition des types de pointeurs de fonction
//
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	void	(Func_ReajustPos)( U8 col ) ;

#pragma	aux	Arg_ReajustPos	"*"	\
		parm caller []		\
		modify [eax ecx edx]

#pragma	aux	(Arg_ReajustPos)	Func_ReajustPos

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	void	(Func_GetShadow)( S32, S32, S32 ) ;

#pragma	aux	Arg_GetShadow	"*"	\
		parm caller []		\
		modify [eax ecx edx]

#pragma	aux	(Arg_GetShadow)		Func_GetShadow


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	void	(Func_DoAnim)( U8 ) ;

#pragma	aux	Arg_DoAnim	"*"	\
		parm caller []		\
		modify [eax ecx edx]

#pragma	aux	(Arg_DoAnim)		Func_DoAnim


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	U8	(Func_WorldColBrick)( S32, S32, S32 ) ;

#pragma	aux	Arg_WorldColBrick	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_WorldColBrick)		Func_WorldColBrick


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	U8	(Func_WorldColBrickVisible)( S32, S32, S32, S32, S32, S32 ) ;

#pragma	aux	Arg_WorldColBrickVisible	"*"	\
		parm caller []				\
		modify [eax ecx edx]

#pragma	aux	(Arg_WorldColBrickVisible)	Func_WorldColBrickVisible


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	void	(Func_ReajustPosExtra)( T_EXTRA *, U8, S16 * ) ;

#pragma	aux	Arg_ReajustPosExtra	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_ReajustPosExtra)	Func_ReajustPosExtra


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	S32	(Func_TreeInsert)( S16 numtype, S32 posx, S32 posy, S32 posz,
				   S32 txmin, S32 tymin, S32 tzmin,
				   S32 txmax, S32 tymax, S32 tzmax ) ;

#pragma	aux	Arg_TreeInsert		"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_TreeInsert)	Func_TreeInsert


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
typedef	void	(Func_Init3DView)( void ) ;

#pragma	aux	Arg_Init3DView		"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_Init3DView)	Func_Init3DView


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#ifdef	LBA_EDITOR
typedef	void	(Func_InitGrille)( char * ) ;
#else	// LBA_EDITOR
typedef	void	(Func_InitGrille)( U32 ) ;
#endif	// LBA_EDITOR

#pragma	aux	Arg_InitGrille		"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_InitGrille)	Func_InitGrille


//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#ifndef	LBA_EDITOR
typedef	U8	(Func_FullWorldColBrick)(S32,S32,S32,S32,S32,S32) ;

#pragma	aux	Arg_FullWorldColBrick	"*"	\
		parm caller []			\
		modify [eax ecx edx]

#pragma	aux	(Arg_FullWorldColBrick)	Func_FullWorldColBrick
#endif	// LBA_EDITOR

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#endif	// PTRFUNC_H

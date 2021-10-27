#ifndef	PATCH_H
#define	PATCH_H

typedef struct {
	S16	Size	;
	S16	Offset	;
	} T_PATCH ;

#define	MAX_PATCHES        500

#ifdef	COMPILATOR
extern	S32	MaxPatches  	;
extern	T_PATCH	ListPatches[MAX_PATCHES] ;
#else
extern	T_PATCH *ListPatches ;
#endif

extern	S32	NbPatches  ;

#ifdef	COMPILATOR
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
extern	void	AddPatch( S16 size, S16 offset ) ;
#else
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
extern	void	SetPatches( void ) ;
#endif	// COMPILATOR

#endif	// PATCH_H

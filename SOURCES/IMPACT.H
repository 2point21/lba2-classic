/*
				IMPACT.H

		   Gestion des impacts des projectiles

*/

#ifndef	IMPACT_H
#define	IMPACT_H

#ifdef	LBA_EDITOR
#define	PATH_IMPACT	"f:\\projet\\lba2\\datas\\impact\\"
#endif

#define	MAX_IMPACT_ENTITIES	20
#define	MAX_IMPACT		100

// Types d'entit‚

#define	IMP_NOP		-1
#define	IMP_END		0
#define	IMP_REM		1
#define	IMP_SAMPLE	2
#define	IMP_FLOW	3
#define	IMP_THROW	4
#define	IMP_THROW_POF	5
#define	IMP_THROW_OBJ	6
#define	IMP_SPRITE	7
#define	IMP_FLOW_SPRITE	8
#define	IMP_FLOW_OBJ	9
#define	IMP_FLOW_POF	10

#define	NB_IMP_COMMAND	11


#ifdef	LBA_EDITOR
// Pour pr‚compiler les impacts

typedef struct {
	char name[10] ;
	U8   *buf     ;
	} T_IMPACT ;

extern	U32		NbImpacts ;
extern	T_IMPACT	ListImpact[MAX_IMPACT] ;
#else
//extern	T_HQR_HEADER	*HQR_Impact ;
extern	U8	*BufferImpact ;
#endif

#ifdef	LBA_EDITOR
extern	S8	GetImpCommand( void ) ;
extern	S32	GetNumDef( char *pathdef, char *name ) ;
extern	S32	GetNumDefAddIfNot( char *pathdef, char *name, char *rem ) ;
extern	S32	CompileImpact( U8 *buffer ) ;
extern	U8	*SearchImpact( char *name ) ;

#ifndef	COMPILATOR
extern	void	DoImpact( char *name, S32 x, S32 y, S32 z, U8 owner ) ;
#endif
#else
extern	void   	DoImpact( S32 num, S32 x, S32 y, S32 z, U8 owner ) ;
#endif

#endif	// IMPACT_H

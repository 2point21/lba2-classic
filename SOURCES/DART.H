/*ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*
	  Û฿฿฿฿ Û     Û฿฿฿฿ Û฿฿฿฿ Û   Û Û฿฿฿฿ ฿฿Û฿฿ ฿฿Û฿฿ Û฿฿฿฿ ÛÛ฿฿฿
	  ÛÛ฿฿  ÛÛ    ÛÛ฿฿  ÛÛ    ÛÛ฿฿Û ÛÛ฿฿    ÛÛ    ÛÛ  ÛÛ฿฿  ฿฿฿฿Û
	  ฿฿    ฿฿฿฿฿ ฿฿฿฿฿ ฿฿฿฿฿ ฿฿  ฿ ฿฿฿฿฿   ฿฿    ฿฿  ฿฿฿฿฿ ฿฿฿฿฿
 *ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ*/
/*ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ*/

#ifndef	COMPILATOR

#ifndef	DART_H
#define	DART_H

#define	MAX_DARTS	3

#ifdef	LBA_EDITOR
//#define	BODY_3D_DART    "F:\\projet\\lba2\\graphn\\3D\\throw\\dart.O4D"
#define	BODY_3D_DART    "F:\\projet\\lba2\\graph\\3D\\throw\\dart.O4D"
#else
#define	BODY_3D_DART	61	// f:\projet\lba2\graph\3d\objfix.lst
#endif

typedef struct
{
//	T_OBJ_3D	Obj ;		// Structure de l'objet
	S32		PosX;
	S32		PosY ;
	S32		PosZ ;
	S32		Alpha ;
	S32		Beta ;
	S32		Body ;
	S32		NumCube ;	// Numro du cube dans lequel se trouve la flchette
	U32		Flags	;

	S32		XMin	;	// ZV des flechettes
	S32		YMin	;
	S32		ZMin	;
	S32		XMax	;
	S32		YMax	;
	S32		ZMax	;
} T_DART ;

// Flags des flechettes
#define	DART_TAKEN      (1<<0)

// Fonctions des flechettes (DART.CPP)
/*--------------------------------------------------------------------------*/
extern	void	InitDarts( void ) 	;
/*--------------------------------------------------------------------------*/
extern	S32	GetDart( void )		;
/*--------------------------------------------------------------------------*/
extern	void	TakeAllDarts( void )	;
/*--------------------------------------------------------------------------*/
extern	void	CheckDartCol( T_OBJET *ptrobj )	;
/*--------------------------------------------------------------------------*/
#ifdef	LBA_EDITOR
extern	void	DrawZVDart( T_DART *ptrd ) ;
#endif
/*--------------------------------------------------------------------------*/

#endif	// DART_H

#endif	// COMPILATOR

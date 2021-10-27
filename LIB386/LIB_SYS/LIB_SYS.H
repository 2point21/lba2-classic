/*--------------------------------------------------------------------------*/
/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		Û  Üß Ûßßßß Û  Üß ÛßßÛ  ÛßßßÛ ÛßßßÛ ÛßßßÛ ÛßßßÜ
		ÛÛßÜ  ÛÛßß  ÛÛß   ÛÛßßÛ ÛÛ  Û ÛÛßßÛ ÛÛßÛß ÛÛ  Û
		ßß  ß ßßßßß ßß    ßßßßß ßßßßß ßß  ß ßß  ß ßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

#ifdef	WATCOM9
	#pragma	library	("f:\projet\lib386\lib_sys\lb9_sys.lib");
#else
	#pragma	library	("f:\projet\lib386\lib_sys\lib_sys.lib");
#endif

#include	"f:\projet\lib386\lib_sys\dll.h"

extern	volatile	UWORD	Key	;	/* scan code keyboard */

#define	K_ESC		1

#define	K_F1		59
#define	K_F2		60
#define	K_F3		61
#define	K_F4		62
#define	K_F5		63
#define	K_F6		64
#define	K_F7		65
#define	K_F8		66
#define	K_F9		67
#define	K_F10		68
#define	K_F11		87
#define	K_F12		88


#define	K_PLUS		78
#define	K_MOINS		74
#define	K_BACKSPACE	14
#define	K_TAB		15
#define	K_NUMLOCK	69

#define	K_A		16
#define	K_B		48
#define	K_C		46
#define	K_D		32
#define	K_E		18
#define	K_F		33
#define	K_G		34
#define	K_H		35
#define	K_I		23
#define	K_J		36
#define	K_K		37
#define	K_L		38
#define	K_M		39
#define	K_N		49
#define	K_O		24
#define	K_P		25
#define	K_Q		30
#define	K_R		19
#define	K_S		31
#define	K_T		20
#define	K_U		22
#define	K_V		47
#define	K_W		44
#define	K_X		45
#define	K_Y		21
#define	K_Z		17

#define	K_1		2
#define	K_2		3
#define	K_3		4
#define	K_4		5
#define	K_5		6
#define	K_6		7
#define	K_7		8
#define	K_8		9
#define	K_9		10
#define	K_10		11

extern	volatile	UWORD	FuncKey	;	/* mask bit touches fonctions */

#define	FK_F1		1
#define	FK_F2		2
#define	FK_F3		4
#define	FK_F4		8
#define	FK_F5		16
#define	FK_F6		32
#define	FK_F7		64
#define	FK_F8		128
#define	FK_F9		256
#define	FK_F10		512
#define	FK_F11		1024
#define	FK_F12		2048

extern	volatile	UWORD	Joy	;	/* emulation cursor key */

#define	J_UP		1
#define	J_DOWN		2
#define	J_LEFT		4
#define	J_RIGHT		8

extern	volatile	UWORD	Fire	;	/* emulation touches fire */

#define	F_SPACE		1
#define	F_RETURN	2
#define	F_CTRL		4
#define	F_ALT           8
#define	F_SUPPR		16
#define	F_SHIFT		32

void	InitKeyboard(void) ;		/* init de Key Joy Fire */
void	ClearKeyboard(void) ;

UWORD	GetAscii(void) ;		/* retourne code ascii dos */
void	ClearAsciiBuffer(void) ;	/* vide buffer dos */

extern	UWORD	AsciiMode ;	/* TRUE/FALSE gestion buffer keyboard */

#define	A_F1		0x3B00
#define	A_F2		0x3C00
#define	A_F3		0x3D00
#define	A_F4		0x3E00
#define	A_F5		0x3F00
#define	A_F6		0x4000
#define	A_F7		0x4100
#define	A_F8		0x4200
#define	A_F9		0x4300
#define	A_F10		0x4400

#define	A_ESC		0x011B

#define	A_UP		0x4800
#define	A_DOWN		0x5000
#define	A_LEFT		0x4B00
#define	A_RIGHT		0x4D00

#define	A_HOME		0x4700
#define	A_END		0x4F00
#define	A_PGUP		0x4900
#define	A_PGDN		0x5100

#define	A_SPACE		0x3920
#define	A_RETURN	0x1C0D

#define	A_N_PLUS	0x4E2B
#define A_N_MOINS	0x4A2D

#define	A_BACKSPACE	0x0E08
#define	A_SUPPR		0x5300

#define	A_CARRE		0x29FD

#define	A_1             0x226
#define	A_2             0x382
#define	A_3             0x422
#define	A_4             0x527
#define	A_5             0x628
#define	A_6             0x77D
#define	A_7             0x88A
#define	A_8             0x95F
#define	A_9             0xA87
#define	A_0             0xB85

#define	A_A		0x1061
#define	A_B		0x3062
#define	A_C		0x2e63
#define	A_D		0x2064
#define	A_E		0x1265
#define	A_F		0x2166
#define	A_G		0x2267
#define	A_H		0x2368
#define	A_I		0x1769
#define	A_J		0x246a
#define	A_K		0x256b
#define	A_L		0x266c
#define	A_M		0x276d
#define	A_N		0x316e
#define	A_O		0x186f
#define	A_P		0x1970
#define	A_Q		0x1e71
#define	A_R		0x1372
#define	A_S		0x1f73
#define	A_T		0x1474
#define	A_U		0x1675
#define	A_V		0x2f76
#define	A_W		0x2c77
#define	A_X		0x2d78
#define	A_Y		0x1579
#define	A_Z		0x117a
#define	FALSE	0
#define	TRUE	1

#define	SEEK_START	0
#define	SEEK_CURRENT	1
#define	SEEK_END	2

#define	Rnd(n)	(rand()%n)

#define	GetRealSeg(p)	((((ULONG)(p)) >> 4) & 0xFFFF )
#define	GetRealOff(p)	(((ULONG)(p)) & 0xF )

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				FILES.C					    */
void	AddExt( char *name, char *ext )				;
LONG	OpenRead( char *name )					;
LONG	OpenWrite( char *name )					;
LONG	OpenReadWrite( char *name )				;
ULONG	Read( LONG handle, void *buffer, ULONG lenread  )	;
ULONG	Write( LONG handle, void *buffer, ULONG lenwrite  )	;
void	Close( LONG handle )					;
LONG	Seek( LONG handle, LONG position,LONG mode )		;
LONG	Delete( char *name )					;
ULONG	FileSize( char *name )					;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				LOADSAVE.C				    */
ULONG	Load( char *name, void *buffer )			;
ULONG	Save( char *name,void *buffer, ULONG size )		;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				MALLOC.C				    */

extern	LONG	ModeTraceMalloc ;

void	*DosMalloc( LONG size, ULONG *handle )			;
void	DosFree( ULONG handle )					;

void	*SmartMalloc( LONG lenalloc )				;
void	*Malloc( LONG lenalloc )				;

void	Free( void *buffer )					;
void	*Mshrink( void *buffer, ULONG taille )			;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				LOADMALL.C				    */

extern	ULONG	LoadMallocFileSize ;

void	*LoadMalloc( char *name )				;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				TIMER.C					    */

/*
extern	UBYTE	*BufferGlobal	;

#define	TimerRef		(*((ULONG *)(BufferGlobal+0)))
#define	NbFramePerSecond        (*((UWORD *)(BufferGlobal+4)))
#define	WaitNbTicks		(*((UWORD *)(BufferGlobal+6)))
#define	CmptFrame               (*((UWORD *)(BufferGlobal+8)))
#define	Cmpt_18                 (*((UWORD *)(BufferGlobal+10)))
*/

extern	volatile	ULONG	TimerSystem ;
extern	volatile	ULONG	TimerRef ;
extern	UWORD	NbFramePerSecond ;
extern	UWORD	WaitNbTicks ;
extern	UWORD	CmptFrame ;
extern	UWORD	Cmpt_18 ;

void	SetTimer( WORD divisor ) ;
WORD	GetTimer() ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				DEF_FILE.C				*/

LONG	Def_WriteString( UBYTE *deffic, UBYTE *indentificateur, UBYTE *string ) ;
LONG	Def_WriteValue( UBYTE *deffic, UBYTE *indentificateur, LONG value ) ;

UBYTE	*Def_ReadString( UBYTE *deffic, UBYTE *indentificateur ) ;
LONG	Def_ReadValue( UBYTE *deffic, UBYTE *indentificateur ) ;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				BUFFER_A.ASM				*/
void	RazMem( void *pt, ULONG size )	;
void	MovMem( void *pt0, void *pt1, ULONG size )	;
LONG	CompBuf( void *pt0, void *pt1, ULONG size )	;

char	*Itoa( LONG value )	;



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				HQ_Ressource				*/

extern	WORD	HQR_Flag ;	// flag de load done par HQR_Get


typedef	struct	{	char	Name[128] ;
			ULONG	MaxSize ;
			ULONG	FreeSize ;
			UWORD	MaxIndex ;
			UWORD	NbIndex ;
			UBYTE	*Buffer ;	}	T_HQR_HEADER ;

typedef struct	{	UWORD	Index ;
			ULONG	Offset ;
			ULONG	Size ;
			ULONG	Time ;		}	T_HQR_BLOC ;

void	*LoadMalloc_HQR( UBYTE *name, UWORD index ) ;
ULONG	Load_HQR( UBYTE *name, void *ptrdest, UWORD index ) ;
ULONG	Size_HQR( char *name, UWORD index ) ;

T_HQR_HEADER	*HQR_Init_Ressource(	char *hqrname,
					ULONG maxsize,
					UWORD maxindex ) ;

LONG		HQR_Change_Ressource( T_HQR_HEADER *header, char *newhqrname ) ;
void		HQR_Reset_Ressource( T_HQR_HEADER *header ) ;
void    	HQR_Free_Ressource( T_HQR_HEADER *header ) ;

void		*HQR_Get( T_HQR_HEADER *header, UWORD index ) ;
void		*HQR_GetSample( T_HQR_HEADER *header, UWORD index ) ;

UWORD		HQR_Del_Bloc( T_HQR_HEADER *header, WORD index ) ;
UWORD		HQR_Del_Bloc_Sample( T_HQR_HEADER *header, WORD index ) ;
T_HQR_BLOC	*HQR_GiveIndex( UWORD index, UWORD nbindex, void *ptrlist ) ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				HQ_MEM.C				*/

extern	ULONG	Size_HQM_Memory ;
extern	ULONG	Size_HQM_Free ;

LONG	HQM_Init_Memory( ULONG size ) ;
void	HQM_Clear_Memory() ;
LONG	HQM_Alloc( ULONG size, void **ptr ) ;
void	HQM_Free_All() ;
void	HQM_Free( void *ptr ) ;
LONG	HQM_Check() ;
void	HQM_Shrink_Last( void *ptr, ULONG newsize ) ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				HQ_R_M.C				*/

LONG	HQRM_Load( UBYTE *name, UWORD index, void **ptrptr ) ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*				ADELINE.C				*/

#define	INIT_SVGA	1
#define	INIT_WAVE	2
#define	INIT_MIDI	4
#define	INIT_MIXER	8
#define	INIT_VESA	16
#define	INIT_MCGA	32

void	InitAdelineSystem( char *name, LONG inits ) ;
void	ClearAdelineSystem() ;

extern	char	PathConfigFile[]	;
extern	char	MidiFM			;
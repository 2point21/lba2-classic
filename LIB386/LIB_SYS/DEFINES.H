#include 	"\projet\lib386\lib_sys\adeline.h"
#include 	"\projet\lib386\lib_sys\lib_sys.h"
#include 	"\projet\lib386\lib_svga\lib_svga.h"
#include 	"\projet\lib386\lib_3D\lib_3D.h"
#include 	"\projet\lib386\lib_menu\lib_menu.h"
#include	<stdlib.h>

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▄ █▀▀▀▀ █▀▀▀▀  █    ██▄ █ █▀▀▀▀ ██▀▀▀
		   ██  █ ██▀▀  ██▀▀   ██   ██▀██ ██▀▀  ▀▀▀▀█
		   ▀▀▀▀  ▀▀▀▀▀ ▀▀     ▀▀   ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/


#define	SIZE_BRICK_XZ	512
#define	SIZE_BRICK_Y	256
#define	DEMI_BRICK_XZ	256
#define	DEMI_BRICK_Y	128


/*---------------- OBJECTS ------------------*/

#define	MAX_OBJETS	10

#define	SIZE_PERSO_NAME		32

#define	MAX_ANIMS		100


typedef	struct
{
	/* life tool */

	UBYTE	Name[SIZE_PERSO_NAME+1] ;
	UBYTE	File3D[_MAX_PATH] ;

	/* game infos */

	WORD	*PtrObj ;

	WORD	PosObjX ;
	WORD	PosObjY ;
	WORD	PosObjZ ;

	WORD	Alpha ;
	WORD	Beta ;
	WORD	Gamma ;

	WORD	Move ;
	UBYTE	Col;
	UBYTE	dummy ;

	WORD	AnimMasterRot ;
	WORD	AnimStepBeta ;

	WORD	AnimStepX ;
	WORD	AnimStepY ;
	WORD	AnimStepZ ;

	WORD	StartAnim ;
	WORD	Anim ;
	WORD	Frame ;
	WORD	FlagAnim ;
	WORD	NextAnim ;
					}	T_OBJET ;
typedef struct
{
	WORD	Z ;
	WORD	NumObj ;		}	T_SORT ;

typedef	struct
{
	WORD	NumObj ;
	WORD	x0 ;
	WORD	y0 ;
	WORD	x1 ;
	WORD	y1 ;			}	T_OBJ_SELECT ;

/*---------------- divers ------------------*/

#define	Rnd(n)	(rand()%n)

#define	NUM_PERSO	0

#define	ISO		1
#define	PCX		2

/*---------------- FlagInfos ------------------*/

#define	INFO_SCREEN_BOX		1
#define	INFO_DRAW_ZV		2
#define	INFO_COOR		4
#define	INFO_FRAME_SPEED	8

/*---------------- InitAnim ------------------*/

#define	ANIM_REPEAT	0
#define	ANIM_THEN	1
#define	ANIM_ALL_THEN	2

/*---------------- Script: defines ------------------*/

#define	NO_MOVE		0
#define	MOVE_MANUAL	1
#define	MOVE_FOLLOW	2
#define	MOVE_GOTO	3

/*---------------- The End: error ------------------*/

#define	FILE_NOT_FOUND	0
#define	NOT_ENOUGH_MEM	1
#define	PROGRAM_OK	2

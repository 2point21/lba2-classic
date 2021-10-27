#ifndef	CREDITS_H
#define	CREDITS_H

typedef struct  {

	S32             NbObjects ;
	S32             OffTxt ;
	S32		OffXpl ;

		}       S_CRED_INFOS_2 ;


typedef struct {

						// reserve place pour player
	T_OBJ_3D	Obj ;			// lib struct
	BOUND_MOVE	RealAngle;		// synchro de la rotation

	S32		Flag ;
	S32		DestX ;
	S32		DestZ ;

	S32		OffBody ;		// offset
	S32		OffAnim[2] ;

	} S_CRED_OBJ_2 ;

#define	CREDITS_HQR_NAME	PATH_RESSOURCE"lba2.hqr"

extern	S32     GamePlayCredits( char *file_name, S32 mode ) ;

#endif	// CREDITS_H

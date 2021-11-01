//яяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяя

#define	MAX_BUGGYS	2

//яяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяя
/*ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ*/
typedef struct {

	S32	Angle ;
	S32	X ;
	S32	Y ;
	S32	Z ;

		}	S_ONE_WHEEL ;

typedef struct {

	S32	X ;
	S32	Y ;
	S32	Z ;
	S32	Cube ;
	S32	Beta	;
	S32	Alpha	;
	S32     Gamma	;

	S_ONE_WHEEL	Wheel[4] ;

	S32		BetaWheel ;

	S32		SpeedInc ;
	S32		SpeedRot ;

	S32		Speed ;

//	S32		YSpeed ;
//	S32		LastYs ;
//	S32		LastSpeedLevel ;

	S32		LastTimer ;

		} S_BUGGY ;

//яяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяя

#define	BUGGY_PRESENT		0x80
#define	NUM_BUGGY		((U8)(NumBuggy&~(BUGGY_PRESENT)))
#define	IsBuggyPresent()        (NumBuggy&BUGGY_PRESENT)

extern	S_BUGGY	ListBuggy[MAX_BUGGYS] ;
extern	U8	NumBuggy	;

//яяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяя

//ююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююю
extern	void	InitBuggy( U8 numobj, U32 flaginit ) ;

//ююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююю
extern	void	ResetBuggy( void ) ;

//ююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююю
extern	void	TakeBuggy( void )	;

//ююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююю
extern	void	LeaveBuggy( U8 newcomportement ) ;

//ююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююю
extern	void	DoAnimBuggy( T_OBJET *ptrobj ) ;

//ююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююююю
extern	void	MoveBuggy( T_OBJET *ptrobj ) ;

//яяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяяя

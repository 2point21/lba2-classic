#ifdef	WATCOM9
	#pragma	library ("f:\projet\lib386\lib_3d\lb9_3d.lib");
#else
	#pragma	library ("f:\projet\lib386\lib_3d\lib_3d.lib");
#endif

#define	NORMAL_UNIT	64

#define	INFO_TRI	1
#define	INFO_ANIM	2

#define	TYPE_ROTATE		0
#define	TYPE_TRANSLATE		1
#define	TYPE_ZOOM		2

#define	MAT_TRISTE	0
#define	MAT_PIERRE	1
#define	MAT_COPPER	2
#define	MAT_BOPPER	3
#define	MAT_MARBRE	4
#define	MAT_TRANS	5
#define	MAT_TRAME	6
#define	MAT_FLAT	7
#define	MAT_GRANIT	8
#define	MAT_GOURAUD	9
#define	MAT_DITHER	10

/* structures */

typedef	struct {	WORD	X ;
			WORD	Y ;
			WORD	Z ;		} T_GROUPE_INFO ;

typedef struct {	WORD	StartValue ;
			WORD	EndValue  ;
			UWORD	TimeValue ;
			ULONG	MemoTicks ;	} T_REAL_VALUE ;

typedef struct {	WORD	Xp ;
			WORD	Yp ;
			WORD	Zrot ;   } T_LIST_POINT ;


extern		WORD	AnimMasterRot ;
extern		WORD	AnimStepAlpha, AnimStepBeta, AnimStepGamma ;
extern		WORD	AnimStepX, AnimStepY, AnimStepZ ;
extern		WORD	ScreenXmin, ScreenXmax, ScreenYmin, ScreenYmax ;

extern		WORD	XCentre, YCentre ;
extern		WORD	IsoScale ;

extern		WORD	Xp, Yp ;
extern		WORD	Z_Min, Z_Max ;

extern		WORD		NbPoints ;
extern		T_LIST_POINT	List_Point ;

extern		LONG	KFactor, LFactorY, LFactorX ;

extern		LONG	X0, Y0, Z0 ;
extern		LONG	EX0, EY0 ;		/* Rot2D sur LONG */

extern		LONG	CameraX, CameraY, CameraZ ;
extern		LONG	CameraXr, CameraYr, CameraZr ;

extern		LONG	Alpha, Beta, Gamma ;
extern		LONG	lAlpha, lBeta, lGamma ;

extern		LONG	AlphaLight, BetaLight, GammaLight ;
extern		LONG	NormalXLight, NormalYLight, NormalZLight ;

extern		LONG	MatriceWorld, MatriceTempo;
extern		LONG	MatriceRot, MatriceDummy;

extern		LONG	Distance ;

extern		LONG	CmptMemoTimerRef ;

void	InitRealValue(	WORD start,
			WORD end,
			WORD time,
			T_REAL_VALUE *ptrstruct ) ;

void	InitRealAngle(	WORD start,
			WORD end,
			WORD time,
			T_REAL_VALUE *ptrstruct ) ;

WORD	GetRealValue(	T_REAL_VALUE *ptrstruct ) ;
WORD	GetRealAngle(	T_REAL_VALUE *ptrstruct ) ;

WORD	StockInterAnim( UBYTE *BufferAnim, UBYTE *PtrObj ) ;

LONG	RegleTrois32( LONG val1, LONG val2, LONG nbstep, LONG step ) ;

LONG	Distance2D( LONG x0, LONG z0, LONG x1, LONG z1 ) ;
LONG	Distance3D(	LONG x0, LONG y0, LONG z0,
			LONG x1, LONG y1, LONG z1 ) ;

void	Rot2D( LONG x, LONG y, LONG angle ) ;

void	WorldRotatePoint( LONG x, LONG y, LONG angle ) ;
void	LongWorldRotatePoint( LONG x, LONG y, LONG angle ) ;

void	LongInverseRotatePoint( LONG x, LONG y, LONG angle ) ;

void	SetLightVector( WORD alpha, WORD beta, WORD gamma ) ;

void	CopyMatrice( LONG *MatSrc, LONG *MatDest );

LONG	AffObjetIso(LONG xwr, LONG ywr, LONG zwr,
		    LONG palpha, LONG pbeta, LONG pgamma,
		    void *ptrobj);

void	PatchObjet(void *ptrobj);

LONG	ProjettePoint(LONG CoorX, LONG CoorY, LONG CoorZ);
LONG	LongProjettePoint(LONG CoorX, LONG CoorY, LONG CoorZ);

void	SetProjection(LONG xc, LONG yc, LONG kfact, LONG lfactx, LONG lfacty);
void	SetIsoProjection(LONG xc, LONG yc, LONG scale);

ULONG	Sqr(ULONG x);

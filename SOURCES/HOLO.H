/*-------------------------------------------------------------------------*/
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴


#define	HOLO_HQR_NAME		PATH_RESSOURCE"holomap.hqr"
//#define	HOLO_HQR_NAME		"f:\\projet\\lba2\\sourcesn\\ress\\holomap.hqr"


//#define	MAX_TRI	25
//#define	ONE_DIR		1	//	all datas in current directory...
#define	P_TWINSUN	0
#define	P_MOON		1
#define	P_ZEELICH	2
#define	P_SUN_0		3
#define	P_SUN_1		4
#define	P_SUN_Z		5

#define	MAX_PLANET	7
#define	RAYON_PLANET	1000
#define	ZOOM_PLANET		8000
#define	ZOOM_PLANET_SOUSGAZ	12000
#define	ZOOM_INIT_PLANET	3000

#define	X_CENTRE_TRAJ_HOLO	320+80
#define	Y_CENTRE_TRAJ_HOLO	240

#define	ZOOM_TWINSUN		8000		//9500
#define	ZOOM_MOON		2000		//1250
#define	ZOOM_ZEELICH		6000
#define	ZOOM_SUN		2000

#define	STEP_ANGLE	(32*4)
#define	SLIG  		(((4096/STEP_ANGLE)+1) * 2)
#define	SIZE_TEXT_GLOBE	(((4096/STEP_ANGLE)+1) * ((2048/STEP_ANGLE)+1) * 2 * 2)
#define	SIZE_COOR_GLOBE	(((4096/STEP_ANGLE)+1) * ((2048/STEP_ANGLE)+1) * 3 * 2)
#define	SIZE_ALT_GLOBE	544
#define	DISTANCE_EXT	(512*10)	//	5 Metres


#define	MAX_ARROW	(10*10)	//	10 fleches par 10 iles
#define	SCE		32768	//	SizeXZ Cube Exterieur

#define	MAX_OBJECTIF	50
#define	MAX_ISLAND	12
#define	MAX_CUBE	255


#define	SIZE_ARROW_ISLAND	10000
#define	SIZE_SMALL_ARROW_ISLAND	2000	//	1621 real size

#define	SCALE_OBJ_GLOBE		8
#define	SCALE_BODY_GLOBE	64
#define	SIZE_ARROW_PLANET	100	//	After Scale

#define	SCALE_BODY_PLAN		2048
#define	SCALE_BUGGY_PLAN	1024
#define	SCALE_DYNO_PLAN		(1024+256)

#define	PLANET			128
#define	UFO			256

#define	HQR_COORMAPP_HMM	0
#define	HQR_TWINSUN_HMT		1
#define	HQR_TWINSUN_HMG		2
#define	HQR_MOON_HMT		3
#define	HQR_MOON_HMG		4
#define	HQR_ZEELICH_HMT		5
#define	HQR_ZEELICH_HMG		6
#define	HQR_SOUSGAZ_HMT		7
#define	HQR_SOUSGAZ_HMG		8
#define	HQR_SUN_HMG		9

#define	HQR_FLECHE		10


#define	HQR_LOFLECHE		11
#define	HQR_ARROWBIN		12
#define	HQR_BUGGY		13
#define	HQR_DYNO		14

#define	HQR_BEGIN_MAP		18

#define	START_ZOOM      9500

#define	CENTERX		320
#define	CENTERY		220
#define	SX		6
#define	SY		6

#define	MAX_STARS	200	//Attention au DEFINE dans FlipBox.c


typedef	struct
{
	S32	X	;//	Position Island X Y Z
	S32	Y	;
	S32	Z	;
	S32	Alpha	;//	Position Planet Alpha, Beta et Altitude
	S32	Beta	;
	S32	Alt	;
	S32	Mess	;//	Message sur Island ou Planet
	S8	ObjFix	;//	Eventual Obj Inventory 3D (FREE NOT USED!)
	U8	FlagHolo;//	Flag for Planet display, active, etc.
	U8	Planet	;
	U8	Island	;

} T_ARROW	;

extern	T_ARROW	TabArrow[MAX_OBJECTIF+MAX_CUBE]		;

extern	U32	DecalTimerRef[MAX_OBJECTIF+MAX_CUBE] 	;

/*-------------------------------------------------------------------------*/

extern	Struc_Point Tab_Points[]	;


extern	U32	AlphaL, BetaL, GammaL	;
extern	S32	HoloAlpha, HoloBeta, HoloGamma	;
extern	S32	NumObjectif		;
extern	S32	OldObjectif		;

extern	U16	PtrTri[]		;//	For 1 Planet
extern	S32	ListTriPlanet[]		;//	For 6 Planets
extern	S32	NbTri			;//	Nb Planet(s) in ListTri
extern	S32	ModeAuto 		;
extern	S32	XPos, YPos, ZPos	;
extern	S32	XDest, YDest, ZDest	;
extern	S32	ZoomDest		;
extern	S32	Destination		;
extern	U32	MyTimerRef		;
extern	U32	RayonPlanet		;
extern	S32	DialStatHolo 		;
extern	S32	SizeRet		;
extern	U8	RotPal[]	;

extern	S16	RotPalPos 	;
extern	S32	FlagMap		;
extern	S32	FlagHoloEnd	;
extern	S32	XpObjectif	;
extern	S32	YpObjectif	;
extern	U32	MemoTimerRot	;
extern	S32	ZoomedIsland	;
extern	S32	TwinsenLinkArrow;
extern	S32	OrgMX, OrgMZ	;
extern	S32	AlphaM, BetaM	;
extern	S32	DistanceM	;
extern	U8	*PtCurrentBody	;//	Tempo : Pointeur dans struc Twinsen
extern	U8	*PtCurrentAnim	;//	Tempo : Pointeur dans struc Twinsen
extern	U32	SizeCurrentBody	;//	Tempo : Pointeur dans struc Twinsen
extern	U32	SizeCurrentAnim	;//	Tempo : Pointeur dans struc Twinsen
extern	U8	*BufBodyTempo	;//	Buffer Tempo Traffiquer Body ( scale & rot )
extern	U8	*BufAnimTempo	;//	Buffer Tempo Traffiquer Anim ( scale & rot )
extern	U16	*PtrMapping	;
extern	U16	*PtrGlobe	;
extern	U8	*PtrAlt		;
extern	U16	*PtrCoorGlobe	;
extern	U8	*PtrTextMap	;
extern	T_OBJ_3D ObjTwinsen	;
extern	S32	NumOldMess	;
extern	S32	HoloMode	;
/*-------------------------------------------------------------------------*/

typedef	struct
{
	S32	XStar	;
	S32	YStar	;
	S32	ZStar	;
	S32	ColStar	;
} T_STARS	;

extern	T_STARS	TabStars[];
/*-------------------------------------------------------------------------*/
typedef	struct
{

	U32	XSpace	;
	U32	YSpace	;
	U32	ZSpace	;
	U16	Rayon	;
	U16     Zoom	;

	U16	*PtrCoorGlobe	;
	U8	*PtrTextMap	;
	U8	dummy		;// I don't like to wait after watcom about struct align ;)

}	T_PLANET		;

extern	T_PLANET	TabPlanet[]	;

/*-------------------------------------------------------------------------*/
extern	U8		*PtUfo		;
extern	U8		*PtAnimUfo	;
extern	T_OBJ_3D        Ufo		;
//----------------------------------------------------------------------------
extern	U8	*BufFleche	;
extern	U8	*BufLoFleche	;
extern	U8	*BufBuggy	;
extern	U8	*BufDyno	;
//----------------------------------------------------------------------------

/*-------------------------------------------------------------------------*/

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//	Objects Scale factor by planet
extern	S32	TabRayon[]	;
extern	S32	TabScale[] 	;
extern	S32	TabZoom[]	;
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//	Functions
extern	int	z_sort( void const *a, void const *b)	;
extern	int	z_sort32( void const *a, void const *b)	;

#ifdef	DEBUG_TOOLS
extern	S32	SizeHoloMalloc					;
#endif
extern	void	InitHoloMalloc(void)				;
extern	U8	*HoloMalloc( U32 size )				;
extern	U8	*LoadHoloMalloc_HQR( char *name, S32 index )	;

extern	void	ScaleRotBody3D( U8 *ptbody, S32 scale, S32 flagrot )	;
extern	U32	GetSizeBody3D( U8 *ptbody )	;
extern	void	ScaleRotAnim3D( U8 *ptanim, S32 scale, S32 flagrot )	;
extern	U32	GetSizeAnim3D( U8 *ptanim )	;
extern	void	InitAngleArrow(void)		;
extern	void	HoloPlan(S32 numplan)		;

extern	void    AffUpperLeft( S32 x, S32 y, S32 flag )	;
extern	void    AffLowerLeft( S32 x, S32 y, S32 flag )	;
extern	void    AffUpperRight( S32 x, S32 y, S32 flag )	;
extern	void    AffLowerRight( S32 x, S32 y, S32 flag )	;
extern	void	DoAnimHolo(void)		        ;
extern	void	AffReticule(void)			;
extern	void	AffHoloMess(void)			;
extern	void	AdjustScreenMax(void)			;
extern	void	ComputeCoorGlobe(void)			;
extern	void	DrawHolomap(void)			;
extern	void	HoloSpace(void)				;

extern	void	InitHoloMap()				;
extern	void	HoloMap(void)				;
extern	S32	SetHoloPos( S32 num )			;
extern	void	ClrHoloPos( S32 num )			;

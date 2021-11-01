#include "c_extern.h"

#define	TAILLE_MAX_CHEAT_CODE	10

typedef	struct	{
			U8	size;
			U8	code[TAILLE_MAX_CHEAT_CODE];
		} T_CODE;

enum	{
	CHEAT_LIFE,	// 0
	CHEAT_MAGIC,	// 1
	CHEAT_FULL,	// 2
	CHEAT_GOLD,	// 3
	CHEAT_SPEED,	// 4
	CHEAT_CLOVER,	// 5
	CHEAT_CLOVERBOX,// 6
	CHEAT_PINGOUIN,	// 7
	} ;

#ifdef	DEBUG_TOOLS
T_CODE	DefCheatCode[]= {
			{ 3, {K_I, K_F, K_E} },		// LIFE
			{ 5, {K_M, K_A, K_G, K_I, K_C} },	// MAGIC
			{ 4, {K_F, K_U, K_L, K_L} },		// FULL
			{ 4, {K_G, K_O, K_L, K_D} },		// GOLD
			{ 5, {K_S, K_P, K_E, K_E, K_D} },	// SPEED
			{ 6, {K_C, K_L, K_O, K_V, K_E, K_R} },	// CLOVER
			{ 3, {K_B, K_O, K_X} },	// BOX
			{ 8, {K_P, K_I, K_N, K_G, K_O, K_U, K_I, K_N} }, // PINGOUIN
			};
#else
T_CODE	DefCheatCode[]= {
			{ 4, {K_L, K_I, K_F, K_E} },		// LIFE
			{ 5, {K_M, K_A, K_G, K_I, K_C} },	// MAGIC
			{ 4, {K_F, K_U, K_L, K_L} },		// FULL
			{ 4, {K_G, K_O, K_L, K_D} },		// GOLD
			{ 5, {K_S, K_P, K_E, K_E, K_D} },	// SPEED
			{ 6, {K_C, K_L, K_O, K_V, K_E, K_R} },	// CLOVER
			{ 3, {K_B, K_O, K_X} },	// BOX
			{ 8, {K_P, K_I, K_N, K_G, K_O, K_U, K_I, K_N} }, // PINGOUIN
			};
#endif

#define	NB_DEF_CHEAT_CODE	(sizeof(DefCheatCode)/sizeof(DefCheatCode[0]))

S32	NbKeyInBuffer ;
U8	BufferCheatCode[TAILLE_MAX_CHEAT_CODE] ;

//***************************************************************************
static inline void	InitCheatCode()
{
	memset( BufferCheatCode, 0, TAILLE_MAX_CHEAT_CODE );
	NbKeyInBuffer = 0;
}

char *CheatText[] = {
	"Life Found",
	"Magic Found",
	"Found Everything !",
	"Kashes Found",
	"Zlitos Found",
	"Frame Rate On",
	"Frame Rate Off",
	"Clover Found",
	"CloverBox Found",
	"MecaPingouin Found"
	} ;

//***************************************************************************
void	GereCheatCode( void )
{
	T_OBJET	*ptrobj ;
	S32	cheat, start ;
	S32	text = -1 ;
	S32	sprite = -1 ;
#ifdef	_WIN32
	S32	key	;

	while((key = GetAscii()) != 0)
	{
		key = toupper(key)	;

		if(	( key < K_A )
		OR	( key > K_Z )	)
		{
			continue;
		}
#else
	{
		if(	( MyKey < K_A )
		OR	( MyKey > K_N )	)
		{
			return	;
		}
#endif
		memmove( BufferCheatCode, BufferCheatCode+1, TAILLE_MAX_CHEAT_CODE-1 );
		BufferCheatCode[TAILLE_MAX_CHEAT_CODE-1] = (U8)MyKey ;

		if( NbKeyInBuffer<TAILLE_MAX_CHEAT_CODE ) NbKeyInBuffer++;

		InitWaitNoKey() ;

		for( cheat=0; cheat<NB_DEF_CHEAT_CODE; cheat++ )
		{
			start = TAILLE_MAX_CHEAT_CODE-NbKeyInBuffer;

			while( start+DefCheatCode[cheat].size <= TAILLE_MAX_CHEAT_CODE )
			{
				if( 	memcmp( BufferCheatCode+start,
						DefCheatCode[cheat].code,
						DefCheatCode[cheat].size ) == 0 )
				{
					goto exit_loop;
				}
				start++;
			}
		}
#ifdef	_WIN32
		continue	;	// Cheat not found
#else
		return		;	// Cheat not found
#endif

exit_loop:
		// clear cheat du buffer
		InitCheatCode();

		ptrobj = &ListObjet[NUM_PERSO] ;

		switch( cheat )
		{
			case CHEAT_LIFE: // barre de vie au maximum
				if( ptrobj->LifePoint > 0 )
				{
					ptrobj->LifePoint = MAX_LIFE_POINTS ;
					text = 0 ;
					sprite = SPRITE_COEUR ;
				}
				break;

			case CHEAT_MAGIC: // points de magie au maximum
				MagicPoint = (U8)(MagicLevel * 20) ;
				sprite = SPRITE_MAGIE ;
				text = 1 ;
				break;

			case CHEAT_FULL: // equivalent instruction FULL_POINT
				if( ptrobj->LifePoint > 0 )
				{
					ptrobj->LifePoint = MAX_LIFE_POINTS ;
				}

				if( ListVarGame[FLAG_CONQUE] )
				{
					TabInv[FLAG_CONQUE].PtMagie = INV_MAX_VALUE ;
				}

				ListVarGame[FLAG_CLOVER] = NbCloverBox ;
				MagicPoint = (U8)(MagicLevel * 20) ;
				text = 2 ;
				break;

			case CHEAT_GOLD: // pieces d'or
				if( Planet>=2 )
				{
					if( NbZlitosPieces<999 )
					{
						NbZlitosPieces += 50 ;
						if( NbZlitosPieces>999 )	NbZlitosPieces = 999 ;
						text = 4 ;
					}
				}
				else
				{
					if( NbGoldPieces<999 )
					{
						NbGoldPieces += 50 ;
						if( NbGoldPieces>999 )		NbGoldPieces = 999 ;
						text = 3 ;
					}
				}
				sprite = SPRITE_PIECE ;
				break ;

			case CHEAT_SPEED: // Frame Rate
				DebugFps ^= 1 ;
				if( DebugFps )	text = 5 ;
				else		text = 6 ;
				break ;

			case CHEAT_CLOVER:// Trefle
				ListVarGame[FLAG_CLOVER] = NbCloverBox ;
				sprite = SPRITE_CLOVER ;
				text = 7 ;
				break ;

			case CHEAT_CLOVERBOX:// boite … Trefle
				if( NbCloverBox<10 )
				{
					NbCloverBox++ ;
					ListVarGame[FLAG_CLOVER] = NbCloverBox ;
					text = 8 ;
					sprite = SPRITE_FULL_CLOVER_BOX ;
				}
				break ;

			case CHEAT_PINGOUIN:// M‚caPingouin
				if( ListVarGame[FLAG_MECA_PINGOUIN]<10 )
				{
					ListVarGame[FLAG_MECA_PINGOUIN]+=5 ;
					text = 9 ;
				}
				break ;
		}

		if( text!=-1 )
		{
			InitMenuIncrustText( CheatText[text], sprite, 2 ) ;
		}

		HQ_MixSample( SAMPLE_FOUND_OBJ, 0x1000, 0, 1, 64, 127 ) ;

		MyKey = 0 ;
	}
}

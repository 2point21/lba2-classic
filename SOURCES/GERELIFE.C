#include	"c_extern.h"

	UBYTE	*PtrPrg ;

//	WORD	OffsetLife ;

	WORD	TypeAnswer ;

	WORD	TextLig = 0 ;

extern	WORD 	IndexGrm ;
extern	WORD	ZoneGrm ;

#define	RET_BYTE	0
#define	RET_WORD	1
#define	RET_STRING	2

#ifdef	CDROM
extern	LONG	FlagDisplayText ;
#endif

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀▄ █▀▀▀█       █      █    █▀▀▀▀ █▀▀▀▀
		   ██  █ ██  █       ██     ██   ██▀▀  ██▀▀
		   ▀▀▀▀  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀▀▀▀  ▀▀   ▀▀    ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	DoFuncLife( T_OBJET *ptrobj )
{
	WORD	num ;
	WORD	angle ;
	LONG	distance ;

	TypeAnswer = RET_BYTE ;

	switch( *PtrPrg++ )	/* func */
	{
		case LF_COL:
			if( ptrobj->LifePoint <= 0 )
			{
				Value = -1 ;
			}
			else
			{
				Value = ptrobj->ObjCol ;
			}
			break ;

		case LF_CHAPTER:
			Value = Chapitre ;
			break ;

		case LF_LIFE_POINT:
			Value = ptrobj->LifePoint ;
			break	;

		case LF_HIT_BY:
			Value = ptrobj->HitBy ;
			break ;

		case LF_ACTION:
			Value = ActionNormal ;
			break ;

		case LF_LIFE_POINT_OBJ:
			Value = ListObjet[*PtrPrg++].LifePoint ;
			break	;

		case LF_COL_OBJ:
//			Value = ListObjet[*PtrPrg++].ObjCol ;
			num = *PtrPrg++ ;
			if( ListObjet[num].LifePoint <= 0 )
			{
				Value = -1 ;
			}
			else
			{
				Value = ListObjet[num].ObjCol ;
			}
			break ;

		case LF_DISTANCE:
			num = (WORD)*PtrPrg++ ;
			TypeAnswer = RET_WORD ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			if( abs(ListObjet[num].PosObjY-ptrobj->PosObjY) < 1500 )
			{
				distance = Distance2D( ptrobj->PosObjX,
							ptrobj->PosObjZ,
							ListObjet[num].PosObjX,
							ListObjet[num].PosObjZ ) ;
				if( distance > 32000 )
					Value = 32000 ;
				else
					Value = distance ;
			}
			else
			{
				Value = 32000 ;
			}
			break ;

		case LF_DISTANCE_3D:
			num = (WORD)*PtrPrg++ ;
			TypeAnswer = RET_WORD ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			distance = Distance3D( ptrobj->PosObjX,
						ptrobj->PosObjY,
						ptrobj->PosObjZ,
						ListObjet[num].PosObjX,
						ListObjet[num].PosObjY,
						ListObjet[num].PosObjZ ) ;

			if( distance > 32000 )
				Value = 32000 ;
			else
				Value = distance ;
			break ;

		case LF_CONE_VIEW:
			num = (WORD)*PtrPrg++ ;
			TypeAnswer = RET_WORD ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			if( abs(ListObjet[num].PosObjY-ptrobj->PosObjY) < 1500 )
			{
				angle = GetAngle(	ptrobj->PosObjX,
							ptrobj->PosObjZ,
							ListObjet[num].PosObjX,
							ListObjet[num].PosObjZ ) ;

				if( Distance > 32000 )
					Distance = 32000 ;
			}
			else
			{
				Distance = 32000 ;
			}


			if( num == NUM_PERSO )
			{
				if( Comportement == C_DISCRET )
				{
					if( (((ptrobj->Beta + 1024 + 128) - (angle + 1024))&1023) <= 256 )
					{
						Value = Distance ;
					}
					else
					{
						Value = 32000 ;
					}
				}
				else	Value = Distance ;
			}
			else
			{
				if( (((ptrobj->Beta + 1024 + 128) - (angle + 1024))&1023) <= 256 )
				{
					Value = Distance ;
				}
				else
				{
					Value = 32000 ;
				}
			}
			break ;

		case LF_ZONE:
			Value = ptrobj->ZoneSce ;
			break ;

		case LF_NB_GOLD_PIECES:
			Value = NbGoldPieces ;
			TypeAnswer = RET_WORD ;
			break ;

		case LF_NB_LITTLE_KEYS:
			Value = NbLittleKeys ;
			break ;

		case LF_COMPORTEMENT_HERO:
			Value = Comportement ;
			break ;

		case LF_MAGIC_LEVEL:
			Value = MagicLevel ;
			break ;

		case LF_MAGIC_POINT:
			Value = MagicPoint ;
			break ;

		case LF_CHOICE:
			Value = GameChoice ;
			TypeAnswer = RET_WORD ;
			break	;

		case LF_FUEL:
			Value = Fuel ;
			break	;

		case LF_L_TRACK:
			Value = ptrobj->LabelTrack ;
			break ;

		case LF_ZONE_OBJ:
			Value = ListObjet[ *PtrPrg++ ].ZoneSce ;
			break ;

		case LF_FLAG_CUBE:
			Value = ListFlagCube[ *PtrPrg++ ] ;
			break ;

		case LF_FLAG_GAME:
			num = *PtrPrg++ ;

			if( (ListFlagGame[FLAG_CONSIGNE] == 0)
			OR  (num >= MAX_INVENTORY) )
			{	// si pas consigne ou pas inventaire
				Value = ListFlagGame[ num ] ;
			}
			else	// si flag consigne
			{
				if( num == FLAG_CONSIGNE )
				{
					Value = ListFlagGame[ num ] ;
				}
				else	// ne possede aucun objet
				{
					Value = 0 ;
				}
			}
			break ;

		case LF_USE_INVENTORY:
			num = *PtrPrg++ ;	// flag game

			if( num >= MAX_INVENTORY )	// a bon ?
			{
				Value = FALSE ;
			}

			if( ListFlagGame[FLAG_CONSIGNE] )
			{
				Value = 0 ;
			}
			else	// si pas flag consigne
			{
				if( InventoryAction == num )
				{ // action émise depuis l'inventaire à cette boucle
					Value = TRUE ;
				}
				else
				{
					if( (ListFlagInventory[ num ] == 1)
					AND (ListFlagGame[num] == 1) )
					{
						Value = TRUE ;
					}
					else
					{
						Value = FALSE ;
					}
				}
				if( Value == TRUE )
				{
				// init incdisp symbol utilisation
					InitIncrustDisp( INCRUST_OBJ,
							 num,
							 0, 0,
							 0, 0, 3 ) ;
				}
			}
			break ;

		case LF_L_TRACK_OBJ:
			Value = ListObjet[ *PtrPrg++ ].LabelTrack ;
			break ;

		case LF_BODY:
			Value = ptrobj->GenBody ;
			break ;

		case LF_BODY_OBJ:
			Value = ListObjet[*PtrPrg++].GenBody ;
			break ;

		case LF_ANIM:
			Value = ptrobj->GenAnim ;
			break ;

		case LF_ANIM_OBJ:
			Value = ListObjet[*PtrPrg++].GenAnim ;
			break ;

		case LF_CARRY_BY:
			Value = ptrobj->CarryBy ;
			break ;

		case LF_CDROM:
#ifdef	CDROM
//Message( "CDROM TRUE" ) ;
			Value = 1 ;
#else
//Message( "CDROM FALSE" ) ;
			Value = 0 ;
#endif
			break ;

	}
}

/*──────────────────────────────────────────────────────────────────────────*/

WORD    DoTest()
{
	UBYTE	test ;
	WORD	valueword ;
	BYTE	valuebyte ;
	UBYTE	*ptrstring ;

	test = *PtrPrg++ ;

	switch( TypeAnswer )
	{
	case RET_BYTE:

		valuebyte = *PtrPrg++ ;
		switch( test )
		{
		case LT_EQUAL:
			return (Value == valuebyte) ;

		case LT_SUP:
			return (Value > valuebyte) ;

		case LT_LESS:
			return (Value < valuebyte) ;

		case LT_SUP_EQUAL:
			return (Value >= valuebyte) ;

		case LT_LESS_EQUAL:
			return (Value <= valuebyte) ;

		case LT_DIFFERENT:
			return (Value != valuebyte) ;
		}

	case RET_WORD:

		valueword = *(WORD*)PtrPrg ;
		PtrPrg += 2 ;
		switch( test )
		{
		case LT_EQUAL:
			return (Value == valueword) ;

		case LT_SUP:
			return (Value > valueword) ;

		case LT_LESS:
			return (Value < valueword) ;

		case LT_SUP_EQUAL:
			return (Value >= valueword) ;

		case LT_LESS_EQUAL:
			return (Value <= valueword) ;

		case LT_DIFFERENT:
			return (Value != valueword) ;
		}

	case RET_STRING:

		valueword = stricmp( String, PtrPrg ) ;
		PtrPrg += strlen( PtrPrg ) + 1 ;

		switch( test )
		{
		case LT_EQUAL:
			return (valueword == 0 ) ;

		case LT_SUP:
		case LT_LESS:
		case LT_SUP_EQUAL:
		case LT_LESS_EQUAL:
			return FALSE ;

		case LT_DIFFERENT:
			return (valueword != 0 ) ;
		}
	}
	return FALSE ;
}

/*──────────────────────────────────────────────────────────────────────────*/


void	DoLife( WORD numobj )
{
	WORD	flag = 0 ;
	UBYTE	macro ;
	T_OBJET	*ptrobj ;

	LONG	obj ;
	LONG	num ;
	UBYTE	index ;
	UBYTE	*ptrmacro ;
	LONG	memoflagdisplaytext ;
	LONG	var ;
	UBYTE	string[256] ;
	LONG	dx, n ;

	ptrobj = &ListObjet[numobj] ;

	PtrPrg = ptrobj->PtrLife + ptrobj->OffsetLife ;

	while( flag != -1 )
	{
		ptrmacro = PtrPrg ;
		switch( *PtrPrg++ )
		{
			case LM_END:
			case LM_END_LIFE:
				ptrobj->OffsetLife = -1 ;
				flag = -1 ;
				break ;

			case LM_RETURN:
			case LM_END_COMPORTEMENT:
				flag = -1 ;
				break ;

			case LM_LABEL:
			case LM_COMPORTEMENT:
#ifdef	LBA_EDITOR
				if ( numobj == ObjSelected )/*	TOOLS	*/
				{
					ComportementDebug = *PtrPrg	;
				}
#endif
				PtrPrg++ ;
				break ;

			case LM_FALLABLE:
				num = *PtrPrg++ * OBJ_FALLABLE ;
				ptrobj->Flags &= ~OBJ_FALLABLE ;
				ptrobj->Flags |= num ;
				break ;

			case LM_COMPORTEMENT_HERO:
				InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NO_ANIM, NUM_PERSO ) ;
				SetComportement( *PtrPrg++ ) ;
				break ;

			case LM_SET_MAGIC_LEVEL:
// si sce permet objets magique ordre quelconque transforme SET en INC
				MagicLevel = *PtrPrg++ ;
				// remplie magie a fond
				MagicPoint = MagicLevel * 20 ;
				break ;

			case LM_SUB_MAGIC_POINT:
				MagicPoint -= *PtrPrg++ ;
				if( MagicPoint < 0 )	MagicPoint = 0 ;
				break ;

			case LM_CAM_FOLLOW:
				num = *PtrPrg++ ;
				if( num != NumObjFollow )
				{
					NumObjFollow = num ;
					StartXCube = ((ListObjet[NumObjFollow].PosObjX+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
					StartYCube = ((ListObjet[NumObjFollow].PosObjY+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
					StartZCube = ((ListObjet[NumObjFollow].PosObjZ+DEMI_BRICK_XZ)/SIZE_BRICK_XZ) ;
					FirstTime = TRUE ;
				}
				break ;

			case LM_INIT_PINGOUIN:
				num = *PtrPrg++ ;
				NumPingouin = num ;
				ListObjet[num].WorkFlags |= OBJ_DEAD	;
				ListObjet[num].Body = -1		;
				ListObjet[num].ZoneSce = -1		;
				break ;

			case LM_KILL_OBJ:
				num = *PtrPrg++	;
                                CheckCarrier( num ) ;
				ListObjet[num].WorkFlags |= OBJ_DEAD	;
				ListObjet[num].Body = -1		;
				ListObjet[num].ZoneSce = -1		;
				ListObjet[num].LifePoint = 0		;
				break	;

			case LM_SUICIDE:
				CheckCarrier( numobj ) ;
				ptrobj->WorkFlags |= OBJ_DEAD	;
				ptrobj->Body = -1		;
				ptrobj->ZoneSce = -1 ;
				ptrobj->LifePoint = 0 ;
				break	;

			case LM_SET_DIR:
				ptrobj->Move = *PtrPrg++ ;
				if( ptrobj->Move == MOVE_FOLLOW )
				{
					ptrobj->Info3 = *PtrPrg++ ;
				}
				break ;

			case LM_SET_DIR_OBJ:
				obj = *PtrPrg++ ;
				num = ListObjet[obj].Move = *PtrPrg++ ;
				if( num == MOVE_FOLLOW )
				{
					ListObjet[obj].Info3 = *PtrPrg++ ;
				}
				break ;

			case LM_SET_LIFE:
			case LM_SET_COMPORTEMENT:
				ptrobj->OffsetLife = *(WORD*)PtrPrg ;
				PtrPrg += 2 ;
				break ;

			case LM_SET_LIFE_OBJ:
			case LM_SET_COMPORTEMENT_OBJ:
				num = *PtrPrg++ ;
				ListObjet[num].OffsetLife = *(WORD*)PtrPrg ;
				PtrPrg += 2 ;
				break ;

			case LM_SET_LIFE_POINT_OBJ:
				num = *PtrPrg++ ;
				ListObjet[num].LifePoint = *PtrPrg++ ;
				break ;

			case LM_SUB_LIFE_POINT_OBJ:
				num = *PtrPrg++ ;
				ListObjet[num].LifePoint -= *PtrPrg++ ;
				if( ListObjet[num].LifePoint < 0 )
					ListObjet[num].LifePoint = 0 ;
				break ;

			case LM_HIT_OBJ:
				num = *PtrPrg++ ;
				HitObj( numobj, num, *PtrPrg++, ListObjet[num].Beta ) ;
				break ;

			case LM_SET_TRACK:
				ptrobj->OffsetTrack = *(WORD*)PtrPrg ;
				PtrPrg += 2 ;
				break ;

			case LM_SET_TRACK_OBJ:
				num = *PtrPrg++ ;
				ListObjet[num].OffsetTrack = *(WORD*)PtrPrg ;
				PtrPrg += 2 ;
				break ;

			case LM_STOP_L_TRACK:
				ptrobj->MemoLabelTrack = ptrobj->OffsetLabelTrack ;
				ptrobj->OffsetTrack = -1 ;
				break ;

			case LM_RESTORE_L_TRACK:
				ptrobj->OffsetTrack = ptrobj->MemoLabelTrack ;
				break ;

			case LM_IF:
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				}
				else
				{
					PtrPrg += 2 ;
				}
				break ;

			case LM_OR_IF:
				DoFuncLife( ptrobj ) ;
				if( DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				}
				else
				{
					PtrPrg += 2 ;
				}
				break ;

			case LM_SWIF:
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				}
				else
				{
					*ptrmacro = LM_SNIF ;
					PtrPrg += 2 ;
				}
				break ;

			case LM_SNIF:	/* swif inversé tjrs jump jusqu'a cond inverse */
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					*ptrmacro = LM_SWIF ;
				}
				PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				break ;

			case LM_ONEIF:
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				}
				else
				{
					PtrPrg += 2 ;
					*ptrmacro = LM_NEVERIF ;
				}
				break ;

			case LM_NEVERIF:
				DoFuncLife( ptrobj ) ;
				DoTest() ;
				PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				break ;

			case LM_OFFSET: 	/* jmp à offset */
				PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				break ;

			case LM_ELSE:
				PtrPrg = ptrobj->PtrLife + *(WORD*)PtrPrg ;
				break ;

			case LM_BODY:
				InitBody( *PtrPrg++, numobj ) ;
				break ;

			case LM_BODY_OBJ:
				num = *PtrPrg++ ;
				InitBody( *PtrPrg++, num ) ;
				break ;

			case LM_ANIM_OBJ:
				num = *PtrPrg++ ;
				InitAnim( *PtrPrg++, ANIM_REPEAT,0, num ) ;
				break ;

			case LM_ANIM:
				InitAnim( *PtrPrg++, ANIM_REPEAT,0,numobj ) ;
				break ;

			case LM_ANIM_SET:
				ptrobj->GenAnim = NO_ANIM ;
				ptrobj->Anim = -1 ;
				InitAnim( *PtrPrg++, ANIM_REPEAT,0,numobj ) ;
				break ;

			case LM_MESSAGE_OBJ:
				SaveTimer() ;
				TestRestoreModeSVGA( TRUE ) ;
				num = *PtrPrg++	;
				if( Bulle )	DrawBulle( num ) ;
				TestCoulDial( ListObjet[num].CoulObj ) ;
#ifdef	CDROM
				NumObjSpeak = num ;
#endif
				Dial( *(WORD*)PtrPrg ) ;
				PtrPrg += 2 ;
				RestoreTimer() ;

				AffScene(TRUE) ;
				WaitReleaseSpace() ;
				break ;

			case LM_MESSAGE:
				SaveTimer() ;
				TestRestoreModeSVGA( TRUE ) ;
				if( Bulle )	DrawBulle( numobj ) ;
				TestCoulDial( ptrobj->CoulObj ) ;
#ifdef	CDROM
				NumObjSpeak = numobj ;
#endif
				Dial( *(WORD*)PtrPrg ) ;
				PtrPrg += 2 ;
				RestoreTimer() ;
				AffScene(TRUE)	;
				WaitReleaseSpace() ;
				break ;

			case LM_SAY_MESSAGE_OBJ:
				obj = *PtrPrg++ ;
				num = *(WORD*)PtrPrg	;
				InitIncrustDisp( INCRUST_TEXT,
						 num,
						 0, 0,
						 obj, 1, 2 ) ;
				PtrPrg += 2 ;
#ifdef	CDROM
				SaveTimer() ;
				Speak( num )	;
				RestoreTimer() ;
#endif
				break ;

			case LM_SAY_MESSAGE:
				num = *(WORD*)PtrPrg	;
				InitIncrustDisp( INCRUST_TEXT,
						 num,
						 0, 0,
						 numobj, 1, 2 ) ;
				PtrPrg += 2 ;
#ifdef	CDROM
				SaveTimer() ;
				Speak( num )	;
				RestoreTimer() ;
#endif
				break ;

			case LM_SET_FLAG_CUBE:
				num = *PtrPrg++ ;
				ListFlagCube[num] = *PtrPrg++ ;
				break ;

			case LM_SET_FLAG_GAME:
				num = *PtrPrg++ ;
				ListFlagGame[num] = *PtrPrg++ ;

//Text( 0,100, "%Fset ListFlagGame %d = %d",num,ListFlagGame[num] ) ;

				break ;

			case LM_SET_USED_INVENTORY:
				num = *PtrPrg++ ;
				if( num < 24 )
					ListFlagInventory[num] = TRUE ;
				break ;

			case LM_GIVE_GOLD_PIECES:
				num = NbGoldPieces ;
				NbGoldPieces -= *(WORD*)PtrPrg ;
				if( NbGoldPieces < 0 )
				{
					NbGoldPieces = 0 ;
				}
				PtrPrg += 2 ;

				// sample cling cling
//				HQ_MixSample( 83, 0x1000, 2, 128, 128 ) ;

				// aff piece
				InitIncrustDisp( INCRUST_SPRITE,
						 3,
						 10, 15,
						 0, 0, 3 ) ;

				// aff cmpt si pas
				{
					// attention MAGOUILLE
					// ne marche que parcequ'il y n'y a
					// qu'UN seul incrust_cmpt dans le jeu

					T_INCRUST_DISP	*ptrdisp ;
					ptrdisp = ListIncrustDisp ;
					var = 0 ;
					for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
					{
						if( ptrdisp->Num == -1 )
							continue ;

						if( ptrdisp->Type==INCRUST_CMPT )
						{
							ptrdisp->Num  = BoundRegleTrois( ptrdisp->Info, ptrdisp->Num, 50*2, ptrdisp->TimerEnd-TimerRef-50 ) ;
							ptrdisp->Info = NbGoldPieces ;
							ptrdisp->TimerEnd = TimerRef + 50 * 3 ;
							var = 1 ;
							break ;
						}
					}

					if( !var )	// yen avait pas
					{
					InitIncrustDisp( INCRUST_CMPT,
							 num,
							 30, 20,
							 NbGoldPieces, 0, 3 ) ;
					}
				}
				break ;

			case LM_USE_ONE_LITTLE_KEY:
				NbLittleKeys-- ;
				if( NbLittleKeys < 0 )	NbLittleKeys = 0 ;
				// affiche little key
				InitIncrustDisp( INCRUST_SPRITE,
						 6,
						 0, 0,
						 NUM_PERSO, 1, 1 ) ;
				break ;

			case LM_INC_CHAPTER:
				Chapitre++ ;
				break ;

			case LM_FOUND_OBJECT:
				SaveTimer() ;
				TestRestoreModeSVGA( TRUE ) ;
				DoFoundObj( *PtrPrg++ ) ;
				RestoreTimer() ;
				AffScene( TRUE ) ;
				break ;

			case LM_SET_DOOR_LEFT:
				ptrobj->Beta = 768 ;
				ptrobj->PosObjX = ptrobj->AnimStepX - *(WORD*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

			case LM_SET_DOOR_RIGHT:
				ptrobj->Beta = 256 ;
				ptrobj->PosObjX = ptrobj->AnimStepX + *(WORD*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

			case LM_SET_DOOR_UP:
				ptrobj->Beta = 512 ;
				ptrobj->PosObjZ = ptrobj->AnimStepZ - *(WORD*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

			case LM_SET_DOOR_DOWN:
				ptrobj->Beta = 0 ;
				ptrobj->PosObjZ = ptrobj->AnimStepZ + *(WORD*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

			case LM_GIVE_BONUS:
				if( ptrobj->OptionFlags & EXTRA_MASK )
				{
					GiveExtraBonus( ptrobj ) ;

				}
				if( *PtrPrg++ )
				{
					// ne donne plus rien ????
					ptrobj->OptionFlags |= EXTRA_GIVE_NOTHING ;
				}
				break ;

			case LM_CHANGE_CUBE:
				NewCube = *PtrPrg++ ;
				FlagChgCube = 2 ;
				break ;

			case LM_PLAY_MIDI:
				PlayMusic( *PtrPrg++ ) ;
				break ;

			case LM_ADD_FUEL:
				Fuel += *PtrPrg++ ;
				if( Fuel > 100 )	Fuel = 100 ;
				break ;

			case LM_SUB_FUEL:
				Fuel -= *PtrPrg++ ;
				if( Fuel < 0 )	Fuel = 0 ;
				break ;

			case LM_SET_HOLO_POS:
				SetHoloPos( *PtrPrg++ ) ;
				if( ListFlagGame[FLAG_HOLOMAP] )
				{
					InitIncrustDisp( INCRUST_OBJ,
							 0,	// holomap
							 0, 0,
							 0, 0, 3 ) ;
				}
				break ;

			case LM_CLR_HOLO_POS:
				ClrHoloPos( *PtrPrg++ ) ;
				break ;

			case LM_SET_GRM:	// ne marche que si pas autre grm
				IncrustGrm( (IndexGrm = *PtrPrg++) ) ;
				break ;

			case LM_INC_CLOVER_BOX:
				if( NbCloverBox < MAX_CLOVER_BOX )
				{
					NbCloverBox++ ;
				}
				break ;

			case LM_OBJ_COL:
				if( *PtrPrg++ )	ptrobj->Flags |= CHECK_OBJ_COL ;
				else		ptrobj->Flags &= ~CHECK_OBJ_COL ;
				break ;

			case LM_INVISIBLE:
				if( *PtrPrg++ )	ptrobj->Flags |= INVISIBLE ;
				else		ptrobj->Flags &= ~INVISIBLE ;
				break ;

			case LM_BRICK_COL:
				num = *PtrPrg++ ;
				ptrobj->Flags &= ~(CHECK_BRICK_COL+COL_BASSE) ;
				if( num == 1 )
				{
					ptrobj->Flags &= ~(CHECK_BRICK_COL+COL_BASSE) ;
					ptrobj->Flags |= CHECK_BRICK_COL  ;
				}
				if( num == 2 )
				{
					ptrobj->Flags |= CHECK_BRICK_COL+COL_BASSE  ;
				}
				break ;

			case LM_ZOOM:
				if( *PtrPrg++ )
				{
					if( (!FlagMCGA) AND (SceZoom!=0) )
					{
						FadeToBlack( PtrPal ) ;
						ExtInitMcga() ;
						SetBlackPal() ;
						FlagFade = TRUE ;
					}
				}
				else
				{
					if( FlagMCGA )
					{
						FadeToBlack( PtrPal ) ;
						ExtInitSvga() ;
						SetBlackPal() ;
						FlagFade = TRUE ;
						FirstTime = TRUE ;
					}
				}
				break ;

			case LM_POS_POINT:
				Value = *PtrPrg++ ;	// index point
				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				ptrobj->PosObjX = X0 ;
				ptrobj->PosObjY = Y0 ;
				ptrobj->PosObjZ = Z0 ;
				break ;

			case LM_PLAY_FLA:
				SaveTimer() ;
				if( FlagPalettePcx )	FadeToBlack( PalettePcx ) ;
				else			FadeToBlack( PtrPal ) ;
				HQ_StopSample() ;
				PlayAnimFla( PtrPrg ) ;
				PtrPrg += strlen( PtrPrg ) + 1 ;
				RestoreTimer() ;
				FlagFade = TRUE ;
				FirstTime = TRUE ;
				break ;

			case LM_ADD_CHOICE:
				GameListChoice[GameNbChoices] = *(WORD*)PtrPrg ;
				PtrPrg += 2 ;
				GameNbChoices++ ;
				break ;

			case LM_ASK_CHOICE:
#ifndef	DEMO
				SaveTimer() ;
				TestRestoreModeSVGA( TRUE ) ;
				if( Bulle )	DrawBulle( numobj ) ;
				TestCoulDial( ptrobj->CoulObj ) ;
				GameAskChoice( *(WORD*)PtrPrg ) ;
				GameNbChoices = 0 ;
				PtrPrg += 2 ;
//				FlagFade = TRUE ;
				RestoreTimer() ;
				AffScene(TRUE)	;
				WaitReleaseSpace() ;
#endif
				break ;

			case LM_ASK_CHOICE_OBJ:
#ifndef	DEMO
				SaveTimer() ;
				num = *PtrPrg++	;
				TestRestoreModeSVGA( TRUE ) ;
				if( Bulle )	DrawBulle( num ) ;
				TestCoulDial( ListObjet[num].CoulObj ) ;
				GameAskChoice( *(WORD*)PtrPrg ) ;
				GameNbChoices = 0 ;
				PtrPrg += 2 ;
//				FlagFade = TRUE ;
				RestoreTimer() ;
				AffScene(TRUE)	;
				WaitReleaseSpace() ;
#endif
				break ;

			case LM_BIG_MESSAGE:
				SaveTimer() ;
				TestRestoreModeSVGA( TRUE ) ;
				BigWinDial() ;
				if( Bulle )	DrawBulle( numobj ) ;
				TestCoulDial( ptrobj->CoulObj ) ;
#ifdef	CDROM
				NumObjSpeak = numobj ;
#endif
				Dial( *(WORD*)PtrPrg ) ;
				PtrPrg += 2 ;
				NormalWinDial() ;
				AffScene(TRUE)	;
				RestoreTimer() ;
				WaitReleaseSpace() ;
				break ;

			case LM_FULL_POINT:
				ListObjet[NUM_PERSO].LifePoint = 50 ;
				MagicPoint = MagicLevel * 20 ;
				break ;

			case LM_BETA:
				ptrobj->Beta = *(WORD*)PtrPrg ;
				ClearRealAngle( ptrobj ) ;
				PtrPrg += 2 ;
				break ;

			case LM_GRM_OFF:
				if( IndexGrm != -1 )
				{
					IndexGrm = ZoneGrm = -1 ;
					CopyMapToCube() ;
					AffScene( TRUE ) ;
				}
				break ;

			case LM_FADE_PAL_RED:
				SaveTimer() ;
				FadeToRed( PtrPal ) ;
				FlagPalettePcx = FALSE ;
				RestoreTimer() ;
				break ;

			case LM_FADE_ALARM_RED:
				SaveTimer() ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_PAL_ALARM ) ;
				FadeToRed( PalettePcx ) ;
				FlagPalettePcx = TRUE ;
				RestoreTimer() ;
				break ;

			case LM_FADE_ALARM_PAL:
				SaveTimer() ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_PAL_ALARM ) ;
				FadePalToPal( PalettePcx, PtrPal ) ;
				FlagPalettePcx = FALSE ;
				RestoreTimer() ;
				break ;

			case LM_FADE_RED_PAL:
				SaveTimer() ;
				FadeRedToPal( PtrPal ) ;
				FlagPalettePcx = FALSE ;
				RestoreTimer() ;
				break ;

			case LM_FADE_RED_ALARM:
				SaveTimer() ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_PAL_ALARM ) ;
				FadeRedToPal( PalettePcx ) ;
				FlagPalettePcx = TRUE ;
				RestoreTimer() ;
				break ;

			case LM_FADE_PAL_ALARM:
				SaveTimer() ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_PAL_ALARM ) ;
				FadePalToPal( PtrPal, PalettePcx ) ;
				FlagPalettePcx = TRUE ;
				RestoreTimer() ;
				break ;

			case LM_EXPLODE_OBJ:
				num = *PtrPrg++	;
				ExtraExplo(
					ListObjet[num].PosObjX + Rnd( 512 ) - 256 ,
					ListObjet[num].PosObjY + Rnd( 256 ) - 128 ,
					ListObjet[num].PosObjZ + Rnd( 512 ) - 256 ) ;
				break;

			case LM_BULLE_ON:
				Bulle = TRUE ;
				break ;

			case LM_BULLE_OFF:
				Bulle = FALSE ;
				break ;

			case LM_SET_DARK_PAL:
				SaveTimer() ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_PAL_MUSEE ) ;
				if( !FlagFade )	Palette( PalettePcx ) ;
				FlagPalettePcx = TRUE ;
				RestoreTimer() ;
				break ;

			case LM_SET_NORMAL_PAL:
				FlagPalettePcx = FALSE ;
				if( !FlagFade )	Palette( PtrPal ) ;
				break ;

			case LM_MESSAGE_SENDELL:
#ifndef	DEMO
				SaveTimer() ;
				TestRestoreModeSVGA( TRUE ) ;

				FadeToBlack( PtrPal ) ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", Screen, RESS_SENDELL_PCR ) ;
				CopyScreen( Screen, Log ) ;
				Load_HQR( PATH_RESSOURCE"ress.hqr", PalettePcx, RESS_SENDELL_PAL ) ;
				Flip() ;
				FadeToPal( PalettePcx ) ;

				BigWinDial() ;
				TestCoulDial( 15 ) ;
#ifdef	CDROM
				memoflagdisplaytext = FlagDisplayText ;
				FlagDisplayText = TRUE ;
#endif
				FlagMessageShade = FALSE ;
				Dial( 6 ) ;
				FlagMessageShade = TRUE ;

				NormalWinDial() ;
				FadeToBlack( PalettePcx ) ;
				Cls() ;
				Flip();
				Palette( PtrPal ) ;
#ifdef	CDROM
				FlagDisplayText = memoflagdisplaytext ;
#endif
				while( (Fire & F_SPACE)
				OR (Key == K_ESC) ) ;
				RestoreTimer() ;
#endif
				break ;

			case LM_HOLOMAP_TRAJ:
				NumHolomapTraj = *PtrPrg++ ; // signal pour chg cube
				break ;

			case LM_GAME_OVER:
				ListObjet[NUM_PERSO].LifePoint = 0 ;
				ListObjet[NUM_PERSO].WorkFlags |= ANIM_END ;
				NbFourLeafClover = 0 ;
				flag = -1 ;
				break ;

			case LM_THE_END:
#ifndef	DEMO
				// Save Partie retour scene de fin (inrejouable ?)
				NbFourLeafClover = 0 ;
				ListObjet[NUM_PERSO].LifePoint = 50 ;
				MagicPoint = 80 ;
				Comportement = SaveComportement ;
				ListObjet[NUM_PERSO].Beta = SaveBeta ;
				NumCube = 113 ;
				SceneStartX = -1 ;
				SaveGame() ;
				FlagTheEnd = 1 ;	// credits
				flag = -1 ;
#endif
				break ;

			case LM_BRUTAL_EXIT:
				FlagTheEnd = 0 ;
				flag = -1 ;
				break ;

			case LM_MIDI_OFF:
				StopMusicMidi() ;
				break ;

			case LM_PLAY_CD_TRACK:
#ifdef	CDROM
				PlayCdTrack( *PtrPrg++ ) ;
#else
				PtrPrg++ ;
#endif
				break ;

			case LM_PROJ_ISO:
				SetIsoProjection( 320-8-1, 240, SIZE_BRICK_XZ ) ;
				SetPosCamera( 0,0,0 ) ;
				SetAngleCamera( 0,0,0 ) ;
				SetLightVector( AlphaLight, BetaLight, 0 ) ;
				break ;

			case LM_PROJ_3D:
				Cls() ;
				CopyScreen( Log, Screen ) ;
				Flip() ;
				FlagAffGrille = FALSE ;
				SetProjection( 320, 240, 128,1024,1024 ) ;
				SetFollowCamera( 0,1500,0, 25,-128,0, 13000 ) ;
				SetLightVector( 896, 950, 0 ) ;
				InitDial( 1 ) ;
				break ;

			case LM_TEXT:
				if( TextLig < 440 )
				{
					num = *(WORD*)PtrPrg ;
					if( !Version_US )
					{
						if( num == 0 )	num = 16 ;
					}
					GetMultiText( num, string ) ;
					PtrPrg += 2 ;
					dx = SizeFont( string ) ;
					CoulFont( 15 ) ;
					Font( 0, TextLig, string ) ;
					if( dx > 639 )	dx = 639 ;
					CopyBlockPhys( 0, TextLig, dx, TextLig+40 ) ;
					TextLig += 40 ;
				}
				break ;

			case LM_CLEAR_TEXT:
				TextLig = 0 ;
				Box( 0,0, 639, 40*6, 0 ) ;
				CopyBlockPhys( 0,0, 639, 40*6 ) ;
				break ;
		}
	}
}

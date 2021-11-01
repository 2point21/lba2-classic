#include	"c_extern.h"
//#include	"messtwin.cpp"

U8	*PtrPrg ;
U8	TypeAnswer ;

extern	S32	InvSelect ;	// INVENT.CPP

#define	RET_S8	0
#define	RET_S16	1
#define	RET_STRING	2
#define	RET_U8	4

#define	CONE_VIEW	1024

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
		   ÛßßßÜ ÛßßßÛ       Û      Û    Ûßßßß Ûßßßß
		   ÛÛ  Û ÛÛ  Û       ÛÛ     ÛÛ   ÛÛßß  ÛÛßß
		   ßßßß  ßßßßß ßßßßß ßßßßß  ßß   ßß    ßßßßß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

void	DoFuncLife( T_OBJET *ptrobj )
{
	S32	num ;
	S32	n ;
	S32	angle ;
	S32	distance ;
	T_ZONE	*ptrz ;

	TypeAnswer = RET_S8 ;

	switch( *PtrPrg++ )	/* func */
	{
//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------
		case LF_CHAPTER:
			Value = ListVarGame[FLAG_CHAPTER] ;
			break ;

//---------------------------------------------------------------------------
		case LF_LIFE_POINT:
			Value = ptrobj->LifePoint ;
			TypeAnswer = RET_S16 ;
			break	;

//---------------------------------------------------------------------------
		case LF_HIT_BY:
			Value = ptrobj->HitBy ;
			break ;

//---------------------------------------------------------------------------
		case LF_HIT_OBJ_BY:
			Value = ListObjet[*PtrPrg++].HitBy ;
			break	;

//---------------------------------------------------------------------------
		case LF_ACTION:
			Value = ActionNormal ;
			break ;

//---------------------------------------------------------------------------
		case LF_LIFE_POINT_OBJ:
			Value = ListObjet[*PtrPrg++].LifePoint ;
			TypeAnswer = RET_S16 ;
			break	;

//---------------------------------------------------------------------------
		case LF_COL_OBJ:
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

//---------------------------------------------------------------------------
		case LF_DISTANCE:
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			if( abs(ListObjet[num].Obj.Y-ptrobj->Obj.Y) < 1500 )
			{
				distance = Distance2D( ptrobj->Obj.X,
							ptrobj->Obj.Z,
							ListObjet[num].Obj.X,
							ListObjet[num].Obj.Z ) ;
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

//---------------------------------------------------------------------------
		case LF_DISTANCE_3D:
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			distance = Distance3D( ptrobj->Obj.X,
						ptrobj->Obj.Y,
						ptrobj->Obj.Z,
						ListObjet[num].Obj.X,
						ListObjet[num].Obj.Y,
						ListObjet[num].Obj.Z ) ;

			if( distance > 32000 )
				Value = 32000 ;
			else
				Value = distance ;
			break ;

//---------------------------------------------------------------------------
		case LF_CONE_VIEW:
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			if( abs(ListObjet[num].Obj.Y-ptrobj->Obj.Y) < 1500 )
			{
				angle = GetAngle2D(	ptrobj->Obj.X,
							ptrobj->Obj.Z,
							ListObjet[num].Obj.X,
							ListObjet[num].Obj.Z ) ;

				distance = Distance2D(	ptrobj->Obj.X,
							ptrobj->Obj.Z,
							ListObjet[num].Obj.X,
							ListObjet[num].Obj.Z ) ;

				if( distance > 32000 )
					distance = 32000 ;
			}
			else
			{
				distance = 32000 ;
			}

			angle = (ptrobj->Obj.Beta-angle+CONE_VIEW/2) & (MAX_ANGLE-1) ;

			if( num == NUM_PERSO )
			{
				if( Comportement == C_DISCRET )
				{
					if( angle <= CONE_VIEW )	// 320: CONE_VIEW
					{
						Value = distance ;
					}
					else
					{
						Value = 32000 ;
					}
				}
				else	Value = distance ;
			}
			else
			{
				if( angle <= CONE_VIEW )	// 320: CONE_VIEW
				{
					Value = distance ;
				}
				else
				{
					Value = 32000 ;
				}
			}
			break ;

//---------------------------------------------------------------------------
		case LF_ZONE:
			Value = ptrobj->ZoneSce ;
			break ;

//---------------------------------------------------------------------------
		case LF_NB_GOLD_PIECES:
			if( Planet>=2  )	Value = NbZlitosPieces ;
			else			Value = NbGoldPieces ;
			TypeAnswer = RET_S16 ;
			break ;

//---------------------------------------------------------------------------
		case LF_NB_LITTLE_KEYS:
			Value = NbLittleKeys ;
			break ;

//---------------------------------------------------------------------------
		case LF_COMPORTEMENT_HERO:
			Value = Comportement ;
			break ;

//---------------------------------------------------------------------------
		case LF_MAGIC_LEVEL:
			Value = MagicLevel ;
			break ;

//---------------------------------------------------------------------------
		case LF_MAGIC_POINT:
			Value = MagicPoint ;
			break ;

//---------------------------------------------------------------------------
		case LF_CHOICE:
			Value = GameChoice ;
			TypeAnswer = RET_S16 ;
			break	;

//---------------------------------------------------------------------------
		case LF_FUEL:
//			Value = ListVarGame[FLAG_CARBURANT] ;
			TypeAnswer = RET_S16 ;
			break	;

//---------------------------------------------------------------------------
		case LF_L_TRACK:
			Value = ptrobj->LabelTrack ;
			TypeAnswer = RET_U8 ;
			break ;

//---------------------------------------------------------------------------
		case LF_ZONE_OBJ:
			Value = ListObjet[ *PtrPrg++ ].ZoneSce ;
			break ;

//---------------------------------------------------------------------------
		case LF_VAR_CUBE:
			Value = ListVarCube[ *PtrPrg++ ] ;
			TypeAnswer = RET_U8 ;
			break ;

//---------------------------------------------------------------------------
		case LF_VAR_GAME:
			Value = ListVarGame[ *PtrPrg++ ] ;
			TypeAnswer = RET_S16 ;
			break ;

//---------------------------------------------------------------------------
		case LF_USE_INVENTORY:
			num = *PtrPrg++ ;	// var game

			if( InventoryAction == num )
			{ // action ‚mise depuis l'inventaire … cette boucle
				Value = TRUE ;
			}
			else
			{
				if( (TabInv[num].FlagInv & FLAG_INV_USED)
				AND (ListVarGame[num]>0 ) )
				{
					Value = TRUE ;
				}
				else
				{
					Value = FALSE ;
				}
			}

			if( DemoSlide )
			{
				Value = TRUE ;
				InvSelect = TabInv[num].Box ;
				SaveTimer()		;
				BackupScreen( TRUE )	;
				MenuInventory( 4*1000 ) ;
				RestoreScreen()	;
				RestoreTimer()	;
			}

			if( Value == TRUE )
			{
				// Gestion de l'utilisation automatique
				if( TabInv[num].FlagInv&FLAG_INV_AUTO_USE )
				{
					TabInv[num].FlagInv |= FLAG_INV_USED ;
				}

				// init incdisp symbol utilisation
				InitIncrustDisp( INCRUST_INVENTORY|INCRUST_YCLIP,
						 (S16)num,
						 0, 0,
						 0, 0, 3 ) ;
			}
			break ;

//---------------------------------------------------------------------------
		case LF_L_TRACK_OBJ:
			Value = ListObjet[ *PtrPrg++ ].LabelTrack ;
			TypeAnswer = RET_U8 ;
			break ;

//---------------------------------------------------------------------------
		case LF_BODY:
			Value = ptrobj->GenBody ;
			break ;

//---------------------------------------------------------------------------
		case LF_BODY_OBJ:
			Value = ListObjet[*PtrPrg++].GenBody ;
			break ;

//---------------------------------------------------------------------------
		case LF_ANIM:
			Value = ptrobj->GenAnim ;
			TypeAnswer = RET_S16 ;
			break ;

//---------------------------------------------------------------------------
		case LF_ANIM_OBJ:
			Value = ListObjet[*PtrPrg++].GenAnim ;
			TypeAnswer = RET_S16 ;
			break ;

//---------------------------------------------------------------------------
		case LF_CARRY_BY:
			Value = ptrobj->CarryBy ;
			break ;

//---------------------------------------------------------------------------
		case LF_CARRY_OBJ_BY:
			Value = ListObjet[*PtrPrg++].CarryBy ;
			break ;

//---------------------------------------------------------------------------
		case LF_CDROM:
#ifdef	CDROM
			Value = 1 ;
#else
			Value = 0 ;
#endif
			break ;

//---------------------------------------------------------------------------
		case LF_ECHELLE:	// teste si echelle ON ou OFF
			num = *PtrPrg++ ;

			ptrz = ListZone ;

			n = 0 ;
			Value = 2 ;

			while( Value==2 AND n<NbZones )
			{
				if( ptrz->Type==6 AND ptrz->Num==num )
				{
					Value = ptrz->Info1 ;
				}
				ptrz++ ;
				n++ ;
			}
			break ;

//---------------------------------------------------------------------------
		case LF_RND:
			Value = *PtrPrg++ ;
			Value = MyRnd(Value) ;
			TypeAnswer = RET_U8 ;
			break ;

//---------------------------------------------------------------------------
		case LF_RAIL:	// teste si Aiguillage ON ou OFF
			num = *PtrPrg++ ;

			ptrz = ListZone ;

			n = 0 ;
			Value = 2 ;

			while( Value==2 AND n<NbZones )
			{
				if( ptrz->Type==9 AND ptrz->Num==num )
				{
					Value = ptrz->Info1 ;
				}
				ptrz++ ;
				n++ ;
			}
			break ;

//---------------------------------------------------------------------------
		case LF_BETA:
			TypeAnswer = RET_S16 ;
			Value = ptrobj->Obj.Beta ;
			break ;

//---------------------------------------------------------------------------
		case LF_BETA_OBJ:
			TypeAnswer = RET_S16 ;
			Value = ListObjet[ *PtrPrg++ ].Obj.Beta ;
			break ;

//---------------------------------------------------------------------------
		case LF_ANGLE:	// mesure l'angle entre 2 objets
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			angle = GetAngle2D( ptrobj->Obj.X, ptrobj->Obj.Z,
					ListObjet[num].Obj.X, ListObjet[num].Obj.Z ) ;

			Value = (ptrobj->Obj.Beta-angle) & (MAX_ANGLE-1) ;
			if( Value>2048 )	Value = 4096-Value ;
			break ;

//---------------------------------------------------------------------------
		case LF_ANGLE_OBJ:	// mesure l'angle entre 2 objets
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			angle = GetAngle2D( ListObjet[num].Obj.X, ListObjet[num].Obj.Z,
					    ptrobj->Obj.X, ptrobj->Obj.Z ) ;

			Value = (ListObjet[num].Obj.Beta-angle) & (MAX_ANGLE-1) ;
			if( Value>2048 )	Value = 4096-Value ;
			break ;

//---------------------------------------------------------------------------
		case LF_REAL_ANGLE:	// mesure l'angle entre 2 objets
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			angle = GetAngle2D( ptrobj->Obj.X, ptrobj->Obj.Z,
					ListObjet[num].Obj.X, ListObjet[num].Obj.Z ) ;

			Value = (ptrobj->Obj.Beta-angle) & (MAX_ANGLE-1) ;

			if( Value>2048 )	Value = 4096-Value ;
			else			Value = -Value ;
			break ;

//---------------------------------------------------------------------------
		case LF_DISTANCE_MESSAGE:
			num = *PtrPrg++ ;
			TypeAnswer = RET_S16 ;

			if( ListObjet[num].WorkFlags & OBJ_DEAD )
			{
				Value = 32000 ;
				break ;
			}

			if( abs(ListObjet[num].Obj.Y-ptrobj->Obj.Y) < 1500 )
			{
				angle = GetAngle2D( ptrobj->Obj.X, ptrobj->Obj.Z,
						ListObjet[num].Obj.X, ListObjet[num].Obj.Z ) ;

				angle = (ptrobj->Obj.Beta-angle+(256*MUL_ANGLE)) & (MAX_ANGLE-1) ;

				if( angle <= (448*MUL_ANGLE) ) // 320: CONE_VIEW
//				if( abs( ptrobj->Obj.Beta-angle )<(128*MUL_ANGLE) )
				{
					distance = Distance2D( ptrobj->Obj.X,
								ptrobj->Obj.Z,
								ListObjet[num].Obj.X,
								ListObjet[num].Obj.Z ) ;

					if( distance > 32000 )
						Value = 32000 ;
					else
						Value = distance ;
				}
				else	Value = 32000 ;
			}
			else
			{
				Value = 32000 ;
			}
			break ;

//---------------------------------------------------------------------------
		case LF_DEMO:
#ifdef	DEMO
			Value = 1 ;
#else
			if( DemoSlide )	Value = 2 ;
			else		Value = 0 ;
#endif
			break ;

//---------------------------------------------------------------------------
		case LF_COL_DECORS:
			if( ptrobj->LifePoint <= 0 )
			{
				Value = 255 ;
			}
			else
			{
				Value = (ptrobj->Col&128)?TRUE:FALSE ;
			}
			TypeAnswer = RET_U8 ;
			break ;

//---------------------------------------------------------------------------
		case LF_COL_DECORS_OBJ:
			num = *PtrPrg++ ;
			if( ListObjet[num].LifePoint <= 0 )
			{
				Value = 255 ;
			}
			else
			{
				Value = (ListObjet[num].Col&128)?TRUE:FALSE ;
			}
			break ;

//---------------------------------------------------------------------------
		case LF_PROCESSOR:
#ifdef	PSX	// bonjour messieurs les codeurs PSX !!!!!!
			Value = 2 ;
#else
			if( ProcessorSignature.Family>4 )
			{
				Value = 0 ;	// Pentium
			}
			else
			{
				Value = 1 ;	// 486
			}
#endif
			break ;

//---------------------------------------------------------------------------
		case LF_OBJECT_DISPLAYED:
			Value = (ListObjet[ *PtrPrg++ ].WorkFlags&WAS_DRAWN)?TRUE:FALSE ;
			break ;
	}
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

S32    DoTest()
{
	U8	test ;
	S32	valueword ;
	S8	valuebyte ;
	U8	valueUbyte ;

	test = *PtrPrg++ ;

	switch( TypeAnswer )
	{
//---------------------------------------------------------------------------
	case RET_S8:

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

//---------------------------------------------------------------------------
	case RET_U8:
		valueUbyte = (U8)*PtrPrg++ ;
		switch( test )
		{
		case LT_EQUAL:
			return (Value == valueUbyte) ;

		case LT_SUP:
			return (Value > valueUbyte) ;

		case LT_LESS:
			return (Value < valueUbyte) ;

		case LT_SUP_EQUAL:
			return (Value >= valueUbyte) ;

		case LT_LESS_EQUAL:
			return (Value <= valueUbyte) ;

		case LT_DIFFERENT:
			return (Value != valueUbyte) ;
		}

//---------------------------------------------------------------------------
	case RET_S16:

		valueword = *(S16*)PtrPrg ;
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

//---------------------------------------------------------------------------
	case RET_STRING:

		valueword = stricmp( String, (char *)PtrPrg ) ;
		PtrPrg += strlen( (char *)PtrPrg ) + 1 ;

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

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

void	CleanTrack( T_OBJET *ptrobj )
{
	U8	*ptrtrack ;

	if( ptrobj->OffsetTrack!=-1 )
	{
	       ptrobj->WorkFlags &= ~(TRACK_MASTER_ROT) ;

	       ptrtrack = ptrobj->PtrTrack+ptrobj->OffsetTrack ;

	       switch( *ptrtrack )
	       {
		       case TM_FACE_TWINSEN:
			       *(S16*)(ptrtrack+1) = -1 ;
			       break ;

		       case TM_WAIT_NB_ANIM:
			       *(U8*)(ptrtrack+2) = 0 ;
			       break ;

		       case TM_WAIT_NB_SECOND:
		       case TM_WAIT_NB_DIZIEME:
		       case TM_WAIT_NB_SECOND_RND:
		       case TM_WAIT_NB_DIZIEME_RND:
			       *(U32*)(ptrtrack+2) = 0 ;
			       break ;

		       case TM_ANGLE:
			       *(S16*)(ptrtrack+1) &= 0x7FFF ;
			       break ;

		       case TM_ANGLE_RND:
			       *(S16*)(ptrtrack+3) = -1 ;
			       break ;

		       case TM_LOOP:
			       *(ptrtrack+2) = *(ptrtrack+1) ;
			       break ;
	       }
	}
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

void	AdjustDirObject( T_OBJET *ptrobj, S32 oldmove )
{
	S32	x, z 	;
	S16	nu 	;

	switch( ptrobj->Move )
	{
		case MOVE_WAGON:
			ptrobj->WorkFlags |= MANUAL_INTER_FRAME ;
			break  ;

		case MOVE_SAME_XZ:
		case MOVE_SAME_XZ_BETA:
			nu = ptrobj->CarryBy ;

			if( nu!=-1 AND !(nu&CARRY_BY_DECORS) )
			{
				ptrobj->CarryBy = -1 ;// on verra … la prochaine boucle
			}

		case MOVE_FOLLOW:
			ptrobj->Info3 = *PtrPrg++ ;
			break ;

		case MOVE_CIRCLE:
		case MOVE_CIRCLE2:
			ptrobj->Info3 = *PtrPrg++ ;

			x = ListBrickTrack[ptrobj->Info3].X ;
			z = ListBrickTrack[ptrobj->Info3].Z ;

			ptrobj->Info  = Distance2D( x,z,ptrobj->Obj.X,ptrobj->Obj.Z ) ;	// Distance entre le drapeau
														// et l'objet
			ptrobj->Info1 = GetAngle2D( x,z,ptrobj->Obj.X,ptrobj->Obj.Z ) ;	// Angle d'origine
			ptrobj->Info2 = TimerRefHR ;
			break ;

		case MOVE_BUGGY:
			if( oldmove!=MOVE_BUGGY_MANUAL )
			{
				TakeBuggy() ;
			}
			ptrobj->WorkFlags &= ~(MANUAL_INTER_FRAME) ;
			break ;

		case MOVE_BUGGY_MANUAL:
			if( oldmove!=MOVE_BUGGY )
			{
				TakeBuggy() ;
			}
			ptrobj->WorkFlags |= MANUAL_INTER_FRAME ;
			break ;
	}
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

void	DoLife( U8 numobj )
{
	S32	flag = 0 ;
	T_OBJET	*ptrobj ;
	T_ZONE	*ptrz ;

	S32	obj ;
	S32	num ;
	U8	*ptrmacro ;
	U8	*memoptr ;
	S32	n ;

	ptrobj = &ListObjet[numobj] ;

	PtrPrg = ptrobj->PtrLife + ptrobj->OffsetLife ;

	while( flag != -1 )
	{
		ptrmacro = PtrPrg ;
		switch( *PtrPrg++ )
		{
//---------------------------------------------------------------------------
			case LM_END:
			case LM_END_LIFE:
				ptrobj->OffsetLife = -1 ;

//---------------------------------------------------------------------------
			case LM_RETURN:
			case LM_END_COMPORTEMENT:
				flag = -1 ;
				break ;

//---------------------------------------------------------------------------
			case LM_COMPORTEMENT:
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_FALLABLE:
				num = *PtrPrg++ ;

				switch( num )
				{
					case 1:
						ptrobj->Flags |= OBJ_FALLABLE ;
						break	;

					// Arrete de tomber si je suis en train de
					// tomber (parametre 2)
					case 2:
						if( ptrobj->WorkFlags&FALLING )
						{
							ptrobj->WorkFlags &= ~FALLING ;
							InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, numobj ) ;
						}
					case 0:
						ptrobj->Flags &= ~OBJ_FALLABLE ;
						break ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_COMPORTEMENT_HERO:
				InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, NUM_PERSO ) ;
				SetComportement( *PtrPrg++ ) ;

				if( DemoSlide AND (FirstTime!=AFF_ALL_FLIP) )	MenuComportement( 2*1000 ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_MAGIC_LEVEL:
// si sce permet objets magique ordre quelconque transforme SET en INC
				MagicLevel = *PtrPrg++ ;
				// remplie magie a fond
				MagicPoint = (U8)(MagicLevel * 20) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SUB_MAGIC_POINT:
				if( MagicPoint>*PtrPrg )
					MagicPoint -= *PtrPrg ;
				else	MagicPoint = 0 ;
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_CAM_FOLLOW:
				NumObjFollow = *PtrPrg++ ;
				FirstTime = AFF_ALL_FLIP ;
				StartXCube = ((ListObjet[NumObjFollow].Obj.X)/SIZE_BRICK_XZ) ;
				StartYCube = ((ListObjet[NumObjFollow].Obj.Y+SIZE_BRICK_Y)/SIZE_BRICK_Y) ;
				StartZCube = ((ListObjet[NumObjFollow].Obj.Z)/SIZE_BRICK_XZ) ;

				// Pour v‚rifier si l'objet … suivre se trouve dans une
				// zone cam‚ra
				CheckZoneSce( &ListObjet[NumObjFollow], NumObjFollow ) ;

				CameraCenter( 0 ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_KILL_OBJ:
				num = *PtrPrg++	;
//				CheckCarrier( (U8)num ) ;
				ListObjet[num].WorkFlags |= OBJ_DEAD	;
				ListObjet[num].Obj.Body.Num = -1		;
				ListObjet[num].ZoneSce = -1		;
				ListObjet[num].LifePoint = 0		;
				break	;

//---------------------------------------------------------------------------
			case LM_SUICIDE:
//				CheckCarrier( numobj ) ;
				ptrobj->WorkFlags |= OBJ_DEAD	;
				ptrobj->Obj.Body.Num = -1		;
				ptrobj->ZoneSce = -1 ;
				ptrobj->LifePoint = 0 ;
				flag = -1 ;
				break	;

//---------------------------------------------------------------------------
			case LM_SET_DIR:
				{
				S32	oldmove = ptrobj->Move ;

				switch( oldmove )
				{
					case MOVE_WAGON:
						ptrobj->WorkFlags &= ~(MANUAL_INTER_FRAME) ;

					case MOVE_CIRCLE:
					case MOVE_CIRCLE2:
					case MOVE_MANUAL:
						ClearRealAngle( ptrobj ) ;
						break ;

					// ATTENTION: QUE Twinsen !!!!!!!!
					case MOVE_BUGGY_MANUAL:
						if( *PtrPrg!=MOVE_BUGGY )
						{
							LeaveBuggy( C_NORMAL ) ;
						}
						ClearRealAngle( ptrobj ) ;
						break ;

					// ATTENTION: QUE Twinsen !!!!!!!!
					case MOVE_BUGGY:
						if( *PtrPrg!=MOVE_BUGGY_MANUAL )
						{
							LeaveBuggy( C_NORMAL ) ;
						}
						break ;
				}

				ptrobj->Move = *PtrPrg++ ;

				AdjustDirObject( ptrobj, oldmove ) ;

				if( numobj==NUM_PERSO
				AND ProtectActif
				AND ptrobj->Move!=MOVE_MANUAL )
				{
					ToggleSortProtection() ;
				}
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SET_DIR_OBJ:
				{
				S32	oldmove  ;
				T_OBJET	*ptrobjt ;

				obj = *PtrPrg++ ;
				ptrobjt = &ListObjet[obj] ;

				if( ptrobjt->LifePoint>0 )
				{
					oldmove = ptrobjt->Move ;

					switch( oldmove )
					{
						case MOVE_WAGON:
							ptrobj->WorkFlags &= ~(MANUAL_INTER_FRAME) ;

						case MOVE_CIRCLE:
						case MOVE_CIRCLE2:
						case MOVE_MANUAL:
							ClearRealAngle( ptrobjt ) ;
							break ;

						// ATTENTION: QUE Twinsen !!!!!!!!
						case MOVE_BUGGY_MANUAL:
							if( *PtrPrg!=MOVE_BUGGY )
							{
								LeaveBuggy( C_NORMAL ) ;
							}
							ClearRealAngle( ptrobjt ) ;
							break ;

						// ATTENTION: QUE Twinsen !!!!!!!!
						case MOVE_BUGGY:
							if( *PtrPrg!=MOVE_BUGGY_MANUAL )
							{
								LeaveBuggy( C_NORMAL ) ;
							}
							break ;
					}

					ptrobjt->Move = *PtrPrg++ ;

					AdjustDirObject( ptrobjt, oldmove ) ;

					if( obj==NUM_PERSO
					AND ProtectActif
					AND ptrobjt->Move!=MOVE_MANUAL )
					{
						ToggleSortProtection() ;
					}
				}
				else	PtrPrg++ ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SET_COMPORTEMENT:
				ptrobj->OffsetLife = *(S16*)PtrPrg ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_COMPORTEMENT_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					ListObjet[num].OffsetLife = *(S16*)PtrPrg ;
				}
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_LIFE_POINT_OBJ:
				num = *PtrPrg++ ;

				if( ListObjet[num].WorkFlags & OBJ_DEAD )
				{
					// Au cas ou on veuille ressuciter 1 objet
					ListObjet[num].WorkFlags &= ~OBJ_DEAD ;
					InitBody( GEN_BODY_NORMAL, (U8)num ) ;
					InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, (U8)num ) ;
				}

				ListObjet[num].LifePoint = *PtrPrg++ ;

				if( ListObjet[num].LifePoint > MAX_LIFE_POINTS )
					ListObjet[num].LifePoint = MAX_LIFE_POINTS ;
				break ;

//---------------------------------------------------------------------------
			case LM_SUB_LIFE_POINT_OBJ:
				num = *PtrPrg++ ;
				ListObjet[num].LifePoint -= *PtrPrg++ ;
				if( ListObjet[num].LifePoint < 0 )
					ListObjet[num].LifePoint = 0 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_LIFE_POINT_OBJ:
				num = *PtrPrg++ ;

				if( ListObjet[num].WorkFlags & OBJ_DEAD )
				{
					// Au cas ou on veuille ressuciter 1 objet
					ListObjet[num].WorkFlags &= ~OBJ_DEAD ;
					InitBody( GEN_BODY_NORMAL, (U8)num ) ;
					InitAnim( GEN_ANIM_RIEN, ANIM_REPEAT, (U8)num ) ;
				}

				ListObjet[num].LifePoint += *PtrPrg++ ;

				if( ListObjet[num].LifePoint > MAX_LIFE_POINTS )
					ListObjet[num].LifePoint = MAX_LIFE_POINTS ;
				break ;

//---------------------------------------------------------------------------
			case LM_HIT_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					HitObj( numobj, (U8)num, (S32)*PtrPrg, ListObjet[num].Obj.Beta ) ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_TRACK:
				CleanTrack( ptrobj ) ;
				ptrobj->OffsetTrack = *(S16*)PtrPrg ;
				PtrPrg += 2 ;
				Track_SetLabel( ptrobj ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_TRACK_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					CleanTrack( &ListObjet[num] ) ;
					ListObjet[num].OffsetTrack = *(S16*)PtrPrg ;
					Track_SetLabel( &ListObjet[num] ) ;
				}
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_STOP_L_TRACK:
				CleanTrack( ptrobj ) ;
				ptrobj->MemoLabelTrack = ptrobj->OffsetLabelTrack ;
				ptrobj->OffsetTrack = -1 ;
				break ;

//---------------------------------------------------------------------------
			case LM_STOP_L_TRACK_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					CleanTrack( &ListObjet[num] ) ;
					ListObjet[num].MemoLabelTrack = ListObjet[num].OffsetLabelTrack ;
					ListObjet[num].OffsetTrack = -1 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_RESTORE_L_TRACK:
				CleanTrack( ptrobj ) ;
				ptrobj->OffsetTrack = ptrobj->MemoLabelTrack ;
				Track_SetLabel( ptrobj ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_RESTORE_L_TRACK_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					CleanTrack( &ListObjet[num] ) ;
					ListObjet[num].OffsetTrack = ListObjet[num].MemoLabelTrack ;
					Track_SetLabel( &ListObjet[num] ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_IF:
			case LM_AND_IF:
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				}
				else
				{
					PtrPrg += 2 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_OR_IF:
				DoFuncLife( ptrobj ) ;
				if( DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				}
				else
				{
					PtrPrg += 2 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SWIF:
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				}
				else
				{
					*ptrmacro = LM_SNIF ;
					PtrPrg += 2 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SNIF:	/* swif invers‚ tjrs jump jusqu'a cond inverse */
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					*ptrmacro = LM_SWIF ;
				}
				PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				break ;

//---------------------------------------------------------------------------
			case LM_ONEIF:
				DoFuncLife( ptrobj ) ;
				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				}
				else
				{
					PtrPrg += 2 ;
					*ptrmacro = LM_NEVERIF ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_NEVERIF:
				DoFuncLife( ptrobj ) ;
				DoTest() ;
				PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				break ;

//---------------------------------------------------------------------------
			case LM_OFFSET: 	/* jmp … offset */
				PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				break ;

//---------------------------------------------------------------------------
			case LM_ELSE:
				PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				break ;

//---------------------------------------------------------------------------
			case LM_SWITCH:
				ptrobj->ExeSwitch.Func = *PtrPrg ;
				DoFuncLife( ptrobj ) ;
				ptrobj->ExeSwitch.Value = (S16)Value ;
				ptrobj->ExeSwitch.TypeAnswer = TypeAnswer ;
				break ;

//---------------------------------------------------------------------------
			case LM_CASE:
				Value = ptrobj->ExeSwitch.Value ;
				TypeAnswer = ptrobj->ExeSwitch.TypeAnswer ;

				memoptr = PtrPrg ;
				PtrPrg+=2 ;	// Saute offset

				if( !DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(S16*)memoptr ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_OR_CASE:
				Value = ptrobj->ExeSwitch.Value ;
				TypeAnswer = ptrobj->ExeSwitch.TypeAnswer ;

				memoptr = PtrPrg ;
				PtrPrg+=2 ;	// Saute offset

				if( DoTest() )
				{
					PtrPrg = ptrobj->PtrLife + *(S16*)memoptr ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_BREAK:	// saute au END_SWITCH
				PtrPrg = ptrobj->PtrLife + *(S16*)PtrPrg ;
				break ;

//---------------------------------------------------------------------------
			case LM_BODY:
				InitBody( *PtrPrg++, numobj ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_BODY_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					InitBody( *PtrPrg, (U8)num ) ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_ANIM_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					InitAnim( *(U16*)PtrPrg, ANIM_REPEAT, (U8)num ) ;
				}
				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ANIM:
				InitAnim( *(U16*)PtrPrg, ANIM_REPEAT,numobj ) ;
				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ANIM_SET:
//				ptrobj->GenAnim = NO_ANIM ;
//				ptrobj->Obj.Anim.Num = -1 ;
//				InitAnim( *(U16*)PtrPrg, ANIM_REPEAT, numobj ) ;
				SetAnim( numobj, *(U16*)PtrPrg ) ;
				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_MESSAGE_OBJ:
				num = *PtrPrg++	;
				if( ListObjet[num].LifePoint>0 )
				{
#ifdef	CDROM
					NumObjSpeak = num ;
#endif
					NumObjDial = (U8)num ;

					// au cas ou l'on soit appel‚ pendant un eclair
					Palette( PtrPal ) ;

					Dial( *(S16*)PtrPrg, TRUE ) ;
				}
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_MESSAGE_OBJ:
				num = *PtrPrg++	;
				if( ListObjet[num].LifePoint>0 )
				{
#ifdef	CDROM
					NumObjSpeak = num ;
#endif
					NumObjDial = (U8)num ;

					// au cas ou l'on soit appel‚ pendant un eclair
					Palette( PtrPal ) ;

					Dial( *(S16*)PtrPrg, TRUE ) ;
				}
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_MESSAGE:
#ifdef	CDROM
				NumObjSpeak = numobj ;
#endif
				NumObjDial = numobj ;

				// au cas ou l'on soit appel‚ pendant un eclair
				Palette( PtrPal ) ;

				Dial( *(S16*)PtrPrg, TRUE ) ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_MESSAGE:
#ifdef	CDROM
				NumObjSpeak = numobj ;
#endif
				NumObjDial = numobj ;

				// au cas ou l'on soit appel‚ pendant un eclair
				Palette( PtrPal ) ;

				Dial( *(S16*)PtrPrg, TRUE ) ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_MESSAGE_ZOE:
#ifdef	CDROM
				NumObjSpeak = numobj ;
#endif
				NumObjDial = NUM_PERSO ;
				ListObjet[NUM_PERSO].CoulObj = COUL_ZOE ;

				// au cas ou l'on soit appel‚ pendant un eclair
				Palette( PtrPal ) ;

				Dial( *(S16*)PtrPrg, TRUE ) ;
				ListObjet[NUM_PERSO].CoulObj = COUL_PERSO ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_VAR_CUBE:
				num = *PtrPrg++ ;
				ListVarCube[num] = *PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_VAR_CUBE:
				num = *PtrPrg++ ;
				if( ListVarCube[num]+*PtrPrg<255 )
					ListVarCube[num] += *PtrPrg ;
				else	ListVarCube[num] = 255 ;
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SUB_VAR_CUBE:
				num = *PtrPrg++ ;
				if( ListVarCube[num]-*PtrPrg>0 )
					ListVarCube[num] -= *PtrPrg ;
				else	ListVarCube[num] = 0 ;
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_VAR_GAME:
				num = *PtrPrg++ ;
				ListVarGame[num] = *(S16*)PtrPrg ;

				if( num==FLAG_MONEY )
				{
					if( Planet>=2 )	NbGoldPieces   = ListVarGame[FLAG_MONEY] ;
					else		NbZlitosPieces = ListVarGame[FLAG_MONEY] ;
				}

				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_VAR_GAME:
				num = *PtrPrg++ ;
				if( ListVarGame[num]+*(S16*)PtrPrg<MAX_S16 )
					ListVarGame[num] += *(S16*)PtrPrg ;
				else	ListVarGame[num] = MAX_S16 ;

				if( num==FLAG_MONEY )
				{
					if( Planet>=2 )	NbGoldPieces   = ListVarGame[FLAG_MONEY] ;
					else		NbZlitosPieces = ListVarGame[FLAG_MONEY] ;
				}

				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SUB_VAR_GAME:
				num = *PtrPrg++ ;
				if( ListVarGame[num]-*(S16*)PtrPrg>MIN_S16 )
					ListVarGame[num] -= *(S16*)PtrPrg ;
				else	ListVarGame[num] = MIN_S16 ;

				if( num==FLAG_MONEY )
				{
					if( Planet>=2 )	NbGoldPieces   = ListVarGame[FLAG_MONEY] ;
					else		NbZlitosPieces = ListVarGame[FLAG_MONEY] ;
				}

				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_USED_INVENTORY:
				num = *PtrPrg++ ;
				if( num<MAX_INVENTORY )
					TabInv[num].FlagInv |= FLAG_INV_USED;
				break ;

//---------------------------------------------------------------------------
			case LM_GIVE_GOLD_PIECES:
				if( Planet>=2 )
				{
					num = NbZlitosPieces ;
					NbZlitosPieces -= *(S16*)PtrPrg ;
					if( NbZlitosPieces < 0 )
					{
						NbZlitosPieces = 0 ;
					}
				}
				else
				{
					num = NbGoldPieces ;
					NbGoldPieces -= *(S16*)PtrPrg ;
					if( NbGoldPieces < 0 )
					{
						NbGoldPieces = 0 ;
					}
				}

				PtrPrg += 2 ;

				// sample cling cling
//				HQ_MixSample( 83, 0x1000, 2, 128, 128 ) ;

				// aff piece
				InitIncrustDisp( INCRUST_SPRITE|INCRUST_YCLIP,
						 (S16)SPRITE_PIECE,	// Kashes ou Zlitos
						 12, 15,
						 0, 0, 3 ) ;

				// aff cmpt si pas
				{
					S32	money ;
					T_INCRUST_DISP	*ptrdisp ;
					ptrdisp = ListIncrustDisp ;

					if( Planet>=2 )	money = NbZlitosPieces 	;
					else		money = NbGoldPieces	;

					for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
					{
						if( ptrdisp->Num == -1 )
							continue ;

						if( ptrdisp->Type==(INCRUST_CMPT|INCRUST_YCLIP) )
						{
							ptrdisp->Num = -1 ;
							break ;
						}
					}

					InitIncrustDisp( INCRUST_CMPT|INCRUST_YCLIP,
							 (S16)num,
							 32, 20,
							 (S16)money, 0, 3 ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_GOLD_PIECES:
				if( Planet>=2 )
				{
					num = NbZlitosPieces ;
					NbZlitosPieces += *(S16*)PtrPrg ;
					if( NbZlitosPieces > 999 )
					{
						NbZlitosPieces = 999 ;
					}
				}
				else
				{
					num = NbGoldPieces ;
					NbGoldPieces += *(S16*)PtrPrg ;
					if( NbGoldPieces > 999 )
					{
						NbGoldPieces = 999 ;
					}
				}

				PtrPrg += 2 ;
				// affiche piece trouvee
				HQ_3D_MixSample( SAMPLE_BONUS_TROUVE, 0x1000, 0, 1,
					ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;

				// incruste pendant 2 sec le bonus trouve
				InitIncrustDisp( INCRUST_SPRITE|INCRUST_YCLIP,
						 (S16)SPRITE_PIECE,	// Kashes ou Zlitos
						 10, 30,
						 0, 0, 2 ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_USE_ONE_LITTLE_KEY:
				if( NbLittleKeys>0 )
				{
					num = NbLittleKeys-- ;
					// affiche little key
/*					InitIncrustDisp( INCRUST_SPRITE,
							 SPRITE_CLE,
							 0, 0,
							 NUM_PERSO, 1, 1 ) ;
*/
					// On pourrait mettre une anim sur Twinsen
					// ou bien juste un sample de serrure ?

					InitIncrustDisp( INCRUST_SPRITE|INCRUST_YCLIP,
							 SPRITE_CLE,	// Kashes ou Zlitos
							 12, 15,
							 0, 0, 2 ) ;

					// aff cmpt si pas
					{
						T_INCRUST_DISP	*ptrdisp ;
						ptrdisp = ListIncrustDisp ;

						for( n=0; n<MAX_INCRUST_DISP; n++, ptrdisp++ )
						{
							if( ptrdisp->Num == -1 )
								continue ;

							if( ptrdisp->Type==INCRUST_CMPT )
							{
								ptrdisp->Num = -1 ;
							}
						}

						InitIncrustDisp( INCRUST_CMPT|INCRUST_YCLIP,
								 (S16)num,
								 32, 20,
								 (S16)NbLittleKeys, 0, 2 ) ;
					}
				}
				break ;

//---------------------------------------------------------------------------
			case LM_INC_CHAPTER:
				ListVarGame[FLAG_CHAPTER]++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_STATE_INVENTORY:
				num = *PtrPrg++ ;	// num vargame
				TabInv[num].IdObj3D = *PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_FOUND_OBJECT:
				SaveTimer() ;
				DoFoundObj( *PtrPrg++ ) ;
				RestoreTimer() ;
//				AffScene( AFF_ALL_FLIP ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_DOOR_LEFT:
				ptrobj->Obj.Beta = (768*MUL_ANGLE) ;
				ptrobj->Obj.X = ptrobj->Obj.LastAnimStepX - *(S16*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_DOOR_RIGHT:
				ptrobj->Obj.Beta = (256*MUL_ANGLE) ;
				ptrobj->Obj.X = ptrobj->Obj.LastAnimStepX + *(S16*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_DOOR_UP:
				ptrobj->Obj.Beta = (512*MUL_ANGLE) ;
				ptrobj->Obj.Z = ptrobj->Obj.LastAnimStepZ - *(S16*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_DOOR_DOWN:
				ptrobj->Obj.Beta = (0*MUL_ANGLE) ;
				ptrobj->Obj.Z = ptrobj->Obj.LastAnimStepZ + *(S16*)PtrPrg ;
				ptrobj->WorkFlags &= ~AUTO_STOP_DOOR ;
				ptrobj->SRot = 0 ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_GIVE_BONUS:
				if( ptrobj->OptionFlags & EXTRA_MASK )
				{
					GiveExtraBonus( ptrobj ) ;

				}
				if( !(*PtrPrg++) )
				{
					// ne donne plus rien ????
					ptrobj->OptionFlags |= EXTRA_GIVE_NOTHING ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_CHANGE_CUBE:
				NewCube = *PtrPrg++ ;
				FlagChgCube = 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_FUEL:
/*				ListVarGame[FLAG_CARBURANT] += *PtrPrg++ ;
				if( ListVarGame[FLAG_CARBURANT] > 100 )
				{
					ListVarGame[FLAG_CARBURANT] = 100 ;
				}
*/
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SUB_FUEL:
/*				ListVarGame[FLAG_CARBURANT] -= *PtrPrg++ ;
				if( ListVarGame[FLAG_CARBURANT] < 0 )
				{
					ListVarGame[FLAG_CARBURANT] = 0 ;
				}
*/
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_HOLO_POS:
				if( SetHoloPos( *PtrPrg++ ) )
				{
					if( ListVarGame[FLAG_HOLOMAP] )
					{
						InitIncrustDisp( INCRUST_INVENTORY|INCRUST_YCLIP,
								 FLAG_HOLOMAP,	// holomap
								 0, 0,
								 0, 0, 3 ) ;
					}
				}
				break ;

//---------------------------------------------------------------------------
			case LM_CLR_HOLO_POS:
				ClrHoloPos( *PtrPrg++ ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_GRM:	// ne marche que si pas autre grm
				num = *PtrPrg++; // num zone

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==3 AND ptrz->Num==num )
					{
						if( *PtrPrg )
						{
							if( !ptrz->Info2 )
								IncrustGrm( ptrz ) ;
						}
						else
						{
							if( ptrz->Info2 )
								DesIncrustGrm( ptrz ) ;
						}

						ptrz->Info2 = *PtrPrg ; // on/off
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_CHANGE_CUBE: // Zone Changement de Cube
				num = *PtrPrg++;

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==0 AND ptrz->Info4==num )
					{
						if( *PtrPrg )	ptrz->Info7 |= ZONE_ON ;
						else		ptrz->Info7 &= ~(ZONE_ON) ;
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_INC_CLOVER_BOX:
				if( NbCloverBox < MAX_CLOVER_BOX )
				{
					NbCloverBox++ ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_OBJ_COL:
				if( *PtrPrg++ )	ptrobj->Flags |= CHECK_OBJ_COL ;
				else		ptrobj->Flags &= ~CHECK_OBJ_COL ;
				break ;

//---------------------------------------------------------------------------
			case LM_INVISIBLE:
				if( *PtrPrg++ )	ptrobj->Flags |= INVISIBLE ;
				else		ptrobj->Flags &= ~INVISIBLE ;
				break ;

//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------
			case LM_POS_POINT:
				{
				S16	nu ;

				Value = *PtrPrg++ ;	// index point
				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				ptrobj->Obj.X = X0 ;
				ptrobj->Obj.Y = Y0 ;
				ptrobj->Obj.Z = Z0 ;

				// Reinit Wagon pour le cas ou il se trouvait
				// dans un virage ou dans un aiguillage
				// (ca foutait la m.... !)
				if( ptrobj->Move==MOVE_WAGON )
				{
					ptrobj->Sprite = 0 ;
					ptrobj->Info = 0 ;
					ptrobj->Info1 = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Info3 = 0 ;
				}

				// Pour v‚rifier si l'objet … suivre se trouve dans une
				// zone cam‚ra
				CheckZoneSce( ptrobj, numobj ) ;

				// Pour dire … un objet qu'il n'est plus port‚
				// par moi
				CheckCarrier( numobj ) ;

				// si je suis port‚ par un objet
				nu = ptrobj->CarryBy ;

				if( nu!=-1 AND !(nu&CARRY_BY_DECORS) )
				{
					ptrobj->CarryBy = -1 ;
				}

				if( numobj==NUM_PERSO )	StartYFalling = 0 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_PLAY_ACF:
				SaveTimer() ;
				if( !FlagFade )	FadeToBlackAndSamples( PtrPal ) ;

				FlagFade = TRUE ;

				if( (PlayAcf( (char *)PtrPrg )==K_ESC)
				AND CinemaMode>1 )
				{
					ResetCinemaMode() ;
					flag = -1 ;	// sort de la boucle
				}
				else
				{
					if( CubeMode==CUBE_INTERIEUR )
					{
						InitGrille( NumCube ) ;
					}
					RazListPartFlow() ;
					ChoicePalette()   ;
				}

				PtrPrg += strlen( (char *)PtrPrg ) + 1 ;
				FlagFade = TRUE ;
				FirstTime = AFF_ALL_FLIP ;
				RestoreTimer() ;
				break ;

//---------------------------------------------------------------------------
			case LM_ADD_CHOICE:
				GameListChoice[GameNbChoices] = *(S16*)PtrPrg ;
				PtrPrg += 2 ;
				GameNbChoices++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_ASK_CHOICE:
				if( !DemoSlide )
				{
					SaveTimer() ;
					TestCoulDial( ptrobj->CoulObj ) ;
					NumObjDial = numobj ;

					// au cas ou l'on soit appel‚ pendant un eclair
					Palette( PtrPal ) ;

					GameAskChoice( *(S16*)PtrPrg ) ;
					GameNbChoices = 0 ;
					PtrPrg += 2 ;
					RestoreTimer() ;
					AffScene( AFF_ALL_FLIP )	;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_ASK_CHOICE_OBJ:
				if( !DemoSlide )
				{
					num = *PtrPrg++	;
					if( ListObjet[num].LifePoint>0 )
					{
						SaveTimer() ;
						TestCoulDial( ListObjet[num].CoulObj ) ;
						NumObjDial = (U8)num ;

						// au cas ou l'on soit appel‚ pendant un eclair
						Palette( PtrPal ) ;

						GameAskChoice( *(S16*)PtrPrg ) ;
						GameNbChoices = 0 ;
						RestoreTimer() ;
						AffScene( AFF_ALL_FLIP )	;
					}
					PtrPrg += 2 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_FULL_POINT:
				ListObjet[NUM_PERSO].LifePoint = MAX_LIFE_POINTS ;
				MagicPoint = (U8)(MagicLevel * 20) ;
				if( ListVarGame[FLAG_CONQUE] )
				{
					TabInv[FLAG_CONQUE].PtMagie = INV_MAX_VALUE ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_BETA:
				ptrobj->Obj.Beta = *(S16*)PtrPrg ;
				ClearRealAngle( ptrobj ) ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_BULLE:
				Bulle = *PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
/*			case LM_MESSAGE_CHAPTER:
				if( ListVarGame[FLAG_CHAPTER]<MAX_CHAPTER )
				{
					NumObjDial = numobj ;

					if( numobj==NUM_PERSO )
					{
						S32 max = 0 ;

						// Compte nombre de messages valides
						// pour ce chapitre
						for( n=0; n<5; n++ )
						{
							if( MessChapitreTwinsen[ListVarGame[FLAG_CHAPTER]][n]!=-1 )
							{
								max++ ;
							}
						}

						if( max )
						{
							// tire un message aleatoirement
							Dial( MessChapitreTwinsen[ListVarGame[FLAG_CHAPTER]][MyRnd(max)], TRUE ) ;
						}
					}
					else
					{
						Dial( ptrobj->MessageChapter[ListVarGame[FLAG_CHAPTER]], TRUE ) ;
					}
				}
				break ;
*/
//---------------------------------------------------------------------------
			case LM_GAME_OVER:
				ListObjet[NUM_PERSO].LifePoint = 0 ;
				ListObjet[NUM_PERSO].WorkFlags |= ANIM_END ;
				ListVarGame[FLAG_CLOVER] = 0 ;
				flag = -1 ;
				break ;

//---------------------------------------------------------------------------
			case LM_THE_END:
				FlagTheEnd = 1 ;	// credits
				flag = -1 ;
				break ;

//---------------------------------------------------------------------------
			case LM_BRUTAL_EXIT:
				FlagTheEnd = 0 ;
				flag = -1 ;
				break ;

//---------------------------------------------------------------------------
			case LM_PLAY_MUSIC:
#ifdef	CDROM
				PlayMusic( *PtrPrg, TRUE ) ;
#endif
				CubeJingle = 255;
				NextMusic = -1 	;// Coupe Empilage

				// on ne veut pas couper la musique de la fin
				if( PLAY_THE_END )	StopLastMusic = FALSE ;

				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_ECHELLE: // Echelle on/off
				num = *PtrPrg++;

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==6 AND ptrz->Num==num )
					{
						ptrz->Info1 = *PtrPrg ;
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_HIT_ZONE: // Zone hit
				num = *PtrPrg++;

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==8 AND ptrz->Num==num )
					{
						ptrz->Info1 = *PtrPrg ;
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_ARMURE:
				ptrobj->Armure = *(S8*)PtrPrg ;
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_ARMURE_OBJ:
				num = *PtrPrg++	;
				if( ListObjet[num].LifePoint>0 )
				{
					ListObjet[num].Armure = *(S8*)PtrPrg ;
				}
				PtrPrg++ ;
				break;

//---------------------------------------------------------------------------
			case LM_SAVE_COMPORTEMENT:
				ptrobj->MemoComportement = ptrobj->OffsetLife ;
				break ;

//---------------------------------------------------------------------------
			case LM_SAVE_COMPORTEMENT_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					ListObjet[num].MemoComportement = ListObjet[num].OffsetLife ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_RESTORE_COMPORTEMENT:
				ptrobj->OffsetLife = ptrobj->MemoComportement ;
				break ;

//---------------------------------------------------------------------------
			case LM_RESTORE_COMPORTEMENT_OBJ:
				num = *PtrPrg++ ;
				if( ListObjet[num].LifePoint>0 )
				{
					ListObjet[num].OffsetLife = ListObjet[num].MemoComportement ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SAMPLE:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				HQ_3D_MixSample( num, 0x1000, 0, 1,
					ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SAMPLE_RND:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				HQ_3D_MixSample( num, 0x800, 0x1000, 1,
					ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SAMPLE_ALWAYS:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;

				if( ptrobj->SampleAlways
				AND ((ptrobj->SampleAlways>>8)&0xFFFF)!=num )
				{
					HQ_StopOneSample( ptrobj->SampleAlways ) ;
					ptrobj->SampleAlways = 0 ;
				}

				if( !ptrobj->SampleAlways
				OR  !IsSamplePlaying( ptrobj->SampleAlways ) )
				{
					ptrobj->SampleVolume = (U8)ParmSampleVolume ;

					ptrobj->SampleAlways = HQ_3D_MixSample( num, 0x1000, 0, 0,
								ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
				}
				else
				{
					if( ptrobj->SampleAlways )	num = ptrobj->SampleAlways ;

					HQ_3D_ChangePanSample( num,
							       ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SAMPLE_STOP:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;

				if( ptrobj->SampleAlways
				AND ((ptrobj->SampleAlways>>8)&0xFFFF)==num )
				{
					num = ptrobj->SampleAlways ;
					ptrobj->SampleAlways = 0 ;
				}

				HQ_StopOneSample( num ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_REPEAT_SAMPLE:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				HQ_3D_MixSample( num, 0x1000, 0, *PtrPrg++,
					ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_BACKGROUND:
				if( *PtrPrg++ )	/* ON */
				{
					if( !(ptrobj->Flags & OBJ_BACKGROUND) )
					{
						ptrobj->Flags |= OBJ_BACKGROUND ;
						if( ptrobj->WorkFlags & WAS_DRAWN )
						{
							FirstTime = AFF_ALL_FLIP ; /* :-( */
						}
					}
				}
				else		/* OFF */
				{
					if( ptrobj->Flags & OBJ_BACKGROUND )
					{
						ptrobj->Flags &= ~OBJ_BACKGROUND ;
						if( ptrobj->WorkFlags & WAS_DRAWN )
						{
							FirstTime = AFF_ALL_FLIP ; /* :-( */
						}
					}
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SET_RAIL: // Zone Rail
				num = *PtrPrg++;

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==9 AND ptrz->Num==num )
					{
						ptrz->Info1 = *PtrPrg ;
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_INVERSE_BETA:
				ptrobj->Obj.Beta = (ptrobj->Obj.Beta+(512*MUL_ANGLE))&(MAX_ANGLE-1) ;

				if( ptrobj->Move==MOVE_WAGON )
				{
					ptrobj->Info1 = 1 ;	// reinit vitesse wagon

					// pour etre propre !

					APtObj = ptrobj ;

					// SizeSHit contient le num‚ro de la brick sous le wagon
					// teste position de l'essieu avant
					AdjustEssieuWagonAvant( ptrobj->SizeSHit ) ;
					// teste position de l'essieu arriere
					AdjustEssieuWagonArriere( ptrobj->SizeSHit ) ;
				}

				// Pour dire … un objet qu'il n'est plus port‚
				// par moi
				CheckCarrier( numobj ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_NO_BODY:
				InitBody( -1, numobj ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_POPCORN:
/*				SaveTimer( ) ;

				if( AdelineSystem( PATH_RESSOURCE"pop\\popcorn.exe" )==-1 )
				{
HERCULE( HPrintf( "errno= %d: %s\n", errno, strerror(errno) ) ) ;
				}

				PaletteSync( PtrPal ) 	;

				AffScene( AFF_ALL_FLIP )	;
				RestoreTimer( ) ;
*/
				break ;

//---------------------------------------------------------------------------
			case LM_FLOW_POINT:
				Value = *PtrPrg++ ;	// index point
				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				CreateParticleFlow( 0, numobj, 0,
						    X0, Y0, Z0, 0, *PtrPrg++ ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_FLOW_OBJ:
				num = *PtrPrg++	;
				if( ListObjet[num].LifePoint>0 )
				{
					CreateParticleFlow( 0, num, 0,
						ListObjet[num].Obj.X + MyRnd( 256 ) - 128 ,
						ListObjet[num].Obj.Y + MyRnd( 128 ),
						ListObjet[num].Obj.Z + MyRnd( 256 ) - 128,
						ListObjet[num].Obj.Beta,
						*PtrPrg ) ;
				}
				PtrPrg++ ;
				break;

//---------------------------------------------------------------------------
			case LM_SET_ANIM_DIAL:
				ptrobj->AnimDial = *(U16*)PtrPrg ;
				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_PCX:
				num = *PtrPrg++ ;
				EffectPcx( (U8)num, (U8*)Screen, *PtrPrg++, TRUE ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_PCX_MESS_OBJ:
				num = *PtrPrg++ ;	// numpcx
				n   = *PtrPrg++ ;	// effect
				obj = *PtrPrg++ ;	// numobj
				if( ListObjet[obj].LifePoint>0 )
				{
					NumObjDial = (U8)obj ;
					EffectPcxMess( (U8)num, (U8*)Screen, (U8)n, *(S16*)PtrPrg ) ;
				}
				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_PALETTE:
				num = *PtrPrg++ ;
				PaletteIndex( (U8)num ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_FADE_TO_PAL:
				num = *PtrPrg++ ;
				FadeToPalIndex( (U8)num ) ;
				if( num==3 )// Palette noire ?
				{
					FlagFade = TRUE ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SET_CAMERA: // Zone Camera
				num = *PtrPrg++;

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==1 AND ptrz->Num==num )
					{
						if( *PtrPrg )	ptrz->Info7 |= ZONE_ON ;
						else		ptrz->Info7 &= ~(ZONE_ON) ;

						ptrz->Info7 &= ~(ZONE_ACTIVE) ;
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_CAMERA_CENTER:
				num = ((*PtrPrg++)*1024)&4095 ;
//				if( num!=AddBetaCam )
				{
					AddBetaCam = num ;
					CameraCenter( 2 ) ;
					FirstTime = AFF_ALL_FLIP ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_MEMO_ARDOISE:
				num = *PtrPrg++ ;
				if( ListVarGame[FLAG_ARDOISE] )
				{
					// Cherche si le plan se trouve deja
					// dans l'ardoise, sinon le rajoute
					for( n=0; n<NbArdoise; n++ )
					{
						if( ListArdoise[n] == num )
						{
							CurrentArdoise = (U8)n ;
							flag = 1 ;
						}
					}

					if( !flag )
					{
						if( NbArdoise<MAX_ARDOISE )
						{
							CurrentArdoise = NbArdoise ;
							ListArdoise[NbArdoise++] = (S8)num ;
						}
					}

					flag = 0 ;

					InitIncrustDisp( INCRUST_INVENTORY|INCRUST_YCLIP,
						 FLAG_ARDOISE,
						 0, 0,
						 0, 0, 3 ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SAVE_HERO:
				SaveComportementHero = Comportement    ;
				SaveBodyHero 	     = ListObjet[NUM_PERSO].GenBody ;
				break ;

//---------------------------------------------------------------------------
			case LM_RESTORE_HERO:
				SetComportement( SaveComportementHero ) ;
				InitBody( SaveBodyHero, NUM_PERSO ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_SHADOW_OBJ:
				num = *PtrPrg++ ;

				if( ListObjet[num].LifePoint>0 )
				{
					if( *PtrPrg )	ListObjet[num].Flags &= ~(NO_SHADOW) ;
					else		ListObjet[num].Flags |= NO_SHADOW ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_TRACK_TO_VAR_GAME:
				num = *PtrPrg++ ;
				ListVarGame[num] = ptrobj->LabelTrack ;
				// Blindage Bug du Marchand Ambulant
				if( ListVarGame[num]<0 )	ListVarGame[num] = 0 ;
				break ;

//---------------------------------------------------------------------------
			case LM_VAR_GAME_TO_TRACK:
				CleanTrack( ptrobj ) ;

				num = *PtrPrg++ ;
				ptrobj->OffsetTrack = SearchOffsetTrack( ptrobj,
									 (U8)ListVarGame[num] ) ;

				// Blindage Bug du Marchand Ambulant
				if( ptrobj->OffsetTrack<0 )
				{
					ptrobj->OffsetTrack = SearchOffsetTrack( ptrobj, 0 ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_ACTION:
				ActionNormal = TRUE ;
				break ;

//---------------------------------------------------------------------------
			case LM_SET_FRAME:
				num = *PtrPrg++ ;

				if( !(ptrobj->Flags&SPRITE_3D) )
				{
					ObjectSetFrame( &(ptrobj->Obj), num ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SET_SPRITE:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;

				if( ptrobj->Flags & SPRITE_3D )
				{
					InitSprite( num, ptrobj ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_SET_FRAME_3DS:
				num = *PtrPrg++ ;

				if( ptrobj->Flags & ANIM_3DS )
				{
					if( num>(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) )
					{
						num = ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb ;
					}

					num += ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb ;

					InitSprite( num, ptrobj ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_IMPACT_OBJ:
				num = *PtrPrg++ ;
				n = *(U16*)PtrPrg ;
				PtrPrg+=2 ;

				if( ListObjet[num].LifePoint>0 )
				{
					DoImpact( n,
						  ListObjet[num].Obj.X,
						  ListObjet[num].Obj.Y+*(S16*)PtrPrg,
						  ListObjet[num].Obj.Z,
						  (U8)num ) ;
				}

				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_IMPACT_POINT:
				Value = *PtrPrg++ ;

				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				DoImpact( *(U16*)PtrPrg, X0, Y0, Z0, numobj ) ;

				PtrPrg += 2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_NO_CHOC:
				if( *PtrPrg++ )	ptrobj->Flags |= NO_CHOC ;
				else		ptrobj->Flags &= ~NO_CHOC ;
				break ;

//---------------------------------------------------------------------------
			case LM_CINEMA_MODE:
				num = *PtrPrg++ ;

				if( num!=CinemaMode )
				{
					CinemaMode = (U8)num ;

					if( !num )
					{
						// Annulation du mode cinema ?
						// on doit restaurer le decors
						FirstTime = AFF_ALL_FLIP ;
						DureeCycleCinema = BoundRegleTrois( 1, 500, 39, ClipWindowYMin ) ;
					}
					else
					{
						DureeCycleCinema = BoundRegleTrois( 1, 500, 39, 39-ClipWindowYMin ) ;
						ListVarGame[FLAG_ESC] = 0 ;
					}

					DebCycleCinema = ClipWindowYMin ;
					LastYCinema = ClipWindowYMin ;
					TimerCinema = TimerRefHR ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_ANIM_TEXTURE:
				AnimateTexture = *PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_END_MESSAGE:
				break ;

//---------------------------------------------------------------------------
			case LM_END_MESSAGE_OBJ:
				num = *PtrPrg++	;
				break ;

//---------------------------------------------------------------------------
			case LM_INIT_BUGGY:
				num = *PtrPrg++ ;
				InitBuggy( numobj, num ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_NEW_SAMPLE:
				num = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				ParmSampleDecalage  = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				ParmSampleVolume    = *PtrPrg++ ;
				ParmSampleFrequence = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				HQ_3D_MixSample( num, 0x1000, 0, 1,
					ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
				break ;

//---------------------------------------------------------------------------
			case LM_PARM_SAMPLE:
				ParmSampleDecalage  = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				ParmSampleVolume    = *PtrPrg++ ;
				ParmSampleFrequence = *(S16*)PtrPrg ;
				PtrPrg+=2 ;
				break ;

//---------------------------------------------------------------------------
			case LM_POS_OBJ_AROUND:
				obj = *PtrPrg++ ;
				num = *PtrPrg++ ;
				if( ListObjet[obj].LifePoint>0 )
				{
					PosObjetAroundAnother( (U8)num, (U8)obj ) ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_ESCALATOR: // Escalator on/off
				num = *PtrPrg++;

				ptrz = ListZone ;

				for( n=0; n<NbZones; n++ )
				{
					if( ptrz->Type==7 AND ptrz->Num==num )
					{
						ptrz->Info1 = *PtrPrg ;
					}
					ptrz++ ;
				}
				PtrPrg++ ;
				break ;

//---------------------------------------------------------------------------
			case LM_ECLAIR: // Eclair n/10s
				num = *PtrPrg++;

				n = InitIncrustDisp( INCRUST_ECLAIR, 0, 0, 0, 0, 0, 1 ) ;

				if( n!=-1 )
				{
					ListIncrustDisp[n].TimerEnd = TimerRefHR + num*100 ;
				}
				break ;

//---------------------------------------------------------------------------
			case LM_PLUIE: // Pluie n/10s
				num = *PtrPrg++;

				n = InitIncrustDisp( INCRUST_PLUIE, 0, 0, 0, 0, 0, 1 ) ;

				if( n!=-1 )
				{
					ListIncrustDisp[n].TimerEnd = TimerRefHR + num*100 ;
					FlagRain = TRUE ;
					StartRainSample() ;
				}
				break ;


//---------------------------------------------------------------------------
		}
	}
}

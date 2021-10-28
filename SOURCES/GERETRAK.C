#include	"c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █▀▀▀▀ █▀▀▀█ █▀▀▀▀       ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀▀ █  ▄▀
	  ██ ▀█ ██▀▀  ██▀█▀ ██▀▀          ██  ██▀█▀ ██▀▀█ ██    ██▀▄
	  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀  ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
WORD	BigSampleRepeat = 1 ;


/*══════════════════════════════════════════════════════════════════════════*/

void	DoTrack( WORD	numobj )
{
	T_OBJET	*ptrobj ;
	WORD	flag ;
	UBYTE	macro ;
	UBYTE	string[256] ;
	WORD	angle ;
	LONG	dist, y ;
	UBYTE	*ptrtrack;
	UBYTE	memooffsettrack;
	WORD	overflow = 0 ;
	UBYTE	*ptr ;
	UBYTE	n, c ;

	ptrobj = &ListObjet[numobj] ;

	flag = TRUE ;
	while( flag )
	{
		memooffsettrack = ptrobj->OffsetTrack ;

		ptrtrack = ptrobj->PtrTrack ;
		ptrtrack += ptrobj->OffsetTrack ;		/* offset macro en cours */

		macro = *ptrtrack++ ;
		ptrobj->OffsetTrack++ ;

		switch( macro )
		{
/*-------------------------------------------------------------------------*/
		case TM_SAMPLE:
			HQ_3D_MixSample( *(WORD*)ptrtrack, 0x1000, 1,
			ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SAMPLE_RND:
			HQ_3D_MixSample( *(WORD*)ptrtrack, 0x800+Rnd(0x800), 1,
			ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SAMPLE_ALWAYS:
			HQ_3D_MixSample( *(WORD*)ptrtrack, 0x1000, 0,
			ptrobj->PosObjX, ptrobj->PosObjY, ptrobj->PosObjZ ) ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SAMPLE_STOP:
			HQ_StopOneSample( *(WORD*)ptrtrack  ) 	;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_REPEAT_SAMPLE:
			BigSampleRepeat = *(WORD*)ptrtrack ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SIMPLE_SAMPLE:
			HQ_MixSample( *(WORD*)ptrtrack, 0x1000, BigSampleRepeat, 128, 128 ) ;
			BigSampleRepeat = 1 ;
			ptrobj->OffsetTrack += 2 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_PLAY_FLA:
			ptr = string ;
			n = 0 ;
			do {    c = *ptrtrack++ ;
				*ptr++ = c ;
				n++ ;
			} while( c != 0 ) ;

			ptrobj->OffsetTrack += n ;

			SaveTimer() ;
			FadeToBlack( PtrPal ) ;
			PlayAnimFla( string ) ;
			FlagFade = TRUE ;
			RestoreTimer() ;
			AffScene( TRUE ) ;

			break ;

/*-------------------------------------------------------------------------*/
		case TM_BODY:
			InitBody( *ptrtrack++, numobj ) ;
			ptrobj->OffsetTrack ++ ;
			break ;

		case TM_NO_BODY:
			InitBody( NO_BODY, numobj ) ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANIM:
			if( !InitAnim( *ptrtrack++, ANIM_REPEAT, 0, numobj ) )
			{

				ptrobj->OffsetTrack = memooffsettrack ;
				flag = FALSE ;
			}
			else
			{
				ptrobj->OffsetTrack++ ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_ANIM:
			if( !(ptrobj->WorkFlags & ANIM_END) )
			{
				ptrobj->OffsetTrack-- ;
				flag = FALSE ;
			}
			else
			{
				flag = FALSE ;
/* attention si autre utilisation ANIM_END */
/*	ptrobj->Flags &= ~ANIM_END ;	*/

				/* sup. ce clear si pb obj avec track pendant
				follow ... */
				ClearRealAngle( ptrobj ) ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_NB_ANIM:/*	LORAN	*/
			ptrobj->OffsetTrack += 2 ;
			if( !(ptrobj->WorkFlags & ANIM_END) )
			{
				flag = FALSE ;
			}
			else
			{
				ptrtrack[1]++ ; // nb anim +=

				if ( *(ptrtrack+1) == *ptrtrack )
				{
					*(ptrtrack+1) = 0		;/* Cpt */
				}
				else	flag = FALSE			;

			}
			if ( flag == FALSE )	ptrobj->OffsetTrack -= 3 	;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_NB_SECOND:
			ptrobj->OffsetTrack += 5 ;

			if( *(ULONG*)(ptrtrack+1) == 0 )
			{
				*(ULONG*)(ptrtrack+1) = TimerRef + ( *ptrtrack * 50 ) ;
			}

			if ( TimerRef < *(ULONG*)(ptrtrack+1) )
			{
				ptrobj->OffsetTrack -= 6 ;
				flag = FALSE ;
			}
			else *(ULONG*)(ptrtrack+1) = 0	;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO_POINT:
			ptrobj->OffsetTrack++ ;
//			if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
			{
				Value = *ptrtrack ;	// index point
				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				angle = GetAngle(	ptrobj->PosObjX,
							ptrobj->PosObjZ,
							X0, Z0 ) ;

				if( ptrobj->Flags & SPRITE_3D )
				{
					ptrobj->Beta = angle ;
				}
				else
				{
					if( Distance < 4000 )
					{

					InitRealAngleConst(	ptrobj->Beta,
								angle,
					ptrobj->SRot/**Distance/4000*/ ,
								&ptrobj->RealAngle ) ;
					}
					else
					{
					InitRealAngleConst(	ptrobj->Beta,
								angle,
								ptrobj->SRot,
								&ptrobj->RealAngle ) ;
					}
				}
				if( Distance > 500 )
				{
					ptrobj->OffsetTrack -= 2 ;
					flag = FALSE ;
				}
			}
/*			else
			{
				ptrobj->OffsetTrack -= 2 ;
				flag = FALSE ;
			}
*/
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO_POINT_3D:
			ptrobj->OffsetTrack++ ;
			if( ptrobj->Flags & SPRITE_3D )
			{
				Value = *ptrtrack ;	// index point
				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				ptrobj->Beta =
					GetAngle(	ptrobj->PosObjX,
							ptrobj->PosObjZ,
							X0, Z0 ) ;

				ptrobj->FlagAnim = /* alpha */
					GetAngle(	ptrobj->PosObjY,0,
							Y0,Distance  ) ;
/*	OpenWindow( &DWin, 10,200, 20,5 ) ;
	WinText( &DWin, "Dist: ", Distance ) ;
	WinText( &DWin, "Flag Y: ", Y0 ) ;
	WinText( &DWin, "Obj Y: ", ptrobj->PosObjY ) ;
	FlipWindow( &DWin ) ;
*/
				if( Distance > 100 )
				{
					ptrobj->OffsetTrack -= 2 ;
					flag = FALSE ;
				}
				else
				{
		/* find_me */

		ptrobj->PosObjX = X0 ;
		ptrobj->PosObjY = Y0 ;
		ptrobj->PosObjZ = Z0 ;
				}

			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO_SYM_POINT:
			ptrobj->OffsetTrack++ ;
//			if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
			{
				Value = *ptrtrack ;	// index point
				X0 = ListBrickTrack[Value].X ;
				Y0 = ListBrickTrack[Value].Y ;
				Z0 = ListBrickTrack[Value].Z ;

				angle = 512 + GetAngle(	ptrobj->PosObjX,
							ptrobj->PosObjZ,
							X0, Z0 ) ;

				if( ptrobj->Flags & SPRITE_3D )
				{
					ptrobj->Beta = angle ;
				}
				else
				{
					InitRealAngleConst(	ptrobj->Beta,
					/* sym */		angle,
								ptrobj->SRot,
								&ptrobj->RealAngle ) ;
				}

				if( Distance > 500 )
				{
					ptrobj->OffsetTrack -= 2 ;
					flag = FALSE ;
				}
			}
/*			else
			{
				ptrobj->OffsetTrack -= 2 ;
				flag = FALSE ;
			}
*/
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANGLE:
		ptrobj->OffsetTrack += 2 ;

		if( !(ptrobj->Flags & SPRITE_3D) )
		{
//			if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
			{
				Value = *(WORD*)ptrtrack ;

				if( !ptrobj->RealAngle.TimeValue )
				{
					InitRealAngleConst(	ptrobj->Beta,
								Value,
								ptrobj->SRot,
								&ptrobj->RealAngle ) ;
				}

				if( ptrobj->Beta != Value )
				{
					ptrobj->OffsetTrack -= 3 ;
					flag = FALSE ;
				}
				else
				{
					ClearRealAngle( ptrobj ) ;
				}
			}
/*			else
			{
				ptrobj->OffsetTrack -= 3 ;
				flag = FALSE ;
			}
*/
		}
		break ;

/*-------------------------------------------------------------------------*/
		case TM_FACE_TWINKEL:
		ptrobj->OffsetTrack+=2 ;
		if( !(ptrobj->Flags & SPRITE_3D) )
		{
//			if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
			{
				Value = *(WORD*)ptrtrack ;

				if( Value == -1 )
				{
					if( !ptrobj->RealAngle.TimeValue )
					{
						Value =	GetAngle(ptrobj->PosObjX,
								ptrobj->PosObjZ,
								ListObjet[NUM_PERSO].PosObjX,
								ListObjet[NUM_PERSO].PosObjZ );

						InitRealAngleConst(	ptrobj->Beta,
									Value,
									ptrobj->SRot,
									&ptrobj->RealAngle ) ;
						*(WORD*)ptrtrack = Value ;
					}
				}

				if( ptrobj->Beta != Value )
				{
					ptrobj->OffsetTrack-=3 ;
					flag = FALSE ;
				}
				else
				{
					ClearRealAngle( ptrobj ) ;
					*(WORD*)ptrtrack = -1 ;
				}
			}
/*			else
			{
				ptrobj->OffsetTrack-=3 ;
				flag = FALSE ;
			}
*/
		}
		break ;

/*-------------------------------------------------------------------------*/
		case TM_ANGLE_RND:
		ptrobj->OffsetTrack += 4 ;

		if( !(ptrobj->Flags & SPRITE_3D) )
		{
//			if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
			{
				Value = *(WORD*)(ptrtrack+2) ;

				if( Value == -1 )
				{
					if( !ptrobj->RealAngle.TimeValue )
					{
						if( rand()&1 )
						{
							Value = *(WORD*)ptrtrack ;
							Value = (ptrobj->Beta + 256 + Value/2 - Rnd( Value ))&1023 ;
						}
						else
						{
							Value = *(WORD*)ptrtrack ;
							Value = (ptrobj->Beta - 256 - Value/2 + Rnd( Value ))&1023 ;
						}
						InitRealAngleConst(	ptrobj->Beta,
									Value,
									ptrobj->SRot,
									&ptrobj->RealAngle ) ;
						*(WORD*)(ptrtrack+2) = Value ;
					}
				}
				if( ptrobj->Beta != Value )
				{
					ptrobj->OffsetTrack -= 5 ;
					flag = FALSE ;
				}
				else
				{
					ClearRealAngle( ptrobj ) ;
					*(WORD*)(ptrtrack+2) = -1 ;
				}
			}
/*			else
			{
				ptrobj->OffsetTrack -= 5 ;
				flag = FALSE ;
			}
*/
		}
		break ;

/*-------------------------------------------------------------------------*/
/* spécifique porte: SPRITE_3D + SPRITE_CLIP */

		case TM_OPEN_LEFT:
		case TM_OPEN_RIGHT:
		case TM_OPEN_UP:
		case TM_OPEN_DOWN:
		ptrobj->OffsetTrack += 2 ;
		if( (ptrobj->Flags & (SPRITE_3D+SPRITE_CLIP)) == SPRITE_3D+SPRITE_CLIP )
		{
			switch( macro )
			{
			case TM_OPEN_LEFT:
				ptrobj->Beta = 768 ;
				break ;
			case TM_OPEN_RIGHT:
				ptrobj->Beta = 256 ;
				break ;
			case TM_OPEN_UP:
				ptrobj->Beta = 512 ;
				break ;
			case TM_OPEN_DOWN:
				ptrobj->Beta = 0 ;
				break ;
			}
			ptrobj->DoorWidth = *(WORD*)ptrtrack ;
			ptrobj->WorkFlags |= AUTO_STOP_DOOR ;
	//speed porte
			ptrobj->SRot = 1000 ; /* reglage global 8( */
			InitRealValue( 0, 1000, 50, &ptrobj->RealAngle ) ;
		}
		break ;

/*-------------------------------------------------------------------------*/
/* spécifique porte: SPRITE_3D + SPRITE_CLIP */
		case TM_CLOSE:
		if( (ptrobj->Flags & (SPRITE_3D+SPRITE_CLIP)) == SPRITE_3D+SPRITE_CLIP )
		{
			ptrobj->WorkFlags |= AUTO_STOP_DOOR ;
			ptrobj->DoorWidth = 0 ;
	//speed porte
			ptrobj->SRot = -1000 ; /* reglage global 8( */
			InitRealValue( 0, -1000, 50, &ptrobj->RealAngle ) ;
		}
		break ;

/*-------------------------------------------------------------------------*/
/* spécifique porte: SPRITE_3D + SPRITE_CLIP */
		case TM_WAIT_DOOR:
		if( (ptrobj->Flags & (SPRITE_3D+SPRITE_CLIP)) == SPRITE_3D+SPRITE_CLIP )
		{
			if( ptrobj->SRot != 0 )
			{
				ptrobj->OffsetTrack-- ;
				flag = FALSE ;
			}
		}
		break ;

/*-------------------------------------------------------------------------*/
		case TM_BETA:
		ptrobj->OffsetTrack += 2 ;
//		if( !(ptrobj->WorkFlags & ANIM_MASTER_ROT) )
		{
			ptrobj->Beta = *(WORD*)ptrtrack ;
			if( !(ptrobj->Flags & SPRITE_3D) )
			{
				ClearRealAngle( ptrobj ) ;
			}
		}
		break ;

/*-------------------------------------------------------------------------*/
		case TM_POS_POINT:

			ptrobj->OffsetTrack ++ ;
			Value = *ptrtrack ;	// index point
			X0 = ListBrickTrack[Value].X ;
			Y0 = ListBrickTrack[Value].Y ;
			Z0 = ListBrickTrack[Value].Z ;

			if( ptrobj->Flags & SPRITE_3D )
			{
				ptrobj->SRot = 0 ;	//arrete deplacement !!!
			}
			ptrobj->PosObjX = X0 ;
			ptrobj->PosObjY = Y0 ;
			ptrobj->PosObjZ = Z0 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_LABEL:
			ptrobj->LabelTrack = *ptrtrack ;	/* label */
			ptrobj->OffsetTrack++ ;
			ptrobj->OffsetLabelTrack = ptrobj->OffsetTrack - 2 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO:
			ptrobj->OffsetTrack = *(WORD*)ptrtrack ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_LOOP:
			/* mettre recherche label */
			ptrobj->OffsetTrack = 0 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SPEED:
			ptrobj->OffsetTrack += 2 ;
			ptrobj->SRot = *(WORD*)ptrtrack ;
			if( ptrobj->Flags & SPRITE_3D )
			{	/* voir pour les 2 types ? */
				InitRealValue( 0, ptrobj->SRot,
						50, &ptrobj->RealAngle ) ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_BACKGROUND:
			ptrobj->OffsetTrack ++ ;
			if( *ptrtrack )	/* ON */
			{
				if( !(ptrobj->Flags & OBJ_BACKGROUND) )
				{
					ptrobj->Flags |= OBJ_BACKGROUND ;
					if( ptrobj->WorkFlags & WAS_DRAWN )
					{
						FirstTime = TRUE ; /* :-( */
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
						FirstTime = TRUE ; /* :-( */
					}
				}
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_END:
//			ptrobj->Move = NO_MOVE ;
			ptrobj->OffsetTrack = -1 ;
			flag = FALSE ;
			break ;

		case TM_STOP:
			ptrobj->OffsetTrack = -1 ;
			flag = FALSE ;
			break ;
		}
	}
}


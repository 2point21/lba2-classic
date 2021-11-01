#include	"c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*
	  █▀▀▀▀ █▀▀▀▀ █▀▀▀█ █▀▀▀▀       ▀▀█▀▀ █▀▀▀█ █▀▀▀█ █▀▀▀▀ █  ▄▀
	  ██ ▀█ ██▀▀  ██▀█▀ ██▀▀          ██  ██▀█▀ ██▀▀█ ██    ██▀▄
	  ▀▀▀▀▀ ▀▀▀▀▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀  ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀  ▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
S32	BigSampleRepeat = 1 ;


/*══════════════════════════════════════════════════════════════════════════*/

// Permet de forcer une track à un label et non à un offset (en gros, comme
// dans l'outil !)

S16	SearchOffsetTrack( T_OBJET *ptrobj, U8 label )
{
	U8	*ptrtrack;
	S16	offsettrack = 0 ;
	U8	macro ;

	while( TRUE )
	{
		ptrtrack = ptrobj->PtrTrack+offsettrack ;

		macro = *ptrtrack++ ;
		offsettrack++ ;

		switch( macro )
		{
/*-------------------------------------------------------------------------*/
		case TM_LABEL:
			if( *ptrtrack==label )
			{
				ptrobj->LabelTrack = label ;	/* label */
				ptrobj->OffsetLabelTrack = (S16)(offsettrack-1) ;
				return( (S16)(offsettrack-1) ) ;
			}
			else	offsettrack++ ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_BODY:
		case TM_GOTO_POINT:
		case TM_GOTO_POINT_3D:
		case TM_GOTO_SYM_POINT:
		case TM_POS_POINT:
		case TM_BACKGROUND:
		case TM_SET_FRAME:
		case TM_SET_FRAME_3DS:
		case TM_SET_START_3DS:
		case TM_SET_END_3DS:
		case TM_START_ANIM_3DS:
		case TM_WAIT_FRAME_3DS:
		case TM_VOLUME:
			offsettrack++ ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANIM:
		case TM_SAMPLE:
		case TM_SAMPLE_RND:
		case TM_SAMPLE_ALWAYS:
		case TM_SAMPLE_STOP:
		case TM_REPEAT_SAMPLE:
		case TM_SIMPLE_SAMPLE:
		case TM_WAIT_NB_ANIM:
		case TM_ANGLE:
		case TM_FACE_TWINSEN:
		case TM_OPEN_LEFT:
		case TM_OPEN_RIGHT:
		case TM_OPEN_UP:
		case TM_OPEN_DOWN:
		case TM_BETA:
		case TM_GOTO:
		case TM_SPEED:
		case TM_SPRITE:
		case TM_DECALAGE:
		case TM_FREQUENCE:
			offsettrack += 2 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANGLE_RND:
		case TM_LOOP:
			offsettrack += 4 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_NB_SECOND:
		case TM_WAIT_NB_SECOND_RND:
		case TM_WAIT_NB_DIZIEME:
		case TM_WAIT_NB_DIZIEME_RND:
			offsettrack += 5 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_PLAY_ACF:
			{
				char	*ptr, c ;
				char	string[256] ;
				S32	n ;

				ptr = string ;
				n = 0 ;
				do {    c = *ptrtrack++ ;
					*ptr++ = c ;
					n++ ;
				} while( c != 0 ) ;

				offsettrack += n ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_END:
			return( -1 ) ;	// label pas trouvé
		}
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
// appellée après un SET_TRACK ou SET_TRACK_OBJ pour positionner tout de
// suite le champ LabelTrack

void	Track_SetLabel( T_OBJET *ptrobj )
{
	U8	*ptrtrack ;

	ptrtrack = ptrobj->PtrTrack ;
	ptrtrack += ptrobj->OffsetTrack ;		/* offset macro en cours */

	switch( *ptrtrack++ )
	{
		case TM_LABEL:
			ptrobj->LabelTrack = *ptrtrack ;	/* label */
			ptrobj->OffsetLabelTrack = ptrobj->OffsetTrack ;
			break ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/

void	DoTrack( U8 numobj )
{
	T_OBJET	*ptrobj ;
	S32	distance ;
	S32	flag ;
	S32	angle ;
	U8	*ptrtrack;
	S16	memooffsettrack;
	U8	macro ;
	U8	n ;

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
			HQ_3D_MixSample( *(S16*)ptrtrack, 0x1000, 0, 1,
			ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SAMPLE_RND:
			HQ_3D_MixSample( *(S16*)ptrtrack, 0x800, 0x1000, 1,
			ptrobj->Obj.X, ptrobj->Obj.Y, ptrobj->Obj.Z ) ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SAMPLE_ALWAYS:
			{
			S32	num = *(S16*)ptrtrack ;

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
			ptrobj->OffsetTrack += 2 ;
			}
			break ;

		case TM_SAMPLE_STOP:
			{
			S32 num = *(S16*)ptrtrack ;

			if( ptrobj->SampleAlways
			AND ((ptrobj->SampleAlways>>8)&0xFFFF)==num )
			{
				num = ptrobj->SampleAlways ;
				ptrobj->SampleAlways = 0 ;
			}

			HQ_StopOneSample( num ) 	;
			ptrobj->OffsetTrack += 2 ;
			}
			break ;

		case TM_REPEAT_SAMPLE:
			BigSampleRepeat = *(S16*)ptrtrack ;
			ptrobj->OffsetTrack += 2 ;
			break ;

		case TM_SIMPLE_SAMPLE:
			HQ_MixSample( *(S16*)ptrtrack, 0x1000, 0, BigSampleRepeat, 64, 127 ) ;
			BigSampleRepeat = 1 ;
			ptrobj->OffsetTrack += 2 ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_PLAY_ACF:
			SaveTimer() ;
			if( !FlagFade )	FadeToBlackAndSamples( PtrPal ) ;
			FlagFade = TRUE ;

			if( (PlayAcf( (char *)ptrtrack )==K_ESC)
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

			ptrobj->OffsetTrack += strlen((char *)ptrtrack)+1 ;
			FlagFade = TRUE ;
			FirstTime = AFF_ALL_FLIP ;
			RestoreTimer() ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_BODY:
			InitBody( *ptrtrack/*++*/, numobj ) ;
			ptrobj->OffsetTrack ++ ;
			break ;

		case TM_NO_BODY:
			InitBody( NO_BODY, numobj ) ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANIM:
			if( !InitAnim( *(U16*)ptrtrack, ANIM_REPEAT, numobj ) )
			{
				ptrobj->OffsetTrack = memooffsettrack ;
				flag = FALSE ;
			}
			else
			{
				ptrobj->OffsetTrack+=2 ;
//				ptrtrack+=2 ;
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
		case TM_WAIT_NB_ANIM:
			ptrobj->OffsetTrack += 2 ;
			if( !(ptrobj->WorkFlags & ANIM_END) )
			{
				flag = FALSE ;
			}
			else
			{
				ptrtrack[1]++ ; // nb anim ++

				if ( *(ptrtrack+1) == *ptrtrack )
				{
					*(ptrtrack+1) = 0		;/* Cpt */
				}
				else
				{
					flag = FALSE			;
				}
			}
			if ( flag == FALSE )	ptrobj->OffsetTrack -= 3 	;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_NB_SECOND:
			ptrobj->OffsetTrack += 5 ;

			if( *(U32*)(ptrtrack+1) == 0 )
			{
				*(U32*)(ptrtrack+1) = TimerRefHR + ( *ptrtrack * 1000 ) ;
			}

			if ( TimerRefHR < *(U32*)(ptrtrack+1) )
			{
				ptrobj->OffsetTrack -= 6 ;
				flag = FALSE ;
			}
			else
			{
				*(U32*)(ptrtrack+1) = 0	;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_NB_SECOND_RND:
			ptrobj->OffsetTrack += 5 ;

			if( *(U32*)(ptrtrack+1) == 0 )
			{
				*(U32*)(ptrtrack+1) = TimerRefHR + ( MyRnd(*ptrtrack) * 1000 ) ;
			}

			if ( TimerRefHR < *(U32*)(ptrtrack+1) )
			{
				ptrobj->OffsetTrack -= 6 ;
				flag = FALSE ;
			}
			else
			{
				*(U32*)(ptrtrack+1) = 0	;
			}
			break ;

/*-------------------------------------------------------------------------*/

		case TM_WAIT_NB_DIZIEME:
			ptrobj->OffsetTrack += 5 ;

			if( *(U32*)(ptrtrack+1) == 0 )
			{
				*(U32*)(ptrtrack+1) = TimerRefHR + ( *ptrtrack * 100 ) ;
			}

			if ( TimerRefHR < *(U32*)(ptrtrack+1) )
			{
				ptrobj->OffsetTrack -= 6 ;
				flag = FALSE ;
			}
			else
			{
				*(U32*)(ptrtrack+1) = 0	;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_NB_DIZIEME_RND:
			ptrobj->OffsetTrack += 5 ;

			if( *(U32*)(ptrtrack+1) == 0 )
			{
				*(U32*)(ptrtrack+1) = TimerRefHR + ( MyRnd(*ptrtrack) * 100 ) ;
			}

			if ( TimerRefHR < *(U32*)(ptrtrack+1) )
			{
				ptrobj->OffsetTrack -= 6 ;
				flag = FALSE ;
			}
			else
			{
				*(U32*)(ptrtrack+1) = 0	;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO_POINT:
			ptrobj->OffsetTrack++ ;

			Value = *ptrtrack ;	// index point
			X0 = ListBrickTrack[Value].X ;
			Y0 = ListBrickTrack[Value].Y ;
			Z0 = ListBrickTrack[Value].Z ;

			angle = GetAngle2D(	ptrobj->Obj.X,
						ptrobj->Obj.Z,
						X0, Z0 ) ;

			distance = Distance2D( 	ptrobj->Obj.X,
						ptrobj->Obj.Z,
						X0, Z0 ) ;

			if( ptrobj->Flags & SPRITE_3D )
			{
				ptrobj->Obj.Beta = angle ;
			}
			else
			{
				ChangeSpeedBoundAngleMove( &ptrobj->BoundAngle,
							ptrobj->SRot,
							angle ) ;
			}

			if( distance > 500 )
			{
				ptrobj->OffsetTrack -= 2 ;
				flag = FALSE ;
			}
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

				ptrobj->Obj.Beta =
					GetAngle2D(	ptrobj->Obj.X,
							ptrobj->Obj.Z,
							X0, Z0 ) ;

				distance = Distance2D( 	ptrobj->Obj.X,
							ptrobj->Obj.Z,
							X0, Z0 ) ;

				ptrobj->Obj.Alpha = /* alpha */
					GetAngle2D(	ptrobj->Obj.Y,0,
							Y0,distance ) ;

				distance = /* alpha */
					Distance2D(	ptrobj->Obj.Y,0,
							Y0,distance  ) ;

				// ATTENTION: peut etre dangereux sur les
				//	      machines lentes

				if( distance > 50 )
				{
					ptrobj->OffsetTrack -= 2 ;
					flag = FALSE ;
				}
				else
				{
					/* find_me */

					ptrobj->Obj.X = X0 ;
					ptrobj->Obj.Y = Y0 ;
					ptrobj->Obj.Z = Z0 ;

					ptrobj->Obj.Alpha = 0 ;
				}
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO_SYM_POINT:
			ptrobj->OffsetTrack++ ;

			Value = *ptrtrack ;	// index point
			X0 = ListBrickTrack[Value].X ;
			Y0 = ListBrickTrack[Value].Y ;
			Z0 = ListBrickTrack[Value].Z ;

			angle = (512*MUL_ANGLE) + GetAngle2D(	ptrobj->Obj.X,
						ptrobj->Obj.Z,
						X0, Z0 ) ;

			distance = Distance2D(  ptrobj->Obj.X,
						ptrobj->Obj.Z,
						X0, Z0 ) ;

			if( ptrobj->Flags & SPRITE_3D )
			{
				ptrobj->Obj.Beta = angle ;
			}
			else
			{
				ChangeSpeedBoundAngleMove( &ptrobj->BoundAngle,
							ptrobj->SRot,
							angle ) ;
			}

			if( distance > 500 )
			{
				ptrobj->OffsetTrack -= 2 ;
				flag = FALSE ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANGLE:
			ptrobj->OffsetTrack += 2 ;

			if( !(ptrobj->Flags & SPRITE_3D) )
			{
				Value = *(S16*)ptrtrack ;

				if( !(Value&0x8000) )
				{
					InitBoundAngleMove( &ptrobj->BoundAngle,
								ptrobj->SRot,
								ptrobj->Obj.Beta,
								Value ) ;

					*(S16*)ptrtrack |= 0x8000 ;
				}

				Value &= 0x7FFF ;

				if( ptrobj->Obj.Beta != Value )
				{
					// Pour ne pas rentrer en conflit avec
					// un MOVE_FOLLOW
					ptrobj->WorkFlags |= TRACK_MASTER_ROT ;

					ptrobj->OffsetTrack -= 3 ;
					flag = FALSE ;
				}
				else
				{
					ptrobj->WorkFlags &= ~(TRACK_MASTER_ROT) ;
					*(S16*)ptrtrack = (S16)Value ;
				}
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_FACE_TWINSEN:
			ptrobj->OffsetTrack+=2 ;
			if( !(ptrobj->Flags & SPRITE_3D) )
			{
				Value = *(S16*)ptrtrack ;

				if( Value == -1 )
				{
					Value =	GetAngle2D(ptrobj->Obj.X,
							ptrobj->Obj.Z,
							ListObjet[NUM_PERSO].Obj.X,
							ListObjet[NUM_PERSO].Obj.Z );

					InitBoundAngleMove( &ptrobj->BoundAngle,
								ptrobj->SRot,
								ptrobj->Obj.Beta,
								Value ) ;

					*(S16*)ptrtrack = (S16)Value ;
				}

				if( ptrobj->Obj.Beta != Value )
				{
					// Pour ne pas rentrer en conflit avec
					// un MOVE_FOLLOW
					ptrobj->WorkFlags |= TRACK_MASTER_ROT ;

					ptrobj->OffsetTrack-=3 ;
					flag = FALSE ;
				}
				else
				{
					ptrobj->WorkFlags &= ~(TRACK_MASTER_ROT) ;
					*(S16*)ptrtrack = -1 ;
				}
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_ANGLE_RND:
			ptrobj->OffsetTrack += 4 ;

			if( !(ptrobj->Flags & SPRITE_3D) )
			{
				Value = *(S16*)(ptrtrack+2) ;

				if( Value == -1 )
				{
					Value = *(S16*)ptrtrack ;
					if( rand()&1 )
					{
						Value = (ptrobj->Obj.Beta + MyRnd( Value ))&(MAX_ANGLE-1) ;
					}
					else
					{
						Value = (ptrobj->Obj.Beta - MyRnd( Value ))&(MAX_ANGLE-1) ;
					}

					InitBoundAngleMove( &ptrobj->BoundAngle,
								ptrobj->SRot,
								ptrobj->Obj.Beta,
								Value ) ;

					*(S16*)(ptrtrack+2) = (S16)Value ;
				}

				if( ptrobj->Obj.Beta != Value )
				{
					// Pour ne pas rentrer en conflit avec
					// un MOVE_FOLLOW
					ptrobj->WorkFlags |= TRACK_MASTER_ROT ;

					ptrobj->OffsetTrack -= 5 ;
					flag = FALSE ;
				}
				else
				{
					ptrobj->WorkFlags &= ~(TRACK_MASTER_ROT) ;
					*(S16*)(ptrtrack+2) = -1 ;
				}
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
					ptrobj->Obj.Beta = 768*MUL_ANGLE ;
					break ;
				case TM_OPEN_RIGHT:
					ptrobj->Obj.Beta = 256*MUL_ANGLE ;
					break ;
				case TM_OPEN_UP:
					ptrobj->Obj.Beta = 512*MUL_ANGLE ;
					break ;
				case TM_OPEN_DOWN:
					ptrobj->Obj.Beta = 0*MUL_ANGLE ;
					break ;
				}
				ptrobj->DoorWidth = *(S16*)ptrtrack ;
				ptrobj->WorkFlags |= AUTO_STOP_DOOR ;

				//speed porte
				ptrobj->SRot = 1000 ; /* reglage global 8( */
				InitMove( &ptrobj->BoundAngle.Move, 1000 ) ;
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
				InitMove( &ptrobj->BoundAngle.Move, -1000 ) ;
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
			ptrobj->Obj.Beta = *(S16*)ptrtrack ;
			if( !(ptrobj->Flags & SPRITE_3D) )
			{
				ClearRealAngle( ptrobj ) ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_POS_POINT:
			{
			S16	nu ;

			ptrobj->OffsetTrack ++ ;
			Value = *ptrtrack ;	// index point
			X0 = ListBrickTrack[Value].X ;
			Y0 = ListBrickTrack[Value].Y ;
			Z0 = ListBrickTrack[Value].Z ;

			if( ptrobj->Flags & SPRITE_3D )
			{
				ptrobj->SRot = 0 ;	//arrete deplacement !!!
			}
			ptrobj->Obj.X = X0 ;
			ptrobj->Obj.Y = Y0 ;
			ptrobj->Obj.Z = Z0 ;

			// Pour vérifier si l'objet à suivre se trouve dans une
			// zone caméra
			CheckZoneSce( ptrobj, numobj ) ;

			// Pour dire à un objet qu'il n'est plus porté
			// par moi
			CheckCarrier( numobj ) ;

			// si je suis porté par un objet
			nu = ptrobj->CarryBy ;

			if( nu!=-1 AND !(nu&CARRY_BY_DECORS) )
			{
				ptrobj->CarryBy = -1 ;
			}

			if( numobj==NUM_PERSO )	StartYFalling = 0 ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_LABEL:
			ptrobj->LabelTrack = *ptrtrack ;	/* label */
			ptrobj->OffsetTrack++ ;
			ptrobj->OffsetLabelTrack = (S16)(ptrobj->OffsetTrack-2) ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_GOTO:
			ptrobj->OffsetTrack = *(S16*)ptrtrack ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SPEED:
			ptrobj->OffsetTrack += 2 ;
			ptrobj->SRot = *(S16*)ptrtrack ;
			if( ptrobj->Flags & SPRITE_3D )
			{	/* voir pour les 2 types ? */
				ChangeSpeedMove( &ptrobj->BoundAngle.Move, ptrobj->SRot ) ;
			}
			else if( ptrobj->Move==MOVE_WAGON )
			{
				ptrobj->Info = 0 ;
				ptrobj->Info1 = 1 ;
			}
			else
			{
				switch(ptrobj->SRot)
				{
				case 0:
					break;
				case 1:
					ptrobj->SRot++ ;
				default:
					ptrobj->SRot = (S16)(51200 / (S32)ptrobj->SRot) ;
					break;
				}
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

/*-------------------------------------------------------------------------*/
		case TM_END:
			ptrobj->OffsetTrack = -1 ;
			flag = FALSE ;
			break ;

		case TM_STOP:
			ptrobj->OffsetTrack = -1 ;
			flag = FALSE ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_LOOP:
			n = *ptrtrack++ ;	// sauve la valeur initiale
			ptrobj->OffsetTrack++ ;
			*ptrtrack -=1 ; 	// decremente compteur

			if( *ptrtrack>0 )
			{
				ptrtrack++ ;
				ptrobj->OffsetTrack = *(S16*)ptrtrack ;
			}
			else
			{
				*ptrtrack = n ; // reinit compteur
				ptrobj->OffsetTrack++ ;  // saute compteur
				ptrobj->OffsetTrack+=2 ; // saute offset Loop
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SPRITE:
			angle = *(S16*)ptrtrack ;	// angle = sprite
			ptrobj->OffsetTrack+=2 ;
			if( ptrobj->Flags & SPRITE_3D )
			{
				InitSprite( angle, ptrobj ) ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SET_FRAME:
			n = *ptrtrack/*++*/ ;
			ptrobj->OffsetTrack++ ;

			if( !(ptrobj->Flags&SPRITE_3D) )
			{
				ObjectSetFrame( &(ptrobj->Obj), n ) ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SET_FRAME_3DS:
			n = *ptrtrack/*++*/ ;
			ptrobj->OffsetTrack++ ;

			if( ptrobj->Flags & ANIM_3DS )
			{
				if( n>(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) )
				{
					n = (U8)(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) ;
				}

				n += ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb ;

				InitSprite( n, ptrobj ) ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SET_START_3DS:
			n = *ptrtrack/*++*/ ;
			ptrobj->OffsetTrack++ ;

			if( ptrobj->Flags & ANIM_3DS )
			{
				if( n>(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) )
				{
					n = (U8)(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) ;
				}

				n += ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb ;

				ptrobj->Coord.A3DS.Deb = n ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_SET_END_3DS:
			n = *ptrtrack/*++*/ ;
			ptrobj->OffsetTrack++ ;

			if( ptrobj->Flags & ANIM_3DS )
			{
				if( n>(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) )
				{
					n = (U8)(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) ;
				}

				n += ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb ;

				ptrobj->Coord.A3DS.Fin = n ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_START_ANIM_3DS:
			n = *ptrtrack/*++*/ ;	// NbFps
			ptrobj->OffsetTrack++ ;

			if( ptrobj->Flags & ANIM_3DS )
			{
				InitSprite( ptrobj->Coord.A3DS.Deb, ptrobj ) ;
				ptrobj->SizeSHit = n ;
				ptrobj->PtrFile3D = (U8*)TimerRefHR ;
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_STOP_ANIM_3DS:
			if( ptrobj->Flags & ANIM_3DS )
			{
				ptrobj->SizeSHit = 0 ;	// NbFps
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_ANIM_3DS:
			if( ptrobj->Flags & ANIM_3DS )
			{
				if( ptrobj->Sprite!=ptrobj->Coord.A3DS.Fin
				AND ptrobj->SizeSHit!=0 )	// NbFps
				{
					ptrobj->OffsetTrack-- ;
					flag = FALSE ;
				}
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_WAIT_FRAME_3DS:
			n = *ptrtrack/*++*/ ;
			ptrobj->OffsetTrack++ ;

			if( ptrobj->Flags & ANIM_3DS )
			{
				if( n>(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) )
				{
					n = (U8)(ListAnim3DS[ptrobj->Coord.A3DS.Num].Fin-ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb) ;
				}

				n += ListAnim3DS[ptrobj->Coord.A3DS.Num].Deb ;

				if( ptrobj->Sprite!=n )
				{
					ptrobj->OffsetTrack-=2 ;
					flag = FALSE ;
				}
			}
			break ;

/*-------------------------------------------------------------------------*/
		case TM_DECALAGE:
			ptrobj->OffsetTrack += 2 ;
			ParmSampleDecalage = *(S16*)ptrtrack ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_FREQUENCE:
			ptrobj->OffsetTrack += 2 ;
			ParmSampleFrequence = *(S16*)ptrtrack ;
			break ;

/*-------------------------------------------------------------------------*/
		case TM_VOLUME:
			ptrobj->OffsetTrack ++ ;
			ParmSampleVolume = *ptrtrack ;
			break ;

/*-------------------------------------------------------------------------*/
		}
	}
}


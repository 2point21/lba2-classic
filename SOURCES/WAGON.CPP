#include	"c_extern.h"

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	DoAnimWagon( void )
{
	S32	n, angle, va   ;
	T_OBJ_3D *obj = &APtObj->Obj ;

	// Magouille pour ne pas ecraser les groupes dans AffOneObjet()
	ObjectSetInterFrame( obj )	;

	// SizeSHit contient le num‚ro de la brick sous le wagon
	// teste position de l'essieu avant
	AdjustEssieuWagonAvant( APtObj->SizeSHit ) ;
	// teste position de l'essieu arriere
	AdjustEssieuWagonArriere( APtObj->SizeSHit ) ;

	if( APtObj->SRot )
	{
		va = APtObj->SRot/(15*20) ;

		obj->CurrentFrame[2].Alpha = (S16)(TimerRefHR*va) ;
		obj->CurrentFrame[3].Alpha = (S16)(TimerRefHR*va) ;

		switch( APtObj->Info )
		{
		case 0: // Rail droit
			// reinit vitesse
			if( APtObj->Info1 )
			{
				InitMove( &APtObj->BoundAngle.Move, APtObj->SRot ) ;
				APtObj->Info1 = 0 ; // ligne droite
			}

			n = GetDeltaMove( &APtObj->BoundAngle.Move ) ;

			// n = vitesse (distance/temps)

			Rotate( 0, n, APtObj->Obj.Beta ) ;
			Nxw = APtObj->Obj.X + X0 ;
			Nzw = APtObj->Obj.Z + Z0 ;
			break ;

		case -1:
		case  1:	// Virage ou aiguillage
			if( APtObj->Info1 != 1 )
			{
				// Init rotation wagon
				InitBoundAngleMove( &APtObj->BoundAngle,
						    (51200*APtObj->SRot)/60319,
//						    (60319/APtObj->SRot)*20,
						    APtObj->Obj.Beta,
						    APtObj->Obj.Beta + (256*MUL_ANGLE)*APtObj->Info ) ;

				APtObj->Info1 = 1 ; // virage
			}

			angle = GetBoundAngleMove( &APtObj->BoundAngle ) ;

			Rotate( 768*APtObj->Info, 0, angle ) ;
			Nxw = APtObj->Info2 - X0 ;
			Nzw = APtObj->Info3 - Z0 ;

			APtObj->Obj.Beta = angle ;

			if( !GetSpeedMove( &APtObj->BoundAngle.Move ) )
			{
				APtObj->Info1 = -1 ;
			}
			break ;
		} // fin switch
	}// Srot != 0
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	DoDirWagon( T_OBJET *ptrobj )
{
	S32	brick	;

	// ATTENTION : Grosse Magouille :
	// Le champ Sprite des objets de type Wagon (forcement des
	// objets 3D) est utilis‚ dans les aiguillages pour savoir
	// dans quel sens on le prend. Cela repare un bug lorqu'on
	// tournait dans un aiguillage et qu'on modifiait en meme
	// temps le sens de l'aiguillage !

	// Autres champs utilises :

	// Info: Type de d‚placement: 0 --> droit
	//			      1 --> Virage Gauche
	//			      2 --> Virage Droite

	// Info2 et Info3: Coordonnees du point de rotation
	// Info1: Flag d'init
	// SizeShit: Num de la brick sur laquelle est le wagon

	// depend du block sur lequel on se trouve
	brick = GetBlockBrick( ptrobj->Obj.X,ptrobj->Obj.Y-1,ptrobj->Obj.Z ) ;
	// depend du style actuel (planete TWINSUN ou ESMER)
	brick = GetNumBrickWagon( brick ) ;

	switch( brick )
	{
		case RAIL_NORD_EST:	// virage NORD-EST
			if( ptrobj->Info1 != 1)
			{
				ptrobj->Obj.Beta = (ptrobj->Obj.Beta/(256*MUL_ANGLE))*(256*MUL_ANGLE) ;
				if( ptrobj->Obj.Beta<=(512*MUL_ANGLE) )
				{
					ptrobj->Info = -1 ;	// Droite
					ptrobj->Info2 =
						(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						+ 2*SIZE_BRICK_XZ ;
					ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
				}
				else
				{
					ptrobj->Info = 1 ;	// Gauche
					ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
					ptrobj->Info3 =
						(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						+ 2*SIZE_BRICK_XZ ;
				}
				ptrobj->Sprite = 0 ;
			}
			break ;

		case RAIL_SUD_EST:	// virage SUD-EST
			if( ptrobj->Info1 != 1)
			{
				ptrobj->Obj.Beta = (ptrobj->Obj.Beta/(256*MUL_ANGLE))*(256*MUL_ANGLE) ;
				if( ptrobj->Obj.Beta<(512*MUL_ANGLE) )
				{
					ptrobj->Info = 1 ;	// Gauche
					ptrobj->Info2 =
						(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						+ 2*SIZE_BRICK_XZ ;
					ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
				}
				else
				{
					ptrobj->Info = -1 ;	// Droite
					ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
					ptrobj->Info3 =
						(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						- SIZE_BRICK_XZ -1 ;
				}
				ptrobj->Sprite = 0 ;
			}
			break ;

		case RAIL_NORD_OUEST:	// virage NORD-OUEST
			if( ptrobj->Info1 != 1)
			{
				ptrobj->Obj.Beta = (ptrobj->Obj.Beta/(256*MUL_ANGLE))*(256*MUL_ANGLE) ;
				if( ptrobj->Obj.Beta<=(256*MUL_ANGLE) )
				{
					ptrobj->Info = -1 ;	// Droite
					ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
					ptrobj->Info3 =
						(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						+ 2*SIZE_BRICK_XZ ;
				}
				else
				{
					ptrobj->Info = 1 ;	/// Gauche
					ptrobj->Info2 =
						(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						- SIZE_BRICK_XZ-1 ;
					ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
				}
				ptrobj->Sprite = 0 ;
			}
			break ;

		case RAIL_SUD_OUEST:	// virage SUD-OUEST
			if( ptrobj->Info1 != 1)
			{
				ptrobj->Obj.Beta = (ptrobj->Obj.Beta/(256*MUL_ANGLE))*(256*MUL_ANGLE) ;
				if( ptrobj->Obj.Beta<=(512*MUL_ANGLE) && ptrobj->Obj.Beta>=(256*MUL_ANGLE) )
				{
					ptrobj->Info = 1 ;	// Gauche
					ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
					ptrobj->Info3 =
						(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						- SIZE_BRICK_XZ-1 ;
				}
				else
				{
					ptrobj->Info = -1 ;	// Droite
					ptrobj->Info2 =
						(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
						- SIZE_BRICK_XZ-1 ;
					ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
				}
				ptrobj->Sprite = 0 ;
			}
			break ;

		case RAIL_NORD_NORD_EST:	// aiguillage NORD-NORD-EST
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 0:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 512*MUL_ANGLE:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1)
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( ptrobj->Info1 != 1)
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
					}
					break ;
			}
			break ;

		case RAIL_NORD_NORD_OUEST:	// aiguillage NORD-NORD-OUEST
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 0:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 512*MUL_ANGLE:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
					}
					break ;
			}
			break ;

		case RAIL_SUD_SUD_EST:	// aiguillage SUD-SUD-EST
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 512*MUL_ANGLE:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 0:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( (ptrobj->Info1 != 1) AND ptrobj->Obj.Beta!=2048 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
					}
					break ;
			}
			break ;

		case RAIL_SUD_SUD_OUEST:	// aiguillage SUD-SUD-OUEST
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 512*MUL_ANGLE:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 0:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
					}
					break ;
			}
			break ;

		case RAIL_EST_EST_NORD:	// aiguillage EST-EST-NORD
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 768*MUL_ANGLE:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 256*MUL_ANGLE:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
					}
					break ;
			}
			break ;

		case RAIL_EST_EST_SUD:	// aiguillage EST-EST-SUD
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 768*MUL_ANGLE:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 256*MUL_ANGLE:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
					}
					break ;
			}
			break ;

		case RAIL_OUEST_OUEST_NORD:	// aiguillage OUEST-OUEST-NORD
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 256*MUL_ANGLE:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 768*MUL_ANGLE:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							- SIZE_BRICK_XZ -1 ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 4:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+1 ;
					}
					break ;
			}
			break ;

		case RAIL_OUEST_OUEST_SUD:	// aiguillage OUEST-OUEST-SUD
			if( !ptrobj->Sprite )
			{
				switch( ptrobj->Obj.Beta )
				{
					case 256*MUL_ANGLE:
						ptrobj->Sprite = 1 ;  // Droit
						break ;

					case 768*MUL_ANGLE:
						if( ptrobj->PtrZoneRail AND ptrobj->PtrZoneRail->Info1 )
							ptrobj->Sprite = 2 ;  // choix virage
						else	ptrobj->Sprite = 3 ;  // choix droit
						break ;

					default:
						ptrobj->Sprite = 4 ;  // Virage
						break ;
				}
			}

			switch( ptrobj->Sprite )
			{
				case 1:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break ;

				case 2:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = 1 ;	// Gauche
						ptrobj->Info2 = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
						ptrobj->Info3 =
							(ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
					}
					break ;

				case 3:
					ptrobj->Info = 0 ;
					ptrobj->Info2 = 0 ;
					ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
					break  ;

				case 4:
					if( ptrobj->Info1 != 1 )
					{
						ptrobj->Info = -1 ;	// Droite
						ptrobj->Info2 =
							(ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ
							+ 2*SIZE_BRICK_XZ ;
						ptrobj->Info3 = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+SIZE_BRICK_XZ-1 ;
					}
					break ;
			}
			break ;

		case RAIL_EST_OUEST:	// droit EST-OUEST
			ptrobj->Sprite = 0 ;
			ptrobj->Info = 0 ;
			ptrobj->Info2 = 0 ;
			ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
			break ;

		case RAIL_NORD_SUD:	// droit NORD-SUD
			ptrobj->Sprite = 0 ;
			ptrobj->Info = 0 ;
			ptrobj->Info2 = 0 ;
			ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
			break ;

		case RAIL_UP_NORD:	// montee NORD
			ptrobj->Sprite = 0 ;
			ptrobj->Info = 0 ;
			ptrobj->Info2 = 0 ;
			ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
			break ;

		case RAIL_UP_SUD:	// montee SUD
			ptrobj->Sprite = 0 ;
			ptrobj->Info = 0 ;
			ptrobj->Info2 = 0 ;
			ptrobj->Obj.X = (ptrobj->Obj.X/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
			break ;

		case RAIL_UP_OUEST:	// montee OUEST
			ptrobj->Sprite = 0 ;
			ptrobj->Info = 0 ;
			ptrobj->Info2 = 0 ;
			ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
			break ;

		case RAIL_UP_EST:	// montee EST
			ptrobj->Sprite = 0 ;
			ptrobj->Info = 0 ;
			ptrobj->Info2 = 0 ;
			ptrobj->Obj.Z = (ptrobj->Obj.Z/SIZE_BRICK_XZ)*SIZE_BRICK_XZ+DEMI_BRICK_XZ+1 ;
			break ;
	} // fin switch

	ptrobj->SizeSHit = (S16)brick ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
S32	GetNumBrickWagon( S32 brick )
{
#ifdef	LBA_EDITOR
	if( ListVarGame[FLAG_PLANETE_ESMER].Value )
#else
	if( ListVarGame[FLAG_PLANETE_ESMER] )
#endif
	{
		switch( brick )
		{
			case RAIL_E_NORD_SUD	: return RAIL_NORD_SUD ;
			case RAIL_E_EST_OUEST	: return RAIL_EST_OUEST ;
			case RAIL_E_UP_NORD    	: return RAIL_UP_NORD ;
			case RAIL_E_UP_SUD     	: return RAIL_UP_SUD ;
			case RAIL_E_UP_EST     	: return RAIL_UP_EST ;
			case RAIL_E_UP_OUEST   	: return RAIL_UP_OUEST ;
			case RAIL_E_NORD_EST   	: return RAIL_NORD_EST ;
			case RAIL_E_NORD_OUEST 	: return RAIL_NORD_OUEST ;
			case RAIL_E_SUD_EST	: return RAIL_SUD_EST ;
			case RAIL_E_SUD_OUEST 	: return RAIL_SUD_OUEST ;
			case RAIL_E_NORD_NORD_EST   : return RAIL_NORD_NORD_EST ;
			case RAIL_E_NORD_NORD_OUEST : return RAIL_NORD_NORD_OUEST ;
			case RAIL_E_SUD_SUD_EST     : return RAIL_SUD_SUD_EST ;
			case RAIL_E_SUD_SUD_OUEST   : return RAIL_SUD_SUD_OUEST ;
			case RAIL_E_OUEST_OUEST_SUD : return RAIL_OUEST_OUEST_SUD ;
			case RAIL_E_OUEST_OUEST_NORD: return RAIL_OUEST_OUEST_NORD ;
			case RAIL_E_EST_EST_SUD     : return RAIL_EST_EST_SUD ;
			case RAIL_E_EST_EST_NORD    : return RAIL_EST_EST_NORD ;
		}

		return( brick ) ;
	}
	else	return( brick ) ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	AdjustEssieuWagonAvant( S32 brickw )
{
	S32	brick ;
	S32	diff = FALSE ;
	S32	xess, yess, zess ;	// x,y et z de l'essieu avant
	S32	ey ;		// ecart y relatif … un pseudo
	S32	y ;		// ordonnee de la pente

	Rotate( 0,400,APtObj->Obj.Beta ) ;

	brick = GetBlockBrick( APtObj->Obj.X+X0,APtObj->Obj.Y-1,APtObj->Obj.Z+Z0 ) ;
	if( !brick )
	{
		brick = GetBlockBrick( APtObj->Obj.X+X0,APtObj->Obj.Y-SIZE_BRICK_Y-1,APtObj->Obj.Z+Z0 ) ;
		diff = TRUE ;
	}

	// depend du style actuel (planete TWINSUN ou ESMER)
	brick = GetNumBrickWagon( brick ) ;

	switch( brick )
	{
		case RAIL_UP_SUD:
			APtObj->Obj.CurrentFrame[0].Type = TYPE_TRANSLATE ;

			if( APtObj->Obj.Beta==0 )
			{
				APtObj->Obj.CurrentFrame[0].Beta = PAS_ESSIEU ;
			}
			else
			{
				zess = APtObj->Obj.Z+Z0 ;
				yess = APtObj->Obj.Y ;
				if( diff OR brickw!=RAIL_UP_SUD )	yess -= SIZE_BRICK_Y ;

				y = BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ,
						zess-(zess&(~511)) ) ;

				ey = APtObj->Obj.Y-(yess&(~255))-50 ;

				APtObj->Obj.CurrentFrame[0].Beta = (S16)(-abs(y-ey)) ;
			}
			break ;

		case RAIL_UP_NORD:
			APtObj->Obj.CurrentFrame[0].Type = TYPE_TRANSLATE ;

			if( APtObj->Obj.Beta==2048 )
			{
				APtObj->Obj.CurrentFrame[0].Beta = PAS_ESSIEU ;
			}
			else
			{
				zess = APtObj->Obj.Z+Z0 ;
				yess = APtObj->Obj.Y ;
				if( diff OR brickw!=RAIL_UP_NORD )	yess -= SIZE_BRICK_Y ;

				y = BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ,
						zess-(zess&(~511)) ) ;

				ey = APtObj->Obj.Y-(yess&(~255))-50 ;

				APtObj->Obj.CurrentFrame[0].Beta = (S16)(-abs(y-ey)) ;
			}
			break ;

		case RAIL_UP_EST:
			APtObj->Obj.CurrentFrame[0].Type = TYPE_TRANSLATE ;

			if( APtObj->Obj.Beta==1024 )
			{
				APtObj->Obj.CurrentFrame[0].Beta = PAS_ESSIEU ;
			}
			else
			{
				xess = APtObj->Obj.X+X0 ;
				yess = APtObj->Obj.Y ;
				if( diff OR brickw!=RAIL_UP_EST )	yess -= SIZE_BRICK_Y ;

				y = BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ,
						xess-(xess&(~511)) ) ;

				ey = APtObj->Obj.Y-(yess&(~255))-50 ;

				APtObj->Obj.CurrentFrame[0].Beta = (S16)(-abs(y-ey)) ;
			}
			break ;

		case RAIL_UP_OUEST:
			APtObj->Obj.CurrentFrame[0].Type = TYPE_TRANSLATE ;

			if( APtObj->Obj.Beta==3072 OR brickw==RAIL_NORD_OUEST )
			{
				APtObj->Obj.CurrentFrame[0].Beta = PAS_ESSIEU ;
			}
			else
			{
				xess = APtObj->Obj.X+X0 ;
				yess = APtObj->Obj.Y ;
				if( diff OR brickw!=RAIL_UP_OUEST )	yess -= SIZE_BRICK_Y ;

				y = BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ,
						xess-(xess&(~511)) ) ;

				ey = APtObj->Obj.Y-(yess&(~255))-50 ;

				APtObj->Obj.CurrentFrame[0].Beta = (S16)(-abs(y-ey)) ;
			}
			break ;

		default:
			APtObj->Obj.CurrentFrame[0].Type = TYPE_TRANSLATE ;

			if( APtObj->Coord.SHit.SHitX<0 )
			{
				ey = APtObj->Obj.Y-(APtObj->Obj.Y&(~255)) ;
				APtObj->Obj.CurrentFrame[0].Beta = (S16)(-ey) ;
			}
			else	APtObj->Obj.CurrentFrame[0].Beta = 0 ;
			break ;
	}

	APtObj->Coord.SHit.SHitX = APtObj->Obj.CurrentFrame[0].Beta ;
}

//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
void	AdjustEssieuWagonArriere( S32 brickw )
{
	S32	brick ;
	S32	diff = FALSE ;
	S32	xess, yess, zess ;	// x,y et z de l'essieu avant
	S32	ey	;		// ecart y relatif … un pseudo
	S32	y	;		// ordonnee de la pente

	Rotate( 0,-400,APtObj->Obj.Beta ) ;

	brick = GetBlockBrick( APtObj->Obj.X+X0,APtObj->Obj.Y,APtObj->Obj.Z+Z0 ) ;
	if( !brick )
	{
		brick = GetBlockBrick( APtObj->Obj.X+X0,APtObj->Obj.Y-SIZE_BRICK_Y-1,APtObj->Obj.Z+Z0 ) ;
		diff = TRUE ;
	}

	// depend du style actuel (planete TWINSUN ou ESMER)
	brick = GetNumBrickWagon( brick ) ;

	switch( brickw )
	{
		case RAIL_UP_SUD:
			switch( brick )
			{
				case RAIL_UP_SUD:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;

					if( APtObj->Obj.Beta==2048 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					}
					else
					{
						zess = APtObj->Obj.Z+Z0 ;
						yess = APtObj->Obj.Y ;
						if( diff )	yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ,
								zess-(zess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				default:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
					if( APtObj->Obj.Beta==0 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-(APtObj->Obj.Y-(APtObj->Obj.Y&(~255)))) ;
					}
					else	APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					break ;
			}
			break ;

		case RAIL_UP_NORD:
			switch( brick )
			{
				case RAIL_UP_NORD:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;

					if( APtObj->Obj.Beta==0 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					}
					else
					{
						zess = APtObj->Obj.Z+Z0 ;
						yess = APtObj->Obj.Y ;
						if( diff )	yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ,
								zess-(zess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				default:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
					if( APtObj->Obj.Beta==2048 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-(APtObj->Obj.Y-(APtObj->Obj.Y&(~255)))) ;
					}
					else	APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					break ;
			}
			break ;

		case RAIL_UP_EST:
			switch( brick )
			{
				case RAIL_UP_EST:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;

					if( APtObj->Obj.Beta==3072 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					}
					else
					{
						xess = APtObj->Obj.X+X0 ;
						yess = APtObj->Obj.Y ;
						if( diff )	yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ,
								xess-(xess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				default:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
					if( APtObj->Obj.Beta==1024 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-(APtObj->Obj.Y-(APtObj->Obj.Y&(~255)))) ;
					}
					else	APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					break ;
			}
			break ;

		case RAIL_UP_OUEST:
			switch( brick )
			{
				case RAIL_UP_OUEST:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;

					if( APtObj->Obj.Beta==1024 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					}
					else
					{
						xess = APtObj->Obj.X+X0 ;
						yess = APtObj->Obj.Y ;
						if( diff )	yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ,
								xess-(xess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				default:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
					if( APtObj->Obj.Beta==3072 )
					{
						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-(APtObj->Obj.Y-(APtObj->Obj.Y&(~255)))) ;
					}
					else	APtObj->Obj.CurrentFrame[1].Beta = PAS_ESSIEU ;
					break ;
			}
			break ;

		default:
			switch( brick )
			{
				case RAIL_UP_SUD:
					if( APtObj->Coord.SHit.SHitZ<0 )
					{
						APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
						zess = APtObj->Obj.Z+Z0 ;

						yess = APtObj->Obj.Y ;
						yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ,
								zess-(zess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				case RAIL_UP_NORD:
					if( APtObj->Coord.SHit.SHitZ<0 )
					{
						APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
						zess = APtObj->Obj.Z+Z0 ;
						yess = APtObj->Obj.Y ;
						yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ,
								zess-(zess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				case RAIL_UP_EST:
					if( APtObj->Coord.SHit.SHitZ<0 )
					{
						APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
						xess = APtObj->Obj.X+X0 ;
						yess = APtObj->Obj.Y ;
						yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( 0, SIZE_BRICK_Y, SIZE_BRICK_XZ,
								xess-(xess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				case RAIL_UP_OUEST:
					if( APtObj->Coord.SHit.SHitZ<0 )
					{
						APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
						xess = APtObj->Obj.X+X0 ;
						yess = APtObj->Obj.Y ;
						yess -= SIZE_BRICK_Y ;

						y = BoundRegleTrois( SIZE_BRICK_Y, 0, SIZE_BRICK_XZ,
								xess-(xess&(~511)) ) ;

						ey = APtObj->Obj.Y-(yess&(~255))-50 ;

						APtObj->Obj.CurrentFrame[1].Beta = (S16)(-abs(y-ey)) ;
					}
					break ;

				default:
					APtObj->Obj.CurrentFrame[1].Type = TYPE_TRANSLATE ;
					APtObj->Obj.CurrentFrame[1].Beta = 0 ;
					break ;
			}
			break ;
	}

	APtObj->Coord.SHit.SHitZ = APtObj->Obj.CurrentFrame[1].Beta ;
}

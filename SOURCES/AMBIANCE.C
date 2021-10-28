
#include	"c_extern.h"
#include	"Balance.h"

#ifdef	CDROM
extern	LONG	FlagVoiceCD ;
LONG	CurrentMusicCD = -1 ;
ULONG	EndMusicCD = 0 ;
#endif

/*══════════════════════════════════════════════════════════════════════════*
		█▀▀▀█ █▄ ▄█ █▀▀█   █    █▀▀▀█ ██▄ █ █▀▀▀▀ █▀▀▀▀
		██▀▀█ ██▀ █ ██▀▀█  ██   ██▀▀█ ██▀██ ██    ██▀▀
		▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀  ▀▀   ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

ULONG	HQ_MixSample( WORD numsample, WORD decalage, WORD repeat, WORD volleft, WORD volright )
{
	UBYTE	string[256] ;
	ULONG	retvalue ;
	UBYTE	*ptr ;

	retvalue = -1 ;

	if( !SamplesEnable )	return -1	;

	if ( numsample == -1 )	return -1	;/* Loran ( Come from GereSceneMenu)*/

	ptr = HQR_GetSample( HQR_Samples, numsample ) ;
	if( ptr )
	{
		retvalue =
		WavePlay( numsample, decalage, repeat, 0, volleft, volright, ptr ) ;
	}

	return retvalue ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	HQ_StopSample()
{
	WaveStop()	;
}
/*══════════════════════════════════════════════════════════════════════════*/
void	HQ_StopOneSample(WORD num)
{
	WaveStopOne( num )	;
}
/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

WORD	GiveBalance( WORD xp, WORD yp, LONG volume, ULONG *volleft, ULONG *volright )
{
	LONG	balance ;
	int	flag = 0 ;

	if( (yp > 240+480)
	OR  (yp < -240) )	return FALSE ;

	// baisse volume vers le haut à partir de Y<0 jusqu'a -480
	if( yp < 0 )
	{
		volume = RegleTrois32( 0, volume, 240, 240+yp ) ;
		flag = 1 ;
	}
	// baisse volume vers le bas à partir de Y>479  jusqu'a 480+480
	if( yp > 479 )
	{
		volume = RegleTrois32( 0, volume, 240, 240+480-yp ) ;
		flag = 1 ;
	}

	// gere attenuation du volume sur le cote gauche
	// utilise le volume eventuellement deja ajuste sur Y
	if( (xp >= -320) AND (xp < 0) )
	{
		*volleft  = RegleTrois32( 0, (volume*100)/128, 320, xp+320 ) ;
//		*volleft  = RegleTrois32( 0, volume, 320, xp+320 ) ;
		*volright = 0 ;
		return TRUE ;
	}

	// gere attenuation du volume sur le cote droit
	if( (xp >= 640) AND (xp < 640+320) )
	{
		*volleft  = 0 ;
		*volright = RegleTrois32( 0, (volume*100)/128, 320, 320+640-xp ) ;
//		*volright = RegleTrois32( 0, volume, 320, 320+640-xp ) ;
		return TRUE ;
	}

	// sinon gere la balance gauche/droite sur l'ecran
	// l'eventuelle attenuation du volume sur Y est toujour la
	if( (xp >= 0) AND (xp < 640) )
	{
		if (flag) volume = (volume*100)/128;

		balance = RegleTrois32( 0,256, 640, xp ) ;
		Balance(	balance, volume,
				volleft, volright  ) ;
		return TRUE ;
	}

	return FALSE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	HQ_3D_MixSample(	WORD numsample, WORD decalage, WORD repeat,
				WORD x, WORD y, WORD z )
{
	ULONG	volleft, volright, longhandle ;
	LONG	balance ;

	if( !SamplesEnable )	return ;

	ProjettePoint(	x-WorldXCube,
			y-WorldYCube,
			z-WorldZCube ) ;

	if( GiveBalance( Xp, Yp, 128, &volleft, &volright ) )
	{
		longhandle = HQ_MixSample(	numsample, decalage, repeat,
						volleft, volright ) ;

		WaveGiveInfo0( longhandle, (LONG)Xp<<16 + Yp ) ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	HQ_ChangeBalanceSamples( WORD oldxporg, WORD oldyporg )
{
	LONG	n, nbsamp ;
	T_WAVE	*ptrlist ;
	LONG	deltaxp, newxp ;
	LONG	deltayp, newyp ;
	ULONG	volleft, volright ;

  return ;
#ifdef	TANTPIS
	if( !SamplesEnable )	return ;

	nbsamp = WaveGetSnap( &ptrlist ) ;

	deltaxp = XpOrgw - oldxporg ;
	deltaxp = YpOrgw - oldyporg ;

	for( n=0; n<nbsamp; n++ )
	{
		if( ptrlist->LongHandle != 123456 )	// son ambiance
		{
			newxp = ptrlist->Info0>>16  - deltaxp ;
			newyp = ptrlist->Info0&0xFFFF - deltayp ;

			if( GiveBalance( newxp, newyp, 128, &volleft, &volright ) )
			{
				WaveChangeVolume( ptrlist->LongHandle, volleft, volright ) ;
				WaveGiveInfo0( ptrlist->LongHandle, newxp<<16 + newyp ) ;
			}
			else
			{
				// coupe ce sample longhandle plus tard
				WaveStopOne( ptrlist->LongHandle & 0xFFFF ) ;
			}
		}
		ptrlist++ ;
	}
#endif
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void	GereAmbiance()
{
	LONG	sample ;
	LONG	numsample ;
	LONG	n ;
	LONG	decal ;
	LONG	repeat ;
	ULONG	longhandle ;

	if( !SamplesEnable )	return ;

	if( TimerRef >= TimerNextAmbiance )
	{
		sample = Rnd( 4 ) ;	/* 0 1 2 3 */

		for( n=0; n<4; n++ )
		{
			if( !(SamplePlayed & (1<<sample)) ) /* si pas joue */
			{
				SamplePlayed |= (1<<sample) ;	/* marque le joué */
				if( SamplePlayed == 15 ) /* tous joue */
					SamplePlayed = 0 ;

				numsample = SampleAmbiance[sample] ;
				if( numsample != -1 ) /* si defini */
				{
					decal = SampleRnd[sample] ;
					repeat = SampleRepeat[sample] ;

/*					if( repeat == 0 ) // infini
					{
						if( WaveInList( numsample ) )
						{

						}
					}
*/
					longhandle =HQ_MixSample(	numsample,
							0x1000+Rnd(decal)-(decal/2),
							repeat, 110, 110 ) ;

					// info son ambiance
//					WaveGiveInfo0( longhandle, 123456 ) ;
					break ;
				}
			}
			sample++ ;
			sample &= 3 ;

		}

		TimerNextAmbiance = TimerRef
					+
				( Rnd( SecondEcart ) + SecondMin ) * 50 ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
		   █▀▀▀█ █▀▀▀█ █     █▀▀▀▀ ▀▀█▀▀ ▀▀█▀▀ █▀▀▀▀
		   ██▀▀▀ ██▀▀█ ██    ██▀▀    ██    ██  ██▀▀
		   ▀▀    ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀    ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void	FadePal( UBYTE r, UBYTE v, UBYTE b, UBYTE *ptrpal, WORD percent )
{
	UBYTE	workpal[768] ;
	LONG	n ;

	for( n=0; n<256; n++ )
	{
		workpal[n*3+0] = RegleTrois32( r, ptrpal[n*3+0], 100, percent ) ;
		workpal[n*3+1] = RegleTrois32( v, ptrpal[n*3+1], 100, percent ) ;
		workpal[n*3+2] = RegleTrois32( b, ptrpal[n*3+2], 100, percent ) ;
	}
	Palette( workpal ) ;
}

void	FadeToBlack( UBYTE *ptrpal )
{
	LONG	n ;

	if( !FlagBlackPal )
	{
		for( n=100; n>=0; n-=2 )
		{
			Vsync() ;
			FadePal( 0,0,0, ptrpal, n ) ;
		}
	}
	FlagBlackPal = TRUE ;
}

void	WhiteFade()
{
	UBYTE	workpal[768] ;
	LONG	n,m ;

	for( n=0; n<=255; n++ )
	{
		memset( workpal, n, 768 ) ;
		Vsync() ;
		Palette( workpal ) ;
	}
}

void	FadeWhiteToPal( UBYTE *ptrpal )
{
	LONG	n ;

	for( n=0; n<=100; n+=1 )
	{
		Vsync() ;
		FadePal( 255,255,255, ptrpal, n ) ;
	}
}

void	FadeToPal( UBYTE *ptrpal )
{
	LONG	n ;

	for( n=0; n<=100; n+=2 )
	{
		Vsync() ;
		FadePal( 0,0,0, ptrpal, n ) ;
	}
	FlagBlackPal = FALSE ;
}

void	SetBlackPal()
{
	LONG	n ;

	for( n=0; n<256; n++ )
	{
		PalOne( n, 0,0,0 ) ;
	}
	FlagBlackPal = TRUE ;
}

void	FadePalToPal( UBYTE *ptrpal, UBYTE *ptrpal1 )
{
	UBYTE	workpal[768] ;
	LONG	n,m ;

	for( m=0; m<=100; m++ )
	{
		for( n=0; n<256; n++ )
		{
			workpal[n*3+0] = RegleTrois32( ptrpal[n*3+0], ptrpal1[n*3+0], 100, m ) ;
			workpal[n*3+1] = RegleTrois32( ptrpal[n*3+1], ptrpal1[n*3+1], 100, m ) ;
			workpal[n*3+2] = RegleTrois32( ptrpal[n*3+2], ptrpal1[n*3+2], 100, m ) ;
		}
		Vsync() ;
		Palette( workpal ) ;
	}
}

void	FadeToRed( UBYTE *ptrpal )
{
	LONG	n ;

	for( n=100; n>=0; n-=2 )
	{
		Vsync() ;
		FadePal( 255,0,0, ptrpal, n ) ;
	}
}

void	FadeRedToPal( UBYTE *ptrpal )
{
	LONG	n ;

	for( n=0; n<=100; n+=2 )
	{
		Vsync() ;
		FadePal( 255,0,0, ptrpal, n ) ;
	}
}

/*══════════════════════════════════════════════════════════════════════════*
			    █▄ ▄█  █    █▀▀▀▄  █
			    ██▀ █  ██   ██  █  ██
			    ▀▀  ▀  ▀▀   ▀▀▀▀   ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

#ifdef	CDROM

void	StopMusicCD( void )
{
	StopCDR() ;
	CurrentMusicCD = -1 ;
}

#endif

/*──────────────────────────────────────────────────────────────────────────*/

void	FadeMusicMidi( ULONG t )
{
	FadeMidiDown( t ) ;
	NumXmi = -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	StopMusicMidi()
{
	StopMidi() ;
	NumXmi = -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	PlayMusic( WORD num )
{
	if( num == -1 )
	{
#ifdef	CDROM
		StopMusicCD() ;
#endif
		StopMusicMidi() ;
		return ;
	}

#ifdef	CDROM

	if( FlagVoiceCD		// voix sur CD music fm
	OR (num < 1)
	OR (num > 9) )		// ou jingle que FM
	{
		PlayMidiFile( num ) ;
	}
	else			// voix sur HD ponheur
	{
		PlayCdTrack( num ) ; // 1ere track = 2
	}

#else
	PlayMidiFile( num ) ;
#endif
}

/*──────────────────────────────────────────────────────────────────────────*/

void	PlayMidiFile( WORD num )
{
	if( !Midi_Driver_Enable )	return ;// hum si on peut

#ifdef	CDROM
	StopMusicCD() ;
#endif

	if( (num != NumXmi)
	OR  (!IsMidiPlaying()) )
	{
		StopMusicMidi() ;
//		HQR_Reset_Ressource( HQR_Midi ) ;
		PtrXmi = HQR_Get( HQR_Midi, num ) ;
		NumXmi = num ;
		PlayMidi( PtrXmi ) ;
		VolumeMidi( 100 ) ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

#ifdef	CDROM
LONG	GetMusicCD()
{
	if ( TimerSystem > EndMusicCD)	CurrentMusicCD = -1 ;
	return CurrentMusicCD ;
}
/*──────────────────────────────────────────────────────────────────────────*/

void	PlayCdTrack( WORD num )
{
	LONG	t ;

	SaveTimer() ;

	FadeMusicMidi( 1 ) ;
	NumXmi = -1 ;

	if (num != GetMusicCD())
	{
		StopMusicCD() ;
		EndMusicCD = ( GetLengthTrackCDR( num+1 ) * 50 ) / 75 + 50 ;
		PlayTrackCDR( num + 1 ) ;
		EndMusicCD += TimerSystem ;
		CurrentMusicCD = num ;
	}

	RestoreTimer() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void	PlayAllMusic( WORD num )
{
	if( Midi_Driver_Enable )
	{
		if( (num != NumXmi)
		OR  (!IsMidiPlaying()) )
		{
			StopMusicMidi() ;
			PtrXmi = HQR_Get( HQR_Midi, num ) ;
			NumXmi = num ;
			PlayMidi( PtrXmi ) ;
			VolumeMidi( 100 ) ;
		}
	}

//	if( (n=num+1) != GetMusicCDR() )
	if( num != GetMusicCD() )
	{
		StopMusicCD() ;
		EndMusicCD = ( GetLengthTrackCDR( num+1 ) * 50 ) / 75 + 50 ;
		PlayTrackCDR( num+1 ) ;
		EndMusicCD += TimerSystem ;
		CurrentMusicCD = num ;
	}
}

/*──────────────────────────────────────────────────────────────────────────*/

#endif

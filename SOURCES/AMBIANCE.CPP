#include        "c_extern.h"

#define BCL_FADE        50
#define FADE_DELAY      200

//#define       DISTANCE_SAMPLE_MAX     963
#define DISTANCE_SAMPLE_MAX     700
#define DISTANCE_SAMPLE_MIN_EXT 5000L
#define DISTANCE_SAMPLE_MAX_EXT 30000L

#ifdef  CDROM
extern  S32     FlagVoiceCD ;
#endif

// Tableau d'indirection des palettes se trouvant dans RESS.HQR
// (voir le fichier f:\projet\lba2\datas\ress.lst)

U8      **ListPalette[MAX_PALETTES] = {
        &PtrPalNormal,          // Palette Normal
        &PtrPalCurrent,         // Palette Courante
        &PtrPalEclair,          // Palette Eclair
        &PtrPalBlack            // Palette Noire
        } ;


S32     RestartRainSample = FALSE       ;

/*──────────────────────────────────────────────────────────────────────────*/

//HQR_GET_CALLBACK      DelBlocHQRSample ;

void    DelBlocHQRSample( char *name, S32 index )
{
        name  = name ;
        index = index ;

        // methode violente : on vire tous les samples
        HQ_StopSample() ;
        // pour remettre la pluie en route
        RestartRainSample = TRUE        ;

        // mise à 0 du pointeur de fonction appelé par HQR_Get()
        HQRGetDelFunc = NULL ;
}

/*══════════════════════════════════════════════════════════════════════════*
                █▀▀▀█ █▄ ▄█ █▀▀█   █    █▀▀▀█ ██▄ █ █▀▀▀▀ █▀▀▀▀
                ██▀▀█ ██▀ █ ██▀▀█  ██   ██▀▀█ ██▀██ ██    ██▀▀
                ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀  ▀▀   ▀▀  ▀ ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
void    InitParmSample( void )
{
        ParmSampleVolume    = 127       ; // plein pot
        ParmSampleDecalage  = 0         ;
        ParmSampleFrequence = 0         ;
}

/*──────────────────────────────────────────────────────────────────────────*/

U32     HQ_MixSample( S32 numsample, S32 frequence, S32 decalage, S32 repeat, S32 pan, S32 volume )
{
        U32     timesample ;
        U32     retvalue = -1 ;
        U8      *ptr ;

        if( SamplesEnable AND numsample != -1 )
	{
		HQR_GET_CALLBACK        *memoerrorfunc ;

		timesample = IsSamplePlaying( numsample ) ;

		if( timesample
		AND (TimerSystemHR-timesample)<SAMPLE_TIME_REPEAT )
		{
			InitParmSample() ;
			return 0 ;
		}

		SaveTimer() ;

		// tient compte du ParmSampleVolume
		if( ParmSampleVolume!=127 )
		{
			volume = RegleTrois( 0, ParmSampleVolume, 127, volume ) ;
		}

		// tient compte du SampleVolume
		volume = RegleTrois( 0, SampleVolume, 127, volume ) ;

		if( ParmSampleFrequence )       frequence = ParmSampleFrequence ;
		if( ParmSampleDecalage  )       decalage  = ParmSampleDecalage  ;

		if( decalage )
		{
			frequence = frequence - decalage/2 + MyRnd(decalage) ;
		}

		// si pas assez de memoire, tant pis, on joue pas
		// le sample, mais on ne sort pas du prog !
		memoerrorfunc   = HQRGetErrorFunc ;
		HQRGetErrorFunc = NULL ;

		HQRGetDelFunc = DelBlocHQRSample ;

		if( numsample&0xFF000000 )      numsample = (numsample>>8)&0xFFFF ;

		// Blindage inutile
                ptr = (U8*)GivePtrSample( numsample ) ;
		if( ptr )
		{
			retvalue = PlaySample( ptr, numsample, frequence, repeat, volume, pan ) ;
		}
		else
		{
			LogPrintf( "Can't play sample %d: not enough memory.\n", numsample ) ;
		}

		HQRGetDelFunc = NULL ;
		HQRGetErrorFunc = memoerrorfunc ;

                RestoreTimer() ;
        }

        InitParmSample() ;

        return retvalue ;
}

/*──────────────────────────────────────────────────────────────────────────*/
U32     HQ_RestartSample( S32 numsample, S32 frequence, S32 repeat, S32 pan, S32 volume )
{
        U32     retvalue = -1 ;
        U8      *ptr ;

        if( SamplesEnable AND numsample != -1 )
        {
                SaveTimer() ;

                HQRGetDelFunc = DelBlocHQRSample ;

                // Blindage inutile
                ptr = (U8*)GivePtrSample( numsample ) ;
                if( ptr )
                {
                        retvalue = PlaySample( ptr, numsample, frequence, repeat, volume, pan ) ;
                }

                HQRGetDelFunc = NULL ;

                RestoreTimer() ;
        }

        InitParmSample() ;

        return retvalue ;
}

/*══════════════════════════════════════════════════════════════════════════*/
void	HQ_StopSample(void)
{
	StopSamples()	;		// stop all samples
	FadeInSamples(0);		// clear fadeout flag
}

/*══════════════════════════════════════════════════════════════════════════*/
void    HQ_PauseSamples(void)
{
        while(FadeOutSamples(200));     // 1/5 s
        PauseSamples()  ;
}
/*══════════════════════════════════════════════════════════════════════════*/
void    HQ_ResumeSamples(void)
{
        ResumeSamples() ;
        while(FadeInSamples(200)!=200); // 1/5 s
}
/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/
S32     GiveBalance( S32 xp, S32 yp, S32 distance, U32 *pan, S32 *vol )
{
        if( (yp > 480+480)
        OR  (yp < -480) )       return FALSE ;

        if( (xp < -320)
        OR  (xp >= 640+320) )   return FALSE ;

        if( xp<160 )            *pan = RegleTrois( 0,  32, 160+319, xp+320 ) ;
        else if( xp>480 )       *pan = RegleTrois( 96,127, 640+319-480, xp-480 ) ;
        else                    *pan = RegleTrois( 32, 96, 480-160, xp-160 ) ;

        if( CubeMode==CUBE_INTERIEUR )
                if( distance>DISTANCE_SAMPLE_MAX )      *vol = 0 ;
                else    *vol = BoundRegleTrois( 127, 0, DISTANCE_SAMPLE_MAX, distance ) ;
        else
        {
                if( distance<=DISTANCE_SAMPLE_MIN_EXT )
                        *vol = 127 ;
                else if( distance>DISTANCE_SAMPLE_MAX_EXT )
                        return FALSE ;
                else    *vol = BoundRegleTrois( 127, 0,
                                           DISTANCE_SAMPLE_MAX_EXT-DISTANCE_SAMPLE_MIN_EXT,
                                           distance-DISTANCE_SAMPLE_MIN_EXT ) ;
        }

        return TRUE ;
}

/*──────────────────────────────────────────────────────────────────────────*/

U32     HQ_3D_MixSample(        S32 numsample, S32 frequence, S32 decalage,
                                S32 repeat,
                                S32 x, S32 y, S32 z )
{
        U32     pan ;
        S32     volume, distance ;
        S32     retvalue = 0 ;

        if( SamplesEnable )
        {
                PtrProjectPoint( x, y,  z ) ;

                if( CubeMode==CUBE_INTERIEUR )
                        distance = Distance2D( 320, 240, Xp, Yp ) ;
                else    distance = Distance3D( x, y, z, CameraX, CameraY, CameraZ ) ;

//              if( distance<DISTANCE_SAMPLE_MAX_EXT )
                {
                        if( GiveBalance( Xp, Yp, distance, &pan, &volume ) )
                        {
                                retvalue = HQ_MixSample( numsample, frequence, decalage, repeat, pan, volume ) ;
                        }
                }
        }

        if( !retvalue AND !repeat )     // sample Always
        {
                retvalue = numsample<<8 ;
        }

        InitParmSample() ;

        return( retvalue ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    HQ_ChangePanSample( S32 numsample, S32 volume, U32 pan )
{
        if( SamplesEnable )
        {
                if( ParmSampleVolume!=127 )
                {
                        volume = RegleTrois( 0, ParmSampleVolume, 127, volume ) ;
                        ParmSampleVolume = 127 ;
                }

                // tient compte du SampleVolume
                volume = RegleTrois( 0, SampleVolume, 127, volume ) ;

                ChangeVolumePanSample( numsample, volume, pan ) ;
        }

        InitParmSample() ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    HQ_3D_ChangePanSample(  S32 numsample, S32 x, S32 y, S32 z )
{
        U32     pan ;
        S32     volume, distance ;

        if( SamplesEnable )
        {
                PtrProjectPoint( x, y,  z ) ;

                if( CubeMode==CUBE_INTERIEUR )
                        distance = Distance2D( 320, 240, Xp, Yp ) ;
                else    distance = Distance3D( x, y, z, CameraX, CameraY, CameraZ ) ;

                volume = 0 ;

                GiveBalance( Xp, Yp, distance, &pan, &volume ) ;

                if( volume>0 )
                {
                        HQ_ChangePanSample( numsample, volume, pan ) ;
                }
                else    HQ_StopOneSample( numsample ) ;
        }

        InitParmSample() ;
}

/*══════════════════════════════════════════════════════════════════════════*/
/*══════════════════════════════════════════════════════════════════════════*/

void    LaunchAmbiance( void )
{
        S32     numsample ;
        S32     n ;
        S32     decal ;
        S32     freq  ;
        S32     volume ;
        S32     repeat ;

	if( !SamplesEnable )    return ;

#ifdef	DEMO
	if( NumCube==NUM_CUBE_PHANTOM )	return ;
#endif

        for( n=0; n<4; n++ )
        {
                numsample = SampleAmbiance[n] ;
                repeat = SampleRepeat[n] ;

                if( numsample != -1 // si defini
                AND repeat==0 )     // on lance immediatement les repeat infini
                {
                        decal  = SampleRnd[n] ;
                        freq   = SampleFreq[n] ;
                        volume = SampleVol[n] ;

                        InitParmSample() ;

                        if( !IsSamplePlaying(numsample) )
                        {
                                HQ_MixSample( numsample,
                                              freq, decal,
                                              repeat, 64, volume ) ;
                        }
                }
        }
}

/*──────────────────────────────────────────────────────────────────────────*/

void    GereAmbiance( void )
{
        S32     sample  ;
        S32     numsample ;
        S32     n       ;
        S32     decal   ;
        S32     repeat  ;
        S32     freq    ;
        S32     volume  ;

        if( !SamplesEnable )    return ;

#ifdef	DEMO
	if( NumCube==NUM_CUBE_PHANTOM )	return ;
#endif

	if( TimerRefHR >= TimerNextAmbiance )
        {
                sample = MyRnd( 4 ) ;   /* 0 1 2 3 */

                for( n=0; n<4; n++ )
                {
                        if( !(SamplePlayed & (1<<sample)) ) /* si pas joue */
                        {
                                SamplePlayed |= (1<<sample) ;   /* marque le joué */
                                if( SamplePlayed == 15 ) /* tous joue */
                                        SamplePlayed = 0 ;

                                numsample = SampleAmbiance[sample] ;
                                repeat = SampleRepeat[sample] ;

                                if( numsample!=-1 // si defini
                                AND repeat!=0 )   // on ne relance jamais 1 repeat infini
                                {
                                        decal  = SampleRnd[sample] ;
                                        freq   = SampleFreq[sample] ;
                                        volume = SampleVol[sample] ;

                                        InitParmSample() ;

                                        if( !IsSamplePlaying(numsample) )
                                        {
                                                HQ_MixSample( numsample,
                                                              freq, decal,
                                                              repeat, MyRnd(127), volume ) ;
                                        }
                                        break ;
                                }
                        }
                        sample++ ;
                        sample &= 3 ;

                }

                TimerNextAmbiance = TimerRefHR
                                        +
                                ( MyRnd( SecondEcart ) + SecondMin ) * 1000 ;
        }

        // on s'assure de ne jamais en jouer un autre
        if( SecondMin<0 )       TimerNextAmbiance = TimerRefHR+200000 ;
}

/*──────────────────────────────────────────────────────────────────────────*/

// Appelée au changement de cube pour couper les ambiance du cube en cours
void    ClearAmbiance( void )
{
        S32     n ;

        for( n=0; n<4; n++ )
        {
                if( SampleAmbiance[n] != -1 ) /* si defini */
                {
                        HQ_StopOneSample( SampleAmbiance[n] ) ;
                }
        }
}

/*══════════════════════════════════════════════════════════════════════════*
                   █▀▀▀█ █▀▀▀█ █     █▀▀▀▀ ▀▀█▀▀ ▀▀█▀▀ █▀▀▀▀
                   ██▀▀▀ ██▀▀█ ██    ██▀▀    ██    ██  ██▀▀
                   ▀▀    ▀▀  ▀ ▀▀▀▀▀ ▀▀▀▀▀   ▀▀    ▀▀  ▀▀▀▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/
void    InitPalette( void )
{
        PtrPalNormal = (U8*)LoadMalloc_HQR( RESS_HQR_NAME, RESS_PAL ) ;
        if( !PtrPalNormal )     TheEndCheckFile( RESS_HQR_NAME ) ;

        PtrPalEclair = (U8*)LoadMalloc_HQR( RESS_HQR_NAME, RESS_PAL_ECLAIR ) ;
        if( !PtrPalEclair )     TheEndCheckFile( RESS_HQR_NAME ) ;

        PtrPalBlack = (U8*)LoadMalloc_HQR( RESS_HQR_NAME, RESS_PAL_BLACK ) ;
        if( !PtrPalBlack )      TheEndCheckFile( RESS_HQR_NAME ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
void    ChoicePalette( void )
{
        if( CubeMode==CUBE_INTERIEUR
        OR  (Island==0 AND TEMPETE_FINIE) )
        {
                IdxPalette = RESS_XPL00 ;       // citabau
        }
        else
        {
                IdxPalette = RESS_XPL0+Island ;
        }

        if( IdxPalette!=LastIdxPalette )
        {
                if( !Load_HQR( RESS_HQR_NAME, PtrXplPalette, IdxPalette ) )
                        TheEnd( ERROR_NOT_FOUND_FILE, RESS_HQR_NAME ) ;

                LastIdxPalette = IdxPalette ;
        }

        PtrXplHeader = (XPL_HEADER *)PtrXplPalette ;

        PtrPalCurrent   = PtrXplPalette + PtrXplHeader->OffsetPalette   ;
        PtrNuances      = PtrXplPalette + PtrXplHeader->OffsetFog       ;
        PtrTransPal     = PtrXplPalette + PtrXplHeader->OffsetTransp    ;
        FogCoul         = PtrXplHeader->FogCoul                         ;
        PalLevel        = PtrXplHeader->ShadeNormalLevel                ;

        // Fixe Palette
        if( CubeMode==CUBE_INTERIEUR OR TEMPETE_FINIE )
        {
                if( IsSamplePlaying(SAMPLE_RAIN) )      HQ_StopOneSample( SAMPLE_RAIN ) ;
        }

        RestartRainSample = TRUE ;
        PtrPal = PtrPalCurrent ;
        PtrCLUTGouraud = PtrCLUTFog = PtrNuances ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    StartRainSample( void )
{
        if( SamplesEnable
	AND CubeMode==CUBE_EXTERIEUR
	AND !TEMPETE_FINIE
        AND !IsSamplePlaying(SAMPLE_RAIN) )
        {
                HQ_MixSample(   SAMPLE_RAIN,
                                0x1000, 300,    // frequence, decalage
                                0, 64, VOLUME_RAIN ) ;
        }

        RestartRainSample = FALSE       ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadePal( U8 r, U8 v, U8 b, U8 *ptrpal, S32 scale )
{
        U8      workpal[768] ;
        S32     n ;

        for( n=0; n<256; n++ )
        {
                workpal[n*3+0] = (U8)RegleTrois( r, ptrpal[n*3+0], FADE_DELAY, scale ) ;
                workpal[n*3+1] = (U8)RegleTrois( v, ptrpal[n*3+1], FADE_DELAY, scale ) ;
                workpal[n*3+2] = (U8)RegleTrois( b, ptrpal[n*3+2], FADE_DELAY, scale ) ;
        }

        PaletteSync( workpal ) ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadeToBlack( U8 *ptrpal )
{
        S32     starttime, delta;

        if( FlagBlackPal )      return  ;

        SaveTimer() ;

        starttime = TimerSystemHR       ;

        do
        {
                ManageTime()    ;

                delta = starttime + FADE_DELAY - TimerSystemHR  ;

                if(delta<0)     delta = 0       ;

                FadePal( 0,0,0, ptrpal, delta ) ;
        }
        while(delta)    ;

        FlagBlackPal = TRUE ;

        RestoreTimer() ;
}

//──────────────────────────────────────────────────────────────────────────
void    WhiteFade()
{
        U8      workpal[768] ;
        S32     starttime, delta;

        SaveTimer() ;

        memset( workpal, 255, 768 ) ;

        starttime = TimerSystemHR       ;

        do
        {
                ManageTime()    ;

                delta = TimerSystemHR - starttime;

                if(delta>FADE_DELAY)    delta = FADE_DELAY      ;

                FadePal( 0,0,0, workpal, delta ) ;
        }
        while(delta!=FADE_DELAY)        ;

        FlagBlackPal = FALSE ;

        RestoreTimer() ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadeWhiteToPal( U8 *ptrpal )
{
        S32     starttime, delta;

        SaveTimer() ;

        starttime = TimerSystemHR ;

        do
        {
                ManageTime()    ;

                delta = TimerSystemHR - starttime;

                if(delta>FADE_DELAY)    delta = FADE_DELAY      ;

                FadePal( 255,255,255, ptrpal, delta ) ;
        }
        while(delta!=FADE_DELAY)        ;

        FlagBlackPal = FALSE ;

        RestoreTimer() ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadeToPal( U8 *ptrpal )
{
        S32     starttime, delta;

        SaveTimer() ;

        starttime = TimerSystemHR ;

        do
        {
                ManageTime()    ;

                delta = TimerSystemHR - starttime;

                if(delta>FADE_DELAY)    delta = FADE_DELAY      ;

                FadePal( 0,0,0, ptrpal, delta ) ;
        }
        while(delta!=FADE_DELAY)        ;

        FlagBlackPal = FALSE ;

        RestoreTimer() ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadeToBlackAndSamples( U8 *ptrpal )
{
        S32     delta = 0 ;

        SaveTimer( ) ;

        if( !FlagBlackPal )
        {
                do
                {
                        delta = FadeOutSamples( FADE_DELAY ) ;
                        FadePal( 0,0,0, ptrpal, delta ) ;
                }
                while(delta!=0) ;
        }

        if( (CubeMode==CUBE_INTERIEUR OR TEMPETE_FINIE)
        AND IsSamplePlaying(SAMPLE_RAIN) )
        {
                HQ_StopOneSample( SAMPLE_RAIN ) ;
        }

        PauseSamples() ;

        FlagBlackPal = TRUE ;

        RestoreTimer( ) ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadeToPalAndSamples( U8 *ptrpal )
{
        S32     delta = FADE_DELAY ;

        SaveTimer( ) ;

        ResumeSamples() ;

        if( (CubeMode==CUBE_INTERIEUR OR TEMPETE_FINIE)
        AND IsSamplePlaying(SAMPLE_RAIN) )
        {
                HQ_StopOneSample( SAMPLE_RAIN ) ;
        }

        do
        {
                delta = FadeInSamples( FADE_DELAY ) ;
                FadePal( 0,0,0, ptrpal, delta ) ;
        }
        while(delta!=FADE_DELAY) ;

        FlagBlackPal = FALSE ;

        RestoreTimer( ) ;
}

//──────────────────────────────────────────────────────────────────────────
void    SetBlackPal()
{
        S32     n ;

        for( n=0; n<256; n++ )
        {
                PalOne( n, 0,0,0 ) ;
        }
        FlagBlackPal = TRUE ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadePalToPal( U8 *ptrpal, U8 *ptrpal1 )
{
        U8      workpal[768] ;
        S32     starttime, delta;

        SaveTimer( ) ;

        starttime = TimerSystemHR ;

        do
        {
                S32     n ;

                ManageTime()    ;

                delta = TimerSystemHR - starttime;

                if(delta>FADE_DELAY)    delta = FADE_DELAY      ;

                for( n=0; n<256; n++ )
                {
                        workpal[n*3+0] = (U8)RegleTrois( ptrpal[n*3+0], ptrpal1[n*3+0], FADE_DELAY, delta ) ;
                        workpal[n*3+1] = (U8)RegleTrois( ptrpal[n*3+1], ptrpal1[n*3+1], FADE_DELAY, delta ) ;
                        workpal[n*3+2] = (U8)RegleTrois( ptrpal[n*3+2], ptrpal1[n*3+2], FADE_DELAY, delta ) ;
                }

                PaletteSync( workpal ) ;
        }
        while(delta!=FADE_DELAY) ;

        RestoreTimer()  ;
}

//──────────────────────────────────────────────────────────────────────────
void    FadeToPalIndex( U8 num )
{

        if( num>=MAX_PALETTES ) return ;

        if( num==3 )    // palette noire demandee ?
                FadeToBlackAndSamples( PtrPal ) ;
        else if( PtrPal==PtrPalBlack )
                FadeToPalAndSamples( *ListPalette[num] ) ;
        else    FadePalToPal( PtrPal, *ListPalette[num] ) ;

        PtrPal = *ListPalette[num] ;
}

//──────────────────────────────────────────────────────────────────────────
void    PaletteIndex( U8 num )
{
        if( num>=MAX_PALETTES ) return ;

        SaveTimer( ) ;

        PaletteSync( *ListPalette[num] ) ;

        PtrPal = *ListPalette[num] ;

        RestoreTimer( ) ;
}


/*══════════════════════════════════════════════════════════════════════════*
                            █▄ ▄█  █    █▀▀▀▄  █
                            ██▀ █  ██   ██  █  ██
                            ▀▀  ▀  ▀▀   ▀▀▀▀   ▀▀
 *══════════════════════════════════════════════════════════════════════════*/
/*──────────────────────────────────────────────────────────────────────────*/

void    ReadVolumeSettings()
{
        //---------------------------------------------------------------
        //      Read config file (LBA2.CFG)
        //---------------------------------------------------------------
        SampleVolume = DefFileBufferReadValueDefault( "WaveVolume", DEF_SAMPLE_VOLUME ) ;

        if( SampleVolume<0 )    SampleVolume = 0 ;
        if( SampleVolume>127 )  SampleVolume = 127 ;

        //---------------------------------------------------------------
        VoiceVolume = DefFileBufferReadValueDefault( "VoiceVolume", DEF_VOICE_VOLUME ) ;

        if( VoiceVolume<0 )     VoiceVolume = 0 ;
        if( VoiceVolume>127 )   VoiceVolume = 127 ;

        //---------------------------------------------------------------
        JingleVolume = DefFileBufferReadValueDefault( "MusicVolume", DEF_JINGLE_VOLUME ) ;

        if( JingleVolume>127 )  JingleVolume = 127 ;

        SetVolumeJingle( JingleVolume ) ;

        //---------------------------------------------------------------
#ifdef  CDROM
        CDVolume = DefFileBufferReadValueDefault( "CDVolume", DEF_CD_VOLUME ) ;

        if( CDVolume>127 )      CDVolume = 127 ;

        SetVolumeCD( CDVolume ) ;
#endif

        //---------------------------------------------------------------
        MasterVolume = DefFileBufferReadValueDefault( "MasterVolume", DEF_MASTER_VOLUME ) ;

        if( MasterVolume<0 )    MasterVolume = 0 ;
        if( MasterVolume>127 )  MasterVolume = 127 ;

        SetMasterVolumeSample( MasterVolume ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

void    WriteVolumeSettings()
{
        DefFileBufferWriteValue( "WaveVolume", SampleVolume ) ;
        DefFileBufferWriteValue( "VoiceVolume", VoiceVolume ) ;
        DefFileBufferWriteValue( "MusicVolume", JingleVolume ) ;
#ifdef  CDROM
        DefFileBufferWriteValue( "CDVolume", CDVolume ) ;
#endif
        DefFileBufferWriteValue( "MasterVolume", MasterVolume ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/

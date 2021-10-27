#ifndef	AMBIANCE_H
#define	AMBIANCE_H

extern	S32	RestartRainSample ;

#define	MAX_PLAYING_SAMPLES	32

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
extern	S32	NbPlayingSamples ;
extern	SAMPLE_PLAYING	ListPlayingSamples[MAX_PLAYING_SAMPLES] ;
#endif

/*--------------------------------------------------------------------------*/
extern void StartRainSample( void ) ;
/*--------------------------------------------------------------------------*/
extern HQR_GET_CALLBACK	DelBlocHQRSample ;
/*--------------------------------------------------------------------------*/
extern U32  HQ_MixSample(S32 numsample,S32 frequence,S32 decalage,S32 repeat,S32 pan,S32 volume);
/*--------------------------------------------------------------------------*/
extern U32  HQ_RestartSample( S32 numsample, S32 frequence, S32 repeat, S32 pan, S32 volume ) ;
/*--------------------------------------------------------------------------*/
extern void HQ_StopSample();
/*--------------------------------------------------------------------------*/
#define	HQ_StopOneSample(num)	StopOneSample(num)
/*--------------------------------------------------------------------------*/
extern void HQ_FadeOutSamples( S32 dureehr ) ;
/*--------------------------------------------------------------------------*/
extern void HQ_FadeInSamples( S32 dureehr ) ;
/*--------------------------------------------------------------------------*/
extern void HQ_PauseSamples(void) ;
/*--------------------------------------------------------------------------*/
extern void HQ_ResumeSamples(void) ;
/*--------------------------------------------------------------------------*/
extern S32  GiveBalance(S32 xp,S32 yp,S32 distance,U32 *pan,S32 *vol);
/*--------------------------------------------------------------------------*/
extern U32  HQ_3D_MixSample(S32 numsample,S32 frequence,S32 decalage,S32 repeat,S32 x,S32 y,S32 z);
/*--------------------------------------------------------------------------*/
extern void HQ_ChangePanSample( S32 numsample, S32 volume, U32 pan ) ;
/*--------------------------------------------------------------------------*/
extern void HQ_3D_ChangePanSample( S32 numsample, S32 x, S32 y, S32 z ) ;
/*--------------------------------------------------------------------------*/
extern void LaunchAmbiance( void ) ;
/*--------------------------------------------------------------------------*/
extern void GereAmbiance(void);
/*--------------------------------------------------------------------------*/
extern void ClearAmbiance( void ) ;
/*--------------------------------------------------------------------------*/
extern void InitPalette( void ) ;
/*--------------------------------------------------------------------------*/
extern void ChoicePalette( void ) ;
/*--------------------------------------------------------------------------*/
extern void FadePal(U8 r,U8 v,U8 b,U8 *ptrpal,S32 percent);
/*--------------------------------------------------------------------------*/
extern void FadeToBlack(U8 *ptrpal);
/*--------------------------------------------------------------------------*/
extern void WhiteFade(void);
/*--------------------------------------------------------------------------*/
extern void FadeWhiteToPal(U8 *ptrpal);
/*--------------------------------------------------------------------------*/
extern void FadeToPal(U8 *ptrpal);
/*--------------------------------------------------------------------------*/
extern void FadeToBlackAndSamples( U8 *ptrpal ) ;
/*--------------------------------------------------------------------------*/
extern void FadeToPalAndSamples( U8 *ptrpal ) ;
/*--------------------------------------------------------------------------*/
extern void SetBlackPal(void);
/*--------------------------------------------------------------------------*/
extern void FadePalToPal( U8 *ptrpal, U8 *ptrpal1 ) ;
/*--------------------------------------------------------------------------*/
extern void FadeToPalIndex( U8 num ) ;
/*--------------------------------------------------------------------------*/
extern void PaletteIndex(U8 num);
/*--------------------------------------------------------------------------*/
extern void ReadVolumeSettings(void);
/*--------------------------------------------------------------------------*/
extern void WriteVolumeSettings(void);
/*--------------------------------------------------------------------------*/

#endif	// AMBIANCE_H

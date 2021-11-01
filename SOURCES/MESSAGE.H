#ifndef	MESSAGE_H
#define	MESSAGE_H

//-------------------- Used by HoloGlob.cpp
extern	char    *PtDial         ;
extern	S32    X_Dial, Y_Dial  	;
extern	S32    Dial_X0		;
extern	S32    Dial_Y0		;
extern	S32    Dial_X1		;
extern	S32    Dial_Y1		;

extern	S32	FlagAnimWhoSpeak;

extern	S32	Language 	; // English

#ifdef	CDROM
extern	S32	LanguageCD 	; // Français, a changer of course!
#endif

extern	S32	FlagRestoreCD	;

/*
#ifdef	LBA_EDITOR
extern void InitAnimDial(char *newanimdial);
#else
extern void InitAnimDial(U16 newanimdial);
#endif
*/

extern  S32     FlagDisplayText ;

#if defined(CDROM)||defined(LBA_EDITOR)
extern	void	AddFileNameOnHD( char *filename, U32 size, U32 timer )	;
extern  U32	*BufMemoSeek    ;
extern	void	InitVoiceFile( void )	;
extern	void	ClearVoiceFile( void )	;
extern	S32	Speak( S32 text )	;
extern	S32	TestSpeak()		;
extern	void	StopSpeak()		;
extern	void	CloseFdNar()		;
#else
#define	StopSpeak()
#endif

extern	S32 IsVoiceFileOnHD( S32 island );
extern void InitLanguage(void);
extern S32  FindText(S32 text);
extern void InitDial(S32 file);
extern void ClearDial(void);
extern void GetNextWord(char *ptchaine,char *mot);
extern void AffOneCar(S32 x,S32 y,char c,S32 coul);
extern void AffAllCar(void);
extern void CoulDial(S32 min,S32 max,S32 step);
extern void TestCoulDial(S32 coul);
extern S32  GereFlagDial( S32 text );
extern S32  GetText(S32 text);
extern void AffFleche(void);
//----- incrust vignette Radio
extern void TestAndDrawRadio( void ) ;
extern void InitDialWindow(void);
extern void HoloInitDialWindow(void);
extern void AffHoloDialWindow(void);
extern void NormalWinDial(void);
extern void BigWinDial(void);
extern void HoloWinDial(void) ;
extern void CommonOpenDial(S32 text);
extern void OpenDial(S32 text);
extern void GetNextLine(void);
extern void InitEndPage(void);
extern void NextDialWindow();
extern S32  NextDialCar(void);
extern void CloseDial(void);
extern void InitWhoSpeak(S32 numobj);
extern void ClearWhoSpeak(void);
extern void SpeakAnimation(void);
extern void Dial(S32 text,S32 drawscene);
extern S32  MyDial(S32 nummess);
extern char *GetMultiText(S32 text,char *dst);
extern void CleanMessage(char *string,S32 flag);
extern void LoadListMessages(S32 file);
extern void FreeListMessages(void);
extern S32  NextMessagePerso(S32 numperso,S32 message);
extern S32  PreviousMessagePerso(S32 numperso,S32 message);
extern S32  VerifyMessagePerso(S32 numperso,S32 message);

#endif	// MESSAGE_H

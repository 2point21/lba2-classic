//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
extern	U32	Old_PM15_Off	;
extern	U32	Old_PM15_Sel	;

extern  U32	Int15DataStart	;
extern  U32	Int15DataEnd	;

extern  U32	Int15CodeStart	;
extern  U32	Int15CodeEnd	;

extern  volatile U32	Int15Trans	;

//──────────────────────────────────────────────────────────────────────────
extern	void	__interrupt	Int15Handler(void)	;

#pragma	aux	Int15Handler	"*"

//──────────────────────────────────────────────────────────────────────────
extern	void	keyboard_cmd(U8 cmd)	;

#pragma aux	keyboard_cmd	"*"	\
	parm	[ebx]			\
	modify	[eax ecx]

//──────────────────────────────────────────────────────────────────────────
extern	void	keyboard_write(U8 cmd)	;

#pragma aux	keyboard_write	"*"	\
	parm	[ebx]			\
	modify	[eax ecx]

//──────────────────────────────────────────────────────────────────────────
extern	S32	keyboard_read()	;

#pragma aux	keyboard_read	"*"	\
	modify	[eax ecx]

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────

//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_SYSTEM_HQRRESS
#define LIB_SYSTEM_HQRRESS

//──────────────────────────────────────────────────────────────────────────
#include	<stdlib.h>

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
extern	"C"	{
#endif

//──────────────────────────────────────────────────────────────────────────
#pragma	pack(push, 4)

//──────────────────────────────────────────────────────────────────────────
typedef struct  {
                        S32		MaxSize	;
                        S32		FreeSize;
                        S32		MaxIndex;
                        S32		NbIndex	;
                        void		*Buffer	;
                        char		Name[_MAX_PATH];
		}	T_HQR_HEADER 		;

//──────────────────────────────────────────────────────────────────────────
#pragma	pack(pop)

//──────────────────────────────────────────────────────────────────────────
typedef	void	(HQR_GET_CALLBACK)(char *hqrname, S32 index);

//──────────────────────────────────────────────────────────────────────────
extern	HQR_GET_CALLBACK	*HQRGetErrorFunc;

#pragma	aux	HqrGetErrorFunc	"*"

//──────────────────────────────────────────────────────────────────────────
extern	HQR_GET_CALLBACK	*HQRGetDelFunc	;

#pragma	aux	HqrGetDelFunc	"*"

//──────────────────────────────────────────────────────────────────────────
extern	S32	HQR_Flag ;

//──────────────────────────────────────────────────────────────────────────
extern	T_HQR_HEADER	*HQR_Init_Ressource(	char	*hqrname,
 						S32	maxsize,
						S32	maxrsrc		);

//──────────────────────────────────────────────────────────────────────────
#ifdef	_WIN32
#define			HQR_Lock_Ressource(header)

#else //_WIN32
extern	void		HQR_Lock_Ressource(	T_HQR_HEADER *header	);

#endif//_WIN32
//──────────────────────────────────────────────────────────────────────────
extern	void		HQR_Reset_Ressource(	T_HQR_HEADER *header	);

//──────────────────────────────────────────────────────────────────────────
extern	S32		HQR_Change_Ressource(	T_HQR_HEADER *header,
						char *newhqrname	);

//──────────────────────────────────────────────────────────────────────────
extern	void		HQR_Free_Ressource(	T_HQR_HEADER *header	);

//──────────────────────────────────────────────────────────────────────────
extern	void		*HQR_Get(		T_HQR_HEADER *header,
						S32 index		);

//──────────────────────────────────────────────────────────────────────────
#ifdef	__cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_SYSTEM_HQRRESS

//──────────────────────────────────────────────────────────────────────────

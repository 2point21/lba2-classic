//──────────────────────────────────────────────────────────────────────────
#ifndef	LIB_MENU_MENUFUNC
#define	LIB_MENU_MENUFUNC

//──────────────────────────────────────────────────────────────────────────
#ifdef __cplusplus
extern "C" {
#endif

//──────────────────────────────────────────────────────────────────────────
extern void 		Message(char *message, S32 flag			);
extern S32 		Confirm(char *message, char *yes, char *no	);

//──────────────────────────────────────────────────────────────────────────
extern void 		InputString(		T_MENU *ptrmenu, S32 handle,
						S32 maxchar		);

//──────────────────────────────────────────────────────────────────────────
extern void 		SwitchButton(		T_MENU *ptrmenu, S32 handle);

extern void 		ChangeButtonFlags(	T_MENU *ptrmenu, S32 handle,
						S32 flags,	 S32 flagaff);

extern void 		ChangeButtonString(	T_MENU *ptrmenu, S32 handle,
						char *string,	 S32 flagaff);

extern void 		ChangeButtonValueLimit( T_MENU *ptrmenu, S32 handle,
						S32 valmin, 	 S32 valmax );

//──────────────────────────────────────────────────────────────────────────
extern	S32		GetButtonFlags(		T_MENU *ptrmenu, S32 handle );

extern	char*		GetButtonString(	T_MENU *ptrmenu, S32 handle );

//──────────────────────────────────────────────────────────────────────────
extern T_CLICK_BOX 	*GetPtrLcb(		T_MENU *ptrmenu,S32 handle  );

//──────────────────────────────────────────────────────────────────────────
#ifdef __cplusplus
}
#endif

//──────────────────────────────────────────────────────────────────────────
#endif//LIB_MENU_MENUFUNC

//──────────────────────────────────────────────────────────────────────────

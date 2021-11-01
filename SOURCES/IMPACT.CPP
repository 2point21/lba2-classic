#include	"c_extern.h"

//
// Uniquement en version Outil: interprétation des impacts
//
// ATTENTION: Utilise ScreenAux pour charger les impacts
//

#ifdef	COMPILATOR
#define	PATH_FLOW	"f:\\projet\\lba2\\datas\\flow\\"
extern	U8	*BufferDef ;
#endif

#ifdef	LBA_EDITOR
U32		NbImpacts = 0 ;
T_IMPACT	ListImpact[MAX_IMPACT] ;

// ATTENTION: Laisser imperativement les instructions FLOW et THROW avant les
// autres FLOW_xxx et THROW_xxx, sinon le principe ne marche plus à cause du
// strnicmp(). (Voir GetImpCommand()).

char	*ImpCommandList[NB_IMP_COMMAND] = {
		"END",				// 0
		";",				// 1
		"SAMPLE",			// 2
		"FLOW",				// 3
		"THROW",			// 4
		"THROW_POF",			// 5
		"THROW_OBJ",			// 6
		"SPRITE",			// 7
		"FLOW_SPRITE",			// 8
		"FLOW_OBJ",			// 9
		"FLOW_POF"   			// 10
		} ;
#else
//T_HQR_HEADER	*HQR_Impact ;
U8	*BufferImpact ;
#endif

#ifdef	LBA_EDITOR
/*──────────────────────────────────────────────────────────────────────────*/
S8	GetImpCommand( void )
{
	char	*ptr ;
	char	c ;
	S8	i ;

	ptr = String ;

	do	/* saute cr/lf espace */
	{
		c = *(char *)PtrFiche++ ;
		if( c == 26 /*EOF*/ )
		{
			PtrFiche-- ;
			return -1 ;
		}
	}
	while( c <= 32 ) ;

	PtrCommand = PtrFiche-1 ;

	if( c == ';' )	return IMP_REM ;	// accelere et evite l'espace obligatoire

	do	/* lit commande */
	{
		*ptr++ = c ;
		c = *(char *)PtrFiche++ ;
	}
	while( c > 32 ) ;
	if( c==26 )	PtrFiche-- ;

	*ptr++ = 0 ;

	for( i=0; i<NB_IMP_COMMAND; i++ )
	{
		if( !strnicmp(	ImpCommandList[i],
				String,
				strlen( String ) ) )
		{
			return i ;
		}
	}

	Beep() ;
	MessageF( TRUE, "ImpCommande inconnue: %s", String ) ;

	return (S8)IMP_NOP ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	GetNumDef( char *pathdef, char *name )
{
	S32	n = 0 ;
	S32	len ;
	char	*ptr ;
	char	*com ;	// Pointeur sur commentaire éventuel

	strupr( name ) ;
	len = strlen( name ) ;

	InitDefFile( pathdef ) ;

	do
	{
		ptr = DefFileBufferReadString( Itoa(n) ) ;

		if( ptr )
		{
			// Vire commentaire éventuel
			com = (char *) strchr( ptr, ';' ) ;
			if( com )	*com = '\0' ;

			strupr( ptr ) ;

			if( !strncmp(ptr,name,len) )	return n ;

			n++ ;
		}
	}
	while( ptr ) ;

	MessageF( TRUE, "Erreur GetNumDef(): %s introuvable", name ) ;

	return -1 ;
}

/*──────────────────────────────────────────────────────────────────────────*/
S32	GetNumDefAddIfNot( char *pathdef, char *name, char *rem )
{
	char	ch[256] ;
	S32	n = 0 ;
	S32	len ;
	char	*ptr ;
	S32	found = FALSE ;

	strupr( name ) ;
	len = strlen( name ) ;

	InitDefFile( pathdef ) ;

	strupr( OrgPtrDef ) ;

	ptr = OrgPtrDef ;

	while( ptr AND ptr<EndPtrDef )
	{
		ptr = strstr( ptr, name ) ;

		if( ptr AND ptr[len]==' ' )	break ;
	}

	if( ptr )	found = TRUE ;
	else		ptr = EndPtrDef ;	// on cherche le dernier numero

	// cherche numero

	while( ptr>=OrgPtrDef AND *ptr!=':' )	ptr-- ;

	if( ptr<OrgPtrDef )	n = 0 ;
	else
	{
		ptr-- ;
		while( ptr>=OrgPtrDef AND (*ptr>='0' AND *ptr<='9') ) ptr-- ;
		if( ptr<OrgPtrDef )	n = 0 ;
		else
		{
			ptr++ ;
			n = atoi(ptr) ;
		}
	}

	if( found )
	{
		return n ;
	}

	n++ ;

	// rajoute en fin de liste
	strcpy( ch, name ) ;
	strcat( ch, " ; " ) ;
	strcat( ch, rem  ) ;	// ajoute une remarque éventuelle

	DefFileBufferWriteString( Itoa(n), ch ) ;

#ifndef	COMPILATOR
	SampleDefModified = TRUE ;
#endif

	return n ;
}

/*──────────────────────────────────────────────────────────────────────────*/
// Entrée: impact en mode texte
// Sortie: impact en mode compilé dans le même buffer
S32	CompileImpact( U8 *buffer )
{
	U8	*ptr = buffer ;
	S8	command ;
	S32	nbentities = 0 ;

	// Traduction et compilation de l'impact
	PtrFiche = buffer ;

	while( (command = GetImpCommand()) != IMP_NOP )
	{
		if( nbentities>=MAX_IMPACT_ENTITIES )
		{
			Beep() ;
			MessageF( TRUE, "Erreur: MAX_IMPACT_ENTITIES(%1d) atteint !", MAX_IMPACT_ENTITIES ) ;
			break ;
		}

		*ptr++ = command ;

		switch( command )
		{
			// SAMPLE nom[.wav] frequence decalage volume
			case IMP_SAMPLE:
				*(S16*)ptr = (S16)GetValue() ;	// Num Sample
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Frequence
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Decalage
				ptr+=2 ;
				*ptr++ = (U8)GetValue() ;       // Volume
				nbentities++ ;
				break ;

			// FLOW nom[.flw]
			case IMP_FLOW:
				*ptr++ = (U8)GetValue() ;	// Num Flow
				nbentities++ ;
				break ;

			// THROW sprite alpha beta speed poids force
			case IMP_THROW:
				*(S16*)ptr = (S16)GetValue() ;	// Sprite
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// alpha
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// beta
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Speed
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Poids
				ptr+=2 ;
				*ptr++ = (U8)GetValue() ;	// Force
				nbentities++ ;
				break ;

			// THROW_OBJ nom[.O4D] alpha beta speed poids force
			case IMP_THROW_OBJ:
				GetFicName() ;			// Body
#ifdef	COMPILATOR
				*(S16*)ptr = SearchIndexBodyFix( FicName, "THROW" ) ;
				ptr += 2 ;
#else
				strcpy( (char*)ptr, FicName ) ;
				ptr+=strlen(FicName) ;
				*ptr++ = '\0' ;
#endif
				*(S16*)ptr = (S16)GetValue() ;	// alpha
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// beta
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Speed
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Poids
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Rotation
				ptr+=2 ;

				*ptr++ = (U8)GetValue() ;	// Force
				nbentities++ ;
				break ;

			// THROW_POF nom[.pof] alpha beta speed poids force
			case IMP_THROW_POF:
				*(S16*)ptr = (S16)GetValue() ;	// Num Pof
				ptr+=2 ;

				*(S16*)ptr = (S16)GetValue() ;	// alpha
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// beta
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Speed
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Poids
				ptr+=2 ;
				*(S32*)ptr = GetValue() ;	// Scale
				ptr+=4 ;
				nbentities++ ;
				break ;

			// SPRITE deb fin tempo scale transparent
			case IMP_SPRITE:
				*(S16*)ptr = (S16)GetValue() ;	// Deb
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Fin
				ptr+=2 ;
				*(S32*)ptr = GetValue() ;	// tempo
				ptr+=4 ;
				*(S32*)ptr = GetValue() ;	// scale
				ptr+=4 ;
				*ptr++ = (U8)GetValue() ;	// transparent on/off
				*ptr++ = (U8)GetValue() ;	// Force
				nbentities++ ;
				break ;

			// FLOW_SPRITE nom[.flow] deb fin tempo scale transparent force
			case IMP_FLOW_SPRITE:
				*ptr++ = (U8)GetValue() ;	// Num Flow
				*(S16*)ptr = (S16)GetValue() ;	// Deb
				ptr+=2 ;
				*(S16*)ptr = (S16)GetValue() ;	// Fin
				ptr+=2 ;
				*(S32*)ptr = GetValue() ;	// tempo
				ptr+=4 ;
				*(S32*)ptr = GetValue() ;	// scale
				ptr+=4 ;
				*ptr++ = (U8)GetValue() ;	// transparent on/off
				*ptr++ = (U8)GetValue() ;	// Force
				nbentities++ ;
				break ;

// ATTENTION: different ici pour la version compil

			// FLOW_OBJ nom[.flw] nom[.O4D] Force
			case IMP_FLOW_OBJ:
				*ptr++ = (U8)GetValue() ;	// Num Flow
				GetFicName() ;			// Body
#ifdef	COMPILATOR
				*(S16*)ptr = SearchIndexBodyFix( FicName, "THROW" ) ;
				ptr += 2 ;
#else
				strcpy( (char*)ptr, FicName ) ;
				ptr+=strlen(FicName) ;
				*ptr++ = '\0' ;
#endif
				*ptr++ = (U8)GetValue() ;	// Force
				nbentities++ ;
				break ;

			// FLOW_POF nom[.flw] nom[.pof] scale_deb scale_fin tempo speedrot
			case IMP_FLOW_POF:
				*ptr++ = (U8)GetValue() ;	// Num Flow
				*(S16*)ptr = (S16)GetValue() ;	// Num Pof
				ptr+=2 ;

				*(S32*)ptr = GetValue() ;	// scale_deb
				ptr+=4 ;
				*(S32*)ptr = GetValue() ;	// scale_fin
				ptr+=4 ;
				*(S32*)ptr = GetValue() ;	// tempo scale
				ptr+=4 ;
				*(S16*)ptr = (S16)GetValue() ;	// SpeedRot
				ptr+=2 ;
				nbentities++ ;
				break ;

			default:
				ptr-- ;
				NextLine() ;
				break ;
		}
	}

	*ptr++ = IMP_END ;

	// retourne taille compilée
	return( ptr-buffer ) ;
}

/*──────────────────────────────────────────────────────────────────────────*/
U8	*SearchImpact( char *name )
{
	char path[ADELINE_MAX_PATH] ;
	S32		n, size ;
	T_IMPACT	*ptri = ListImpact ;

	for( n=0; n<NbImpacts; n++, ptri++ )
	{
		if( !stricmp( name, ptri->name ) )
			return ptri->buf ;
	}

	if( NbImpacts>=MAX_IMPACT )
	{
		Beep() ;
		MessageF( TRUE, "Erreur: MAX_IMPACT(%1d) atteint !", MAX_IMPACT ) ;
	}

	// New Impact
	strcpy( ptri->name, name ) ;

	strcpy( path, PATH_IMPACT ) ;
	strcat( path, name ) ;
	AddExt( path, ".IMP" ) ;

	size = FileSize( path ) 		;
	ptri->buf = (U8 *)Malloc(size+1)	;/* Pas De Mshrink */

	if( !ptri->buf )	return NULL ;

	if( !Load( path, ptri->buf ) )	return NULL ;

	ptri->buf[size] = 26 ;

	if( CompileImpact( ptri->buf ) )
	{
		NbImpacts++ ;
		return ptri->buf ;
	}

	return NULL ;
}
#endif	// LBA_EDITOR

/*──────────────────────────────────────────────────────────────────────────*/
#ifndef	COMPILATOR
#ifdef	LBA_EDITOR
void	DoImpact( char *name, S32 x, S32 y, S32 z, U8 owner )
#else
void   	DoImpact( S32 num, S32 x, S32 y, S32 z, U8 owner )
#endif
{
	S32	wlong, wlong2, wlong3 ;
	S32	beta = 0;
	S32	extra ;
	U8	*ptr	;
	S16	wword, wword2, wword3 ;
	S8	command ;
	U8	wbyte, wbyte2	;

#ifdef	LBA_EDITOR
	SaveTimer() ;

	ptr = SearchImpact( name ) ;

	RestoreTimer() ;
#else
	ptr = BufferImpact + *(((U32*)BufferImpact)+num) ;
#endif

	if( ptr )
	{
		// Exécution de l'impact

		if( owner!=255 )
		{
			beta = ListObjet[owner].Obj.Beta ;
		}

		while( (command = *ptr++) != IMP_END )
		{
			switch( command )
			{
				// SAMPLE nom[.wav] volume
				case IMP_SAMPLE:
					wword  = *(S16*)ptr ;	// NumSample
					ptr+=2 ;
					wword2 = *(S16*)ptr ;	// Frequence
					ptr+=2 ;
					wlong  = *(S16*)ptr ;	// Decalage
					ptr+=2 ;
					ParmSampleVolume = *ptr++ ;	// Volume

#ifdef	LBA_EDITOR
					HQ_3D_MixSample( COMMON_NUM_DEF, wword, wword2, (S16)wlong, 1, x, y, z ) ;
#else
					HQ_3D_MixSample( wword, wword2, (S16)wlong, 1, x, y, z ) ;
#endif
					break ;

				// FLOW nom[.flw]
				case IMP_FLOW:
					wbyte = *ptr++ ;	// num flow
					CreateParticleFlow( 0, owner, 0, x, y, z, beta, wbyte ) ;
					break ;

				// THROW sprite alpha beta speed poids force
				case IMP_THROW:
					wword = *(S16*)ptr ;	// Sprite
					ptr+=2 ;
					wlong = *(S16*)ptr ;	// alpha
					ptr+=2 ;
					wlong2 = *(S16*)ptr ;	// beta
					ptr+=2 ;
					wlong3 = *(S16*)ptr ;	// Speed
					ptr+=2 ;
					wword2 = *(S16*)ptr ;	// Poids
					ptr+=2 ;
					wbyte = *ptr++ ;	// Force

					wlong2 = (wlong2+beta)&4095 ;

					extra = ThrowExtra( owner,
						x, y, z,
						wword,
						wlong, wlong2, wlong3, (U8)wword2,
						wbyte ) ;

#ifdef	LBA_EDITOR
					if( extra!=-1 AND !Shadow )
#else
					if( extra!=-1 AND Shadow<3 )
#endif
					{
						ListExtra[extra].Flags |= EXTRA_NO_SHADOW ;
					}
					break ;

				// THROW_OBJ nom[.O4D] alpha beta speed poids force
				case IMP_THROW_OBJ:
#ifdef	LBA_EDITOR
					strcpy( FicName, (char*)ptr ) ;
					ptr+=strlen(FicName) ;
					ptr++ ;			// Skip '\0'
					wword = 0 ;
#else
					wword = *(S16*)ptr ;	// num body
					ptr+=2 ;
#endif
					wlong = *(S16*)ptr ;	// alpha
					ptr+=2 ;
					wlong2 = *(S16*)ptr ;	// beta
					ptr+=2 ;
					wlong3 = *(S16*)ptr ;	// Speed
					ptr+=2 ;
					wword2 = *(S16*)ptr ;	// Poids
					ptr+=2 ;
					wword3 = *(S16*)ptr ;	// Rotation
					ptr+=2 ;

					wlong2 = (wlong2+beta)&4095 ;

					wbyte = *ptr++ ;	// Force

					extra = ThrowExtraObj( 	owner,
						x, y, z,
#ifdef	LBA_EDITOR
						FicName,
#else
						wword,
#endif
						wlong, wlong2, wlong3,
						wword3, // alpharot
						(U8)wword2, wbyte ) ;

#ifdef	LBA_EDITOR
					if( extra!=-1 AND !Shadow )
#else
					if( extra!=-1 AND Shadow<2 )
#endif
					{
						ListExtra[extra].Flags |= EXTRA_NO_SHADOW ;
					}
					break ;

				// THROW_POF nom[.pof] alpha beta speed poids scale
				case IMP_THROW_POF:
					wword = *(S16*)ptr ;	// Num Pof
					ptr+=2 ;

					wlong  = *(S16*)ptr ;	// alpha
					ptr+=2 ;
					wlong2 = *(S16*)ptr ;	// beta
					ptr+=2 ;
					wlong3 = *(S16*)ptr ;	// Speed
					ptr+=2 ;
					wword2 = *(S16*)ptr ;	// Poids
					ptr+=2 ;

					wlong2 = (wlong2+beta)&4095 ;

					InitExtraPof( x, y, z, wword,
						      wlong, wlong2,
						      wlong3, wword2,
						      *(S32*)ptr, *(S32*)ptr, 0, // Scale
						      0, 0 ) ;

					ptr+=4 ;
					break ;

				// SPRITE deb fin tempo scale transparent
				case IMP_SPRITE:
					wword  = *(S16*)ptr ;	// Deb
					ptr+=2 ;
					wword2 = *(S16*)ptr ;	// Fin
					ptr+=2 ;
					wlong  = *(S32*)ptr ;	// tempo
					ptr+=4 ;
					wlong2 = *(S32*)ptr ;	// scale
					ptr+=4 ;
					wbyte  = *ptr++ ;	// transparent
					wbyte2  = *ptr++ ;	// force

					InitExtraAnimSprite( owner, x, y, z,
							     wword, wword2,
							     wlong, wlong2,
							     wbyte, wbyte2 ) ;
					break ;

				// FLOW_SPRITE nom[.flow] deb fin tempo scale transparent force
				case IMP_FLOW_SPRITE:
					wbyte = *ptr++ ; 	// num flow
					wword = *(S16*)ptr ;	// deb
					ptr+=2 ;
					wword2 = *(S16*)ptr ;	// fin
					ptr+=2 ;
					wlong3 = *(S32*)ptr ;	// tempo
					ptr+=4 ;
					wlong2 = *(S32*)ptr ;	// scale
					ptr+=4 ;
					wbyte2  = *ptr++ ;	// transparent
					wlong = *ptr++ ;	// force

					CreateExtraParticleFlow( FLOW_EXTRA_SPRITE,
							    owner, // owner = tous
							    wword, wword2,
							    x, y, z, 0, wbyte,
							    wlong, wlong2, wbyte2,
							    wlong3 ) ;
					break ;

				// FLOW_OBJ nom[.flw] nom[.O4D] Force
				case IMP_FLOW_OBJ:
					wbyte = *ptr++ ; 	// num flow
#ifdef	LBA_EDITOR
					strcpy( FicName, (char*)ptr ) ;
					ptr+=strlen(FicName) ;
					ptr++ ;			// Skip '\0'
					wword = 0 ;
#else
					wword = *(S16*)ptr ;	// num body
					ptr+=2 ;
#endif
					wlong = *ptr++ ;	// force

					CreateExtraParticleFlow( FLOW_EXTRA_OBJ,
							    owner, // owner = tous
							    wword, 0,
							    x, y, z, beta, wbyte,
							    wlong, 0, 0, 0 ) ;
					break ;

				// FLOW_POF nom[.flw] nom[.pof] scale_deb scale_fin tempo speedrot
				case IMP_FLOW_POF:
					wbyte = *ptr++ ;	// Num Flow

					wword = *(S16*)ptr ;	// Num Pof
					ptr+=2 ;

					wlong  = *(S32*)ptr ;	// scale_deb
					ptr+=4 ;
					wlong2 = *(S32*)ptr ;	// scale_fin
					ptr+=4 ;
					wlong3 = *(S32*)ptr ;	// tempo
					ptr+=4 ;
					wword2 = *(S16*)ptr ;	// SpeedRot
					ptr+=2 ;

					CreateExtraParticleFlow( FLOW_EXTRA_POF,
							    owner, // owner = tous
							    wword, wword2, // speedrot dans num2
							    x, y, z, beta, wbyte,
							    0, wlong, wlong2, // scale_fin dans transparent
							    wlong3 ) ;
					break ;
			}
		}
	}
}
#endif	// COMPILATOR

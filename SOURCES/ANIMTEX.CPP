#include        "c_extern.h"

/*══════════════════════════════════════════════════════════════════════════*/
#define DEF_EFFECT              1
#define MAX_TEX_EFFECTS 	10

/*══════════════════════════════════════════════════════════════════════════*/
typedef struct
	{
	        S32     Type ;
	        void    *PtrStruct ;
        }       T_TEX_EFFECT ;

S32     	NbTexEffects = 0 ;
static	T_TEX_EFFECT    TexEffectList[MAX_TEX_EFFECTS] ;
static	S32     	LastAnimationTimer ;

/*══════════════════════════════════════════════════════════════════════════*/
// plasma struct

typedef struct  {

        S16     *TabVirgule ;
        S16     *TabSpeed   ;
        S32     *TabAcc     ;
        U8      *TabColors  ;

        U8      *TexOffset  ;           // devenu ptr sur effet dans tex

        U8      Interleave ;
        U8      NbActivePoints ;
        U8      NbColors ;
        U8      Speed ;

        U8      data_start ;

        }       T_PLASMA ;

/*══════════════════════════════════════════════════════════════════════════*/

void    ScanTextureAnimation( U8 *ptrtex, S32 nbscanlines )
{
	S32	x ;

	nbscanlines*=256	;
        for(x=0; x<nbscanlines; x++, ptrtex++ )
        {
                if( ptrtex[0] == DEF_EFFECT )
                {
		        U8      *ptr	;
		        S32     width	;
		        S32     effectnum;
			S32	x1, x2	;
			S32	n	;

                        // effet trouvé
                        effectnum = ptrtex[1+2*256] ;

                        TexEffectList[NbTexEffects].Type = effectnum ;

                        // width scan
                        ptr = &ptrtex[1+256]	;
                        x1 = x2 = x&0xFF	;
                        while( *ptr++ == 0 )
                        {
                                x2++ ;
                                if( x2 == 256 )  break ;
                        }
                        width = 2 + x2 - x1 ;

                        if(width & (width-1))
                        {
                                // erreur taille texture animee pas exposant de 2
                                return ;
                        }


                        switch( effectnum )
                        {
//------------------------------------------------------------------
                        case 0: // plasma
                        {
			        T_PLASMA	*PtrPlasma ;
			        U8      	*ptrcol ;
			        U8      	*ptrc ; // tabcouls
			        S16     	*ptrv ; // tabvirgule
			        S16     	*ptrs ; // tabspeed
			        S32     	*ptra ; // tabacc
			        S32     	speed ;
			        S32     	nbactivepoints ;
			        S32     	interleave ;
			        S32     	delta ;
			        S32     	nbcols ;
			        S32		os, ov ;

	                        // lecture des parametres

	                        // nb colors
	                        nbcols = 0 ;
	                        ptrcol = &ptrtex[1+3*256] ;
	                        while( ptrcol[nbcols] != DEF_EFFECT )	nbcols++ ;

	                        nbactivepoints = ptrtex[2+2*256] ;
	                        if( !nbactivepoints )   nbactivepoints = 4 ;

	                        speed = ptrtex[3+2*256] ;
	                        if( !speed )    speed = 5 ;

	                        delta = width/nbactivepoints ;
	                        interleave = 1 ;
	                        while( !((delta>>=1)&1)  )  interleave++ ;
	                        delta = 1<<interleave	;

	                        // init structures
	                        PtrPlasma = (T_PLASMA*)Malloc( sizeof( T_PLASMA )
	                        			       + (nbactivepoints*nbactivepoints*delta*2)
	                        			       + (nbactivepoints*nbactivepoints*2)
	                        			       + (nbactivepoints*nbactivepoints*4)
	                        			       + nbcols
	                        			     );



	                        if( !PtrPlasma )  return ;

	                        TexEffectList[NbTexEffects].PtrStruct = (U8*)PtrPlasma ;

	                	PtrPlasma->TexOffset = ptrtex	;
	                        PtrPlasma->Interleave = (U8)interleave ;
	                        PtrPlasma->NbActivePoints = (U8)nbactivepoints ;
	                        PtrPlasma->NbColors = (U8)nbcols ;
	                        PtrPlasma->Speed = 0 ;

	                        ptrv = (S16*) &PtrPlasma->data_start ;
	                        ptrs = ptrv + (nbactivepoints*nbactivepoints*delta) ;
	                        ptra = (S32*)( ptrs + (nbactivepoints*nbactivepoints) ) ;
	                        ptrc = (U8*)( ptra + (nbactivepoints*nbactivepoints) ) ;

	                        PtrPlasma->TabVirgule= ptrv ;
	                        PtrPlasma->TabSpeed  = ptrs ;
	                        PtrPlasma->TabAcc    = ptra ;
	                        PtrPlasma->TabColors = ptrc ;

	                        // generation des sommets aleatoires
	                        nbactivepoints *= nbactivepoints;

	                        for(os=0, ov=0; os<nbactivepoints; os++, ov+=delta)
	                        {
					S32	va ;

	                                // generation des sommets aleatoires
	                                ptrv[ov] = (S16)MyRnd( (nbcols-1)*256 ) ;
	                                // generation des vitesses aleatoires
	                                va = (S16)(speed*512 + MyRnd( speed*512 )) ;
	                                if( Rnd( 2 ) ) va = -va ;

	                                ptrs[os] = (S16)va    ;
	                                ptra[os] = 500        ;
	                        }

	                        // color table
	                        ptrcol = &ptrtex[1+3*256] ;
	                        for(os=0; os<nbcols; os++ )
	                        {
	                                ptrc[os] = ptrcol[os] ;
	                        }
			}
                        break ;
//------------------------------------------------------------------
                        case 1: // fire
			{
				U8	*PtrFire	;
			        U8      *ptrcol 	;

	                        // init structures
	                        PtrFire = (U8*)Malloc( + 4     // tex ptr
	                        		       + 32*36
	                        		       + 32*36
	                        		       + 32
	                        		     );


	                        if( !PtrFire )  return ;

	                        TexEffectList[NbTexEffects].PtrStruct = PtrFire ;

	                        // nb colors = 16
	                        ptrcol = &ptrtex[1+3*256] ;

	                        *((U8**)PtrFire) = ptrtex ;

	                        // color table
	                        for(n=0; n<16; n++ )
	                        {
	                                PtrFire[4+32*36*2+n] = 0 ;
	                        }
	                        for(n=0; n<16; n++ )
	                        {
	                                PtrFire[4+32*36*2+n+16] = *ptrcol++ ;
	                        }
			}
                        break ;
//------------------------------------------------------------------
                        }

                        // clear box effect (connue ou pas)
			ptr = ptrtex	;
                        for(n=0; n<width; n++, ptr+=256 )
                        {
                                memset( ptr, 0, width ) ;
                        }

                        NbTexEffects++ ;
                }
        }
}

/*══════════════════════════════════════════════════════════════════════════*/

// boucle pour initialiser les plasmas,fire ...

void    InitTextureAnimation( void )
{
        ManageTime() ;
        LastAnimationTimer = TimerSystemHR ;// init 1st boucle DoTextureAnimation()

        DoTextureAnimation() ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void    DoTextureAnimation()
{
        S32     	n ;
        S32     	deltatimer, timer ;

	ManageTime() ;

        timer = TimerSystemHR ;
        deltatimer = timer - LastAnimationTimer ;

        if(deltatimer<20)	return	;

        LastAnimationTimer = timer ;

        for( n=0; n<NbTexEffects; n++ )
        {
                switch( TexEffectList[n].Type )
                {
//--------------------------------------------------------------------
                case 0: // plasma
                {
		        S16     	*ptrs, *ptrv ;
		        S32     	*ptra ;
		        T_PLASMA        *ptreffect ;
		        S32     	nbactivepoints, delta ;
		        S32     	nbcols ;
                        S32		ov, os ;

                        ptreffect = (T_PLASMA*)TexEffectList[n].PtrStruct ;

                        ptrv = ptreffect->TabVirgule ;
                        ptrs = ptreffect->TabSpeed ;
                        ptra = ptreffect->TabAcc ;

                        delta = 1<<ptreffect->Interleave ;
                        nbactivepoints = ptreffect->NbActivePoints ;
                        nbactivepoints *= nbactivepoints;
                        nbcols = ptreffect->NbColors<<8 ;

                        // mouvements des sommets du plasma
                        for(os=0, ov=0; os<nbactivepoints; os++, ov+=delta )
                        {
                                S32	sv	;

                                ptra[os] += ptrs[os] * deltatimer;

                                sv = GetDeltaAccMove(&ptra[os])  ;

                                if(!sv)	continue ;

                                sv += ptrv[ov] 	;

                                if( sv <= 0 )
                                {
                                        sv = 0 ;
                                        ptrs[os] = (S16)-ptrs[os];
                                }
                                else if( sv >= nbcols )
                                {
                                        sv = nbcols - 1 ;
                                        ptrs[os] = (S16)-ptrs[os];
                                }

                                ptrv[ov] = (S16)sv ;
                        }

                        Do_Plasma( (U8*)TexEffectList[n].PtrStruct ) ;
		}
		break ;
//--------------------------------------------------------------------
                case 1: // fire
                {
                	U8	*ptrb	;

                        ptrb = (U8*)TexEffectList[n].PtrStruct ;
                        Do_Fire( ptrb+4, ptrb+32*36+4, ptrb+32*36*2+4, *(U8**)ptrb ) ;
                }
		break ;
//--------------------------------------------------------------------
                }
        }
}

/*══════════════════════════════════════════════════════════════════════════*/
//
//      ATTENTION: Routines dédiées aux menus de LBA2
//
/*══════════════════════════════════════════════════════════════════════════*/
S32     PlasmaMenuIndex ;

void    InitPlasmaMenu( void )
{
        S32     n ;
        U8      *ptr ;
        U8      *texdest ;
        U8      *ptrorg ;

	if( !TexEffectList[NbTexEffects].PtrStruct )
        {
                ptrorg = (U8*)(SkySeaTexture+256*(128+64)) ;

                texdest = ptrorg ;

		memset( ptrorg, 0, 256 ) ;

		////////////////////////////////////////////////////////

                memset( texdest, 1, 64 ) ;
                texdest += 256 ;
                memset( texdest, 0, 64 ) ;
                texdest[0] = 1 ;
                texdest[63] = 1 ;
                texdest += 256 ;
                texdest[0] = 1 ;
                texdest[1] = 0 ;
                texdest[2] = 4 ;        // nb control point
                texdest[3] = 2 ;        // speed
                ptr = texdest + 256 ;
                *ptr++ = 1 ;
                for( n=10; n>=0; n-- )  *ptr++ = (U8)(n+12*16) ;
                for( n=0; n<2; n++ )    *ptr++ = 0 ;
                *ptr++ = 1 ;
/*
                memset( texdest, 1, 32 ) ;
                texdest += 256 ;
                memset( texdest, 0, 32 ) ;
                texdest[0] = 1 ;
                texdest[31] = 1 ;
                texdest += 256 ;
                texdest[0] = 1 ;
                texdest[1] = 0 ;
                texdest[2] = 4 ;        // nb control point
                texdest[3] = 1 ;        // speed
                ptr = texdest + 256 ;
                *ptr++ = 1 ;
                for( n=0; n<6; n++ )    *ptr++ = (U8)(n+12*16) ;
                *ptr++ = 1 ;
*/

                PlasmaMenuIndex = NbTexEffects ;

                ScanTextureAnimation( ptrorg, 1 ) ;
                InitTextureAnimation() ;
        }
        else
        {
                NbTexEffects = PlasmaMenuIndex+1 ;
        }
}

/*══════════════════════════════════════════════════════════════════════════*/

void    SelectPlasmaBank( U8 bank )
{
        S32     n ;
        U8      *ptr ;

        ptr = ((T_PLASMA *)TexEffectList[PlasmaMenuIndex].PtrStruct)->TabColors ;

        for( n=10; n>=0; n-- )  *ptr++ = (U8)(n+bank*16) ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void    ClearPlasmaMenu( void )
{
        NbTexEffects = PlasmaMenuIndex ;
}

/*══════════════════════════════════════════════════════════════════════════*/

void    FixePlasmaPtrMap( void )
{
        PtrMap = ((T_PLASMA *)TexEffectList[PlasmaMenuIndex].PtrStruct)->TexOffset ;
}

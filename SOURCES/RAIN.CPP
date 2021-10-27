#include 	"c_extern.h"

//----------------------------------------------------------------------------
#define	RAIN_VX		200
#define RAIN_VY		2500
#define RAIN_WEIGHT	30

//----------------------------------------------------------------------------
#define	RAIN_STOP	0

#define	RAIN_DELTA_X	128
#define	RAIN_DELTA_Y	256
#define	RAIN_DELTA_Z	128

T_RAIN	TabRain[MAX_RAIN]	;

S32	LastTimer=0		;
S32	DeltaRain=0		;

//----------------------------------------------------------------------------
void	InitOneRain( T_RAIN *pt )
{
	S32	rndy	;

	rndy = MyRnd(VueOffsetY + 10000) ;

	pt->YRain = VueOffsetY + rndy;

	rndy = rndy/2 + 15000 		;

	pt->XRain = VueOffsetX - rndy + MyRnd( 30000 ) ;
	pt->ZRain = VueOffsetZ - rndy + MyRnd( 30000 ) ;
	pt->Timer = 0 ;
}

//----------------------------------------------------------------------------
#define	RestartOneRain	InitOneRain

/*
void	RestartOneRain( T_RAIN *pt )
{
	pt->YRain = VueOffsetY + 10000;
	pt->XRain = VueOffsetX - 10000/2 - 15000 + MyRnd( 30000 ) ;
	pt->ZRain = VueOffsetZ - 10000/2 - 15000 + MyRnd( 30000 ) ;
	pt->Timer = 0 ;
}
*/
//----------------------------------------------------------------------------
// faire init special entre chaque changement de camera qui clear tous les
// impact et qui init les goutes a n'importe quel Y
void	InitRain()
{
	S32	i	;

	for ( i = 0; i < MAX_RAIN; i++)
	{
		InitOneRain( &TabRain[i] )	;
	}

	LastTimer = 0 ;
}

//----------------------------------------------------------------------------
void	ClearImpactRain()
{
	S32	i ;

	for ( i = 0 ; i < MAX_RAIN ; i++)
	{
		if( TabRain[i].Timer )	// eclat de gouttes
		{
			InitOneRain(&TabRain[i])	;
		}
	}
}

//----------------------------------------------------------------------------
void	GereRain()
{
	S32	temp	;
	S32	i 	;

	temp	  = TimerRefHR					;
	DeltaRain = LastTimer ? (temp-LastTimer) * 10 : 0	;
	LastTimer = temp					;

	for ( i = 0 ; i < MAX_RAIN ; i++)
	{
		if( !TabRain[i].Timer )
		{
			TabRain[i].XRain += DeltaRain/2		;
			TabRain[i].ZRain += DeltaRain/2		;
			TabRain[i].YRain -= DeltaRain		;
		}
	}
}

//----------------------------------------------------------------------------
void	AffRain()
{
	S32	i, c	;
	S32	xp, yp	;
	S32	f 	;

	UnsetClip() ;

	for ( i = 0 ; i < MAX_RAIN ; i++)
	{
		if( TabRain[i].Timer )
		{
			S32	x0, y0, x1, y1	;
			S32	x, y, z		;
			S32	dt		;

			dt = LastTimer - TabRain[i].Timer ;

			c  = TabRain[i].XRain>>16	;
 			x  = (S16)(TabRain[i].XRain&0xFFFF) ;
 			y  = TabRain[i].YRain	     	;
			z  = TabRain[i].ZRain		;

			yp = (RAIN_VY-RAIN_WEIGHT*dt)*dt/256 ;
			if(yp<0)
			{
				yp = 0	;
				xp = RAIN_VX*RAIN_VY/RAIN_WEIGHT/256	;
			}
			else
			{
				xp = RAIN_VX*dt/256	;

				yp = (yp * LFactorY) / z;
			}

			xp = (xp * LFactorX) / z;

			x0 = x - xp ;
			x1 = x + xp ;

			y0 = y - yp ;
			y1 = y	    ;

			z  = RegleTrois( 0, 65535, ClipZFar, z ) ;

			f  = LineRain( x, y, z, x0, y0, z, c ) ;
			f |= LineRain( x, y, z, x1, y0, z, c ) ;

			if(f&1)
			{
				BoxMovingAdd( x0, y0, x1, y1 ) ;
			}

			if(dt&&!yp)
			{
				RestartOneRain(&TabRain[i]);
			}
		}
		else
		{
			S32	z0, z1 	;

			if ( TabRain[i].YRain <= RAIN_STOP )
			{
				RestartOneRain(&TabRain[i]);
			}

			LongWorldRotatePoint( 	TabRain[i].XRain-RAIN_DELTA_X,
						TabRain[i].YRain+RAIN_DELTA_Y,
						TabRain[i].ZRain-RAIN_DELTA_Z	) ;

			if(!LongProjectPoint(X0, Y0, Z0))
			{
				continue	;
			}

			xp = Xp	;
			yp = Yp	;
			z0 = RegleTrois( 0, 65535, ClipZFar, CameraZr-Z0 ) ;

			LongWorldRotatePoint(	TabRain[i].XRain,
						TabRain[i].YRain,
						TabRain[i].ZRain ) ;

			if(!LongProjectPoint(X0, Y0, Z0))
			{
				continue	;
			}

			Z0 = CameraZr-Z0	;

			z1 = RegleTrois( 0, 65535, ClipZFar, Z0 ) ;

			c = BoundRegleTrois( 16*3+10, 16*3+3, ClipZFar-StartZFog, Z0 ) ;

			f = LineRain( xp, yp, z0, Xp, Yp, z1, c )	;

			if(f & 1)
			{
				BoxMovingAdd(ScreenXMin, ScreenYMin, ScreenXMax, ScreenYMax);
			}

			if(f & 2)
			{
				if(f&1)
				{
					TabRain[i].XRain = ((xp&0xFFFF)|(c<<16)) ;
					TabRain[i].YRain = yp ;
					TabRain[i].ZRain = Z0 ;

					// eclat des gouttes
					TabRain[i].Timer = LastTimer ;
				}
				else
				{
					RestartOneRain(&TabRain[i]);
				}
			}

		}
	}
}

//----------------------------------------------------------------------------

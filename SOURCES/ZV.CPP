#include	"c_extern.h"

#define		TESTINZV(X, Y, Z)				\
		if ( ((X)>=x2) && ((X)<=x2+dx2) &&		\
		     ((Y)>=y2) && ((Y)<=y2+dy2) &&		\
		     ((Z)>=z2) && ((Z)<=z2+dz2)			\
		   )						\
			return TRUE;				\
								\
		if ( ((X+dx1)>=x2) && ((X+dx1)<=x2+dx2) &&      \
		     ((Y)>=y2) && ((Y)<=y2+dy2) &&              \
		     ((Z)>=z2) && ((Z)<=z2+dz2)                 \
		   )                                            \
			return TRUE;                            \
								\
								\
		if ( ((X+dx1)>=x2) && ((X+dx1)<=x2+dx2) &&      \
		     ((Y+dy1)>=y2) && ((Y+dy1)<=y2+dy2) &&      \
		     ((Z)>=z2) && ((Z)<=z2+dz2)                 \
		   )                                            \
			return TRUE;                            \
								\
		if ( ((X+dx1)>=x2) && ((X+dx1)<=x2+dx2) &&      \
		     ((Y+dy1)>=y2) && ((Y+dy1)<=y2+dy2) &&      \
		     ((Z+dz1)>=z2) && ((Z+dz1)<=z2+dz2)         \
		   )                                            \
			return TRUE;                            \
								\
								\
		if ( ((X)>=x2) && ((X)<=x2+dx2) &&              \
		     ((Y+dy1)>=y2) && ((Y+dy1)<=y2+dy2) &&      \
		     ((Z)>=z2) && ((Z)<=z2+dz2)                 \
		   )                                            \
			return TRUE;                            \
								\
		if ( ((X)>=x2) && ((X)<=x2+dx2) &&              \
		     ((Y+dy1)>=y2) && ((Y+dy1)<=y2+dy2) &&      \
		     ((Z+dz1)>=z2) && ((Z+dz1)<=z2+dz2)         \
		   )                                            \
			return TRUE;                            \
								\
		if ( ((X)>=x2) && ((X)<=x2+dx2) &&              \
		     ((Y)>=y2) && ((Y)<=y2+dy2) &&              \
		     ((Z+dz1)>=z2) && ((Z+dz1)<=z2+dz2)         \
		   )                                            \
			return TRUE;                            \
								\
		if ( ((X+dx1)>=x2) && ((X+dx1)<=x2+dx2) &&      \
		     ((Y)>=y2) && ((Y)<=y2+dy2) &&              \
		     ((Z+dz1)>=z2) && ((Z+dz1)<=z2+dz2)         \
		   )                                            \
			return TRUE;











S32	IntersectZV(	S32  x1, S32  y1, S32  z1,
			S32 dx1, S32 dy1, S32 dz1,
			S32  x2, S32  y2, S32  z2,
			S32 dx2, S32 dy2, S32 dz2,
			S32 xp1, S32 yp1, S32 zp1
		    )
{
	S32	tt;

		// First tests.

	if ( (x1>(x2+dx2)) && (xp1>(x2+dx2)) ) return FALSE;
	if ( ((x1+dx1)<x2) && ((xp1+dx1)<x2) ) return FALSE;

	if ( (y1>(y2+dy2)) && (yp1>(y2+dy2)) ) return FALSE;
	if ( ((y1+dy1)<y2) && ((yp1+dy1)<y2) ) return FALSE;

	if ( (z1>(z2+dz2)) && (zp1>(z2+dz2)) ) return FALSE;
	if ( ((z1+dz1)<z2) && ((zp1+dz1)<z2) ) return FALSE;

		// Now it is possible that our 2 objects have intersected one another...



				// *** Test on X plane ***
	if (x1!=xp1)
	{
	       tt = ((x2-xp1)<<12)/(x1-xp1);
	       if ( (tt>=0) && (tt<=4096) )
	       {
		       TESTINZV(x2,
				yp1+( (tt*(y1-yp1)) >>12 ),
				zp1+( (tt*(z1-zp1)) >>12 )
			       );
	       }

	       tt = ((x2-xp1-dx1)<<12)/(x1-xp1);
	       if ( (tt>=0) && (tt<=4096) )
	       {
		       TESTINZV(x2-dx1,
				yp1+( (tt*(y1-yp1)) >>12 ),
				zp1+( (tt*(z1-zp1)) >>12 )
			       );
	       }

	       tt = ((x2+dx2-xp1)<<12)/(x1-xp1);
	       if ( (tt>=0) && (tt<=4096) )
	       {
		       TESTINZV(x2+dx2,
				yp1+( (tt*(y1-yp1)) >>12 ),
				zp1+( (tt*(z1-zp1)) >>12 )
			       );
	       }

	       tt = ((x2+dx2-xp1-dx1)<<12)/(x1-xp1);
	       if ( (tt>=0) && (tt<=4096) )
	       {
		       TESTINZV(x2+dx2-dx1,
				yp1+( (tt*(y1-yp1)) >>12 ),
				zp1+( (tt*(z1-zp1)) >>12 )
			       );
	       }
	}


				// *** Test on Y plane ***
	if (y1!=yp1)
	{
		tt = ((y2-yp1)<<12)/(y1-yp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 y2,
				 zp1+( (tt*(z1-zp1)) >>12 )
				);
		}

		tt = ((y2-yp1-dy1)<<12)/(y1-yp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 y2-dy1,
				 zp1+( (tt*(z1-zp1)) >>12 )
				);
		}

		tt = ((y2+dy2-yp1)<<12)/(y1-yp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 y2+dy2,
				 zp1+( (tt*(z1-zp1)) >>12 )
				);
		}

		tt = ((y2-yp1-dy1)<<12)/(y1-yp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 y2+dy2-dy1,
				 zp1+( (tt*(z1-zp1)) >>12 )
				);
		}
	}



				// *** Test on Z plane ***
	if (z1!=zp1)
	{
		tt = ((z2-zp1)<<12)/(z1-zp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 yp1+( (tt*(y1-yp1)) >>12 ),
				 z2
				);
		}

		tt = ((z2-zp1-dz1)<<12)/(z1-zp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 yp1+( (tt*(y1-yp1)) >>12 ),
				 z2-dz1
				);
		}

		tt = ((z2+dz2-zp1)<<12)/(z1-zp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 yp1+( (tt*(y1-yp1)) >>12 ),
				 z2+dz2
				);
		}

		tt = ((z2+dz2-zp1-dz1)<<12)/(z1-zp1);
		if ( (tt>=0) && (tt<=4096) )
		{
			TESTINZV(xp1+( (tt*(x1-xp1)) >>12 ),
				 yp1+( (tt*(y1-yp1)) >>12 ),
				 z2+dz2-dz1
				);
		}
	}

	return FALSE;
}






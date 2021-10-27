#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "adeline.h"
#include "lib_sys.h"

char	ItoaTempoString[40] ;		/* c'est 17 le max je crois */

char	*Itoa( LONG value )
{
	return itoa( value, ItoaTempoString, 10 ) ;
}

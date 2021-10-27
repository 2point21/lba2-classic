#include 	"c_extern.h"

#include	<process.h>

#ifdef	DEMO
char	*Version	= "LBA Demo ("
			  __DATE__
			  " / "
			  __TIME__
			  ")\n"		;
#else
char	*Version	= "LBA/Relentless ("
			  __DATE__
			  " / "
			  __TIME__
			  ")\n"		;
#endif
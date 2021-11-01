#include	"c_extern.h"

static char *CriticalErrMsg[] = {
	"write protect",
	"unknown unit",
	"drive not ready",
	"unknown command",
	"data error (CRC)",
	"bad request",
	"seek error",
	"unknown media type",
	"sector not found",
	"printer out of paper",
	"error writing",
	"error reading",
	"general failure",
	"reserved",
	"reserved",
	"invalid disk change"
	} ;

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

S32	CriticalErrorCount = 0 ;
char	msg[255]  ;

int __far CriticalErrorHandler(	unsigned int deverr,
				unsigned int errcode,
				unsigned int __far *devhdr )
{
	// message() ??? only system calls from 0x01 to 0x0C and 0x59 are allowed
	// cprintf/cputs are ok

	devhdr  = devhdr ;	// for warning

	// if this not disk error then another device having trouble
	if( deverr&0x8000 )
	{
#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
		HERCULE( HPrintf("Device error\n") ) ; // report the error
#endif

		// return to the program directly requesting abort
		return( 5 ) ;// 5 = DOS "access denied" error
	}

	CriticalErrorCount++ ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
HERCULE( HStringf( 0, 0, "CriticalErrorCount: %d  ", CriticalErrorCount ) ) ;
#endif

	if( CriticalErrorCount>=CRITICAL_ERROR_COUNT_FAILURE )
	{
		// report which error it was
		sprintf( msg, "Critical Error: %s drive %c.", CriticalErrMsg[(errcode&0x00FF)], 'A'+(deverr&0x00FF) ) ;

#if defined(DEBUG_TOOLS)||defined(TEST_TOOLS)
		// return to program
		return( _HARDERR_FAIL ) ;
#else
		InitTheEnd( PROGRAM_OK, msg ) ;

//		return( _HARDERR_ABORT ) ;
		return( _HARDERR_FAIL ) ;// sinon, plantage !!!!!!!
//		TheEnd( PROGRAM_OK, msg ) ;
#endif
	}

	// return to program
	return( _HARDERR_RETRY ) ;
}

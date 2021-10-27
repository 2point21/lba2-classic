//--------------------------------------------------------------------------//
#ifndef	LIB_SYSTEM_FILES
#define LIB_SYSTEM_FILES

//--------------------------------------------------------------------------//
#include	<stdio.h>
#include	<io.h>
#include	<sys\types.h>
#include	<sys\stat.h>
#include        <sys\utime.h>
#include	<fcntl.h>

//--------------------------------------------------------------------------//
#ifdef	__cplusplus
extern	"C"	{
#endif

//--------------------------------------------------------------------------//
#ifdef	_WIN32

//--------------------------------------------------------------------------//
enum	{	MODE_READ,
		MODE_WRITE,
		MODE_READWRITE
	}	;

//--------------------------------------------------------------------------//
#define SEEK_START              	FILE_BEGIN
#define SEEK_CURRENT            	FILE_CURRRENT
#ifdef	SEEK_END
#undef	SEEK_END
#endif//SEEK_END
#define SEEK_END	            	FILE_END

//--------------------------------------------------------------------------//
extern	S32	OpenMode(char *name, int mode)			;

//--------------------------------------------------------------------------//
extern	U32	Read(S32 handle, void *buffer, U32 lenread)	;

//--------------------------------------------------------------------------//
extern	U32	Write(S32 handle, void *buffer, U32 lenwrite)	;

//--------------------------------------------------------------------------//
extern	void	Touch(char *name)				;

//--------------------------------------------------------------------------//
#define	Close(handle)			CloseHandle((HANDLE)(handle))
#define	Seek(handle, position, mode)	SetFilePointer((HANDLE)(handle), position, NULL, mode)
#define	Delete(name)			DeleteFile(name)

//--------------------------------------------------------------------------//
#else// _WIN32

//--------------------------------------------------------------------------//
#define	MODE_READ			(O_RDONLY|O_BINARY)
#define	MODE_WRITE			(O_WRONLY|O_BINARY|O_CREAT|O_TRUNC)
#define	MODE_READWRITE			(O_RDWR  |O_BINARY|O_CREAT)

//--------------------------------------------------------------------------//
#define SEEK_START              	SEEK_SET
#define SEEK_CURRENT            	SEEK_CUR

//--------------------------------------------------------------------------//
extern	S32    OpenMode( char *name, int mode)	;

//--------------------------------------------------------------------------//
//#define Delete	!remove

extern	U32	Delete(char *name)		;

#ifdef	__SW_3R
#pragma	aux Delete	=			\
"	call	remove	"			\
"	test	eax, eax"			\
"	mov	eax, 1	"			\
"	jz	ok	"			\
"	xor	eax, eax"			\
"ok:			"			\
	parm caller	[eax]			\
	value		[eax]
#else//	__SW_3R
#pragma	aux Delete	=			\
"	call	remove	"			\
"	test	eax, eax"			\
"	mov	eax, 1	"			\
"	jz	ok	"			\
"	xor	eax, eax"			\
"ok:			"			\
	parm caller	[]			\
	modify		[eax ecx edx]		\
	value		[eax]
#endif//__SW_3R


//--------------------------------------------------------------------------//
#define Read(handle, buffer, lenread)   read(handle, buffer, lenread)
#define Write(handle, buffer, lenwrite) write(handle, buffer, lenwrite)
#define Close(handle)                   close(handle)
#define Seek(handle, position, mode)    lseek(handle, position, mode)
#define Touch(filename)                 utime(filename, NULL)

//--------------------------------------------------------------------------//
#endif//_WIN32

//--------------------------------------------------------------------------//
#define	OpenRead(name)			OpenMode(name, MODE_READ)
#define	OpenWrite(name)			OpenMode(name, MODE_WRITE)
#define	OpenReadWrite(name)		OpenMode(name, MODE_READWRITE)

//--------------------------------------------------------------------------//
extern	U32	FileSize( char *name )		;

//--------------------------------------------------------------------------//
#ifdef	__cplusplus
}
#endif

//--------------------------------------------------------------------------//
#endif//LIB_SYSTEM_FILES

//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//
#include <system\adeline.h>
#include <system\files.h>

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>

//--------------------------------------------------------------------------//
#ifdef	_WIN32

//--------------------------------------------------------------------------//
S32	OpenMode( char *name, int mode)
{
	HANDLE	handle	;

	if(mode==MODE_READ)
	{
		handle = CreateFile(	name,GENERIC_READ,
					FILE_SHARE_READ,
					NULL,OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,NULL)	;
	}
	else if(mode==MODE_WRITE)
	{
		handle = CreateFile(	name,GENERIC_WRITE,
					FILE_SHARE_READ,
					NULL,CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,NULL)	;
	}
	else if(mode==MODE_READWRITE)
	{
		handle = CreateFile(	name,GENERIC_READ|GENERIC_WRITE,
					FILE_SHARE_READ,
					NULL,OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,NULL)	;
	}
	else
	{
		handle = INVALID_HANDLE_VALUE	;
	}

	return	handle == INVALID_HANDLE_VALUE ? 0 : (S32)handle	;
}

//--------------------------------------------------------------------------//
U32	Read(S32 handle, void *buffer, U32 size)
{
	U32	len	;

	ReadFile((HANDLE)handle, buffer, size, &len, NULL)	;

	return	len	;
}

//--------------------------------------------------------------------------//
U32	Write(S32 handle, void *buffer, U32 size)
{
	U32	len	;

	WriteFile((HANDLE)handle, buffer, size, &len, NULL)	;

	return	len	;
}

//--------------------------------------------------------------------------//
U32	FileSize(char *name)
{
	HANDLE	handle	;
	U32	size	;

	handle = CreateFile(	name,GENERIC_READ,
				FILE_SHARE_READ,
				NULL,OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,NULL)	;

	if(handle == INVALID_HANDLE_VALUE)	return 0	;

	size = GetFileSize(handle, NULL)	;

	CloseHandle(handle)	;

	return	size	;
}

//--------------------------------------------------------------------------//
void	Touch(char *name)
{
	HANDLE		handle	;
	FILETIME	ftime	;
	SYSTEMTIME	stime	;

	handle = CreateFile(	name,GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,NULL)	;

	if(handle == INVALID_HANDLE_VALUE)		return	;

	GetSystemTime(&stime)	;

	SystemTimeToFileTime(&stime, &ftime)	;

	SetFileTime(handle, &ftime, &ftime, &ftime)	;

	CloseHandle(handle)	;
}

//--------------------------------------------------------------------------//
#else// _WIN32

//--------------------------------------------------------------------------//
S32    OpenMode( char *name, int mode)
{
	S32	handle	;

	handle = open( name, mode, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)	;

	return handle == -1 ? 0 : handle	;
}

//--------------------------------------------------------------------------//
U32   FileSize( char *name )
{
	int     handle  ;
	S32	fsize	;

	handle = OpenRead( name )		;
	if (!handle)	return(0)   		;

	fsize = lseek( handle, 0, SEEK_END )	;
	Close( handle )				;

	return (U32)(fsize == -1 ? 0 : fsize)	;
}

//--------------------------------------------------------------------------//
#endif//_WIN32

//--------------------------------------------------------------------------//

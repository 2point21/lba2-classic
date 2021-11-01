/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include	<system\adeline.h>
#include	<system\files.h>
#include	<system\lz.h>
#include	<system\hqr.h>
#include	<system\hqfile.h>

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	S32			HQF_File	;
static	COMPRESSED_HEADER 	HQF_header	;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
U32	HQF_Init(char *name, S32 index)
{
	S32	nbbloc	;

	HQF_File = OpenRead( name ) 		;
	if( !HQF_File )	return 0		;

	index *= 4				;
	Read(HQF_File, &nbbloc, 4) 		;

	if( index >= nbbloc)
	{
error:		HQF_Close()			;
		return 0			;
	}

	Seek(HQF_File, index, SEEK_START) 	;
	Read(HQF_File, &index, 4) 		;

	if(!index)
	{
		goto	error			;
	}

	Seek(HQF_File, index, SEEK_START) 	;
	Read(HQF_File, (void*)&HQF_header, sizeof(HQF_header));

	return HQF_header.SizeFile		;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	HQF_Close()
{
	if(HQF_File)
	{
		Close(HQF_File)	;
		HQF_File = 0	;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
U32	HQF_LoadClose(void *ptr)
{
	if(!HQF_File)
	{
		return	0	;
	}

	if(HQF_header.CompressMethod==0)
	{
		/* Stored */
		Read(HQF_File, ptr, HQF_header.SizeFile) ;
	}
	else if(HQF_header.CompressMethod<=2)
	{
		void	*ptrdecomp	;

		ptrdecomp = (void*)((U8*)ptr + HQF_header.SizeFile - HQF_header.CompressedSizeFile + RECOVER_AREA) ;

		Read(HQF_File, ptrdecomp, HQF_header.CompressedSizeFile)	;

		/* LZSS/LZMIT */
		ExpandLZ(ptr, ptrdecomp, HQF_header.SizeFile, HQF_header.CompressMethod+1);
	}
	else
	{
		HQF_header.SizeFile = 0	;
	}

	HQF_Close()			;
	return HQF_header.SizeFile	;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
S32	HQF_ResSize(char *name, S32 index)
{
	U32	size			;

	size = HQF_Init(name, index)	;
	HQF_Close()			;

	return size			;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// retourne le nombre de bloc d'un HQR ( 0 si HQR non trouv‚ )
S32	HQF_NbRes(char *name)
{
	S32	handle			;
	S32	nbbloc			;

	handle = OpenRead(name)		;
	if(!handle)
	{
		return	0		;
	}

	Read( handle, &nbbloc, 4 )	;
	Close( handle )			;

	return( nbbloc/4-1 )		;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

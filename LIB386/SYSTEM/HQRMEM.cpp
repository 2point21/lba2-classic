//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
#include	<system\adeline.h>
#include	<system\lz.h>
#include	<system\hqrress.h>
#include	<system\hqrmem.h>

#include 	<string.h>

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
// retourne un pointeur sur un bloc demande(index) d'un hqr charg‚ en m‚moire
COMPRESSED_HEADER *GetPtrBlockMemoryHQR(void *ptrhqr, S32 index)
{
	U32		seekindex 	;

	index	*= 4			;

	// si num bloc demande > num bloc dans hqr exit
	if( index >= *(U32*)ptrhqr )
	{
		return NULL 		;
	}

	// recupere offset bloc
	seekindex	= *(U32*)((U8*)ptrhqr + index)	;

	// saute header
	return	(COMPRESSED_HEADER*)((U8*)ptrhqr + seekindex + sizeof(T_HQR_HEADER));
}

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД
// decompresse un bloc d'un hqr charge en m‚moire
// retourne:	taille du bloc(decompress‚) si Ok
//		0 si bloc_demande>bloc existant
U32	LoadMemoryHQR( void *ptrhqr, void *ptrdest, S32 index )
{
	COMPRESSED_HEADER	*header	;

	header	= GetPtrBlockMemoryHQR(ptrhqr, index);

	if(!header)
	{
		return	NULL	;
	}

	switch( header->CompressMethod )
	{
		case 0: 	// Stored
			memmove(ptrdest, (void*)(header+1), header->SizeFile) ;
			break 	;

		case 1:
		case 2: 	// LZSS/LZMIT
			ExpandLZ(ptrdest, (void*)(header+1), header->SizeFile, header->CompressMethod+1) ;
			break 	;

	}
	return header->SizeFile	;
}

//ДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДДД


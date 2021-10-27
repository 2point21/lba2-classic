/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*
				SAMP.C 386
			      (c) Adeline 1993
 *컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"
#include "\projet\lib386\lib_samp\lib_wave.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<dos.h>
#include	<i86.h>

char	*WaveError = "Error WaveDriver:";

extern	void	*Wave_listfcts;

extern	LONG	Wave_Driver_Enable;

ULONG	_InitCard(		void *Buffer		);
void	_ClearCard(		void			);
ULONG	_GetBufferSize(		void			);

	void	*BUFFER_DMA;
	void	*END_BUFFER_DMA;

/*-------------------------------------------------------------------------*/
LONG	WaveInitDLL(char *driverpathname)
{
	char	*dll, *drvr;

   //
   // Load driver file
   //

	dll = FILE_read( driverpathname, NULL);
	if (dll==NULL)
	{
		printf("%s Could not load driver '%s'.\n", WaveError, driverpathname );
		return FALSE ;
	}

	drvr=DLL_load(dll,DLLMEM_ALLOC | DLLSRC_MEM,NULL);
	if (drvr==NULL)
	{
		printf("%s Invalid DLL image.\n", WaveError );
		return FALSE ;
	}

	Free(dll);

	Wave_listfcts = *(void **)drvr;
	printf("%s", drvr+4);

	return (Wave_Driver_Enable = TRUE);
}

/*-------------------------------------------------------------------------*/
ULONG	InitWave()
{
	union	REGS	r;
	struct	SREGS	sr;
	void 	far	*fh;
	int		i;
	int	IRQ_number;
	int	kilo_buffer;
	ULONG	bad_handle[10];

	kilo_buffer = _GetBufferSize();

/*- Reserve some space for a sound buffer that doesn't cross a 64K page -*/

//	for(i=0; i<10; i++)
//		bad_handle[i]=NULL;
	i = 0;
	do
	{
		if (!(BUFFER_DMA = DosMalloc( kilo_buffer, &bad_handle[i] )))
		{
			BUFFER_DMA = NULL;
			i+=2;
			break;
		}
		END_BUFFER_DMA = (void *)((ULONG)BUFFER_DMA + kilo_buffer - 1);
		if (((LONG)BUFFER_DMA & 0xFFFF0000) != ((LONG)END_BUFFER_DMA & 0xFFFF0000))
		{
			BUFFER_DMA = NULL;
		}
		i++;
	} while ((BUFFER_DMA == NULL) && (i<=10));

//	for(i-=2; i>=0; i--)
//		DosFree( bad_handle[i] );

	if (BUFFER_DMA == NULL)
	{
		printf("%s Couldn't allocate a contiguous buffer in DOS memory.\n", WaveError);
		return 0;
	}

/*----- Init Sound Card (and IRQ controller) ---*/
	Wave_Driver_Enable = _InitCard(BUFFER_DMA);

	if (!Wave_Driver_Enable)
	{
		printf("%s Sound card not found.\n", WaveError);
	}

	return Wave_Driver_Enable;
}

void	ClearWave()
{
	union	REGS	r;
	struct	SREGS	sr;

	if (!Wave_Driver_Enable)
		return;

/*----- Reset Sound Card (and IRQ controller) ---*/
	_ClearCard();
}
/*-------------------------------------------------------------------------*/

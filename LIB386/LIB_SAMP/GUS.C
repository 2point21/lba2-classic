#include "\projet\lib386\lib_sys\adeline.h"
#include "\projet\lib386\lib_sys\lib_sys.h"

#pragma	library	("\gussdk21\libs\ultra0wc.lib");

#include "forte.h"
#include "gf1proto.h"
#include "extern.h"
#include "ultraerr.h"


extern	void	NewIRQ(void);


extern	void	*BUFFER_DMA;

extern	ULONG	R_BUFFER_CARD;
extern	ULONG	MID_R_BUFFER_CARD;
extern	ULONG	CURRENT_R_BUFFER_CARD;

extern	ULONG	L_BUFFER_CARD;
extern	ULONG	MID_L_BUFFER_CARD;
extern	ULONG	CURRENT_L_BUFFER_CARD;

extern	LONG	BUFFER_SIZE;
extern	UWORD	PlayRate;


void	ResetCard(void)
{
	ULTRA_CFG	config;
	UBYTE		RMode, LMode;

	/* Get the ULTRASND environment string parameters */
	UltraGetCfg(&config);

	/* Open the card with 14 voices (44 Khz) */
	UltraOpen(&config, 14);

	/* Get a chunk of memory on the card */
	UltraMemAlloc(BUFFER_SIZE << 1, &R_BUFFER_CARD);
	MID_R_BUFFER_CARD = R_BUFFER_CARD + BUFFER_SIZE;
	CURRENT_R_BUFFER_CARD = MID_R_BUFFER_CARD;

	/* Reset memory on the card to 0 */
	UltraDownload(BUFFER_DMA, DMA_8|DMA_CVT_2, R_BUFFER_CARD, BUFFER_SIZE, TRUE);
	UltraDownload(BUFFER_DMA, DMA_8|DMA_CVT_2, MID_R_BUFFER_CARD, BUFFER_SIZE, TRUE);

	/* Get a chunk of memory on the card */
	UltraMemAlloc(BUFFER_SIZE << 1, &L_BUFFER_CARD);
	MID_L_BUFFER_CARD = L_BUFFER_CARD + BUFFER_SIZE;
	CURRENT_L_BUFFER_CARD = MID_L_BUFFER_CARD;

	/* Reset memory on the card to 0 */
	UltraDownload(BUFFER_DMA, DMA_8|DMA_CVT_2, L_BUFFER_CARD, BUFFER_SIZE, TRUE);
	UltraDownload(BUFFER_DMA, DMA_8|DMA_CVT_2, MID_L_BUFFER_CARD, BUFFER_SIZE, TRUE);

	/* set Balance for each voice */
	UltraSetBalance(0, 7);
	UltraSetBalance(1, 7);

	/* Set Frequency for each voice to PlayRate */
	UltraSetFrequency(0, PlayRate);
	UltraSetFrequency(1, PlayRate);

	/* set Volume for each voice */
	UltraSetVolume(0, 511);
	UltraSetVolume(1, 511);

	/* get voices ready... */
	RMode = UltraPrimeVoice(0, R_BUFFER_CARD, R_BUFFER_CARD,
				R_BUFFER_CARD + (BUFFER_SIZE << 1), 0x20 | 0x08 | 0x04);
	LMode = UltraPrimeVoice(1, L_BUFFER_CARD, MID_L_BUFFER_CARD,
				L_BUFFER_CARD + (BUFFER_SIZE << 1), 0x20 | 0x08 | 0x04);

	/* Plug in our IRQ handler for wave events */
	UltraWaveHandler(NewIRQ);

	/*set, go ! */
	UltraGoVoice(0, RMode);
	UltraGoVoice(1, LMode);
}

void	StartDMACard(void)
{
	UltraDownload(BUFFER_DMA, DMA_8|DMA_CVT_2, CURRENT_R_BUFFER_CARD, BUFFER_SIZE, FALSE);
}
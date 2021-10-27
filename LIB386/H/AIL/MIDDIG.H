//--------------------------------------------------------------------------
#ifndef	LIB_AIL_MIDDIG
#define	LIB_AIL_MIDDIG

//--------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------
//	InitSampleSynthesizer:		Create a Wavetable siynthesizer
//
//			  wavelib:	Pointer to the wave library
//			  sizelib:	library size
//			  voices : 	number of sample voices allocated
//
//	Returns	: 			nothing
//--------------------------------------------------------------------------
void	InitSampleSynthesizer(	void *wavelib,
				S32 sizelib,
				S32 voices 		)	;

//--------------------------------------------------------------------------
//	ClearSampleSynthesizer: 	destroy the Wavetable synthesizer
//
//	Returns	: 			nothing
//--------------------------------------------------------------------------
void	ClearSampleSynthesizer(	void			)	;

//--------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------
#endif//LIB_AIL_MIDDIG

//--------------------------------------------------------------------------

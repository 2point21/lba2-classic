//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
#ifndef	LIB_AIL_MIXER
#define	LIB_AIL_MIXER

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#ifdef	__cplusplus
extern	"C"	{
#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

typedef	struct
	{
		S32	DeviceID;
		S32	ControlID;
		S32	Channels;
	}	VOLUMECONTROL	;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

// MIXERLINE_COMPONENTTYPE_DST_FIRST
// MIXERLINE_COMPONENTTYPE_DST_UNDEFINED
// MIXERLINE_COMPONENTTYPE_DST_DIGITAL
// MIXERLINE_COMPONENTTYPE_DST_LINE
// MIXERLINE_COMPONENTTYPE_DST_MONITOR
// MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
// MIXERLINE_COMPONENTTYPE_DST_HEADPHONES
// MIXERLINE_COMPONENTTYPE_DST_TELEPHONE
// MIXERLINE_COMPONENTTYPE_DST_WAVEIN
// MIXERLINE_COMPONENTTYPE_DST_VOICEIN
// MIXERLINE_COMPONENTTYPE_DST_LAST

// MIXERLINE_COMPONENTTYPE_SRC_FIRST
// MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED
// MIXERLINE_COMPONENTTYPE_SRC_DIGITAL
// MIXERLINE_COMPONENTTYPE_SRC_LINE
// MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE
// MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER
// MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC
// MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE
// MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER
// MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT
// MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY
// MIXERLINE_COMPONENTTYPE_SRC_ANALOG
// MIXERLINE_COMPONENTTYPE_SRC_LAST

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

S32	GetMixerVolDevice(VOLUMECONTROL *volcontrol, S32 type)	;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

S32	GetMixerVolume(VOLUMECONTROL *volcontrol)		;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	SetMixerVolume(VOLUMECONTROL *volcontrol, S32 volume)	;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#ifdef	__cplusplus
}
#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#endif//LIB_AIL_MIXER

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

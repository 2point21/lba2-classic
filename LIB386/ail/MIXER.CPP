//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#include	<system\adeline.h>
#include	"mixer.h"

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

static MIXERCONTROLDETAILS VolumeDetails;
static MIXERCONTROLDETAILS_UNSIGNED VolumeValue[2];

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

S32	GetMixerVolDevice(VOLUMECONTROL *volcontrol, S32 type)
{
	S32 			voldeviceid;
	S32			i ;
	MIXERLINE 		lineinfo;
	MIXERLINECONTROLS 	linecontrols;
	MIXERCONTROL 		volumecontrol;

	 // try the mixer api for volume control first
	 for(i=mixerGetNumDevs()-1;i>=0;i--)
	 {
		if (mixerOpen((HMIXER*)&voldeviceid,i,0,0,0)!=MMSYSERR_NOERROR)
		{
			continue	;
		}
		memset(&lineinfo,0,sizeof(lineinfo));
		lineinfo.cbStruct=sizeof(lineinfo);
		lineinfo.dwComponentType=type;

		if (mixerGetLineInfo((HMIXEROBJ)voldeviceid,&lineinfo,MIXER_GETLINEINFOF_COMPONENTTYPE)!=MMSYSERR_NOERROR)
		{
			goto error	;
		}

		memset(&linecontrols,0,sizeof(linecontrols));
		memset(&volumecontrol,0,sizeof(volumecontrol));
		linecontrols.cbStruct=sizeof(linecontrols);
		linecontrols.dwLineID=lineinfo.dwLineID;
		linecontrols.dwControlType=MIXERCONTROL_CONTROLTYPE_VOLUME;
		linecontrols.cControls=1;
		linecontrols.cbmxctrl=sizeof(volumecontrol);
		linecontrols.pamxctrl=&volumecontrol;

		if (mixerGetLineControls((HMIXEROBJ)voldeviceid,&linecontrols,MIXER_GETLINECONTROLSF_ONEBYTYPE)!=MMSYSERR_NOERROR)
		{
error:			mixerClose((HMIXER)voldeviceid);
			continue;
		}

		memset(&VolumeDetails,0,sizeof(VolumeDetails));
		memset(VolumeValue,0,sizeof(VolumeValue[0])*2);
		VolumeDetails.cbStruct=sizeof(MIXERCONTROLDETAILS);
		VolumeDetails.dwControlID=volumecontrol.dwControlID;
		VolumeDetails.cChannels=lineinfo.cChannels;
		VolumeDetails.cbDetails=sizeof(VolumeValue[0]);
		VolumeDetails.paDetails=VolumeValue;

		if (mixerGetControlDetails((HMIXEROBJ)voldeviceid,&VolumeDetails,MIXER_SETCONTROLDETAILSF_VALUE)==MMSYSERR_NOERROR)
		{
			volcontrol->DeviceID = voldeviceid			;
			volcontrol->ControlID = volumecontrol.dwControlID	;
			volcontrol->Channels = lineinfo.cChannels		;

			return	TRUE	;
		}
	}

	volcontrol->DeviceID = -1	;

	return FALSE	;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

S32	GetMixerVolume(VOLUMECONTROL *volcontrol)
{
	if(volcontrol->DeviceID == -1)
	{
		return 0 ;
	}

	VolumeDetails.dwControlID=volcontrol->ControlID;
	VolumeDetails.cChannels=volcontrol->Channels;

	mixerGetControlDetails((HMIXEROBJ)volcontrol->DeviceID,&VolumeDetails,MIXER_SETCONTROLDETAILSF_VALUE);

	return((VolumeValue[0].dwValue&0xffff)>>9);
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	SetMixerVolume(VOLUMECONTROL *volcontrol, S32 volume)
{
	if(volcontrol->DeviceID == -1)
	{
		return	;
	}

	if(volume<0)
	{
		volume = 0	;
	}
	else if(volume>127)
	{
		volume = 127	;
	}

	VolumeDetails.dwControlID=volcontrol->ControlID;
	VolumeDetails.cChannels=volcontrol->Channels;

	volume = (volume<<9)+(volume<<2)+(volume>>5)	;
	VolumeValue[0].dwValue=volume ;
	VolumeValue[1].dwValue=volume ;

	mixerSetControlDetails((HMIXEROBJ)volcontrol->DeviceID,&VolumeDetails,MIXER_SETCONTROLDETAILSF_VALUE);
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

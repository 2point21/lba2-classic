#include	<system\adeline.h>

#include	<stdio.h>
#include	<string.h>
#include	<dos.h>

#include	<ail\cd.h>

#include 	<stdlib.h>

#ifdef	_WIN32
#include	<ail\mssw.h>
#else
#include	<ail\mss.h>
#endif

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

static	HREDBOOK	Cd		;
static	U32		Tracks		;
static	U32		Track		;
static	U32		PausedTrack	;
static	U32		Cur		;
static	U32		End		;

static	S32		OldVolumeCD	;

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

static	S32	CheckCD()
{
	if(!Cd)	return	FALSE	;

	return(AIL_redbook_status(Cd) != REDBOOK_ERROR) ;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

char	OpenCD(char *volume_name)
{
	char	drive	;

	for(drive='A'; drive<='Z'; drive++)
	{
		Cd = AIL_redbook_open_drive(drive)	;
		if(CheckCD())
		{
			Tracks = AIL_redbook_tracks(Cd)	;

			if(Tracks>1)
			{
				AIL_redbook_stop(Cd)	;

				if(!volume_name||!volume_name[0])	break	;
#ifdef	_WIN32
				{
					char		rootpath[4]		;
					char		volume[_MAX_PATH]	;
					unsigned long	maxlength, fsflags	;

					sprintf(rootpath, "%c:\\", drive);

					if(GetDriveType(rootpath)!=DRIVE_CDROM)
					{
						goto error 	;
					}

					if(!GetVolumeInformation(	rootpath,
									volume,
									_MAX_PATH,
									NULL,
									&maxlength,
									&fsflags,
									NULL,
									0
								   ))
					{
						goto error 	;
					}

					if(!strcmp(volume, volume_name))
					{
						break		;
					}
				}
#else
				{
					struct find_t	fileinfo	;
					char		filter[7]	;

					sprintf(filter, "%c:\\*.*", drive);

					if(!_dos_findfirst(filter, _A_VOLID, &fileinfo))
					{
						if(!strcmp(volume_name, fileinfo.name))
						{
							break	;
						}
					}
				}
#endif
			}
		}
error:
		if(Cd)
		{
			AIL_redbook_close(Cd)	;
			Cd = NULL		;
		}
	}

	if(Cd)
	{
		atexit(CloseCD)		;

		OldVolumeCD = GetVolumeCD()	;

		return	drive		;
	}
	else
	{
		return 0		;
	}
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	CloseCD()
{
	if(!Cd)		return	;

	StopCD()		;

	PausedTrack = 0		;

	ChangeVolumeCD(OldVolumeCD);

	AIL_redbook_close(Cd)	;

	Cd = NULL		;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	PlayCD(S32 track)
{
	U32	start, end	;

	if(!CheckCD() || !track || (track>Tracks))	return	;

	StopCD()		;

	AIL_redbook_track_info(Cd, track, &start, &end);

	AIL_redbook_play(Cd, start, end);
	Track = track		;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	ChangeVolumeCD(S32 volume)
{
	if( !CheckCD() )	return ;

	if(volume>127)
	{
		volume = 127	;
	}
	else if (volume<0)
	{
		volume = 0	;
	}

	AIL_redbook_set_volume(Cd, volume)	;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

S32	GetVolumeCD()
{
	return	AIL_redbook_volume(Cd)	;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	StopCD()
{
	if( !CheckCD() || !Track)	return 	;

	AIL_redbook_stop(Cd)	;
	Track = 0		;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	PauseCD()
{
	if( !CheckCD() || !Track || !IsCDPlaying())	return	;

	AIL_redbook_track_info(Cd, Track, NULL, &End);
	Cur = AIL_redbook_position(Cd)	;
	PausedTrack = Track		;

	StopCD()			;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

void	ResumeCD()
{
	if( !CheckCD() || !PausedTrack)	return	;

	AIL_redbook_play(Cd, Cur, End)	;
	Track = PausedTrack		;
	PausedTrack = 0			;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

S32	IsCDPlaying()
{
	S32	playing	;

	if( !CheckCD() || !Track)	return FALSE ;

	playing = (AIL_redbook_status(Cd) != REDBOOK_STOPPED) || PausedTrack ;

	if(playing)
	{
		if(Track)	playing = Track		;
		else		playing = PausedTrack	;
	}
	else
	{
		Track = 0	;
	}


	return	playing		;
}

//лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

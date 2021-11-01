//··········································································
#ifndef	LIB_SYSTEM

#error ADELINE: you need to include SYSTEM.H

#endif

//··········································································
#ifdef	DEBUG_MALLOC

	atexit(SafeErrorMallocMsg)	;

#endif

//··········································································
#ifndef	inits

#error ADELINE: you need to define inits before including INITADEL.C

#endif

//··········································································
#ifndef	lname

#error ADELINE: you need to define lname before including INITADEL.C

#endif

//··········································································
#ifndef	_STDLIB_H_INCLUDED

#error ADELINE: you need to include STDLIB.H

#endif

//··········································································
#ifndef	_STDIO_H_INCLUDED

#error ADELINE: you need to include STDIO.H

#endif

//··········································································
#ifndef	_STRING_H_INCLUDED

#error ADELINE: you need to include STRING.H

#endif

//··········································································
#ifndef	_DOS_H_INCLUDED

#error ADELINE: you need to include DOS.H

#endif

//··········································································
#ifndef	_DIRECT_H_INCLUDED

#error ADELINE: you need to include DIRECT.H

#endif

//··········································································
#ifndef	_CTYPE_H_INCLUDED

#error ADELINE: you need to include CTYPE.H

#endif

//··········································································
#ifndef	RESOLUTION_X

#define	RESOLUTION_X		640

#endif

//··········································································
#ifndef	RESOLUTION_Y

#define	RESOLUTION_Y		480

#endif

//··········································································
#ifndef	RESOLUTION_DEPTH

#define	RESOLUTION_DEPTH	8

#endif

//··········································································
#if	(RESOLUTION_X&7)

#error ADELINE: horizontal resolution must be a multiple of 8

#endif

//··········································································
{

//··········································································
        char    *Adeline="ADELINE" 	;
        char    *name=lname	 	;
	char	*defname		;
	char	old_path[_MAX_PATH]=""	;
	char	old_path2[_MAX_PATH]=""	;

//··········································································
#if	defined(_WIN32)&&!defined(_ARG)

//··········································································
#ifndef	APPNAME
#define	APPNAME	"DefaultName"
#endif//APPNAME

	InitWindow(hInstance, nCmdShow, APPNAME)	;

//··········································································
#endif//defined(_WIN32)&&!defined(_ARG)

//··········································································
// Quiet Log
#if	((inits) & INIT_QUIET)
	QuietLog = TRUE			;
#endif

//··········································································
	getcwd(old_path, _MAX_PATH)	;

	PathConfigFile[0] = 0		;

        defname = getenv(Adeline) 	;
        if(defname)
	{
		char 	drive[ _MAX_DRIVE ]	;
		char 	dir[ _MAX_DIR ]		;
		unsigned int	n		;

		if(FileSize(defname))
		{
			_splitpath( defname, drive, dir, NULL, NULL );
		}
		else
		{
			// env var contain only directory name with no file specified
			strcpy(PathConfigFile, defname)	;
			strcat(PathConfigFile, "\\dummy.tmp");
			_splitpath(PathConfigFile, drive, dir, NULL, NULL );
		}
		_makepath(PathConfigFile, drive, dir, "", "");
		if(name)
		{
			strcat(PathConfigFile, name)	;
		}
		_dos_setdrive(toupper(drive[0])-'A'+1, &n);
		n = strlen(dir)-1		;
		while((dir[n]=='\\')||(dir[n]=='/'))
		{
			dir[n] = 0		;
			n--			;
		}
		getcwd(old_path2, _MAX_PATH)	;
		chdir(dir)			;
	}
	else
	{
		strcpy(PathConfigFile, old_path);
		strcat(PathConfigFile, "\\")	;
		if(name)
		{
			strcat(PathConfigFile, name);
		}

		chdir("Drivers")		;
	}

//··········································································
// LOG
#if	((inits) & INIT_LOG)
		CreateLog(PathConfigFile)	;
#endif

//··········································································
// Config File
	if(name&&!FileSize(PathConfigFile))
	{
		LogPrintf("Error: Can't find config file %s\n\n", PathConfigFile);
		exit(1);
	}

//··········································································
// CMDLINE

#if	defined(_WIN32)&&!defined(_ARG)
	GetCmdLineWin(lpCmdLine);
#else//	defined(_WIN32)&&!defined(_ARG)
	GetCmdLine(argc, argv)	;
#endif//defined(_WIN32)&&!defined(_ARG)

//··········································································
// OS
	LogPuts("\nIdentifying Operating System. Please wait...\n");

	if(!FindAndRemoveParam("/OSNodetect"))
	{
		DetectOS()		;
	}

	DisplayOS()			;

	if(ParamsOS())
	{
		LogPuts("\nSome command Line Parameters override OS detection.\nNew OS parameters:\n");

		DisplayOS()		;
	}

//··········································································
// CPU
	LogPuts("\nIdentifying CPU. Please wait...\n");

	if(!FindAndRemoveParam("/CPUNodetect"))
	{
		ProcessorIdentification();
	}

	DisplayCPU()			;

	if(ParamsCPU())
	{
		LogPuts("\nSome command Line Parameters override CPU detection.\nNew CPU parameters:\n");

		DisplayCPU()		;
	}

//··········································································
// CmdLine Parser
#ifdef	paramparser

	{
		char	cur_path[_MAX_PATH]	;

		getcwd(cur_path, _MAX_PATH)	;
		if(defname)
		{
			chdir(old_path2);
		}
		ChDiskDir(old_path)	;

	        paramparser()  		;

		ChDiskDir(cur_path)	;
	}

#endif

//··········································································
// AIL API init (for vmm_lock/timer)

	InitAIL() ;

//··········································································
// svga
#ifdef _WIN32
#if   ((inits) & (INIT_SVGA|INIT_VESA))

#ifndef	LIB_SVGA

#error ADELINE: you need to include SVGA.H

#endif

	LogPuts("\nInitialising SVGA device. Please wait...\n");

	if(ParamsSvga())
	{
		LogPuts("\nSome command Line Parameters override SVGA detection.\n");
	}

        if(InitGraphSvga(RESOLUTION_X, RESOLUTION_Y, RESOLUTION_DEPTH))
        {
        	exit(1)	;
        }

#endif
#endif

//··········································································
// Midi device

#if ((inits) & INIT_MIDI)

#ifndef	LIB_AIL

#error ADELINE: you need to include AIL.H

#endif

	LogPuts("\nInitialising Midi device. Please wait...\n");

	if( !InitMidiDriver(NULL) )		exit( 1 ) ;

#endif

//··········································································
// Sample device

#if ((inits) & INIT_SAMPLE)

#ifndef	LIB_AIL

#error ADELINE: you need to include AIL.H

#endif

	LogPuts("\nInitialising Sample device. Please wait...\n");

	if( !InitSampleDriver(NULL) )		exit( 1 ) ;

#endif

//··········································································
// Smacker

#if ((inits) & INIT_SMACKER)

#ifndef	LIB_SMACKER

#error ADELINE: you need to include SMACKER.H

#endif

	LogPuts("\nInitialising Smacker. Please wait...\n");

	InitSmacker()	;

#endif

//··········································································
// keyboard
#if   ((inits) & INIT_KEYB)

        InitKeyboard()  ;

#endif

//··········································································
// svga
#ifndef _WIN32
#if   ((inits) & (INIT_SVGA|INIT_VESA))

#ifndef	LIB_SVGA

#error ADELINE: you need to include SVGA.H

#endif

	LogPuts("\nInitialising SVGA device. Please wait...\n");

	if(ParamsSvga())
	{
		LogPuts("\nSome command Line Parameters override SVGA detection.\n");
	}

        if(InitGraphSvga(RESOLUTION_X, RESOLUTION_Y, RESOLUTION_DEPTH))
        {
        	exit(1)	;
        }

#endif
#endif

//··········································································
// mouse
#if   ((inits) & INIT_MOUSE)

        InitMouse()	;
#endif

//··········································································
// init Timer

#if   ((inits) & INIT_TIMER)

#ifdef	_WIN32

        InitTimer() ;

#else //_WIN32

	if(!InitTimerAIL())
        {
	        LogPuts( "Error: Could not link timer routine with AIL INT8 handler.\n") ;
	        exit( 1 ) ;
        }

#endif//_WIN32

#endif

//··········································································
	if(defname)
	{
		chdir(old_path2);
	}

	ChDiskDir(old_path)	;

//··········································································
// Quiet Log
#if	!((inits) & INIT_QUIET)
	QuietLog = TRUE			;
#endif

//··········································································
// DefFile
#if	((inits) & INIT_DEFFILE)

#ifndef	ibuffer

#error ADELINE: you need to define ibuffer

#endif

#ifndef	ibuffersize

#error ADELINE: you need to define ibuffersize

#endif
	DefFileBufferInit(PathConfigFile, (void*)(ibuffer), ibuffersize);
#endif

//··········································································
}

//··········································································

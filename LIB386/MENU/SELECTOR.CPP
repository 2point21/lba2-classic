#include	<system.h>
#include	<svga.h>
#include	<menu.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys\types.h>
#include	<direct.h>
#include	<string.h>
#include	<dos.h>
#include	<mem.h>

//──────────────────────────────────────────────────────────────────────────
#define MAX_FILES	800
#define MAX_DIRS	100

#ifndef	MAX_NAME
#define	MAX_NAME	MAX_FNAME
#endif

//──────────────────────────────────────────────────────────────────────────
	char		SelectorPathname [_MAX_PATH] ;
static	char		SDrive[_MAX_DRIVE] ;
static	char		SDir[_MAX_DIR] ;
static	char		SName[_MAX_FNAME] ;
static	char		SExt[_MAX_EXT] ;

static	S32		NbFiles ;
static	char		*ListFiles ;
static	char		**IndexFiles ;
static	S32		NbDirs ;
static	char		*ListDirs ;
static	char		**IndexDirs ;

static	S32		StartFile = 0 ;

static	char		SearchPathname[_MAX_PATH] ;

static	T_MENU		Selector ;

static	T_WINLIST	FilesWindow ;
static	T_WINLIST	DirsWindow ;

static	S32		FlagScanDir = 0 ;

//──────────────────────────────────────────────────────────────────────────
void	CutFileName( char *filename )
{
	char	drive[_MAX_DRIVE] ;
	char	dir[_MAX_DIR] ;
	char	name[_MAX_FNAME] ;
	char	ext[_MAX_EXT] ;

	_splitpath( filename, drive, dir, name, ext ) ;
	strcpy(filename, name);

	while( (*filename!=0) AND (*filename!='.') )
		filename++ ;
	*filename = 0 ;

}

//──────────────────────────────────────────────────────────────────────────
static void	DrawListFiles( S32 flag )
{
	S32	nb ;

	GetWindowButton( &Selector, 10, &FilesWindow ) ;

	if( NbFiles )
	{
		if( StartFile >= NbFiles )	StartFile = NbFiles-1 ;
		nb = NbFiles - StartFile ;

		if( flag & SELECT_SHOW_EXT )
			WinList( &FilesWindow, IndexFiles+StartFile, nb, 14 ) ;
		else
			WinList( &FilesWindow, IndexFiles+StartFile, nb, 9 ) ;
	}
	else
	{
		WinList( &FilesWindow, IndexFiles, 0, 14 ) ;
	}
	FlipWindow( &FilesWindow ) ;
}

//──────────────────────────────────────────────────────────────────────────
static void	ScanFiles( char *selection, S32 flag )
{
	struct	find_t	fileinfo ;
	U32	rc ;
	char	*ptr ;
	char	path[_MAX_PATH] ;

	_splitpath( selection, SDrive, SDir, SName, SExt ) ;
	_makepath( path, "", SearchPathname, SName, SExt ) ;

	rc = _dos_findfirst( path, _A_NORMAL, &fileinfo ) ;
	NbFiles = 0 ;
	ptr = ListFiles ;
	while( !rc )
	{
		if( NbFiles == MAX_FILES )
		{
			Message( "Warning: Truncated List", TRUE ) ;
			break ;
		}
		IndexFiles[NbFiles] = ptr ;
		if( !(flag & SELECT_SHOW_EXT) ) CutFileName( fileinfo.name ) ;
		strcpy( ptr, fileinfo.name ) ;
		ptr += _MAX_NAME ;
		NbFiles++ ;
		rc = _dos_findnext( &fileinfo ) ;
	}

	if( NbFiles )	// ?
		qsort( (void *)IndexFiles, NbFiles, sizeof( char * ), MySortCompFunc );

	DrawListFiles( flag ) ;
}

//──────────────────────────────────────────────────────────────────────────
static inline S32 ThereIsFiles( char *path )
{
	struct	find_t	fileinfo ;

	return !_dos_findfirst( path, _A_NORMAL, &fileinfo )	;
}

//──────────────────────────────────────────────────────────────────────────
static void	ScanDirs( char *selection )
{
	struct	find_t	fileinfo ;
	U32	rc ;
	char	*ptr ;
	S32	n ;
	char	path[_MAX_PATH] ;

	rc = _dos_findfirst( "*.*", _A_SUBDIR, &fileinfo ) ;
	NbDirs = 0 ;
	ptr = ListDirs ;

	IndexDirs[NbDirs] = ptr	;
	strcpy( ptr, ".." )	;
	ptr += _MAX_NAME 	;
	NbDirs++ 		;

	while( !rc )
	{
		if( fileinfo.attrib & _A_SUBDIR )
		{
			if( NbDirs == MAX_FILES )
			{
				Message( "Warning: Truncated List", TRUE ) ;
				break ;
			}
			if( strcmp(fileinfo.name, "..") AND strcmp(fileinfo.name, ".") )
			{
				IndexDirs[NbDirs] = ptr ;
				strcpy( ptr, fileinfo.name ) ;
				ptr += _MAX_NAME ;
				NbDirs++ ;
			}
		}
		rc = _dos_findnext( &fileinfo ) ;
	}

	GetWindowButton( &Selector, 11, &DirsWindow ) ;
	WinList( &DirsWindow, IndexDirs, NbDirs, 14 ) ;
	FlipWindow( &DirsWindow ) ;

	// if option scan files

	if( FlagScanDir	)
	{
		for( n=0; n<NbDirs; n++ )
		{
			strcpy( path, IndexDirs[n] ) ;
			strcat( path, "\\" ) ;
			strcat( path, selection ) ;
			if( ThereIsFiles( path ) )
			{
				WinSelect( &DirsWindow, n, B_RED+1 ) ;
			}
		}
	}
}

//──────────────────────────────────────────────────────────────────────────
static void	ScanDrives()
{
	unsigned	total ;
	unsigned	drive ;
	unsigned	orgdrive ;
	S32		n ;
 	char		*ptr ;

	_dos_getdrive( &orgdrive ) ;
	NbDirs = 0 ;
	ptr = ListDirs ;
	for( n=1; n<=26; n++ )
	{
		_dos_setdrive( n, &total ) ;
		_dos_getdrive( &drive ) ;
		if( drive == n )
		{
			IndexDirs[NbDirs] = ptr ;
			strcpy( ptr, "A:" ) ;
			ptr[0] = (char)(drive+'A'-1) ;
			ptr += 3 ;
			NbDirs++ ;
		}
	}
	_dos_setdrive( orgdrive, &total ) ;

	GetWindowButton( &Selector, 11, &DirsWindow ) ;
	WinList( &DirsWindow, IndexDirs, NbDirs, 14 ) ;
	FlipWindow( &DirsWindow ) ;
}

//──────────────────────────────────────────────────────────────────────────
S32	FileSelector(	char *titre,
			char *selection,
			char *defaultname,
			S32 flags )
{
	S32		n, num ;
	S32		flag, handle ;
	S32		mode = 0 ;
	unsigned	total ;
	unsigned	testdrive = 0 ;
	S32		rcode ;
	S32		oldnum = -1 ;
	char		memopath[_MAX_PATH] ;
	char		workstring[256] ;
	S32		memoasciimode ;

	// inits

	MemoClip() ;
	UnsetClip() ;

	// memo drive/dir org
	getcwd( memopath, _MAX_PATH ) ;

	// init directory fichier par defaut
	strncpy( SelectorPathname , defaultname, _MAX_PATH-1 ) ;
	_splitpath( SelectorPathname , SDrive, SDir, SName, SExt ) ;

	if( strlen( SDir ) > 2 )
	{
		if( SDir[strlen(SDir)-1] == '\\' ) SDir[strlen(SDir)-1] = 0 ;
	}

	if( SDrive[0] != 0 )
	{
		SDrive[0] &= ~0x20 ;

		_dos_setdrive( SDrive[0]-'A'+1, &total ) ;
		_dos_getdrive( &testdrive ) ;
		if( testdrive == SDrive[0]-'A'+1 )
		{
			chdir( SDir ) ;
		}
		else
		{
			Message( "Drive Error", TRUE ) ;
		}
	}
	else	chdir( SDir ) ;

	strcpy( SelectorPathname, SName ) ;

	_splitpath( selection, SDrive, SDir, SName, SExt ) ;
	strcat( SelectorPathname, SExt ) ;

	getcwd( SearchPathname, _MAX_PATH ) ;

	// window

	if( !OpenMenu( &Selector, 32,20 ) )	return 0 ;
	AddText(   &Selector,	  0, 0, 32, 1, FLAG_CENTRE+FLAG_CONTOUR, titre ) ;
	AddText(   &Selector,	  0, 1,  3, 1, NO_FLAG, "Path" ) ;
	AddButton( &Selector, 0,  3, 1, 29, 1, NO_FLAG, SearchPathname ) ;
	AddText(   &Selector,	  0, 2,  3, 1, NO_FLAG, "File" ) ;
	AddButton( &Selector, 4,  3, 2, 29, 1, FLAG_RED, SelectorPathname  ) ;

	AddWindow( &Selector,10,  0, 4, 16,14, NO_FLAG ) ;

	AddWindow( &Selector,11, 16, 4, 16,14, NO_FLAG ) ;

	AddText(   &Selector,	  0, 3, 16, 1, FLAG_CENTRE+FLAG_CONTOUR, selection ) ;
	AddButton( &Selector, 2, 17, 3,  7, 1, FLAG_CENTRE+FLAG_PUSHED, "Dirs" ) ;
	AddSwitch( &Selector, 7, 16, 3,  1, 1, NO_FLAG, "S", &FlagScanDir, 1 ) ;
	AddButton( &Selector, 3, 24, 3,  8, 1, FLAG_CENTRE, "Drives" ) ;

	AddButton( &Selector, 5, 16,18, 16, 2, FLAG_CENTRE+FLAG_CONTOUR, "Cancel" ) ;
	AddButton( &Selector, 6,  0,18, 16, 2, FLAG_CENTRE+FLAG_CONTOUR+FLAG_RED, "OK" ) ;

	DrawMenu( &Selector, -1,-1 ) ;
	ShowMouse( 1 ) ;

	BoxUpdate()	;

	IndexFiles = (char**)Malloc( MAX_FILES * sizeof(char*) ) ;
	IndexDirs = (char**)Malloc( MAX_DIRS * sizeof(char*) ) ;
	ListFiles = (char*)Malloc( MAX_FILES * _MAX_NAME ) ;
	ListDirs = (char*)Malloc( MAX_DIRS * _MAX_NAME   ) ;
	// test erreur malloc
	if( !IndexFiles OR !IndexDirs OR !ListFiles OR !ListDirs )
	{
		if(IndexFiles)	Free(IndexFiles)	;
		if(IndexDirs)	Free(IndexDirs)		;
		if(ListFiles)	Free(ListFiles)		;
		if(ListDirs)	Free(ListDirs)		;
		Message( "File List Malloc Error", TRUE ) ;
		return 0 ;
	}

	if( !(flags & SELECT_KEEP_POS) )	StartFile = 0 ;

	ScanFiles(selection, flags)	;
	ScanDirs(selection)		;

	while( Click )
	{
		BoxUpdate()	;
	}

	rcode = -1 ;
	flag = 1 ;
	while( flag != -1 )
	{
		if( Key == K_ESC )	rcode = 0 ;
		if( Key == K_ENTER )	rcode = 1 ;

		handle = GereMenu( &Selector ) 	;

		BoxUpdate()			;

		switch( handle )
		{
		case 10:	// files list
			if( Click & 2 )
			{
				GetMouseDep() ;
				while( Click & 2 )
				{
					GetMouseDep() ;
					StartFile -= MouseYDep ;
					if( StartFile < 0 )	StartFile = 0 ;
					DrawListFiles(flags) ;
					BoxUpdate()	;
				}
				oldnum = -1 ;
			}
			else
			{
				num = GetIndexWinList( &FilesWindow ) + StartFile ;
				if( oldnum != -1 )
				{
					if( num == oldnum )	// double click
					{
						rcode = 1 ;
						break ;
					}
					WinSelect( &FilesWindow, oldnum-StartFile, BLACK ) ; // efface
					BoxUpdate()	;
				}
				if( num != -1 )
				{
					WinSelect( &FilesWindow, num-StartFile, WHITE ) ;
					oldnum = num ;
					if( flags & SELECT_SHOW_EXT )
					{
						strcpy( SelectorPathname, IndexFiles[num] ) ;
					}
					else
					{
						strcpy( SelectorPathname, IndexFiles[num] ) ;
						_splitpath( selection, SDrive, SDir, SName, SExt ) ;
						strcat( SelectorPathname, SExt ) ;
					}
					ChangeButtonString( &Selector, 4, SelectorPathname , TRUE ) ;
					while( Click )
					{
						BoxUpdate()	;
					}
				}
			}
			break ;

		case 11:	// dirs/drive list
			num = GetIndexWinList( &DirsWindow ) ;
			if( num != -1 )
			{
				WinSelect( &DirsWindow, num, WHITE ) ;

				BoxUpdate()	;

				if( !mode )	// dir
				{
					chdir( IndexDirs[num] ) ;
					getcwd( SearchPathname, _MAX_PATH ) ;
					ChangeButtonString( &Selector, 0, SearchPathname, TRUE ) ;
					ScanDirs( selection ) ;
				}
				else		// drive
				{
					n = IndexDirs[num][0] - 'A' + 1 ;
					memoasciimode = AsciiMode ;
					AsciiMode = TRUE ;
					_dos_setdrive( n, &total ) ;
					getcwd( SearchPathname, _MAX_PATH ) ;
					AsciiMode = memoasciimode ;
					ScanDrives() ;
				}
				getcwd( SearchPathname, _MAX_PATH ) ;
				ChangeButtonString( &Selector, 0, SearchPathname, TRUE ) ;
				ScanFiles( selection, flags ) ;
				oldnum = -1 ;
				while( Click )
				{
					BoxUpdate()	;
				}
			}
			break ;

		case 0: // input new searchpath
			InputString( &Selector, 0, _MAX_PATH ) ;
			_splitpath( SearchPathname, SDrive,SDir,SName,SExt ) ;
			_dos_setdrive( SDrive[0]-'A'+1, &total ) ;
			chdir( SDir ) ;
			getcwd( SearchPathname, _MAX_PATH ) ;
			ChangeButtonString( &Selector, 0, SearchPathname, TRUE ) ;
			ScanFiles( selection, flags ) ;
			ScanDirs( selection ) ;
			oldnum = -1 ;
			break ;

		case 2: // dirs
			ChangeButtonFlags( &Selector, 2, FLAG_CENTRE+FLAG_PUSHED, 1 ) ;
			ChangeButtonFlags( &Selector, 3, FLAG_CENTRE, 1 ) ;
			ScanDirs( selection ) ;
			mode = 0 ;
			oldnum = -1 ;
			break ;

		case 3: // drives
			ChangeButtonFlags( &Selector, 2, FLAG_CENTRE, 1 ) ;
			ChangeButtonFlags( &Selector, 3, FLAG_CENTRE+FLAG_PUSHED, 1 ) ;
			ScanDrives() ;
			mode = 1 ;
			oldnum = -1 ;
			break ;

		case 4: // selected
			InputString( &Selector, 4, _MAX_PATH ) ;
			_splitpath( SelectorPathname, SDrive, SDir, SName, SExt ) ;
			strcpy( SelectorPathname, SName ) ;
			_splitpath( selection, SDrive,SDir,SName,SExt ) ;
			strcat( SelectorPathname, SExt ) ;
			ChangeButtonString( &Selector, 4, SelectorPathname, 1 ) ;
			oldnum = -1 ;
			break ;

		case 7: // auto scan dir on/off
			if( FlagScanDir )
			{
				if( !mode )	// dir
					ScanDirs( selection ) ;
				else		// drive
					ScanDrives() ;
			}
			break ;


		case 6: // ok
			rcode = 1 ;
			break ;

		case 5: // cancel
			rcode = 0 ;
			break ;
		}


		if( rcode == 1 )
		{
			_splitpath( SelectorPathname, SDrive, SDir, SName, SExt ) ;
			strcpy( SelectorPathname, SearchPathname ) ;
			if ( strlen(SelectorPathname) > 3 )
				strcat( SelectorPathname, "\\" ) ;// 28-04-1995 cause crash racine!!!
			strcat( SelectorPathname, SName ) ;
			strcat( SelectorPathname, SExt ) ;

			if( flags & SELECT_TEST_EXIST )
			{
				if( FileSize( SelectorPathname ) != 0L )
				{
					strcpy( defaultname, SelectorPathname ) ;
					flag = -1 ;	// OK
				}
				else
				{
					strcpy( workstring, SelectorPathname ) ;
					strcat( workstring, " n'existe pas !" ) ;
					Message( workstring, TRUE ) ;
				}
			}
			else if( flags & SELECT_TEST_OVERWRITE )
			{
				if( FileSize( SelectorPathname ) != 0L )
				{
					strcpy( workstring, SelectorPathname ) ;
					strcat( workstring, " existe déjà !" ) ;
					if( Confirm(workstring, "Ecrase", "Oups") == 1 )
					{	// ok
						strcpy( defaultname, SelectorPathname ) ;
						flag = -1 ;
					}
				}
				else
				{
					strcpy( defaultname, SelectorPathname ) ;
					flag = -1 ;	// OK
				}
			}
			else
			{
				strcpy( defaultname, SelectorPathname ) ;
				flag = -1 ;	// OK
			}

			if( flag != -1 )
			{
				DrawMenu( &Selector, -1,-1 ) ;
				ScanFiles( selection, flags ) ;
				if( !mode )	// dir
				{
					ScanDirs( selection ) ;
				}
				else		// drive
				{
					ScanDrives() ;
				}
				rcode = -1 ;
			}
		}

		if( rcode == 0 )
		{
			strcpy( SelectorPathname, "" ) ;
			flag = -1 ;
		}
	}

	Free( IndexFiles ) ;
	Free( ListFiles ) ;
	Free( IndexDirs ) ;
	Free( ListDirs ) ;

	do
	{
		BoxUpdate()	;
	} while( Click OR Key )	;

	CloseMenu( &Selector ) 	;

	RestoreClip() 		;

	// restore drive/dir org
	_dos_setdrive( memopath[0]-'A'+1, &total ) ;
	chdir( memopath ) ;

	return rcode ;
}

//══════════════════════════════════════════════════════════════════════════

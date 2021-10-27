#include	"\projet\lib386\lib_sys\adeline.h"
#include	"\projet\lib386\lib_sys\lib_sys.h"
#include	"\projet\lib386\lib_svga\lib_svga.h"
#include	"\projet\lib386\lib_menu\lib_menu.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys\types.h>
#include	<direct.h>
#include	<string.h>
#include	<dos.h>

#ifndef	WATCOM9
#include	<mem.h>
#endif

#define	MAX_FILES	500	
#define	MAX_DIRS	100

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*
    Ûßßßß  Û    Û     Ûßßßß ÛÛßßß Ûßßßß Û     Ûßßßß Ûßßßß ßßÛßß ÛßßßÛ ÛßßßÛ
    ÛÛßß   ÛÛ   ÛÛ    ÛÛßß  ßßßßÛ ÛÛßß  ÛÛ    ÛÛßß  ÛÛ      ÛÛ  ÛÛ  Û ÛÛßÛß
    ßß     ßß   ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß ßßßßß   ßß  ßßßßß ßß  ß
 *ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

UBYTE	SelectorPathname [_MAX_PATH] ;
UBYTE	SDrive[_MAX_DRIVE] ;
UBYTE	SDir[_MAX_DIR] ;
UBYTE	SName[_MAX_FNAME] ;
UBYTE	SExt[_MAX_EXT] ;

WORD	NbFiles ;
UBYTE	*ListFiles ;
UBYTE	**IndexFiles ;
WORD	NbDirs ;
UBYTE	*ListDirs ;
UBYTE	**IndexDirs ;

WORD	StartFile = 0 ;

UBYTE	SearchPathname[_MAX_PATH] ;

T_MENU	Selector ;

T_WINLIST	FilesWindow ;
T_WINLIST	DirsWindow ;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	CutFileName( UBYTE *filename )
{
	while( (*filename!=0) AND (*filename!='.') )
		filename++ ;
	*filename = 0 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	DrawListFiles( WORD flag )
{
	WORD	nb ;

	ShowMouse( 0 ) ;
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
	ShowMouse( 1 ) ;
}

/*
int my_sort_function( char **a, char **b)
{
	return( strcmp( *a, *b) ) ;
}
*/

void	ScanFiles( UBYTE *selection, LONG flag )
{
	struct	find_t	fileinfo ;
	ULONG	rc ;
	UBYTE	*ptr ;
	UBYTE	path[_MAX_PATH] ;

	_splitpath( selection, SDrive, SDir, SName, SExt ) ;
	_makepath( path, "", SearchPathname, SName, SExt ) ;

	rc = _dos_findfirst( path, _A_NORMAL, &fileinfo ) ;
	NbFiles = 0 ;
	ptr = ListFiles ;
	while( !rc )
	{
		IndexFiles[NbFiles] = ptr ;
		if( !(flag & SELECT_SHOW_EXT) )	CutFileName( fileinfo.name ) ;
		strcpy( ptr, fileinfo.name ) ;
		ptr += strlen( fileinfo.name ) + 1 ;
		NbFiles++ ;
		if( NbFiles == MAX_FILES )
		{
			Message( "Warning: Truncated List", TRUE ) ;
			break ;
		}
		rc = _dos_findnext( &fileinfo ) ;
	}

	if( NbFiles )	/* ? */
		qsort( (void *)IndexFiles, NbFiles, sizeof( char * ), MySortCompFunc );

	DrawListFiles( flag ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

WORD	ThereIsFiles( UBYTE *path )
{
	struct	find_t	fileinfo ;

	if( !_dos_findfirst( path, _A_NORMAL, &fileinfo ) )
		return 1 ;
	else
		return 0 ;
}

void	ScanDirs( UBYTE *selection )
{
	struct	find_t	fileinfo ;
	ULONG	rc ;
	UBYTE	*ptr ;
	WORD	n ;
	UBYTE	path[_MAX_PATH] ;

	rc = _dos_findfirst( "*.*", _A_SUBDIR, &fileinfo ) ;
	NbDirs = 0 ;
	ptr = ListDirs ;
	while( !rc )
	{
		if( fileinfo.attrib & _A_SUBDIR )
		{
			IndexDirs[NbDirs] = ptr ;
			strcpy( ptr, fileinfo.name ) ;
			ptr += strlen( fileinfo.name ) + 1 ;
			NbDirs++ ;
			if( NbDirs == MAX_FILES )
			{
				Message( "Warning: Truncated List", TRUE ) ;
				break ;
			}
		}
		rc = _dos_findnext( &fileinfo ) ;
	}

	ShowMouse( 0 ) ;
	GetWindowButton( &Selector, 11, &DirsWindow ) ;
	WinList( &DirsWindow, IndexDirs, NbDirs, 14 ) ;
	FlipWindow( &DirsWindow ) ;

	/* if option scan files */

	for( n=0; n<NbDirs; n++ )
	{
		strcpy( path, IndexDirs[n] ) ;
		strcat( path, "\\" ) ;
		strcat( path, selection ) ;
		if( ThereIsFiles( path ) )
		{
			WinSelect( &DirsWindow, n, B_ROUGE+1 ) ;
		}
	}

	ShowMouse( 1 ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void	ScanDrives()
{
	unsigned	total ;
	unsigned	drive ;
	unsigned	orgdrive ;
	WORD		n ;
	UBYTE		*ptr ;

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
			ptr[0] = drive + 'A' -1 ;
			ptr += 3 ;
			NbDirs++ ;
		}
	}
	_dos_setdrive( orgdrive, &total ) ;

	ShowMouse( 0 ) ;
	GetWindowButton( &Selector, 11, &DirsWindow ) ;
	WinList( &DirsWindow, IndexDirs, NbDirs, 14 ) ;
	FlipWindow( &DirsWindow ) ;
	ShowMouse( 1 ) ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

LONG	NumDrive( UBYTE *drive )
{
	return ((*drive)&=~20)-'A'+1 ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

WORD	FileSelector(	UBYTE *titre,
			UBYTE *selection,
			UBYTE *defaultname,
			LONG flags )
{
	WORD	n, num ;
	WORD	flag, handle ;
	WORD	mode = 0 ;
	unsigned	total ;
	unsigned	testdrive = 0 ;
	WORD	rcode ;
	WORD	oldnum = -1 ;
	char	memopath[_MAX_PATH] ;
	UBYTE	workstring[256] ;
	WORD	memoasciimode ;

	/* inits */

	MemoClip() ;
	UnSetClip() ;

	/* memo drive/dir org */
	getcwd( memopath, _MAX_PATH ) ;

	/* init directory fichier par defaut */
	strcpy( SelectorPathname , defaultname ) ;
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

	/* window */

	if( !OpenMenu( &Selector, 32,20 ) )	return 0 ;
	AddText(   &Selector,     0, 0, 32, 1, FLAG_CENTRE+FLAG_CONTOUR, titre ) ;
	AddText(   &Selector,     0, 1,  3, 1, NO_FLAG, "Path" ) ;
	AddButton( &Selector, 0,  3, 1, 29, 1, NO_FLAG, SearchPathname ) ;
	AddText(   &Selector,     0, 2,  3, 1, NO_FLAG, "File" ) ;
	AddButton( &Selector, 4,  3, 2, 29, 1, FLAG_RED, SelectorPathname  ) ;

	AddWindow( &Selector,10,  0, 4, 16,14, NO_FLAG ) ;

	AddWindow( &Selector,11, 16, 4, 16,14, NO_FLAG ) ;

	AddText(   &Selector,     0, 3, 16, 1, FLAG_CENTRE+FLAG_CONTOUR, selection ) ;
	AddButton( &Selector, 2, 16, 3,  8, 1, FLAG_CENTRE+FLAG_PUSHED, "Dirs" ) ;
	AddButton( &Selector, 3, 24, 3,  8, 1, FLAG_CENTRE, "Drives" ) ;

	AddButton( &Selector, 5, 16,18, 16, 2, FLAG_CENTRE+FLAG_CONTOUR, "Cancel" ) ;
	AddButton( &Selector, 6,  0,18, 16, 2, FLAG_CENTRE+FLAG_CONTOUR+FLAG_RED, "OK" ) ;

	DrawMenu( &Selector, -1,-1 ) ;
	ShowMouse( 1 ) ;

	IndexFiles = Malloc( MAX_FILES * sizeof(UBYTE*) ) ;
	IndexDirs = Malloc( MAX_DIRS * sizeof(UBYTE*) ) ;
	ListFiles = Malloc( MAX_FILES * 13 ) ;
	ListDirs = Malloc( MAX_DIRS * 13 ) ;
	/* test erreur malloc */
	if( !IndexFiles OR !ListFiles OR !IndexDirs OR !ListDirs )
	{
		Message( "File List Malloc Error", TRUE ) ;
		return 0 ;
	}

	if( !(flags & SELECT_KEEP_POS) )	StartFile = 0 ;

	ScanFiles( selection, flags ) ;
	ScanDirs( selection ) ;

	while( Click ) AffMouse() ;

	rcode = -1 ;
	flag = 1 ;
	while( flag != -1 )
	{
		if( Key == K_ESC )	rcode = 0 ;
		if( Fire == F_RETURN )	rcode = 1 ;

		AffMouse() ;

		handle = GereMenu( &Selector ) ;

		switch( handle )
		{
		case 10:	/* files list */
			if( Click & 2 )
			{
				GetMouseDep() ;
				while( Click & 2 )
				{
					GetMouseDep() ;
					StartFile -= Mouse_Y_Dep ;
					if( StartFile < 0 )	StartFile = 0 ;
					DrawListFiles(flags) ;
				}
				oldnum = -1 ;
			}
			else
			{
			num = GetIndexWinList( &FilesWindow ) + StartFile ;
			if( oldnum != -1 )
			{
				if( num == oldnum )	/* double click */
				{
					rcode = 1 ;
					break ;
				}
				WinSelect( &FilesWindow, oldnum-StartFile, 0 ) ; /* efface */
			}
			if( num != -1 )
			{
				WinSelect( &FilesWindow, num-StartFile, 15 ) ;
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
				while( Click ) AffMouse() ;
			}
			}
			break ;

		case 11:	/* dirs/drive list */
			num = GetIndexWinList( &DirsWindow ) ;
			if( num != -1 )
			{
				WinSelect( &DirsWindow, num, 15 ) ;

				if( !mode )	/* dir */
				{
					chdir( IndexDirs[num] ) ;
					getcwd( SearchPathname, _MAX_PATH ) ;
					ChangeButtonString( &Selector, 0, SearchPathname, TRUE ) ;
					ScanDirs( selection ) ;
				}
				else		/* drive */
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
				while( Click ) AffMouse() ;
			}
			break ;

		case 0:	/* input new searchpath */
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

		case 2:	/* dirs */
			ChangeButtonFlags( &Selector, 2, FLAG_CENTRE+FLAG_PUSHED, 1 ) ;
			ChangeButtonFlags( &Selector, 3, FLAG_CENTRE, 1 ) ;
			ScanDirs( selection ) ;
			mode = 0 ;
			oldnum = -1 ;
			break ;

		case 3:	/* drives */
			ChangeButtonFlags( &Selector, 2, FLAG_CENTRE, 1 ) ;
			ChangeButtonFlags( &Selector, 3, FLAG_CENTRE+FLAG_PUSHED, 1 ) ;
			ScanDrives() ;
			mode = 1 ;
			oldnum = -1 ;
			break ;

		case 4:	/* selected */
			InputString( &Selector, 4, _MAX_PATH ) ;
			_splitpath( SelectorPathname, SDrive, SDir, SName, SExt ) ;
			strcpy( SelectorPathname, SName ) ;
			_splitpath( selection, SDrive,SDir,SName,SExt ) ;
			strcat( SelectorPathname, SExt ) ;
			ChangeButtonString( &Selector, 4, SelectorPathname, 1 ) ;
			oldnum = -1 ;
			break ;

		case 6: /* ok */
			rcode = 1 ;
			break ;

		case 5:	/* cancel */
			rcode = 0 ;
			break ;
		}


		if( rcode == 1 )
		{
			_splitpath( SelectorPathname, SDrive, SDir, SName, SExt ) ;
			strcpy( SelectorPathname, SearchPathname ) ;
			strcat( SelectorPathname, "\\" ) ;
			strcat( SelectorPathname, SName ) ;
			strcat( SelectorPathname, SExt ) ;

			if( flags & SELECT_TEST_EXIST )
			{
				if( FileSize( SelectorPathname ) != 0L )
				{
					strcpy( defaultname, SelectorPathname ) ;
					flag = -1 ;	/* OK */
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
					strcat( workstring, " existe d‚j… !" ) ;
					if( Confirm(workstring, "Ecrase", "Oups") == 1 )
					{	/* ok */
						strcpy( defaultname, SelectorPathname ) ;
						flag = -1 ;
					}
				}
				else
				{
					strcpy( defaultname, SelectorPathname ) ;
					flag = -1 ;	/* OK */
				}
			}
			else
			{
				strcpy( defaultname, SelectorPathname ) ;
				flag = -1 ;	/* OK */
			}

			if( flag != -1 )
			{
				DrawMenu( &Selector, -1,-1 ) ;
				ScanFiles( selection, flags ) ;
				if( !mode )	/* dir */
				{
					ScanDirs( selection ) ;
				}
				else		/* drive */
				{
					ScanDrives() ;
				}
				ShowMouse( 1 ) ;
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

	CloseMenu( &Selector ) ;

	while( Click OR Key OR Fire ) AffMouse() ;

	RestoreClip() ;

	/* restore drive/dir org */
	_dos_setdrive( memopath[0]-'A'+1, &total ) ;
	chdir( memopath ) ;

	return rcode ;
}

/*ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ*/

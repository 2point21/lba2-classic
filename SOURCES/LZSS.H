#ifndef	LZSS_H
#define	LZSS_H

#define INDEX_BIT_COUNT      12
#define LENGTH_BIT_COUNT     4
#define WINDOW_SIZE          ( 1 << INDEX_BIT_COUNT )
#define RAW_LOOK_AHEAD_SIZE  ( 1 << LENGTH_BIT_COUNT )
#define BREAK_EVEN           ( ( 1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT ) / 9 )
#define LOOK_AHEAD_SIZE      ( RAW_LOOK_AHEAD_SIZE + BREAK_EVEN )
#define TREE_ROOT            WINDOW_SIZE
#define UNUSED               -1
#define MOD_WINDOW( a )      ( ( a ) & ( WINDOW_SIZE - 1 ) )

#ifdef __cplusplus
extern "C" {
#endif

extern	int Current_position;
#pragma	aux	Current_position	"*"

extern	int Match_position;
#pragma	aux	Match_position	"*"

/*
 * These are the two global data structures used in this program.
 * The window[] array is exactly that, the window of previously seen
 * text, as well as the current look ahead text.  The tree[] structure
 * contains the binary tree of all of the strings in the window sorted
 * in order.
*/

extern	unsigned char window[ WINDOW_SIZE * 5 ];
#pragma	aux	window	"*"

struct deftree {
    long parent;
    long smaller_child;
    long larger_child;
} ;

extern	struct deftree tree [ WINDOW_SIZE + 2 ];
#pragma	aux	tree	"*"

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	int	AddString();

#pragma	aux	AddString	"*"	\
		parm caller []		\
		modify [eax ecx edx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
extern	int	DeleteString(int p);

#pragma	aux	DeleteString	"*"	\
		parm caller []		\
		modify [eax ecx edx]

//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

#ifdef __cplusplus
}
#endif

extern void InitTree(int r);
extern unsigned long Compress_LZSS(char *input,char *output,unsigned long length);

#endif	// LZSS_H

/************************** Start of LZSS.C ************************
 *
 * This is the LZSS module, which implements an LZ77 style compression
 * algorithm.  As iplemented here it uses a 12 bit index into the sliding
 * window, and a 4 bit length, which is adjusted to reflect phrase lengths
 * of between 2 and 17 bytes.
 */

/* 			 ASSEZ FORTEMENT MODIFIE
			       LE 21/06/93
*/

#include	"c_extern.h"

/*
 * Various constants used to define the compression parameters.  The
 * INDEX_BIT_COUNT tells how many bits we allocate to indices into the
 * text window.  This directly determines the WINDOW_SIZE.  The
 * LENGTH_BIT_COUNT tells how many bits we allocate for the length of
 * an encode phrase. This determines the size of the look ahead buffer.
 * The TREE_ROOT is a special node in the tree that always points to
 * the root node of the binary phrase tree.  END_OF_STREAM is a special
 * index used to flag the fact that the file has been completely
 * encoded, and there is no more data.  UNUSED is the null index for
 * the tree. MOD_WINDOW() is a macro used to perform arithmetic on tree
 * indices.
 *
 */

char *CompressionName = "LZSS Encoder";
char *Usage           = "in-file out-file\n\n";

int Current_position;
int Match_position;

/*
 * These are the two global data structures used in this program.
 * The window[] array is exactly that, the window of previously seen
 * text, as well as the current look ahead text.  The tree[] structure
 * contains the binary tree of all of the strings in the window sorted
* in order.
*/

unsigned char window[ WINDOW_SIZE * 5 ];

struct deftree tree[ WINDOW_SIZE + 2 ] ;

/*
 * Function prototypes for both ANSI C compilers and their K&R brethren.
 */

void InitTree( int r );
// void ContractNode( int old_node, int new_node );
// void ReplaceNode( int old_node, int new_node );
// int FindNextNode( int node );
// void DeleteString( int p );
// int AddString();
unsigned long Compress(char *input,char *output, unsigned long length );

/*
 * However, to make the tree really usable, a single phrase has to be
 * added to the tree so it has a root node.  That is done right here.
*/
void InitTree( int r )
{
    int i;

    for (i=0; i<=WINDOW_SIZE; i++) {
	tree[ i ].parent = UNUSED;
	tree[ i ].larger_child = UNUSED;
	tree[ i ].smaller_child = UNUSED;
    }
    tree[ TREE_ROOT ].larger_child = r;
    tree[ r ].parent = TREE_ROOT;
}

/*
 * This routine is used when a node is being deleted.  The link to
 * its descendant is broken by pulling the descendant in to overlay
 * the existing link.
 */

/*
void ContractNode( old_node, new_node )
int old_node;
int new_node;
{
    tree[ new_node ].parent = tree[ old_node ].parent;
    if ( tree[ tree[ old_node ].parent ].larger_child == old_node )
	tree[ tree[ old_node ].parent ].larger_child = new_node;
    else
	tree[ tree[ old_node ].parent ].smaller_child = new_node;
    tree[ old_node ].parent = UNUSED;
}
*/
/*
 * This routine is also used when a node is being deleted.  However,
 * in this case, it is being replaced by a node that was not previously
 * in the tree.
 */
/*
void ReplaceNode( old_node, new_node )
int old_node;
int new_node;
{
    int parent;

    parent = tree[ old_node ].parent;
    if ( tree[ parent ].smaller_child == old_node )
	tree[ parent ].smaller_child = new_node;
    else
	tree[ parent ].larger_child = new_node;
    tree[ new_node ] = tree[ old_node ];
    if ( tree[ new_node ].smaller_child != UNUSED )
	tree[ tree[ new_node ].smaller_child ].parent = new_node;
    if ( tree[ new_node ].larger_child != UNUSED )
	tree[ tree[ new_node ].larger_child ].parent = new_node;
    tree[ old_node ].parent = UNUSED;
}
*/
/*
 * This routine is used to find the next smallest node after the node
 * argument.  It assumes that the node has a smaller child.  We find
 * the next smallest child by going to the smaller_child node, then
 * going to the end of the larger_child descendant chain.
*/
/*
int FindNextNode( node )
int node;
{
    int next;

    next = tree[ node ].smaller_child;
    while ( tree[ next ].larger_child != UNUSED )
	next = tree[ next ].larger_child;
    return( next );
}
*/
/*
 * This routine performs the classic binary tree deletion algorithm.
 * If the node to be deleted has a null link in either direction, we
 * just pull the non-null link up one to replace the existing link.
 * If both links exist, we instead delete the next link in order, which
 * is guaranteed to have a null link, then replace the node to be deleted
 * with the next link.
 */
/*
void DeleteStringOld( p )
int p;
{
    int  replacement;

    if ( tree[ p ].parent == UNUSED )
	return;
    if ( tree[ p ].larger_child == UNUSED )
	ContractNode( p, tree[ p ].smaller_child );
    else if ( tree[ p ].smaller_child == UNUSED )
	ContractNode( p, tree[ p ].larger_child );
    else {
	replacement = FindNextNode( p );
	DeleteString( replacement );
	ReplaceNode( p, replacement );
    }
}
*/
/*
 * This where most of the work done by the encoder takes place.  This
 * routine is responsible for adding the new node to the binary tree.
 * It also has to find the best match among all the existing nodes in
 * the tree, and return that to the calling routine.  To make matters
 * even more complicated, if the new_node has a duplicate in the tree,
 * the old_node is deleted, for reasons of efficiency.
 */
/*
int AddStringOld()
{
	int	i;
	int	test_node;
	int	delta;
	int	match_length;
	long	*child;


	test_node = tree[ TREE_ROOT ].larger_child;
	match_length = 0;
	for ( ; ; )
	{
		for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ )
		{
	    		delta =   window[ MOD_WINDOW( Current_position + i ) ]
				- window[ MOD_WINDOW( test_node + i ) ];
	    		if ( delta != 0 )
				break;		// sort de la boucle FOR
		}



		if ( i >= match_length )
		{
	    		match_length = i;
	    		Match_position = test_node;

			if ( match_length >= LOOK_AHEAD_SIZE )
			{
				ReplaceNode( test_node, Current_position );
				return( match_length );
			}

		}

		if ( delta >= 0 )
	    		child = &tree[ test_node ].larger_child;
		else
	    		child = &tree[ test_node ].smaller_child;
		if ( *child == UNUSED )
		{
	    		*child = Current_position;
	    		tree[ Current_position ].parent = test_node;
	    		tree[ Current_position ].larger_child = UNUSED;
	    		tree[ Current_position ].smaller_child = UNUSED;
	    		return( match_length );
		}
		test_node = *child;
    	}
}


*/




/*
 * This is the compression routine.  It has to first load up the look
 * ahead buffer, then go shorto the main compression loop.  The main loop
 * decides whether to output a single character or an index/length
 * token that defines a phrase.  Once the character or phrase has been
 * sent out, another loop has to run.  The second loop reads in new
 * characters, deletes the strings that are overwritten by the new
 * character, then adds the strings that are created by the new
 * character.
 */

unsigned long Compress_LZSS(char *input,char *output, unsigned long length )
{
int 		i;
char 		*read;
char 		*write;
char 		*info;
int 		look_ahead_bytes;
int 		replace_count;
int 		match_length;
int 		count_bits	= 0;
char 		mask		= 1;
unsigned long 	len		= 0;
unsigned long 	save_length;



	save_length = length;
	write = output;
	read = input;
	Current_position = 0;

	for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ )
	{
		if (length == 0 ) break;
		window[ Current_position + i ] = *read++;
		length--;
	}

	look_ahead_bytes = i;
	InitTree( Current_position );
	match_length = 0;
	Match_position = 0;
	info = write++;

	if ( ++len >= save_length ) return( save_length );

	*info = 0;

	while ( look_ahead_bytes > 0 )
	{
		if ( match_length > look_ahead_bytes )
    			match_length = look_ahead_bytes;

		if ( match_length <= BREAK_EVEN )
		{
    			replace_count = 1;
    			*info |= mask;
    			*write++ = window[ Current_position ];
    			if ( ++len >= save_length )
			return( save_length );
		}
		else
		{
    			if ( ( len = len + 2 ) >= save_length )
			return( save_length );


			*(short *)write =
		(short)(( MOD_WINDOW( Current_position - Match_position - 1 )
		<< LENGTH_BIT_COUNT ) | ( match_length - BREAK_EVEN - 1 ));

			write+=2;
    			replace_count = match_length;
		}

		if (++count_bits == 8)
		{
    			if ( ++len >= save_length ) return( save_length );
    			info = write++;
    			*info = 0;
    			count_bits = 0;
    			mask = 1;
		}
		else mask = (char)(mask << 1);

		for ( i = 0 ; i < replace_count ; i++ )
		{
    			DeleteString( MOD_WINDOW( Current_position + LOOK_AHEAD_SIZE ) );
    			if ( length == 0 ) look_ahead_bytes--;
    			else
    			{
				window[ MOD_WINDOW( Current_position + LOOK_AHEAD_SIZE ) ]
				= *read++;
				length--;
    			}

			Current_position = MOD_WINDOW( Current_position + 1 );
    			if ( look_ahead_bytes )
				match_length = AddString();
		}
	}

	if ( count_bits == 0 ) len--;
	return( len );
}

/************************** End of LZSS.C *************************/


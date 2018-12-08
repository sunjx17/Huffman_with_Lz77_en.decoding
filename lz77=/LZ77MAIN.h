
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "getopt.h"
#include "bitio.h"
#include "lz77.h"
/***************************************************************************
*                                CONSTANTS
***************************************************************************/
#define MIN_LA_SIZE 2       /* min lookahead size */
#define MAX_LA_SIZE 255     /* max lookahead size */
#define MIN_SB_SIZE 0       /* min search buffer size */
#define MAX_SB_SIZE 65535   /* max search buffer size */

/***************************************************************************
*                            TYPE DEFINITIONS
***************************************************************************/
typedef enum{
	ENCODE,
	DECODE
} MODES;
int main_encdec(char from[],char to[],MODES mode);
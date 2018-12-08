/***************************************************************************
*          Lempel, Ziv Encoding and Decoding
*
*   File    : main.c
*   Authors : David Costa and Pietro De Rosa
*
*                              DESCRIPTION
*
*   LZ77 is a lossless data compression algorithms published by Abraham 
*   Lempel and Jacob Ziv in 1977. It is a dictionary coder and maintains a
*   sliding window during compression. The sliding window is divided in two
*   parts: Search-Buffer (dictionary - encoded data) and Lookahead (uncomp-
*   ressed data). LZ77 algorithms achieve compression by addressing byte 
*   sequences from former contents instead of the original data. All data 
*   will be coded in the same form (called token): -Address to already 
*   coded contents; -Sequence length; -First deviating symbol.
*   The window is contained in a fixed size buffer.
*   The match between SB and LA is made by a binary tree, implemented in an
*   array.
***************************************************************************/

/***************************************************************************
*                             INCLUDED FILES
***************************************************************************/
#include"LZ77MAIN.h"
#pragma warning(disable:4996)


/***************************************************************************
*                            USER INTERFACE
* Syntax: ./lz77 <options>
* Options: -c: compression mode
*          -d: decompression mode
*          -i <filename>: input file
*          -o <filename>: output file
*          -l <value> : lookahead size (default 15)
*          -s <value> : search-buffer size (default 4095)
*          -h: help
***************************************************************************/
int main(){
	main_encdec("Ma.bmp","out.lz",ENCODE);
	main_encdec("out.lz","out.lzs",ENCODE);
}
int main_encdec(char from[],char to[],MODES mode)
{
	/* variables */
	//int opt;
	//FILE *file = NULL;
	//struct bitFILE *bitF = NULL;
	//MODES mode = -1;
	//char *filenameIn = NULL, *filenameOut = NULL;
	//int la_size = -1, sb_size = -1; /* default size */
	//
	//    while ((opt = getopt(argc, argv, "cdi:o:l:s:h")) != -1)
	//    {
	//        switch(opt)
	//        {
	//            case 'c':       /* compression mode */
	//                mode = ENCODE;
	//                break;
	//                
	//            case 'd':       /* decompression mode */
	//                mode = DECODE;
	//                break;
	//                
	//            case 'i':       /* input file name */
	//                if (filenameIn != NULL){
	//                    fprintf(stderr, "Multiple input files not allowed.\n");
	//                    goto error;
	//                }
	//                filenameIn = malloc(strlen(optarg) + 1);
	//                strcpy(filenameIn, optarg);
	//                
	//                break;
	//                
	//            case 'o':       /* output file name */
	//                if (filenameOut != NULL){
	//                    fprintf(stderr, "Multiple output files not allowed.\n");
	//                    goto error;
	//                }
	//                filenameOut = malloc(strlen(optarg) + 1);
	//                strcpy(filenameOut, optarg);
	//                
	//                break;
	//                
	//            case 'l':       /* lookahead size */
	//                la_size = atoi(optarg);
	//                if (la_size < MIN_LA_SIZE || la_size > MAX_LA_SIZE){
	//                    fprintf(stderr, "Bad lookahead size value.\n");
	//                    goto error;
	//                }
	//                break;
	//                
	//            case 's':       /* search-buffer size */
	//                sb_size = atoi(optarg);
	//                if (sb_size < MIN_SB_SIZE || sb_size > MAX_SB_SIZE){
	//                    fprintf(stderr, "Bad search-buffer size value.\n");
	//                    goto error;
	//                }
	//                break;
	//                
	//            case 'h':       /* help */
	//                printf("Usage: lz77 <options>\n");
	//                printf("  -c : Encode input file to output file.\n");
	//                printf("  -d : Decode input file to output file.\n");
	//                printf("  -i <filename> : Name of input file.\n");
	//                printf("  -o <filename> : Name of output file.\n");
	//                printf("  -l <value> : Lookahead size (default 15)\n");
	//                printf("  -s <value> : Search-buffer size (default 4095)\n");
	//                printf("  -h : Command line options.\n\n");
	//                break;
	//                
	//        }
	//    }
	//    
	//    /* validate command line */
	//    if (filenameIn == NULL){
	//        fprintf(stderr, "Input file must be provided\n");
	//        goto error;
	//    }else if (filenameOut == NULL)
	//    {
	//        fprintf(stderr, "Output file must be provided\n");
	//        goto error;
	//    }
	//    
	struct bitFILE*bitf=NULL;
	FILE*in=NULL;
	if (mode == ENCODE){
		//        if ((file = fopen(filenameIn, "rb")) == NULL){
		//            perror("Opening input file");
		//            goto error;
		//        }
		//        if ((bitF = bitIO_open(filenameOut, BIT_IO_W)) == NULL) {
		//            perror("Opening output file");
		//            goto error;
		//        }

		if ((in=fopen(from,"rb")) == NULL){
			perror("Opening input file");
			goto error;
		}

		if ((bitf = bitIO_open(to, BIT_IO_W)) == NULL) {
			perror("Opening output file");
			goto error;
		}

		encode(in, bitf, -1, -1);
		//            
	}
	else if (mode == DECODE){
		if ((bitf = bitIO_open(from, BIT_IO_R)) == NULL) {
			            perror("Opening input file");
			            goto error;
		}
		if ((in = fopen(to, "w")) == NULL){
			            perror("Opening output file");
			            goto error;
		}
		decode(bitf, in);
		//            
		//    }else{
		//        fprintf(stderr, "Select ENCODE or DECODE mode\n");
		//        goto error;
	}
	//    
	fclose(in);
	bitIO_close(bitf);
	return 0;
	//    
	//    /* handle error */
error:
	if (in != NULL){
		fclose(in);
	}
	if (bitf != NULL){
		bitIO_close(bitf);
	}
	return 1;
	//    exit (EXIT_FAILURE);
}
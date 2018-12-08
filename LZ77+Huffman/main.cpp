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
#include"Huffman.h"
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
#define BUF_S 4096
int myenc_(char f[],char t[],char lzs){
	int size=getfilelen(f);
	int reads=(size-1)/BUF_S+1;
	FILE *fr=nullptr,*tr=nullptr;
	fr=fopen(f,"rb");
	if(fr==nullptr){
		perror("error opening file.");
		exit(0);
	}
	tr=fopen(f,"wb");
	if(tr==nullptr){
		perror("error opening file.");
		exit(0);
	}
	//	char sv[4];
	fwrite(&lzs,1,1,tr);
	for(int i=0;i<reads;i++){
		char buf[BUF_S];
		size_t rsize=fread(buf,1,BUF_S,fr);
		fwrite(buf,1,rsize,tr);
	}
	fclose(fr);
	fclose(tr);
	return 0;
}
int mudec_(char f[],char t[],int &lzs){

	return 0;
}
template<typename T>
void ex(T& a,T& b){ T c=a;a=b;b=c;//a = a^b; b = a^b; a = a^b;
}
int main(
	int argc,char**argv
	){
		/*int argc=4;
		char *(argv[10])={"","er","out1","e"};
		char *(argv[10])={"","out1","out","d"};*/
		char *tmp="tmp.o",*tmp2="tmp2.o";
		char *from,*to;
		MODES mode;
		int la=-1,sb=-1;
		char lzrnds=1;
		if(argc>3){
			from=argv[1];
			to=argv[2];
			if(argv[3][0]=='e'){
				mode=ENCODE;

				if(argc>=6){
					la=atoi(argv[4]);
					sb=atoi(argv[5]);
					if(la<4 || la>256){
						printf("arg 4:la val noe compatible(4-256),using default 15.\n");
						la=-1;
					}
					if(sb<1024 || sb>256000){
						printf("arg 5:sb val noe compatible(1024-256000),using default 4095.\n");
						sb=-1;
					}
					if(argc>=7){
						lzrnds=atoi(argv[6]);
						if(lzrnds>4 ){
							printf("lz77 too much,may be not good.\n");
							lzrnds=4;
						}
						else if( lzrnds<0){
							printf("lz77 <0 error.\n");
							lzrnds=1;
						}
					}

				}
				switch (lzrnds)
				{
				case 1:
					if(main_encdec(from,tmp,mode,la,sb)!=0){
						perror("Errors. exit.");
						exit(1);
					};
					if(huffman_encode(tmp,to,lzrnds)!=0){
						perror("Errors. exit.");
						//unlink (tmp);
						exit(0);
					};
					////unlink (tmp);
					break;
				case 2:
					if(main_encdec(from,tmp2,mode,la,sb)!=0){
						perror("Errors. exit.");
						////unlink(tmp2);
						exit(1);
					};
					if(0!=main_encdec(tmp2,tmp,mode,la,sb)){
						perror("Errors. exit.");
						//unlink(tmp2);
						exit(1);
					};
					////unlink(tmp2);
					if(huffman_encode(tmp,to,lzrnds)!=0){
						perror("Errors. exit.");
						//unlink (tmp);
						//unlink (tmp2);
						exit(1);
					};
					//unlink (tmp);
					//unlink (tmp2);
					break;
				case 3:
					if(main_encdec(from,tmp2,mode,la,sb)!=0){
						perror("Errors. exit.");
						////unlink(tmp2);
						exit(1);
					};
					if(0!=main_encdec(tmp2,tmp,mode,la,sb)){
						perror("Errors. exit.");
						//unlink(tmp2);
						exit(1);
					};
					if(0!=main_encdec(tmp,tmp2,mode,la,sb)){
						perror("Errors. exit.");
						//unlink(tmp);
						//unlink(tmp2);
						exit(1);
					};
					////unlink(tmp2);
					if(huffman_encode(tmp2,to,lzrnds)!=0){
						perror("Errors. exit.");
						//unlink (tmp2);
						//unlink(tmp);
						exit(1);
					};
					//unlink (tmp2);
					//unlink(tmp);
					break;
				case 4:
					if(main_encdec(from,tmp2,mode,la,sb)!=0){
						perror("Errors. exit.");
						////unlink(tmp2);
						exit(1);
					};
					if(0!=main_encdec(tmp2,tmp,mode,la,sb)){
						perror("Errors. exit.");
						//unlink(tmp2);
						exit(1);
					};
					if(0!=main_encdec(tmp,tmp2,mode,la,sb)){
						perror("Errors. exit.");
						//unlink (tmp2);
						//unlink(tmp);
						exit(1);
					};
					if(0!=main_encdec(tmp2,tmp,mode,la,sb)){
						perror("Errors. exit.");
						//unlink (tmp2);
						//unlink(tmp);
						exit(1);
					};
					////unlink(tmp2);
					if(huffman_encode(tmp,to,lzrnds)!=0){
						perror("Errors. exit.");
						//unlink (tmp2);
						//unlink(tmp);
						exit(1);
					};
					//unlink (tmp2);
					//unlink(tmp);
					break;
				default:
					perror("unrecognized error.");
					exit(1);
					break;
				}


			}
			else if(argv[3][0]=='d'){
				mode=DECODE;
				size_t h=huff_deccode(from,tmp);
				for(size_t i=0;i<h-1;++i){
					ex(tmp,tmp2);
					main_encdec(tmp2,tmp,mode,0,0);
					////unlink(tmp2);
				}
				main_encdec(tmp,to,mode,0,0);
				////unlink(tmp);
			}
			else{
				perror("Please input a mode");
				exit(1);
			}
		}
		else{
			perror("args must be 3+");
			printf("usage: fromfile tofile mode str_len window LZ77times");
			exit(1);
		}
		return 0;
		//main_encdec("Ma.bmp","out.lz",ENCODE);
		//main_encdec("out.lz","out.lzs",ENCODE);
}
int main_encdec(char from[],char to[],MODES mode,int la,int sb)
{
 
	struct bitFILE*bitf=NULL;
	FILE*in=NULL;
	if (mode == ENCODE){

		if ((in=fopen(from,"rb")) == NULL){
			perror("Opening input file");
			goto error;
		}

		if ((bitf = bitIO_open(to, BIT_IO_WB)) == NULL) {
			perror("Opening output file");
			goto error;
		}
		encode(in, bitf, la, sb);
		//            
	}
	else if (mode == DECODE){
		if ((bitf = bitIO_open(from, BIT_IO_R)) == NULL) {
			perror("Opening input file");
			goto error;
		}
		if ((in = fopen(to, "wb")) == NULL){
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
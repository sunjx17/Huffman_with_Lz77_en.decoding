
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include <windows.h>
#include <io.h>
#include <sys\stat.h>
#include "bitio.h"
typedef unsigned char mytype;
typedef unsigned int big_size;
//int comp(const void*a,const void*b){
//	return ((Huff_nodes*)a)->wei-((Huff_nodes*)b)->wei;
//}
int getfilelen(char filen[]);
class Huff_nodes{
public:
	bool IsLeave;
	big_size wei;
	Huff_nodes(big_size weight,bool isl);
};
class Huff_pars:public Huff_nodes{
public:

	Huff_nodes*lch,*rch;
	Huff_pars(Huff_nodes *lptr,Huff_nodes *rptr,big_size weight);
};
class Huff_leaves:public Huff_nodes{
public:

	mytype value;
	Huff_leaves(big_size weight,mytype svalue);
};
class Huff_tree{

public:
	Huff_pars *root;
	Huff_tree(size_t weights[],mytype values[],size_t number);
	~Huff_tree();
	void delete_me(Huff_nodes *root);
};
void print(Huff_nodes *root);



void pint(size_t v,unsigned char s[]);



#define INT_TYPE 0xff00ff00
void encode_int(size_t ns[],unsigned char cs[],size_t num,unsigned char *result);
#define CHAR_TYPE 0xf0f0f0f0
void encode_char(unsigned char ns[],unsigned char cs[],size_t num,unsigned char *result);




#define CHAR_TYPE_NODICT 0x0ff0f00f
void encode_char_nodict(const unsigned char ns[],unsigned char *result);
#define INT_TYPE_NODICT 0x0ff0f00f
void encode_int_nodict(const size_t ns[],unsigned int bits,unsigned char *result);

unsigned char getbit(unsigned char Dic[],unsigned int pos);
void setbit(unsigned char *s,unsigned int pos,unsigned char val);
void setbits(unsigned char *s,unsigned char dict[32],unsigned char dict_size,unsigned int&pos);
void setbits_bitfile(bitFILE*s,unsigned char dict[32],size_t dict_size);
void get_dict(unsigned char dict[256][32],unsigned char dict_size[256],unsigned char tmpdict[32],unsigned char nowlen,const Huff_nodes *node);
unsigned int getuint(unsigned char *s);
int huffman_encode(char file[],char to[],char lzs);

unsigned int huff_deccode(char*from,char*to);
	//int _rmain(int argc,char**argv){
	//	if(argc==3){
	//		_smain(argv[1],argv[2]);
	//	}
	//	else{
	//		char f[400],t[400];
	//		gets(f);
	//		gets(t);
	//		_smain(f,t);
	//		getchar();getchar();
	//	}
	//
	//}
	//int _tmain(){
	//	size_t nums;
	//	//printf("input number:\n");
	//	scanf("%u",&nums);
	//	//printf("input weighs and values:\n");
	//	size_t *w=new size_t[nums];
	//	mytype *v=new mytype[nums];
	//	for(size_t i=0;i<nums;i++){
	//		scanf("%u%c",w+i,v+i);
	//	}
	//	Huff_tree *hf_t=new Huff_tree(w,v,nums);
	//	//print(hf_t->root);
	//	delete hf_t;
	//	putchar('\n');
	//	getchar();getchar();
	//	return 1;
	//}
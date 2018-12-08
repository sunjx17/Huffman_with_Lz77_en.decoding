#include "Huffman.h"
#pragma warning(disable:4996)

#define MAX_SIZE (1024*4)
#define HEAD_SIZE (256*4+4+4)
int getfilelen(char filen[]){
	struct _stat info;
	_stat(filen, &info);
	if(info.st_size<=0){
		perror("getfilelen error,empty file?");
		exit(1);
	}
	return info.st_size;
}
Huff_nodes::Huff_nodes(big_size weight,bool isl):wei(weight),IsLeave(isl){}

Huff_pars::Huff_pars(Huff_nodes *lptr,Huff_nodes *rptr,big_size weight):lch(lptr),rch(rptr),Huff_nodes(weight,false){}

Huff_leaves::Huff_leaves(big_size weight,mytype svalue):Huff_nodes(weight,true),value(svalue){};
Huff_tree::Huff_tree(size_t weights[],mytype values[],size_t number):root(nullptr){
	size_t ns=0;
	for(size_t i=0;i<number;++i){
		if(weights[i]!=0)ns++;
	}

	Huff_nodes **hf=new Huff_nodes*[ns];
	for(size_t i=0,j=0;i<number;++i){
		if(weights[i]!=0){
			hf[j]=new Huff_leaves(weights[i],values[i]);
			j++;
		}
	}
	number=ns;
	//qsort(hf,number,sizeof(Huff_nodes*),comp);
	size_t min,dmin;
	for(size_t i=0;i<number-1;i++){
		min=i,dmin=i+1;
		for(size_t j=dmin;j<number;j++){
			if(hf[j]->wei < hf[min]->wei){
				dmin=min;
				min=j;
			}
			else if(hf[j]->wei <hf[dmin]->wei){
				dmin=j;
			}
		}
		root=new Huff_pars(hf[min],hf[dmin],hf[min]->wei+hf[dmin]->wei);
		hf[min]=root;
		hf[dmin]=hf[i];
		hf[i]=nullptr;

	}
	delete [] hf;
}
Huff_tree::~Huff_tree(){
	delete_me(root);
}
void Huff_tree::delete_me(Huff_nodes *root){
	if(root!=nullptr){
		if(root->IsLeave){
			delete root;
		}
		else{
			delete_me(((Huff_pars*)root)->lch);
			delete_me(((Huff_pars*)root)->rch);
			delete root;
		}
	}
}

void print(Huff_nodes *root){
	if(root!=nullptr){
		if(root->IsLeave){
			//printf("(%lu,%d)",root->wei,((Huff_leaves*)root)->value);
		}
		else{
			//printf("(%u){",root->wei);
			print(((Huff_pars*)root)->lch);
			putchar('|');
			print(((Huff_pars*)root)->rch);
			putchar('}');
		}
	}
}



 void pint(size_t v,unsigned char s[]){
	s[0]=v & 0xff;
	s[1]=(v & 0xff00)>>8;
	s[2]=(v & 0xff0000)>>16;
	s[3]=(v & 0xff000000)>>24;
}




void encode_int(size_t ns[],unsigned char cs[],size_t num,unsigned char *result){
	pint(INT_TYPE,result);
	result+=4;
	pint(num,result);
	result+=4;
	for(size_t i=0;i<num;++i){
		pint(ns[i],result);
		result+=4;
	}
	memcpy(result,cs,num);
}

void encode_char(unsigned char ns[],unsigned char cs[],size_t num,unsigned char *result){
	pint(CHAR_TYPE,result);
	result+=4;
	pint(num,result);
	result+=4;
	memcpy(result,ns,num);
	memcpy(result+num,cs,num);
}





void encode_char_nodict(const unsigned char ns[],unsigned char *result){
	pint(CHAR_TYPE_NODICT,result);
	result+=4;
	memcpy(result,ns,256);
}

void encode_int_nodict(const size_t ns[],unsigned int bits,unsigned char *result){
	pint(CHAR_TYPE_NODICT,result);
	result+=4;
	pint(bits,result);
	result+=4;
	//printf("%x %u...\n",CHAR_TYPE_NODICT,bits);
	for(size_t i=0;i<256;++i){
		pint(ns[i],result);
		result+=4;
	}
}
size_t encode_nodct(const size_t ns[],unsigned int bits,unsigned char *result){
	unsigned int r=0u;
	for(size_t i=0;i<256;i++){
		r|=ns[i];
	}
	unsigned char codev=4;
	unsigned char s[4];
	pint(r,s);
	for(size_t i=3;i>=0;--i){
		if(s[i]==0){
			codev--;

		}
		else{
			break;
		}
	}
	if(codev==0)codev=1;
	result[0]=codev;
	result++;
	pint(bits,result);
	result+=4;
	for(size_t i=0;i<256;i++){
		pint(ns[i],s);
		for(size_t j=0;j<codev;j++){
			*result=s[j];
			result++;
		}
	}
	return codev;
}
unsigned char getbit(unsigned char Dic[],unsigned int pos){
	size_t ns=pos>>3;
	size_t bits=pos&0x07;
	unsigned char t=1<<bits;
	return (Dic[ns]&t)?1:0;
}
void setbit(unsigned char *s,unsigned int pos,unsigned char val){
	size_t ns=pos>>3;
	size_t bits=pos&0x07;
	if(val==1){
		val<<=bits;
		s[ns]|=val;
	}
	else{
		val=~(1<<bits);
		s[ns]&=val;
	}
}
void setbits(unsigned char *s,unsigned char dict[32],unsigned char dict_size,unsigned int&pos){
	for(size_t i=0;i<dict_size;i++){
		setbit(s,pos,getbit(dict,i));
		pos++;
	}

}
 void setbits_bitfile(bitFILE*s,unsigned char dict[32],size_t dict_size){
	//for(size_t i=0;i<dict_size;i++){
	//setbit(s,pos,getbit(dict,i));
	bitIO_write(s,dict,dict_size);
	//pos++;
	//}

}
void get_dict(unsigned char dict[256][32],unsigned char dict_size[256],unsigned char tmpdict[32],unsigned char nowlen,const Huff_nodes *node){
	if(node->IsLeave){
		memcpy(dict[((Huff_leaves*)node)->value],tmpdict,32);
		dict_size[((Huff_leaves*)node)->value]=nowlen;
	}
	else{
		setbit(tmpdict,nowlen,0);
		get_dict(dict,dict_size,tmpdict,nowlen+1,((Huff_pars*)node)->lch);
		setbit(tmpdict,nowlen,1);
		get_dict(dict,dict_size,tmpdict,nowlen+1,((Huff_pars*)node)->rch);
	}
}
unsigned int getuint(unsigned char *s){
	unsigned int t=0;
	t|=s[0];
	t|=(s[1]<<8);
	t|=(s[2]<<16);
	t|=(s[3]<<24);
	return t;
}
int huffman_encode(char file[],char to[],char lzs){
	////printf("%d\n",sizeof(unsigned long long));
	FILE *f=NULL;
	bitFILE*ot;


	int size =getfilelen(file);
	int readcnts=(size-1)/MAX_SIZE+1;
	//if(size<0 || size>MAX_SIZE){
	//	perror("huffman can not deal with this file.\n");
	//	return 1;
	//}

	unsigned char head[HEAD_SIZE];
	//unsigned char *tmp=new unsigned char[size];
	unsigned char tmp[MAX_SIZE];
	unsigned char sdic[256];

	size_t weis[256];

	size_t n;

	for(unsigned int i=0;i<256;i++){
		sdic[i]=i;
	}

	f=fopen(file,"rb");
	if(f==NULL){
		perror("huffman can not open this file.\n");
		//delete [] tmp;
		return 1;
	}
	ot=bitIO_open(to,BIT_IO_WB);
	if(ot==NULL){
		perror("huffman can not open output file.\n");
		//delete [] tmp;
		return 1;
	}
	//fwrite(lzs,1,1,ot);
	bitIO_write(ot,&lzs,8);
	memset(weis,0,256*sizeof(weis[0]));
	for(int i=0;i<readcnts;i++){
		n=fread(tmp,1,MAX_SIZE,f);
		for(size_t i=0;i<n;++i){
			weis[tmp[i]]++;
		}
	}
	rewind(f);



	Huff_tree *hf=new Huff_tree(weis,sdic,256);
	//print(hf->root);
	memset(sdic,0,256);


	//unsigned char outtmp[MAX_SIZE];
	unsigned char dict[256][32]={0};
	unsigned char tmpdic[32];
	memset(tmpdic,0,32);

	get_dict(dict,sdic,tmpdic,0,hf->root);

	/*for(size_t i=0;i<256;i++){
		printf("%u..\n",weis[i]);
	}*/
	delete hf;
	size_t nowpos=0;
	size_t u=encode_nodct(weis,nowpos,head);//Head encoding finished
	setbits_bitfile(ot,head,(256*u+1+4)*8);
	for(int i=0;i<readcnts;i++){
		n=fread(tmp,1,MAX_SIZE,f);
		for(size_t i=0;i<n;++i){
			setbits_bitfile(ot,dict[tmp[i]],sdic[tmp[i]]);
			nowpos+=sdic[tmp[i]];
		}
	}
	unsigned char tmp4[4]="\0\0\0";
	setbits_bitfile(ot,tmp4,4*8);
	pint(nowpos,tmp4);
	for(int i=0;i<4;++i){
		bitIO_w_putchar(ot,2+i,tmp4+i);
	}

	/*if(f==NULL){
	perror("huffman can not write output.\n");
	delete [] tmp;
	delete [] outtmp;
	return 1;
	}*/
	/*fwrite(head,sizeof(unsigned char),256*4+4+4,f);
	fwrite(outtmp,sizeof(unsigned char),now_pos/8+1,f);
	delete [] outtmp;*/
	//delete [] tmp;
	fclose(f);
	bitIO_close(ot);

	return 0;
	//getchar();getchar();
}

unsigned int huff_deccode(char*from,char*to){
	int size = getfilelen(from);
	FILE*f=nullptr;
	bitFILE*sou=nullptr;
//	unsigned char buff[MAX_SIZE];
	sou=bitIO_open(from,BIT_IO_R);
	if(sou==nullptr){
		perror("huffman can not open this file.\n");
		//getchar();getchar();
		exit(0);
	}
	f=fopen(to,"wb");
	if(f==nullptr){
		perror("huffman can not open this file.\n");
		//getchar();getchar();
		exit(0);
	}
	

	unsigned char lz77s=0;
	bitIO_read(sou,&lz77s,1,8);
	unsigned char codelen=0;
	bitIO_read(sou,&codelen,1,8);

	unsigned char q[4];
	bitIO_read(sou,q,4,32);
	unsigned int bitsizes=getuint(q);

	unsigned char sdict[256]={0};
	for(unsigned int i=0;i<256;i++){
		sdict[i]=i;
	}
	unsigned int diclens[256];
	for(size_t i=0;i<256;i++){
		unsigned char v[4]={0};
		bitIO_read(sou,v,4,codelen*8);
		diclens[i]=getuint(v);
		//printf("%u\n",diclens[i]);
	}
	Huff_tree *hf=new Huff_tree(diclens,sdict,256);
	Huff_nodes *now=hf->root;

	
	unsigned char outdic[MAX_SIZE];
	
	size_t now_out=0;
	for(size_t i=0;i<bitsizes;i++){
		bool tmp=bitIO_readbit(sou);
		if(tmp){
			now=((Huff_pars*)now)->rch;
			//putchar('1');
		}
		else{
			now=((Huff_pars*)now)->lch;
			//putchar('0');
		}
		if(now->IsLeave){
			if(now_out>=MAX_SIZE){
				fwrite(outdic,1,MAX_SIZE,f);
				now_out=0;
			}
			outdic[now_out]=((Huff_leaves*)now)->value;
			now=hf->root;
			now_out++;
		}
	}

	if(now_out>0)fwrite(outdic,1,now_out,f);
	fclose(f);
	bitIO_close(sou);
	delete hf;
	//delete [] outdic;
	return lz77s;
}


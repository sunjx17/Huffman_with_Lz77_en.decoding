#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include <windows.h>
#include <io.h>
#include <sys\stat.h>
typedef unsigned char mytype;
typedef unsigned int big_size;
//int comp(const void*a,const void*b){
//	return ((Huff_nodes*)a)->wei-((Huff_nodes*)b)->wei;
//}

class Huff_nodes{
public:
	bool IsLeave;
	big_size wei;
	Huff_nodes(big_size weight,bool isl):wei(weight),IsLeave(isl){}
};
class Huff_pars:public Huff_nodes{
public:

	Huff_nodes*lch,*rch;
	Huff_pars(Huff_nodes *lptr,Huff_nodes *rptr,big_size weight):lch(lptr),rch(rptr),Huff_nodes(weight,false){}
};
class Huff_leaves:public Huff_nodes{
public:

	mytype value;
	Huff_leaves(big_size weight,mytype svalue):Huff_nodes(weight,true),value(svalue){};
};
class Huff_tree{

public:
	Huff_pars *root;
	Huff_tree(size_t weights[],mytype values[],size_t number):root(nullptr){
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
	~Huff_tree(){
		delete_me(root);
	}
	void delete_me(Huff_nodes *root){
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
};
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



inline void pint(size_t v,unsigned char s[]){
	s[0]=v & 0xff;
	s[1]=(v & 0xff00)>>8;
	s[2]=(v & 0xff0000)>>16;
	s[3]=(v & 0xff000000)>>24;
}



#define INT_TYPE 0xff00ff00
void encode_int(size_t ns[],unsigned char cs[],size_t num,unsigned char *result){
	pint(INT_TYPE,result);
	result+=4;
	pint(num,result);
	result+=4;
	for(int i=0;i<num;++i){
		pint(ns[i],result);
		result+=4;
	}
	memcpy(result,cs,num);
}
#define CHAR_TYPE 0xf0f0f0f0
void encode_char(unsigned char ns[],unsigned char cs[],size_t num,unsigned char *result){
	pint(CHAR_TYPE,result);
	result+=4;
	pint(num,result);
	result+=4;
	memcpy(result,ns,num);
	memcpy(result+num,cs,num);
}




#define CHAR_TYPE_NODICT 0x0ff0f00f
void encode_char_nodict(const unsigned char ns[],unsigned char *result){
	pint(CHAR_TYPE_NODICT,result);
	result+=4;
	memcpy(result,ns,256);
}
#define INT_TYPE_NODICT 0x0ff0f00f
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
#define MAX_SIZE 60*1024*1024
int _smain(char file[],char to[]){
	////printf("%d\n",sizeof(unsigned long long));
	FILE *f;

	struct _stat info;
	_stat(file, &info);
	size_t size = info.st_size;
	unsigned char head[257*4];
	unsigned char *tmp=new unsigned char[size];

	unsigned char sdic[256];

	size_t weis[256];

	size_t n;

	for(unsigned int i=0;i<256;i++){
		sdic[i]=i;
	}

	f=fopen(file,"rb");
	n=fread(tmp,1,size,f);
	memset(weis,0,256*sizeof(weis[0]));
	fclose(f);


	for(size_t i=0;i<n;++i){
		weis[tmp[i]]++;
	}


	Huff_tree *hf=new Huff_tree(weis,sdic,256);
	//print(hf->root);
	memset(sdic,0,256);


	unsigned char *outtmp=new unsigned char[size+1024];
	unsigned char dict[256][32]={0};
	unsigned char tmpdic[32];
	memset(tmpdic,0,32);

	get_dict(dict,sdic,tmpdic,0,hf->root);

	for(size_t i=0;i<256;i++){
		//printf("%u..\n",weis[i]);
	}
	delete hf;

	size_t now_pos=0;
	for(size_t i=0;i<n;++i){
		setbits(outtmp,dict[tmp[i]],sdic[tmp[i]],now_pos);
	}
	encode_int_nodict(weis,now_pos,head);//Head encoding finished
	f=fopen(to,"wb");
	fwrite(head,sizeof(unsigned char),256*4+4+4,f);
	fwrite(outtmp,sizeof(unsigned char),now_pos/8+1,f);
	delete [] outtmp;
	delete [] tmp;
	fclose(f);
	return 0;
	//getchar();getchar();
}



unsigned int getuint(unsigned char *s){
	unsigned int t=0;
	t|=s[0];
	t|=(s[1]<<8);
	t|=(s[2]<<16);
	t|=(s[3]<<24);
	return t;
}
int main(int argc,char**argv){
	if(argc==3){
		_smain(argv[1],argv[2]);
	}
	else{
		char f[400],t[400];
		gets(f);
		gets(t);
		_smain(f,t);
		getchar();getchar();
	}

}
int _tmain(){
	size_t nums;
	//printf("input number:\n");
	scanf("%u",&nums);
	//printf("input weighs and values:\n");
	size_t *w=new size_t[nums];
	mytype *v=new mytype[nums];
	for(size_t i=0;i<nums;i++){
		scanf("%u%c",w+i,v+i);
	}
	Huff_tree *hf_t=new Huff_tree(w,v,nums);
	//print(hf_t->root);
	delete hf_t;
	putchar('\n');
	getchar();getchar();
	return 1;
}
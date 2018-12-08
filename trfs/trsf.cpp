#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <sys\stat.h>
const char UNEX[4]={(char)0x0f,(char)0x1f,(char)0x2f,(char)0x3f};
const char OCCEX[4]={(char)0xf0,(char)0xf1,(char)0xf3,(char)0xf3};
const char UNEX2[4]={(char)0xef,(char)0xdf,(char)0xcf,(char)0xbf};
const char OCCEX2[4]={(char)0xfe,(char)0xfd,(char)0xfc,(char)0xfb};
struct bestoccs{
	int pos;
	char val;
	int len;
	bestoccs():pos(0x8fffffff),val(0xff),len(0){}
} ;
int cmp(const void *a,const void*b){
	return ((struct bestoccs*)a)->len-((struct bestoccs*)b)->len;
}
int cmp2(const void *a,const void*b){
	return ((struct bestoccs*)a)->pos-((struct bestoccs*)b)->pos;
}
inline void pint(int v,char s[]){
	s[0]=v & 0xff;
	s[1]=(v & 0xff00)>>8;
	s[2]=(v & 0xff0000)>>16;
	s[3]=(v & 0xff000000)>>24;
}
void _smain(char*from,char*to){
	FILE *f;

	struct _stat info;
	_stat(from, &info);
	int size = info.st_size;
	if(size>0){
		char *s=new char [size];
		//char *v=new char[size];
		f=fopen(from,"rb");
		fread(s,1,size,f);
		fclose(f);

		/*struct bestoccs occ;*/
		int lelens=1;
		int lastunpos=0;
		int lastfnpos=0;
		int vpos=0;
		bool has=false;
		f=fopen(to,"wb");
		char sv[4];
		for(int i=0;i<size-1;++i){
			if(s[i]==s[i+1]){
				if(lelens==1){

					lastfnpos=i;
				}
				lelens++;
			}
			else{

				if(lelens>8){
					has=true;
					if(lastfnpos!=lastunpos){
						pint(lastfnpos-lastunpos,sv);
						int v;
						for(v=3;v>=0;){
							if(sv[i]!=0)break;
							v++;
						}
						//v[vpos]=UNEX;
						fwrite(&(UNEX[v]),1,1,f);
						//vpos++;

						//pint(lastfnpos-lastunpos,v+vpos);

						fwrite(sv,1,v+1,f);
						//vpos+=4;
						fwrite(s+lastunpos,1,lastfnpos-lastunpos,f);
						//memcpy(v+vpos,s+lastunpos,lastfnpos-lastunpos);
						//vpos+=(lastfnpos-lastunpos);
					}
					//v[vpos]=OCCEX;
					
					//vpos++;
					pint(lelens,sv);
					int v;
					for(v=3;v>=0;){
						if(sv[i]!=0)break;
						v++;
					}
					fwrite(OCCEX+v,1,1,f);
					fwrite(sv,1,v+1,f);
					//pint(lelens,v+vpos);
					//vpos+=4;
					fwrite(s+i,1,1,f);
					//v[vpos]=s[i];
					//vpos++;
					lastunpos=i+1;

				}
				lelens=1;
			}


		}

		if(has==false){
			fwrite(&(UNEX),1,1,f);
			pint(size,sv);
			fwrite(sv,1,4,f);
			fwrite(s,1,size,f);
		}
		else{
			if(lelens<=8){
				fwrite(&(UNEX),1,1,f);
				pint(size-lastunpos,sv);
				fwrite(sv,1,4,f);
				fwrite(s,1,size-lastunpos,f);
			}
		}
		fclose(f);
		delete []s;
		//delete []v;
	}

}
void _smain_2(char*from,char*to){
	FILE *f;

	struct _stat info;
	_stat(from, &info);
	int size = info.st_size;
	if(size>0){
		char *s=new char [size];
		//char *v=new char[size];

		f=fopen(from,"rb");
		fread(s,1,size,f);
		fclose(f);

		/*struct bestoccs occ;*/
		int lelens=1;
		int lastunpos=0;
		int lastfnpos=0;
		int vpos=0;
		bool has=false;
		f=fopen(to,"wb");
		char sv[4];
		for(int i=0;i<size-3;++i){
			if(s[i]==s[i+2] && s[i+1]==s[i+3]){
				if(lelens==1){
					lastfnpos=i;
				}
				lelens++;
			}
			else{

				if(lelens>=5){
					has=true;
					if(lastfnpos!=lastunpos){
						pint(lastfnpos-lastunpos,sv);
						int v;
						for(v=3;v>=0;){
							if(sv[i]!=0)break;
							v++;
						}
						//v[vpos]=UNEX;
						fwrite(&(UNEX[v]),1,1,f);
						//vpos++;

						//pint(lastfnpos-lastunpos,v+vpos);

						fwrite(sv,1,v+1,f);
						//vpos+=4;
						fwrite(s+lastunpos,1,lastfnpos-lastunpos,f);
						//memcpy(v+vpos,s+lastunpos,lastfnpos-lastunpos);
						//vpos+=(lastfnpos-lastunpos);
					}
					//v[vpos]=OCCEX;
					
					//vpos++;
					pint(lelens,sv);
					int v;
					for(v=3;v>=0;){
						if(sv[i]!=0)break;
						v++;
					}
					fwrite(OCCEX+v,1,1,f);
					fwrite(sv,1,v+1,f);
					//pint(lelens,v+vpos);
					//vpos+=4;
					fwrite(s+i,1,1,f);
					//v[vpos]=s[i];
					//vpos++;
					lastunpos=i+1;

				}
				lelens=1;
			}


		}

		if(has==false){
			fwrite(&(UNEX),1,1,f);
			pint(size,sv);
			fwrite(sv,1,4,f);
			fwrite(s,1,size,f);
		}
		else{
			if(lelens<=8){
				fwrite(&(UNEX),1,1,f);
				pint(size-lastunpos,sv);
				fwrite(sv,1,4,f);
				fwrite(s,1,size-lastunpos,f);
			}
		}
		fclose(f);
		delete []s;
		//delete []v;
	}

}


int main(int argc,char**argv){
	if(argc==3){
		_smain(argv[1],argv[2]);
	}
	else{
		char f[400]="v.txt",t[400]="o.txt";
		//gets(f);
		//gets(t);
		_smain(f,t);
		getchar();getchar();
	}
}

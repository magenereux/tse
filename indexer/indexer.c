/* indexer.c --- 
 * 
 * 
 * Author: Sherrina Y. Hwang
 * Created: Thu Oct 22 10:59:31 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pageio.h"
#include "webpage.h"
#include "hash.h"
#include "queue.h"
#include "indexio.h"
#include <dirent.h>

static int sum=0;

typedef struct wordCount {
	char *key;
	queue_t *Docs; //queue of docs that have the word
} wordCount_t;

//entry in queue of docs
typedef struct docCount {
	int DocID;
	int count;//number of times word occurs in Doc
} docCount_t;


int NormalizeWord(char *word) {
	if (strlen(word)<3){
		free(word);
		return 1;
	}
	for (int i=0;i<strlen(word);i++) {
		if (isalpha(word[i])==0){
			return 1;
		} else {
			word[i]=tolower(word[i]);
		}
	}
	return 0;
}
bool searchfn(void *indx, const void* searchword) {
	wordCount_t *w1=(wordCount_t *)indx;
	char *w2=(char*)searchword;
	if (strcmp(w1->key,w2)==0){
		return true;
	}
	return false;
}
bool searchID(void *indx, const void* searchword) {
	docCount_t *d1 = (docCount_t*)indx;
	int* d2=(int*)searchword;
	if (d1->DocID==*d2){
		return true;
	}
	return false;
}
void freeWords(void *ip){
	wordCount_t *p;
	p = (wordCount_t *)ip;
	free(p->key);
	p->key = NULL;
}
void sumQueue(void* ip) {
	docCount_t *d;
	d = (docCount_t *)ip;
	sum=sum+d->count;
}
//count in docCount struct find pointer and get entry
void sumWords(void* ip) {
	wordCount_t *p;
	queue_t *d;
	p = (wordCount_t *)ip;
	d = p->Docs;
	qapply(d,sumQueue);
}

void closeQueue(void *ip){
	wordCount_t *p;
	p = (wordCount_t *)ip;
	qclose(p->Docs);
	p->Docs=NULL;
}

int main (int argc, char* argv[]) {
	if (argc!=3) {                                                                  
    printf("usage: indexer <pagedir><indexnm>\n");                        
    return -1;                                                                    
  }                                                                               
  if (argv[1]==NULL||argv[2]==NULL||atoi(argv[1])<0) {             
    printf("usage: indexer <pagedir><indexnm>\n");                        
    return -1;                                                                    
  } 

	//queue_t *filename=qopen();
	char *pagedir = (char *)calloc(strlen(argv[1])+1,sizeof(char));              
  strcpy(pagedir,argv[1]);
	char *dirname = (char *)calloc(strlen(argv[1])+1,sizeof(char));
	strcpy(dirname,argv[2]);
	int countID=0;
	struct dirent *de;
	char path[200];
	sprintf(path,"../%s",pagedir);
	DIR *dr= opendir(path);
	if (dr==NULL)
		return -1;
	while ((de=readdir(dr)) !=NULL){
		//sprintf(path,"%s/%s",path,de->d_name); 
		if (access(path,W_OK)==0 &&strcmp(de->d_name,"..")!=0 && strcmp(de->d_name,".")!=0){
			countID++;	
			//printf("%s\n",de->d_name);
		}
	}
	//printf("%d\n",countID);
	closedir(dr);
	char *word;
	int pos=0;
	hashtable_t *htp=hopen(100);
	hashtable_t *ip=hopen(100);
	for (int ID=1;ID<=countID;ID++) {
		webpage_t *wp=pageload(ID,pagedir);	
		while ((pos=webpage_getNextWord(wp,pos,&word))>0) {
			if (NormalizeWord(word)==0) {
				wordCount_t *target=(wordCount_t*)hsearch(htp,searchfn,word,strlen(word));
				if (target==NULL) {
					wordCount_t *indx=malloc(sizeof(wordCount_t));
					docCount_t *dc = malloc(sizeof(docCount_t));
					indx->key = word;	
					indx->Docs=qopen();
					dc->DocID = ID;
					dc->count = 1;
					qput(indx->Docs,dc);
					hput(htp,(void *)indx,word,strlen(word));
				}	else {
					docCount_t *targetDoc=(docCount_t*)qsearch(target->Docs,searchID,&ID);
					if (targetDoc==NULL) {
						docCount_t *dc = malloc(sizeof(docCount_t));
						dc->DocID = ID;
						dc->count = 1;
						qput(target->Docs,dc);
					} else {			
						targetDoc->count=(targetDoc->count)+1;
					}
					free(word);
				}
			}
		}
		ip=indexload(ID,pagedir);
		indexsave(ip,ID,dirname);
		printf("saved to %s\n",dirname);
		webpage_delete(wp);
		pos=0;
	}

	happly(htp,sumWords);
	printf("sum: %d\n",sum);
	happly(ip,freeWords);
	happly(ip,closeQueue);
	happly(htp,freeWords);
	happly(htp,closeQueue);
	hclose(htp);
	hclose(ip);
  exit(EXIT_SUCCESS);
}

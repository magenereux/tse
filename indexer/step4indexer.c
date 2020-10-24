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

int main (void) {
	int ID=1;
	webpage_t *wp=pageload(ID,"pages");
	char *word;
	int pos=0;
	hashtable_t *htp=hopen(100);
	//open queue of words in index queue_t *q=qopen();
	//qput(q,index);
	while ((pos=webpage_getNextWord(wp,pos,&word))>0) {
		if (NormalizeWord(word)==0) {
			wordCount_t *target=(wordCount_t*)hsearch(htp,searchfn,word,strlen(word));
			if (target==NULL) {
				//printf("hi");
				wordCount_t *indx=malloc(sizeof(wordCount_t));
				docCount_t *dc = malloc(sizeof(docCount_t));
				indx->key = word;	
				indx->Docs=qopen();
				dc->DocID = ID;
				dc->count = 1;
				//printf("%d %d\n",dc->DocID,dc->count);
				qput(indx->Docs,dc);
				//docCount_t *targetDoc=qget(indx->Docs);
				//printf("%d %d\n",targetDoc->DocID,targetDoc->count);
				hput(htp,(void *)indx,word,strlen(word));
				//printf("after put\n");		
			}
			else {
				docCount_t *targetDoc=(docCount_t*)qsearch(target->Docs,searchID,&ID);
				//printf("target ID:%d Count:%d\n",targetDoc->DocID,targetDoc->count);
				targetDoc->count=(targetDoc->count)+1;
				free(word);
			}
		}
	}
	
//pageload every file in pages>create a hashtable/index of words in file
//put those entries into queue? 

	// to print the total sum of counts
	happly(htp,sumWords);
	printf("sum: %d\n",sum);
	happly(htp,freeWords);
	happly(htp,closeQueue);
	webpage_delete(wp);
	hclose(htp);
  exit(EXIT_SUCCESS);
}

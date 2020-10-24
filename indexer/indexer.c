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

static int sum=0;

typedef struct wordCount {
	char *key;
	int count;
} wordCount_t;

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
	//printf("w2=%s\n",w2);
	if (strcmp(w1->key,w2)==0){
		//	printf("%s==%s\n",w1->key,w2);
		return true;
	}
	//printf("%s!=%s\n",w1->key,w2);
	return false;
}

void freeWords(void *ip){
	wordCount_t *p;
	p = (wordCount_t *)ip;
	free(p->key);
	p->key = NULL;
}

void sumWords(void* ip) {
	//printf("in sumwords!");
	wordCount_t *p;
	p = (wordCount_t *)ip;
	sum=sum+p->count;
	//printf("%s %d %d\n",p->key,p->count,sum);
}

int main (void) {
	webpage_t *wp=pageload(1,"pages");
	char *word;
	int pos=0;
	hashtable_t *htp=hopen(100);
	
	while ((pos=webpage_getNextWord(wp,pos,&word))>0) {
		//printf("word: %s\n",word);
		if (NormalizeWord(word)==0) {
			//printf("Normalized word: %s\n",word);
			wordCount_t *target=(wordCount_t*)hsearch(htp,searchfn,word,strlen(word));
			if (target==NULL) {
				wordCount_t *indx=malloc(sizeof(wordCount_t));
				//strcpy(indx->key,word);
				indx->key = word;	
				//printf("adding: %s\n",word);
				indx->count=1;
				//printf("%d\n",indx->count);
				hput(htp,(void *)indx,word,strlen(word));			
			}
			else {
				target->count=(target->count)+1;
				//	printf("found: %s count: %d\n",target->key,target->count);
				free(word);
			}
		}
	}
	
	// to print the total sum of counts
	
	happly(htp,sumWords);
	printf("sum: %d\n",sum);

	happly(htp,freeWords);
	webpage_delete(wp);
	hclose(htp);
  exit(EXIT_SUCCESS);
}

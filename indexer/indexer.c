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

int sum=0;

typedef struct indexed {
	char *key;
	int count;
} indexed_t;

int NormalizeWord(char *word) {
	if (strlen(word)<3)
		return 1;
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
	indexed_t *w1=(indexed_t *)indx;
	//printf("w1=%s\n",w1->key);
	char *w2=(char*)searchword;
	//printf("w2=%s\n",w2);
	if (strcmp(w1->key,w2)==0)
		return true;
	return false;
}

void sumWords(void* ip) {
	//printf("in sumwords!");
	sum=sum+((indexed_t*)ip)->count;
	//printf("%d\n",((indexed_t*)ip)->count);
}

int main (void) {
	webpage_t *wp=pageload(1,"pages");
	char *word;
	int pos=0;
	hashtable_t *htp=hopen(webpage_getHTMLlen(wp));

	while ((pos=webpage_getNextWord(wp,pos,&word))>0) {
		//printf("word: %s\n",word);
		if (NormalizeWord(word)==0) {
			indexed_t *target=(indexed_t*)hsearch(htp,searchfn,word,strlen(word));
			printf("target: %s\n",(char*)target);
			if (target==NULL) {
				indexed_t *indx=malloc(sizeof(indexed_t));
				strcpy(indx->key,word);
				indx->count=0;
				hput(htp,indx,word,strlen(word));			
			} else {
				target->count=(target->count)+1;
				//printf("%s\n",target->word);
			}
		
		free(word);
		}
	}

	// to print the total sum of counts
	happly(htp,sumWords);
				 /*
	int toAdd=0;
	for (int i=0;i<counter;i++) {
		toAdd=htp[i]->count;
		sum+=toAdd;
		} */
	printf("sum: %d\n",sum);

	webpage_delete(wp);
	hclose(htp);
  exit(EXIT_SUCCESS);
}

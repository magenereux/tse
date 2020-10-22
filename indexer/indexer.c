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

bool searchfn(void *word, const void* searchword) {
	char *w1=(char*)word;
	char *w2=(char*)searchword;
	if (strcmp(w1,w2)==0)
		return true;
	return false;
}
/*
void sumofCounts(void *indexed) {
	*sum+=(indexed_t*)indexed->count;
}
*/
typedef struct indexed {
	char *word;
	int count;
} indexed_t;

int main (void) {
	webpage_t *wp=pageload(1,"pages");
	char *word;
	int pos=0;
	hashtable_t *htp=hopen(webpage_getHTMLlen(wp));
	//	indexed_t *target=malloc(sizeof(indexed_t));
	int counter=0;
	while ((pos=webpage_getNextWord(wp,pos,&word))>0) {
	  if (NormalizeWord(word)==0) {
			indexed_t *target=(indexed_t*)hsearch(htp,searchfn,word,strlen(word));
			if (target==NULL) {
				indexed_t *indx=malloc(sizeof(indexed_t));
				indx->word=word;
				indx->count=0;
				hput(htp,word,word,strlen(word));
				counter++;
			} else {
				target->count++;
			}
		free(word);
		}
	}

	// to print the total sum of counts
	int sum=0;
	//happly(htp,sumofCounts;
	int toAdd=0;
	for (int i=0;i<counter;i++) {
		toAdd=((indexed_t*)htp[i]->count);
		sum+=toAdd;
	}
	printf("sum: %d\n",sum);

	webpage_delete(wp);
	hclose(htp);
  exit(EXIT_SUCCESS);
}

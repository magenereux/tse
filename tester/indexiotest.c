/* indexiotest.c --- 
 * 
 * 
 * Author: Sherrina Y. Hwang
 * Created: Wed Oct 21 22:01:16 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "pageio.h"
#include "webpage.h"
#include "indexio.h"
#include "queue.h"

typedef struct wordCount {
	char *key;
	queue_t *Docs; //queue of docs that have the word
} wordCount_t;

//entry in queue of docs
typedef struct docCount {
	int DocID;
	int count;//number of times word occurs in Doc
} docCount_t;

void freeWords(void *ip){
	wordCount_t *p;
	p = (wordCount_t *)ip;
	free(p->key);
	p->key = NULL;
}

void closeQueue(void *ip){
	wordCount_t *p;
	p = (wordCount_t *)ip;
	qclose(p->Docs);
	p->Docs=NULL;
}

int main (void) {
	printf("entering main");
    
    hashtable_t *ip=indexload(3,"index_testfiles");
    indexsave(ip,3, "indexnm");

	//hashtable_t *ip2= hopen(100);
    hashtable_t *ip2=indexload(3,"indexnm");

    happly(ip,freeWords);
	happly(ip,closeQueue);
    hclose(ip);
    happly(ip2,freeWords);
	happly(ip2,closeQueue);
    hclose(ip2);

	return 0;
}
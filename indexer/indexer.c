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
#include "pageio.h"
#include "webpage.h"


int main (void) {
	webpage_t *wp=pageload(1,"pages");
	char *word;
	int pos=webpage_getNextWord(wp,0,&word);
	// printf("pos=%d",pos);
	//	int counter=0;
	FILE *f = fopen("output","w");
	while (pos!=-1) {
		fprintf(f,"%s\n",word);
		free(word);
		pos=webpage_getNextWord(wp,pos,&word);
		//counter++;
	}
	//iterate through word array, free each, free array
	
	webpage_delete(wp);
	return 0;
}

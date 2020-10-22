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

int main (void) {
	webpage_t *wp=pageload(1,"pages");
	char *word;
	int pos=0;

	FILE *f = fopen("output","w");
	while ((pos=webpage_getNextWord(wp,pos,&word))>0) {
	  if (NormalizeWord(word)==0)
			fprintf(f,"%s\n",word);
		free(word);
	}
	//iterate through word array, free each, free array
	webpage_delete(wp);
  fclose(f);
  exit(EXIT_SUCCESS);
}

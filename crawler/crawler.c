/* crawler.c --- 
 * 
 * 
 * Author: Marguerite Genereux
 * Created: Thu Oct 15 09:48:27 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>                                    
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"


bool searchfn(void *elementURL, const void* searchURL){
	webpage_t *e = (webpage_t*)elementURL;
	char *s = (char*)searchURL;
	char *eURL = webpage_getURL(e);
	if(strcmp(eURL,s)==0){
		return true;
	}
	return false;
}

int main(void){
	printf("hello\n");
	char *url1 = "https://thayer.github.io/engs50/";
	webpage_t* wp1 = webpage_new(url1, 0, NULL);


	if (webpage_fetch(wp1)){
		int pos = 0;
 		char *result;
		hashtable_t *h = hopen(30); //ask about size
 		
		 while (( pos = webpage_getNextURL(wp1, pos, &result)) > 0) {
			if (IsInternalURL(result)){
				webpage_t* wp = webpage_new(result, 0, NULL);
				if (hsearch(h,searchfn,result,strlen(result))==NULL){
					int eval =  hput(h,wp,result,strlen(result));
					if(eval==0){
						printf("%s\n",webpage_getURL(wp));
					}
				}
				//webpage_delete(wp);
			}
			else{
				free(result);	
			}
		}
		
		webpage_delete(wp1);
		hclose(h);
		exit(EXIT_SUCCESS);
	}
	else{
		webpage_delete(wp1);
		exit(EXIT_FAILURE);
	}
}

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
	char *e = (char*)elementURL;
	char *s = (char*)searchURL;
	//char *eURL = webpage_getURL(e);
	if(strcmp(e,s)==0){
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
		hashtable_t *h = hopen(30); //make 70 for step 6 ask about size
		queue_t *q= qopen();
		
		while (( pos = webpage_getNextURL(wp1, pos, &result)) > 0) {
			if (IsInternalURL(result)){
				if (hsearch(h,searchfn,result,strlen(result))==NULL){
					webpage_t* wp = webpage_new(result, 0, NULL);
					int eval =  hput(h,result,result,strlen(result));
					qput(q,wp);
					if(eval==0){
						printf("%s\n",webpage_getURL(wp));
					}
				}else
					free(result);
			} else
				free(result);
		}
		webpage_t *q1 = qget(q);
		
		while(q1 != NULL){
			printf("%s\n", webpage_getURL(q1));
			webpage_delete(q1);
			q1 = qget(q);
		}
		
		webpage_delete(wp1);
		qclose(q);
		hclose(h);
		exit(EXIT_SUCCESS);
	}
	else{
		webpage_delete(wp1);
		exit(EXIT_FAILURE);
	}
}

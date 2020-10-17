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
#include "webpage.h"
#include "queue.h"
#include "hash.h"


int main(void){
	printf("hello\n");
	char *url1 = "https://thayer.github.io/engs50/";
	webpage_t* wp1 = webpage_new(url1, 0, NULL);

	if (webpage_fetch(wp1)){
		//char *html1 = webpage_getHTML(wp1);
		int pos = 0;
 		char *result;
		queue_t *q = qopen();
 		while (( pos = webpage_getNextURL(wp1, pos, &result)) > 0) {
			if (IsInternalURL(result)){
				webpage_t* wp = webpage_new(result, 0, NULL);
				qput(q,wp);
			}
			free(result);	
		}
		
		//print the queue
		webpage_t *q1 = qget(q);
		while(q1 != NULL){
			printf("%s\n",webpage_getURL(q1));
			webpage_delete(q1);
			q1 = qget(q);
		}
		qclose(q);
		webpage_delete(wp1);
		exit(EXIT_SUCCESS);
	}
	else{
		webpage_delete(wp1);
		exit(EXIT_FAILURE);
	}
}

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
	//free(url1);

	if (webpage_fetch(wp1)){
		//char *html1 = webpage_getHTML(wp1);
		int pos = 0;
 		char *result;

 		while (( pos = webpage_getNextURL(wp1, pos, &result)) > 0) {
			printf("Found url: %s ", result);
			if (IsInternalURL(result)){
				printf("is internal.\n");
			}
			else{
				printf("is external.\n");			
			}
			free(result);	
		}
		webpage_delete(wp1);
		exit(EXIT_SUCCESS);
	}
	else{
		webpage_delete(wp1);
		exit(EXIT_FAILURE);
	}
}

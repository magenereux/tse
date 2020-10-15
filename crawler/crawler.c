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

int main(void){
	printf("hello\n");
	char *url1 = "https://thayer.github.io/engs50/";
	webpage_t* wp1 = webpage_new(url1, 0, NULL;

	if (webpage_fetch(wp1)){
		char *html1 = webpage_getHTML(wp1);
		int pos = 0;
 		char *result;

 		while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
		bool internal = IsInternalURL(char *result);
		printf("Found url: %s ", result);
		if (internal){
			printf("is internal.\n");
		}
		else{
			printf("is external.\n");			
		}
		free(result);
		free(wp1);
		exit(EXIT_SUCCESS);
	}
	else{
		free(wp1);
		exit(EXIT_FAILURE);
	}
}

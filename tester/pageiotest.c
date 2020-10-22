/* pageiotest.c --- 
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

int main (void) {
	webpage_t *wp=pageload(3,"pages");
	
	printf("url: %s\n",webpage_getURL(wp));
	printf("depth: %d\n",webpage_getDepth(wp));
	printf("htmllen: %d\n",webpage_getHTMLlen(wp));
	printf("html: %s\n",webpage_getHTML(wp));
	
	pagesave(wp,2,"tester");
	webpage_t *wp2=pageload(2,"tester");
	
	printf("url: %s\n",webpage_getURL(wp2));
	printf("depth: %d\n",webpage_getDepth(wp2));
	printf("htmllen: %d\n",webpage_getHTMLlen(wp2));
	printf("html: %s\n",webpage_getHTML(wp2));

	return 0;
}

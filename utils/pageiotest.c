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
	webpage_t *wp=pageload(1,"pages");
	printf("%s",webpage_getURL);
	return 0;
}

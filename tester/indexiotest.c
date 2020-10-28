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

int main (void) {
	printf("entering main");
    
    hashtable_t *ip=indexload(3,"index_testfiles");
    indexsave(ip,3, "indexnm");

	//hashtable_t *ip2= hopen(100);
    //ip2=indexload(3,"indexnm");

    //system("diff ./../index_testfiles/3 ./../indexnm/3");
    
    hclose(ip);
    //hclose(ip2);

	return 0;
}

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
#include "pageio.h"
#include <sys/stat.h>
#include <unistd.h>


bool searchfn(void *elementURL, const void* searchURL){
	char *e = (char*)elementURL;
	char *s = (char*)searchURL;
	//char *ecat = strcat(e,"/");
	//char *scat = strcat(s,"/");
	if(strcmp(e,s)==0 ){
		return true;
	}
	return false;
}

int main(int argc, char* argv[]){ //ask about how to take in arguments (seedURL, pagedir, maxDepth_
	if (argc!=4) {
		printf("usage: crawler <seedURL><pagedir><maxDepth>\n");
		return -1;
	}
	if (argv[1]==NULL||argv[2]==NULL||argv[3]==NULL||atoi(argv[3])<0) {
		printf("usage: crawler <seedURL><pagedir><maxDepth>\n");
		return -1;
	}
	char *url = (char *)calloc(strlen(argv[1])+1,sizeof(char));
	strcpy(url,argv[1]);
	//char *url = argv[1];
	char *pagedir=argv[2];
	int maxDepth=atoi(argv[3]);
	int depth=0;

	//char *url1 = "https://thayer.github.io/engs50/"; // seedURL argument
	webpage_t *first = webpage_new(argv[1], depth, NULL);
	// need to allocate memory for first strcopy --> then put in hashtable
	
	 int id=1;
	 int pos = 0;
 	 char *result;
	 hashtable_t *h = hopen(200);
	 queue_t *q= qopen();
	 hput(h,url,url,strlen(url));
	 qput(q,first);
	 webpage_t *wp=qget(q);

	 while(wp != NULL){
		 if (webpage_getDepth(wp)<=maxDepth) {
			 pos = 0;
			 if(webpage_fetch(wp)){
				 pagesave(wp,id,pagedir);
				 id++;
				 while (( pos = webpage_getNextURL(wp, pos, &result)) > 0) {
					 if (IsInternalURL(result)){
						 if (hsearch(h,searchfn,result,strlen(result))==NULL){
							 webpage_t *newwp = webpage_new(result,webpage_getDepth(wp)+1,NULL);
							 int eval =  hput(h,result,result,strlen(result)); 
							 qput(q,newwp);
							 if(eval==0)
								 printf("%s\n",webpage_getURL(newwp));
						 }else{
							 free(result);
						 }
					 } else{
						 free(result);
					 }
				 }
			 }
			 
		 }
		 webpage_delete(wp);
		 wp=(webpage_t*)qget(q);
		}
		
		qclose(q);
		hclose(h);
		exit(EXIT_SUCCESS);
}

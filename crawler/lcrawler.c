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
#include "lqueue.h" 
#include "lhash.h"
#include "pageio.h"
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h> 

typedef struct arg {
    lhashtable_t *htp;
    lqueue_t *lqp;
	int maxDepth;
	char *pageDir;
}arg_t;


bool searchfn(void *elementURL, const void* searchURL){
	char *e = (char*)elementURL;
	char *s = (char*)searchURL;
	if(strcmp(e,s)==0 ){
		return true;
	}
	return false;
}

void* thread_getFunction(void *ARG){
	arg_t *ARGS = (arg_t *)ARG;
	int id=1;
	int pos = 0;
	char *result;
	webpage_t *wp=lqget(ARGS->lqp);
	while(wp != NULL){
		if (webpage_getDepth(wp)<=ARGS->maxDepth) {
			pos = 0;
			if(webpage_fetch(wp)){
				pagesave(wp,id,ARGS->pageDir);
				id++;
				while (( pos = webpage_getNextURL(wp, pos, &result)) > 0) {
					if (IsInternalURL(result)){
						if (lhsearch(ARGS->htp,searchfn,result,strlen(result))==NULL){
							webpage_t *newwp = webpage_new(result,webpage_getDepth(wp)+1,NULL);
							int eval =  lhput(ARGS->htp,result,result,strlen(result)); 
							lqput(ARGS->lqp,newwp);
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
		wp=(webpage_t*)qget(ARGS->lqp);
	}
	return NULL;
}

int main(int argc, char* argv[]){ //ask about how to take in arguments (seedURL, pagedir, maxDepth_
	if (argc!=5) {
		printf("usage: crawler <seedURL><pagedir><maxDepth><numthreads>\n");
		return -1;
	}
	if (argv[1]==NULL||argv[2]==NULL||argv[3]==NULL||atoi(argv[3])<0||atoi(argv[4])<1) {
		printf("usage: crawler <seedURL><pagedir><maxDepth><numthreads>\n");
		return -1;
	}
	char *url = (char *)calloc(strlen(argv[1])+1,sizeof(char));
	strcpy(url,argv[1]);
	char *pagedir=argv[2];
	int mDepth=atoi(argv[3]);
	int numthreads=atoi(argv[4]);
	int depth=0;
	pthread_t threads[numthreads];
	
	//char *url1 = "https://thayer.github.io/engs50/"; // seedURL argument
	webpage_t *first = webpage_new(argv[1], depth, NULL);
	// need to allocate memory for first strcopy --> then put in hashtable
	

	lhashtable_t *h = lhopen(200);
	lqueue_t *q= lqopen();
	lhput(h,url,url,strlen(url));
	lqput(q,first);
	arg_t *ARGS= malloc(sizeof(arg_t));
	ARGS->htp=h;
	ARGS->lqp=q;
	ARGS->maxDepth=mDepth;
	ARGS->pageDir=pagedir;
	
	for (int i=0;i<numthreads;i++){
		if(pthread_create(&threads[i],NULL,thread_getFunction,ARGS)!=0){ 
			printf("FAILURE: thread %d did not create\n",i);
			lhclose(h);
			pthread_exit(NULL);
			exit(EXIT_FAILURE);
		}
		printf("SUCCESS: thread %d created\n",i);
	}

	for (int i=0;i<numthreads;i++){
		if(pthread_join(threads[i],NULL)!=0){ 
			printf("FAILURE: threads did not join\n");
			lhclose(h);
			pthread_exit(NULL);
			exit(EXIT_FAILURE);
		}
	} 



	lqclose(q);
	lhclose(h);
	exit(EXIT_SUCCESS);
}

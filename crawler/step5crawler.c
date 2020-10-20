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
#include <sys/stat.h>
#include <unistd.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	struct stat st= {0};
	char path[260];
	sprintf(path,"../%s",dirname);

	if (stat(path,&st)==-1){ //if unable to get file properties 
		if (mkdir(path,0700)!=-1){ //make dir
			sprintf(path,"%s/%d",path,id); //now the path to the specific file
			
			FILE *fp = fopen(path,"w");//make file 
			if (access(path,W_OK)!=0)//checks users permission
				return -1;//file does not exist
			fprintf(fp,"%s\n",webpage_getURL(pagep));
			fprintf(fp,"%d\n",webpage_getDepth(pagep));
			fprintf(fp,"%d\n",webpage_getHTMLlen(pagep));
			fprintf(fp,"%s\n",webpage_getHTML(pagep));
			fclose(fp);
		}
	} else {
			sprintf(path,"%s/%d",path,id); //now the path to the specific file
			
			FILE *fp = fopen(path,"w");//make file 
			if (access(path,W_OK)!=0)//checks users permission
				return -1;//file does not exist
			fprintf(fp,"%s\n",webpage_getURL(pagep));
			fprintf(fp,"%d\n",webpage_getDepth(pagep));
			fprintf(fp,"%d\n",webpage_getHTMLlen(pagep));
			fprintf(fp,"%s\n",webpage_getHTML(pagep));
			fclose(fp);
	}
	
	return 0;
}

bool searchfn(void *elementURL, const void* searchURL){
	char *e = (char*)elementURL;
	char *s = (char*)searchURL;
	if(strcmp(e,s)==0){
		return true;
	}
	return false;
}

int main(void){
	printf("hello\n");
	char *url1 = "https://thayer.github.io/engs50/"; // seedURL argument
	webpage_t* wp1 = webpage_new(url1, 0, NULL);

	if (webpage_fetch(wp1)){
		pagesave(wp1,1,"pages");
		
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

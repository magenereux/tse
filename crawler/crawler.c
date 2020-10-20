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

int main(int argc, char* argv[]){ //ask about how to take in arguments (seedURL, pagedir, maxDepth_
	if (argc!=4) {
		printf("usage: crawler <seedURL><pagedir><maxDepth>");
		return -1;
	}
	if (argv[1]==NULL||argv[2]==NULL||argv[3]==NULL||atoi(argv[3])<0) {
		printf("usage: crawler <seedURL><pagedir><maxDepth>");
		return -1;
	}
	char *url=argv[1];
	char *pagedir=argv[2];
	int maxDepth=atoi(argv[3]);
	int depth=0;

	//char *url1 = "https://thayer.github.io/engs50/"; // seedURL argument
	webpage_t *first = webpage_new(url, depth, NULL);
	// need to allocate memory for first strcopy --> then put in hashtable
	if (webpage_fetch(first)){
		int id=1;
		pagesave(first,id,pagedir);
		int pos = 0;
 		char *result;
		hashtable_t *h = hopen(70);
		queue_t *q= qopen();
		hput(h,url,url,strlen(url));
		qput(q,first);
		webpage_t *wp=first;
		//pos=webpage_getNextURL(wp,pos,&result);
		while (webpage_getDepth(wp)<maxDepth) {
		 
			while (( pos = webpage_getNextURL(wp, pos, &result)) > 0) {
				if (IsInternalURL(result)){
					if (hsearch(h,searchfn,result,strlen(result))==NULL){
						webpage_t *newwp = webpage_new(result,webpage_getDepth(wp)+1,NULL);
						id++;
						webpage_fetch(newwp);
						pagesave(newwp,id,pagedir);
						int eval =  hput(h,result,result,strlen(result)); 
						qput(q,wp);
						if(eval==0)
							printf("%s\n",webpage_getURL(newwp));
					}else
						free(result);
				} else
					free(result);
			}
			wp=(webpage_t*)qget(q);
			//delete each webpage we get
		}
		/*
		webpage_t *q1 = qget(q);
		
		while(q1 != NULL){
			printf("%s\n", webpage_getURL(q1));
			webpage_delete(q1);
			q1 = qget(q);
			}*/
		
		webpage_delete(wp); //might need to be first
		qclose(q);
		hclose(h);
		exit(EXIT_SUCCESS);
	}
	else{
		exit(EXIT_FAILURE);
	}
}

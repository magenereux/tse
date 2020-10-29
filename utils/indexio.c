/* 
 * indexio.c --- 
 * 
 * Author: 
 * Created:
 * Version: 1.0
 * 
 * Description: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include "hash.h"
#include "queue.h"

FILE *fp;

typedef struct wordCount {                                 
  char *key;                                               
  queue_t *Docs; //queue of docs that have the word        
} wordCount_t;                                             
                                                           
//entry in queue of docs                                   
typedef struct docCount {                                  
  int DocID;                                               
  int count;//number of times word occurs in Doc           
} docCount_t;  

void docGet(void *p){
	docCount_t *dc = (docCount_t*)p;
	fprintf(fp, "%d %d ",dc->DocID, dc->count);
}

void wordGet(void *wordCount){
	wordCount_t *wc = (wordCount_t*)wordCount;
	queue_t *dc = (queue_t*)(wc->Docs);
	printf("w -> key = %s ", wc->key);
	fprintf(fp,"%s ", wc->key);
	qapply(dc,docGet);
	fprintf(fp,"\n");
}

/*
 * indexsave -- saves index (a hashtable) into indexnm.
 * saves index to file, each line contains:
 * <word> <docID1> <count1> <docID2> <count2> ....<docIDN> <countN>
 * 
 * returns: 0 for success; nonzero otherwise
 *

 */
int32_t indexsave(hashtable_t *index, int id, char *dirnm) {
	struct stat st= {0};
	char path[260];
	sprintf(path,"../%s",dirnm); // making page we're working with

	if (stat(path,&st)==-1){ //if unable to get file properties 
		if (mkdir(path,0700)==-1) //make dir
			return -1;	
	}
	sprintf(path,"%s/%d",path,id); //now the path to the specific file
	fp = fopen(path,"w");//make file 
	if (access(path,W_OK)!=0)//checks users permission
		return -1;//file does not exist
	happly(index,wordGet);
	fclose(fp);
	
	return 0;
}

/* 
 * indexload -- loads the numbered filename <id> in direcory <dirnm>
 * into a new hashtable.
 *
 * returns: non-NULL for success; NULL otherwise
 */

hashtable_t *indexload(int id, char *dirnm){
	char path[260];
	char* keywordp;
	char keyword[180];
	keywordp = keyword;
	int ID;
	int count;
	//docCount_t *dc = NULL;
	//wordCount_t *wc = NULL;
	hashtable_t *index;
  	sprintf(path,"../%s/%d",dirnm,id);     
	printf("loading %s\n", path);                            
	
  	if((fp=fopen(path,"r")) == NULL){
		printf("unable to open file %s\n",path);
		return NULL;  
	  }                                                                                          

	index = hopen(100);

  	while ((fscanf(fp,"%s",keywordp))!= EOF) {
		printf("%s ", keywordp);
		//fflush(stdout);
		wordCount_t *wc = malloc(sizeof(wordCount_t));
		wc->key = malloc(strlen(keywordp)+1);
		strcpy(wc->key, keywordp);
		wc->Docs = qopen();
		while ((fscanf(fp,"%d%d",&ID, &count))== 2){
			docCount_t *dc = malloc(sizeof(docCount_t));
			dc->DocID = ID;
			dc->count = count;
			qput(wc->Docs,dc);
			printf("%d %d ", ID, count);
			//free(dc);
			//fflush(stdout);
		}
		printf("\n");
		hput(index,(void*)wc,keywordp, strlen(keywordp));
		//free(wc->key);
	}

                                                                       
  	fclose(fp);
	return index;
	                          

}

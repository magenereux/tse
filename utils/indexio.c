/* indexio.c --- 
 * 
 * 
 * Author: Marguerite Genereux
 * Created: Tue Oct 27 11:26:41 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#pragma once
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
#include <webpage.h>
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
	queue_t *dc = (queue_t*)wc->Docs;
	
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
	char path [260];
	char* key;
	int docID;
	int count;
	int i = 0;
  sprintf(path,"../%s/%d",dirnm,id);                                 
                                   
  if((fp=fopen(path,"r")) == NULL)                                     
    return NULL;                                                       
                                                                       
  fscanf(fp,"%s",key);
	for(i;i<??;i++) {
		fscanf(fp,"%d",&docID);                                              
		fscanf(fp,"%d\n",&count);                                          
	}                                              
  char *buffer = (char*)malloc(htmllen+1); //1 for null character      
  char *bp = buffer;                                                   
                                                                       
  for (int i=0;i<htmllen;i++,bp++) {                                   
    *bp=fgetc(fp);                                                     
  }                                                                    
  *bp='\0'; //includes the null character that signifies end of string 
                                                                       
  fclose(fp);                          

}

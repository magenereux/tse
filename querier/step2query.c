/* query.c --- 
 * 
 * 
 * Author: Rachelle A. Louie
 * Created: Thu Oct 29 15:52:44 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <ctype.h>
 #include "hash.h"
 #include "queue.h"
 #include "indexio.h"
typedef struct wordCount {                                                                       
  char *key;                                                                                     
  queue_t *Docs; //queue of docs that have the word                                              
} wordCount_t;                                                                                   
                                                                                                 
//entry in queue of docs                                                                         
typedef struct docCount {                                                                        
  int DocID;                                                                                     
  int count;//number of times word occurs in Doc                                                 
} docCount_t;    

bool searchfn(void *indx, const void* searchword) {                              
	wordCount_t *w1=(wordCount_t *)indx;
  char *w2=(char*)searchword;
  if (strcmp(w1->key,w2)==0){                                                    
		return true;                                                                 
  }                                                                              
  return false;
}
bool searchID(void *indx, const void* ID) {                                              
	docCount_t *d1 = (docCount_t*)indx;
  int* d2=(int*)ID;                                                                      
  if (d1->DocID==*d2){
    return true;                                                                                 
  }                                                                                              
  return false;                                                                                         
}
void closeQueue(void *ip){                                                                       
  wordCount_t *p;                                                                                
  p = (wordCount_t *)ip;                                                                         
  qclose(p->Docs);                                                                               
  p->Docs=NULL;                                                                                  
}
void freeWords(void *ip){                                                                        
  wordCount_t *p;                                                                                
  p = (wordCount_t *)ip;                                                                         
  free(p->key);                                                                                  
  p->key = NULL;                                                                                 
}    
/*
void findingWordinHash(void *ip) {
	wordCount_t *wc=(wordCount*)ip;
	wordCount_t *target=(wordCount_t*)hsearch(loadedhtp,searchfn,token,strlen(token));
	// print out string here 
}
void findingDocinQueue(void *ep) {
	docCount_t *dc=(docCount*)ep;
	int count=targetdoc->count;
	//docCount_t *targetdoc=(docCount_t*)qsearch(target->Docs,searchID,&ID);//can probably be check if equal to target instead of qsearch
	printf(" %d",targetdoc->count);
	if (targetdoc->count < count) 
	  count=targetdoc->count;
	printf("- %d",count);
 
 }
void loopingThroughWsearch(void *wp) {
	// loop through each word in wsearch and find it in findingWordinHash
	// call both finding functions and connect them here
	
	}*/
int main(int argc, char* argv[]){
    char input[100];
		char dir[100]="indexnm";
		char *dirname=dir;
		int ID=1;
		int minimum=0;
		int counter=0;
    int invalid=0;
   
    //queue_t *wsearch=qopen();
		hashtable_t *loadedhtp=indexload(ID,dirname); //ask about indexio hsize
    //prompts user for input,reads string, prints lower case words back
    printf(">");
    while(fgets(input,sizeof(input),stdin)){
			input[strlen(input)-1]='\0';
			char *token= strtok(input," ");
			if (input[0]!= '\n'){
				while (token!=NULL){
					char *word=(char*)calloc(strlen(token),sizeof(char));
					for(int i=0; i<strlen(token);i++){
						if (isalpha(token[i])==0 && token[i]!='\n'){  
							invalid=invalid+1;                                                              
						} else {
							word[i]=tolower(token[i]);
						}
					}
					if(invalid==0){
						if (strlen(word)>=3&&strcmp(word,"and")!=0) {
							counter++;
							
							wordCount_t *target=(wordCount_t*)hsearch(loadedhtp,searchfn,word,strlen(word));
							printf("%s:",target->key);
							
							if (target!=NULL) {
								docCount_t *targetdoc=(docCount_t*)qsearch(target->Docs,searchID,&ID);
								printf("%d ",targetdoc->count);																					 
								if (counter==1) //initializes minumum to first count
									minimum=targetdoc->count;
								if (targetdoc->count<minimum)
									minimum=targetdoc->count;
							}
						}
					}
					free(word);
					token = strtok(NULL," ");
				}
				if(invalid>0){
					printf("[invalid query]\n");
					invalid=0;
				}
			}
			printf("- %d\n",minimum);
			printf(">");
    }
		//qclose(wsearch);
		happly(loadedhtp,freeWords);                                                                       
		happly(loadedhtp,closeQueue);
		hclose(loadedhtp);
    printf("\n");
    return 0;
}

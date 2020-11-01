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
                                                                                                                                                                         
typedef struct docCount {                                                                        
  int DocID;                                                                                     
  int count;//number of times word occurs in Doc                                                 
} docCount_t;

typedef struct queryCount {
	int DocID;
	int count;
	int numqueries;
} queryCount_t;

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

void freeWords(void *ip){                                                                        
  wordCount_t *p;                                                                                
  p = (wordCount_t *)ip;                                                                        
  free(p->key);                                                                                  
  p->key = NULL;     
  qclose(p->Docs);                                                                             
}    

bool validate(char *line) {
	for(int i=0; i<strlen(line);i++){
		if (isalpha(line[i])==0 && isspace(line[i])==0) 
			return false;                                                             
	}
	return true;
}

int parse(char *input,char **word) { //returns limit of array
	char s[2]=" ";
	char *token=strtok(input,s);
	int i=0;
	while (token!=NULL) {
		NormalizeWord(input);
		word[i]=token;
		token=strtok(NULL,s);
		i++;
	}
	return i;
}
// only keep track of the minimum count of any of the input words
// for first word, just add the docs and their counts to the queue
// for next words, if the doc is already in the queue, just compare counts and keep minimum
// also need to check if there is a doc in the queue that is not in the current list of docs --> remove it
// use qget and a backup queue to check for words in queue that aren't in the word's docs

int main(int argc, char* argv[]){
    char input[100];
	char dir[100]="indexnm";
	char *dirname=dir;
	int ID=7;
	int minimum=0;
	int numwords=0;
    int invalid=0;
   
	hashtable_t *loadedhtp=indexload(ID,dirname);
	queue_t *queried=qopen();
    //prompts user for input,reads string, prints lower case words back
    printf(">");
    while(fgets(input,sizeof(input),stdin)){
		input[strlen(input)-1]='\0';
/* 		char *token= strtok(input," ");
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
						numwords++; */
		if (!validate(input)) {	// case for invalid input
			printf("Invalid query\n");
			continue;
		}
		if (strlen(input)<=1) { // case for empty input
			printf("> ");
			continue;
		}
		int maxWords=strlen(input)/2; //guess for max length of an input word
		char **word=calloc(maxWords,sizeof(char*));
		int limit=parse(input,word);
						
		wordCount_t *target=(wordCount_t*)hsearch(loadedhtp,searchfn,word,strlen(word));
		//printf("%s:",target->key);
		
		if (target!=NULL) {
			for (int i=0;i<=ID;i++) {//loops through docs word shows up in in hashtable
				docCount_t *targetdoc=(docCount_t*)qsearch(target->Docs,searchID,&i);
				if (targetdoc!=NULL) {// if doc has word
					//printf("targetdoc->DocID:%d\n",targetdoc->DocID);
					queryCount_t *targetqueried=(queryCount_t*)qsearch(queried,searchID,&i);//checks if in queried queue
					if (targetqueried==NULL) {//creates new queryCount_t if not already in there
						//printf("targetqueried->DocID:%d\n",targetqueried->DocID);
						queryCount_t *qp=malloc(sizeof(queryCount_t));
						qp->DocID=targetdoc->DocID;
						qp->count=targetdoc->count;
						qp->numqueries=1;
						qput(queried,qp);
					} else { //if already in queryCount_t increments its query count
						targetqueried->numqueries=(targetqueried->numqueries)+1;
						//printf("docID:%d numqueries:%d\n",targetqueried->DocID,targetqueried->numqueries);
					}
					//printf("%d ",targetdoc->count);																					 
					if (numwords==1) //initializes minumum to first count
						minimum=targetdoc->count;
					if (targetdoc->count<minimum)
						minimum=targetdoc->count;
				}
			}
		}
		free(word);
		printf(">");
    }
	happly(loadedhtp,freeWords);                                                                       
	hclose(loadedhtp);
    return 0;
}

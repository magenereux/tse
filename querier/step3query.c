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
 #include "pageio.h"
 #include "webpage.h"


typedef struct wordCount {                                                                       
  char *key;                                                                                     
  queue_t *Docs; //queue of docs that have the word                                              
} wordCount_t;                                                                                   
                                                                                                                                                                         
typedef struct docCount {                                                                        
  int DocID;                                                                                     
  int count;//number of times word occurs in Doc                                                 
} docCount_t;

int NormalizeWord(char *word) {                                                  
  if (strlen(word)<3){                                                           
    free(word);                                                                  
    return 1;                                                                    
  }                                                                              
  for (int i=0;i<strlen(word);i++) {                                             
    if (isalpha(word[i])==0){                                                    
      return 1;                                                                  
    } else {                                                                     
      word[i]=tolower(word[i]);                                                  
    }                                                                            
  }                                                                              
  return 0;                                                                      
} 

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

bool searchRank(void *indx, const void* ID) {                                              
	docCount_t *d1 = (docCount_t*)indx;
  int* d2=(int*)ID;                                                                      
  if (d1->count==*d2){
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

int cmpfunc(const void* a, const void* b) {
	return(*(int*)b - *(int*)a);
}

int compareRanks(queue_t *hashQueue, queue_t *unranked, int *rankArray, int limit) {
	// go through unranked -> check if in hash table queue -> if it's in there, compare and update count if minimum -> if not, delete from unranked (free)

	queue_t *backup = qopen();
	docCount_t *currDoc=qget(unranked);
	int i=0;
	while (currDoc!=NULL) { 
		docCount_t *found = (docCount_t*)qsearch(hashQueue,searchID,&(currDoc->DocID));
		if (found!=NULL) {
			if (found->count<currDoc->count) {
				currDoc->count=found->count;
			}
			qput(backup,currDoc); 
			rankArray[i] = currDoc->count; 
			i++;
		} else {
			free(currDoc);
		}
		currDoc=qget(unranked);
	}
	qconcat(unranked,backup);
	return i;
}

void rankDocs(hashtable_t* loadedhtp,char **word, int limit) {
	queue_t* backup=qopen();
	queue_t* unranked=qopen();
	
	// case of first word
	wordCount_t *target=(wordCount_t*)hsearch(loadedhtp,searchfn,word[0],strlen(word[0]));	
	docCount_t *currDoc;
	int sizeRankArray=0; // find max number of docs in query from first word docs
	int i;
	
	if (target==NULL) {
		qclose(backup);
		qclose(unranked);
		return;
	}
	currDoc=(docCount_t*)qget(target->Docs);
	while (currDoc != NULL) {
		docCount_t *copy=(docCount_t*)malloc(sizeof(docCount_t));
		copy->count = currDoc->count;
		copy->DocID = currDoc->DocID;
		qput(backup,currDoc);
		qput(unranked,copy);
		sizeRankArray++;
		currDoc=qget(target->Docs);
	}
	
	qconcat(target->Docs,backup);

	int *rankArrayp=(int*)calloc(sizeRankArray,sizeof(int));

	// case for all of the words
	for (i=0;i<limit;i++) { // rest of the words
		target=(wordCount_t*)hsearch(loadedhtp,searchfn,word[i],strlen(word[i]));
		if (target==NULL) {
			printf("Word not found\n");
			qclose(unranked);
			return;
		} 
		sizeRankArray=compareRanks(target->Docs,unranked,rankArrayp,limit);
	}

	qsort(rankArrayp,sizeRankArray,sizeof(int),cmpfunc);

	docCount_t *toPrint;
	char dir[100]="pages";
	char *dirname=dir;
	i=0;
	while (i<sizeRankArray) {
		toPrint = (docCount_t *)qremove(unranked,searchRank,&rankArrayp[i]);
		webpage_t *wp=pageload(toPrint->DocID,dirname);  
		
		printf("rank: %d doc: %d url: %s\n", toPrint->count, toPrint->DocID, webpage_getURL(wp));
		webpage_delete(wp);
		i++;
		free(toPrint);
	}
	free(rankArrayp);
	qclose(unranked);
}

int main(int argc, char* argv[]){
    char input[100];
	char dir[100]="indexnm";
	char *dirname=dir;
	int ID=7;
   
	hashtable_t *loadedhtp=indexload(ID,dirname);
    printf(">"); //prompts user for input,reads string, prints lower case words back
    while(fgets(input,sizeof(input),stdin)){
		input[strlen(input)-1]='\0';
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

		rankDocs(loadedhtp,word,limit);

		free(word);
		printf(">");
    }

	happly(loadedhtp,freeWords);                                                                       
	hclose(loadedhtp);
    return 0;
}

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

int rank;

typedef struct wordCount {                                                                       
  char *key;                                                                                     
  queue_t *Docs; //queue of docs that have the word                                              
} wordCount_t;                                                                                   
                                                                                                 
//entry in queue of docs                                                                         
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

void printCount(void *ele) {
	if (ele!=NULL) {
		docCount_t *dc=(docCount_t*)ele;
		printf("%d ",dc->count);
		if (dc->count < rank) {
			rank=dc->count;
		}
	}
}

void printQuery(hashtable_t *htp,char **words,int max) {
	if (words==NULL) 
		return;
	rank=INT_MAX; //sets to largest value possible
	for (int i=0;i<max;i++) {
		char *current=words[i];
		if (strlen(current)>=3 && strcmp(current,"and")!=0) {
			wordCount_t *target=(wordCount_t*)hsearch(htp,searchfn,current,strlen(current));
			if (target!=NULL) {
				printf("%s: ",target->key);
				qapply(target->Docs,printCount);
			}
		}
	}
	if (rank==INT_MAX)
		rank=0;	
	printf("- %d\n",rank);
}

int main(int argc, char* argv[]){
    char input[100];
	char dir[100]="indexnm";
	char *dirname=dir;
	int ID=1;
   
	hashtable_t *loadedhtp=indexload(ID,dirname);
    //prompts user for input,reads string, prints lower case words back
    printf(">");
    while(fgets(input,sizeof(input),stdin)!=NULL){
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
		printQuery(loadedhtp,word,limit);
		free(word);
		printf("> ");
    }
	happly(loadedhtp,freeWords);                                                                       
	hclose(loadedhtp);
    return 0;
}

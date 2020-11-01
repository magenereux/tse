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
/*
typedef struct queryCount {
	int DocID;
	int count;
	int numqueries;
} queryCount_t; */

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


// }
// void addDocs(void *ep) {
// 	// loop through all docCount_t in Docs and add them to the queried queue
// 	// make all the necessary checks and comparisons with counts
// 	// 	docCount_t *targetdoc=(docCount_t*)qsearch(target->Docs,searchID,&i);
// 	if (first word) {
// 		qp->DocID=targetdoc->DocID;
// 		qp->count=targetdoc->count;
// 		qput(queried,qp);
// 	} else if (current doc exists in queried queue) {
// 		compare the counts and only keep minimum
// 	} else if (doc exists in queried queue but not current doc queue) {
// 		remove the doc in queried queue
// 	}
// }
 
void firstWordCase(hashtable_t* loadedhtp,char **word,queue_t *backup, queue_t *unranked) {
	// case of first word
	wordCount_t *target=(wordCount_t*)hsearch(loadedhtp,searchfn,word[0],strlen(word[0]));	
	if (target==NULL)
		return;
	while (currDoc=qget(target->Docs)!=NULL) {
		docCount_t *copy=(docCount_t*)malloc(sizeof(docCount_t));
		copy->key=currDoc->key;
		copy->Docs=currDoc->Docs;
		qput(backup,currDoc);
		qput(unranked,copy);
	}
	qconcat(target->Docs,backup);
	}
	// case of rest of the words
	for (int i=1;i<limit;i++) { // rest of the words
		target=(wordCount_t*)hsearch(loadedhtp,searchfn,word[i],strlen(word[i]));	
		restWordCase(target->Docs,unranked);
	}
}

void restWordCase(queue_t *hashQueue, queue_t *unranked) {
	// go through unranked -> check if in hash table queue -> if it's in there, compare and update count if minimum -> if not, delete from unranked (free)
	docCount_t *currDoc=qget(unranked);
	while (currDoc!=NULL) { 
		docCount_t *found = (docCount_t*)qsearch(hashQueue,searchID,currDoc->DocID,strlen(currDoc->DocID))
		if (found!=NULL) {
			if (found->count<currDoc->count) {
				unranked->count=found->count;
			}
		} else {
			qremove(currDoc); //maybe free?
		}
		currDoc=qget(unranked);
	}

}

// only keep track of the minimum count of any of the input words
// for first word, just add the docs and their counts to the queue
// for next words, if the doc is already in the queue, just compare counts and keep minimum
// also need to check if there is a doc in the queue that is not in the current list of docs --> remove it
// use qget and a backup queue to check for words in queue that aren't in the word's docs

		// for first word, make a copy docCount_t of each docs 
		// qget the first doc from the word queue and put into the backup queue
		// use qget from unranked and then place into backup and then concat back
		//make separate function to check if each doc in unranked is in current queue of docs
		// if not, then free that doc

		// use qsort on int array of the ranks of each doc --> sorts by ranking from greatest to least
		// remove from unranked by rank then add back in --> unranked is now ranked

		// for the first word

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
						
		queue_t* backup=qopen();
		queue_t* unranked=qopen();

		firstWordCase(loadedhtp,word,backup,unranked);
		call anotherfunction
										 
		free(word);
		printf(">");
    }
	happly(loadedhtp,freeWords);                                                                       
	hclose(loadedhtp);
    return 0;
}

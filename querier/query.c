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


static int rankSize=0;

typedef struct wordCount {                                                                       
  char *key;                                                                                     
  queue_t *Docs; //queue of docs that have the word                                              
} wordCount_t;                                                                                   
                                                                                                                                                                         
typedef struct docCount {                                                                        
  int DocID;                                                                                     
  int count;//number of times word occurs in Doc                                                 
} docCount_t;

int NormalizeWord(char *word) {     
  if (strlen(word) < 3) {
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
  if (strcmp(word,"and")==0) {
	free (word);
	return 1;
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
// check for "and" -> take it out and treat normally (unless it's the first or last word)
// first check for implicit ands or spaces -> take minimum for each word individually 
// then check for or's -> add the minimums together
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

void rankArrayCounter(void *unranked){
	rankSize++;	
}
void printQueue(void *ep){
	docCount_t *element = (docCount_t *)ep;
	printf("DocID: %d ",element->DocID);
	printf("count: %d\n",element->count);
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
			//printf("putting %d in rankArray in i=%d\n",rankArray[i],i);
			i++;
		} else {
			free(currDoc);
		}
		currDoc=qget(unranked);
	}
	qconcat(unranked,backup);
	return i;
}
/*
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
*/
//add contents of q1 to q2
void addToQueue(queue_t* q1, queue_t* q2, bool isOr){
	docCount_t *currDoc;
	queue_t* backup=qopen();
	//might have to account for q1 being NULL
	//printf("in addtoqueue\n");
	if(q1!=NULL){
		currDoc=(docCount_t*)qget(q1); //
		while (currDoc != NULL) {
			docCount_t *found = (docCount_t*)qsearch(q2,searchID,&(currDoc->DocID));
			docCount_t *copy=(docCount_t*)malloc(sizeof(docCount_t));
			if (found!=NULL) {
				if (isOr) 
					found->count = found->count+currDoc->count; //change rank
				else {
					if (found->count<currDoc->count)
						currDoc->count=found->count;
				}
			} else {
				copy->count= currDoc->count;
				copy->DocID = currDoc->DocID;
				qput(backup,currDoc);
				qput(q2,copy);
				//free(currDoc);
			}
			currDoc=qget(q1);
		}
		qconcat(q1,backup);
	}
}

void ranking(char** words, hashtable_t* htp, int limit, queue_t* all){
	queue_t *temp;
	//int sizeRankArray = 0;
	int *rankArrayp;
	//printf("in rank\n");
	bool isOr;

	for (int i=0; i<limit; i++){
		if (strcmp(words[i],"or") == 0){
			isOr=1;
			addToQueue(temp,all,isOr);
			qclose(temp);
			temp = NULL;
		}
		else {
			//printf("line 237\n");
			wordCount_t *target=(wordCount_t*)hsearch(htp,searchfn,words[i],strlen(words[i]));
			if (i==0){ //if first word find how many docs are associated to find rankSize
				qapply(target->Docs,rankArrayCounter);
				printf("ranksize %d\n",rankSize);
				temp=qopen();
				isOr=0;
				addToQueue(target->Docs, temp,isOr);
				rankArrayp=(int*)calloc(rankSize,sizeof(int));
				rankSize=compareRanks(target->Docs, temp, rankArrayp, limit); // wouldn't ranksize be the same? why need compareranks?
				printf("when i=0 ranksize %d\n",rankSize);
				//printf("after qapply i=0\n");
			}
			else{
				if (target == NULL){ //edge case: dont find the word close everything 
					qclose(temp);
					temp = NULL;
					//queue_t *temp = qopen(); // why do you need to open it back up?
					for (int j = i+1; j<limit; j++){
						if (strcmp(words[j],"or")==0)//if 'or' continue going through words
							continue;
						if (j == limit-1)//get out of for loop
							i = limit;
					}
				} else if (temp == NULL){ //checks if it exists
					temp=qopen();//so we assign value qget in addToQueue		
					//printf("here\n");
					isOr=0;
					addToQueue(target->Docs, temp,isOr);

				} else {
					//rankSize=0;
					//qapply(target->Docs,rankArrayCounter);
					rankArrayp=(int*)realloc(rankArrayp,rankSize);
					rankSize=compareRanks(target->Docs, temp, rankArrayp, limit);
					printf("in else ranksize %d\n",rankSize);
					isOr=0;
					addToQueue(target->Docs, temp,isOr);
				}
			}
			
		}
	}
	isOr=0;
	addToQueue(temp,all,isOr);
	qapply(all,printQueue);
	//printf("before sort\n");
	//qsort all
	qsort(rankArrayp,rankSize,sizeof(int),cmpfunc);
	//printf("after sort\n");
	docCount_t *toPrint;
	char dir[100]="pages";
	char *dirname=dir;
	int i=0;
	//printf("ranksize %d\n",rankSize);
	while (i<rankSize-1) {
		toPrint = (docCount_t *)qremove(all,searchRank,&rankArrayp[i]);
		webpage_t *wp=pageload(toPrint->DocID,dirname);  
		printf("rank: %d doc: %d url: %s\n", toPrint->count, toPrint->DocID, webpage_getURL(wp));
		webpage_delete(wp);
		i++;
		free(toPrint);
	}
	free(rankArrayp);
	qclose(all);
	qclose(temp);
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
		
		if ( (strcmp(word[0],"and") == 0) || (strcmp(word[0],"or") == 0)){
			if (limit>1 && ((strcmp(word[limit],"and") == 0) || (strcmp(word[limit],"or") == 0))){
				printf("invalid query\n");
				continue;
			}
			else{
				printf("invalid query\n");
				continue;
			}
		} else {
			
			queue_t *all = qopen();
			ranking(word,loadedhtp,limit,all);	
			//rankDocs(loadedhtp,word,limit);
		}

		free(word);
		printf(">");
    }

	happly(loadedhtp,freeWords);                                                                       
	hclose(loadedhtp);
    return 0;
}

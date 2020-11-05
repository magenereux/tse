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

void NormalizeWord(char *word) {       
	if (word!=NULL) {
		for (char*p = word;*p!='\0';p++) {      
            *p=tolower(*p);
		}                                                                  
    }                                                                                                                                                                                                                
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
	NormalizeWord(token);
	if (strcmp(token,"and")==0 || strcmp(token,"or")==0) // checks for first word
		return -1;
	word[0]=token;
	int i=1;
	while (token!=NULL) {
		NormalizeWord(token);
		word[i]=token;
		token=strtok(NULL,s);
		if (strcmp(word[i-1],"and")==0 || strcmp(word[i-1],"or")==0) { // checks if there are two in a row
			if (strcmp(word[i],"and")==0 || strcmp(word[i],"or")==0)
				return -1;
		}
		i++;
	}
	if (strcmp(word[i-1],"and")==0 || strcmp(word[i-1],"or")==0) // checks for the last word
		return -1;
	return i;
}

int cmpfunc(const void* a, const void* b) {
	return(*(int*)b - *(int*)a);
}

void rankArrayCounter(void *unranked){
	rankSize++;	
}
void printQueue(void *ep){
	if (ep!=NULL) {
		docCount_t *element = (docCount_t *)ep;
		char *dirname="../pages";
		char filename[100];
		sprintf(filename,"%s/%d",dirname,element->DocID);
		char url[100];
		FILE *fp=fopen(filename,"r");
		fscanf(fp,"%s",url);
		fclose(fp);
		printf("rank:%d doc:%d url:%s\n",element->count,element->DocID,url);
	}
}

//add contents of q2 to q1
void addToQueue(queue_t* q1, queue_t* q2){ // between all and tmp
	docCount_t *currDoc;

	if(q1!=NULL){
		while ((currDoc=qget(q2)) != NULL) {
			docCount_t *found = (docCount_t*)qsearch(q1,searchID,&(currDoc->DocID));
			if (found!=NULL) {
					found->count+=currDoc->count; //change rank
					free(currDoc);
			} else {
				qput(q1,currDoc);
			}
		}
		qclose(q2);
	}
}

queue_t* copy(queue_t* q1, queue_t* q2) { // index queue to temp 
	docCount_t *currDoc;
	queue_t *backup=qopen();

	if(q1!=NULL){
		while ((currDoc=qget(q2)) != NULL) {
			docCount_t *copy=malloc(sizeof(docCount_t));
			copy->DocID=currDoc->DocID;
			copy->count=currDoc->count;
			qput(q1,copy);
			qput(backup,currDoc);
		}
		qclose(q2);
	}
	return(backup);
}

queue_t* update(queue_t* q1, queue_t* q2) { //and case 
	docCount_t *currDoc;
	queue_t *backup=qopen();

	if(q1!=NULL){
		while ((currDoc=qget(q1)) != NULL) {
			docCount_t *found = (docCount_t*)qsearch(q2,searchID,&(currDoc->DocID));
			if (found!=NULL) {
				if (currDoc->count>found->count) {
					currDoc->count=found->count;
				}
				qput(backup,currDoc);
			} else {
				free(currDoc);
			}
		}
		qclose(q1);
	}
	return(backup);
}

int sort(queue_t *q1) {
	queue_t *backup=qopen();
	docCount_t *currDoc;
	int array[500];
	int i=0;
	while ((currDoc=qget(q1))!=NULL) { 
		qput(backup,currDoc);
		array[i]=currDoc->count;
		i++;
	}
	if (i>0) { //checks that there is actually something in the queue
		qsort(array,i,sizeof(int),cmpfunc);
		for (int j=0;j<i;j++) {
			int rank=array[j];
			docCount_t *data=qremove(backup,searchRank,&rank);
			qput(q1,data);
		}
	}
	qclose(backup);
	return i-1; // makes it so when i=0, doesn't return 0 but -1
}

void ranking(char** words, hashtable_t* htp, int limit, queue_t* all){
	queue_t *temp=NULL;

	for (int i=0; i<limit; i++){
		if (strcmp(words[i],"or") == 0){
			addToQueue(all,temp);
			temp = NULL;
		}
		else if (strlen(words[i])>2 && strcmp(words[i],"and")!=0){
			wordCount_t *target=(wordCount_t*)hsearch(htp,searchfn,words[i],strlen(words[i]));
			if (target==NULL) {
				qclose(temp);
				temp=NULL;
				int j;
				for (j=i+1;j<limit;j++) {
					if (strcmp(words[j],"or")==0) {
						break;
					}
				}
				i=j-1; // sets to last to get out of for loop
			} else if (temp==NULL) { // either just got or or first word
				temp=qopen();
				target->Docs=copy(temp,target->Docs);
			} else {
				temp=update(temp,target->Docs);
			}
		}
	}
	if (temp!=NULL) {
		addToQueue(all,temp);
		temp=NULL;
	}

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
		if (limit<0) {
			printf("Invalid query\n");
			free(word);
			printf(">");
			continue;
		}
			
		queue_t *all = qopen();
		ranking(word,loadedhtp,limit,all);	
		int ret=sort(all);
		if (ret<0) {
			printf("No result\n");
		}
		qapply(all,printQueue);

		free(word);
		qclose(all);
		printf(">");
    }

	happly(loadedhtp,freeWords);                                                                       
	hclose(loadedhtp);
    return 0;
}

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
/*
bool searchfn(void *indx, const void* searchword) {                              
  wordCount_t *w1=(wordCount_t *)indx;                                           
  char *w2=(char*)searchword;                                                    
  if (strcmp(w1->key,w2)==0){                                                    
    return true;                                                                 
  }                                                                              
  return false;                                                                  
}*/

int main(int argc, char* argv[]){
    char input[100];
    
    int invalid=0;
    //create queue of inputed words search for each word in indexnm
    queue_t *wsearch=qopen();
    //prompts user for input,reads string, prints lower case words back
    printf(">");
    while(fgets(input,sizeof(input),stdin)){   
        char *token= strtok(input," ");
        if (input[0]!= '\n'){
            while (token!=NULL){
                for(int i=0; i<strlen(token);i++){
                    if (isalpha(token[i])==0 && token[i]!='\n'){  
                        invalid=invalid+1;                                                                 
                    } else {  
                        token[i]=tolower(token[i]);
                    }
                }
                if(invalid==0){
                    printf("putting:%s\n",token);
                    //if(strcmp(token,"and")!=0 || strcmp(token,"or")!=0)
                    qput(wsearch,token);
                }
                token = strtok(NULL," ");
            }
            if(invalid>0){
                printf("[invalid query]\n");
                invalid=0;
            }
        }
        printf(">");
        
    }

    //loads index 
    //hashtable_t* indx=hopen(100);
    //indx=indexload(1,"indexnm");


    printf("\n");
    return 0;
}

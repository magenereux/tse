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

int main(int argc, char* argv[]){
    char input[80];
    printf(">");
    int invalid=0;
    
while(scanf(" %99[^\n]s",input)!=EOF){
    
    char *token= strtok(input," ");
    printf("input:%d",token[0]);
    if (input[0]!= '\n'){
        while (token!=NULL){
            for(int i=0; i<strlen(token);i++){
                if (isalpha(token[i])==0){  
                    invalid=invalid+1;                                                                 
                } else {  
                token[i]=tolower(token[i]);
                }
            }
            if(invalid>0)
                printf("[invalid query]");
            else
                printf("%s",token);
            token = strtok(NULL," ");
        }
    }
    printf("\n");
    printf(">");
}
printf("\n");
return 0;
}

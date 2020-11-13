#include "queue.h"  
#include "lqueue.h" 
#include "lhash.h"
#include "hash.h"  
#include <stdlib.h>                                                             
#include <stdio.h> 
#include <string.h> 
#include <pthread.h>  
#include <unistd.h>

#define MAXREG 10
#define NUM_THREADS 1
typedef struct muffin {
    char flavor[MAXREG];
    double price;
    int expdate;
}muffin_t;

bool search(void* elementp, const void* keyp){
    printf("in serarch\n");
    muffin_t *mp=(muffin_t*)elementp;                                                  
    if (strcmp(mp->flavor,keyp)==0)      
        return true;                                                           
    return false;                                                                   
}

void* thread_putFunction(lhashtable_t *lhp){
    lhsetdelay(10);
    muffin_t *muffin1=malloc(sizeof(muffin_t));
    strcpy(muffin1->flavor,"blueberry");
    muffin1->price=5.00;
    muffin1->expdate=202012;
    int ret;
    ret=lhput(lhp,muffin1,"blueberry",9);
    if (ret==0)
        printf("SUCCESS: hput worked\n");
    return lhp;
}
void* thread_removeFunction(lhashtable_t *lhp){
    lhsetdelay(5);
    muffin_t *data = lhremove(lhp,search,"blueberry",9);
    if (data != NULL) {
        printf("%s found and removed\n",data->flavor);
    }
    free(data);
    
    return NULL;
}

int main (void){
    lhashtable_t *lhp= lhopen(100);
    pthread_t thrd1,thrd2;

    if(pthread_create(&thrd1,NULL,thread_putFunction,lhp)!=0){ 
        printf("FAILURE: thread 1 did not create\n");
        lhclose(lhp);
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    printf("SUCESS:Thread 1 created\n");

    if(pthread_create(&thrd2,NULL,thread_removeFunction,lhp)!=0){   
        printf("FAILURE: thread2 did not create\n");
        lhclose(lhp);
        exit(EXIT_FAILURE);
    }
    printf("SUCCESS: Thread 2 created\n");

    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    lhclose(lhp);
    pthread_exit(NULL);
    
    return 0;

}

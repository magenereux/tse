#include "queue.h"  
#include "lqueue.h" 
#include <stdlib.h>                                                             
#include <stdio.h> 
#include <string.h> 
#include <pthread.h>  

#define MAXREG 10
#define NUM_THREADS 1
typedef struct muffin {
    char flavor[MAXREG];
    double price;
    int expdate;
}muffin_t;

typedef struct args {
    lqueue_t *lq;
    muffin_t *mp;
} arg_t;

typedef struct searchArgs {
    lqueue_t *lq;
    char *mp;

} searchArg_t;

bool search(void* elementp, const void* keyp){
    muffin_t *mp=(muffin_t*)elementp;                                                  
    if (strcmp(mp->flavor,keyp)==0)      
        return true;                                                           
    return false;                                                                   
}

void* thread_searchFunction(void *lqpArgs){
    searchArg_t *lArg = (searchArg_t*)lqpArgs;
    
    muffin_t *mp = lqsearch(lArg->lq, search,lArg->mp);
    if (mp != NULL) {
        printf("object found");
    }
    
    return NULL;
}

int main (void){
    pthread_t thrd1;

    lqueue_t *lqp= lqopen();

    muffin_t *muffin1=malloc(sizeof(muffin_t));
    strcpy(muffin1->flavor,"blueberry");
    muffin1->price=5.00;
    muffin1->expdate=202012;
    lqput(lqp,muffin1);

    muffin_t *muffin2=malloc(sizeof(muffin_t));
    strcpy(muffin2->flavor,"pumpkin");
    muffin2->price=4.00;
    muffin2->expdate=202016;
    lqput(lqp,muffin2);


    searchArg_t *lqpArg=malloc(sizeof(searchArg_t));
    lqpArg->lq = lqp;
    lqpArg->mp = malloc(sizeof(strlen("pumpkin"))+1);
    strcpy(lqpArg->mp, "pumpkin");

    if(pthread_create(&thrd1,NULL,thread_searchFunction,(void*)lqpArg)!=0){
        printf("FAILURE: thread1 did not create\n");
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS: Thread 1 lqsearch worked\n");
    
    pthread_join(thrd1,NULL);
    free(lqpArg->mp);
    free(lqpArg);
    lqclose(lqp);
    
    return 0;

}

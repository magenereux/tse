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
} arg_t;


bool search(void* elementp, const void* keyp){
    muffin_t *mp=(muffin_t*)elementp;                                                  
    if (strcmp(mp->flavor,keyp)==0)      
        return true;                                                           
    return false;                                                                   
}

void doublePrice(void* elementp){
    muffin_t *mp = (muffin_t*)elementp;
    printf("original price: %f\n", mp->price);
    mp->price *= 2.0;
    printf("modified price: %f\n", mp->price);

}

void* thread_applyFunction(void *lqpArgs){
    arg_t *lArg = (arg_t*)lqpArgs;
    lqapply(lArg->lq, doublePrice);
    return NULL;
}

int main (void){
    pthread_t thrd1,thrd2;

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


    arg_t *lqpArg=malloc(sizeof(arg_t));
    lqpArg->lq = lqp;

    if(pthread_create(&thrd1,NULL,thread_applyFunction,(void*)lqpArg)!=0){
        printf("FAILURE: thread1 did not create\n");
        exit(EXIT_FAILURE);
    }
    printf("SUCCESS: Thread 1 lqapply worked\n");
    
    if(pthread_create(&thrd2,NULL,thread_applyFunction,(void*)lqpArg)!=0){
        printf("FAILURE: thread2 did not create\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    free(lqpArg);
    lqclose(lqp);
    
    return 0;

}

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

void* thread_putFunction(void *lqpArgs){
    arg_t *lArg = (arg_t*)lqpArgs;
    lqput(lArg->lq,lArg->mp);
    return lqpArgs;
}

int main (void){
    muffin_t *muffin1=malloc(sizeof(muffin_t));
    strcpy(muffin1->flavor,"blueberry");
    muffin1->price=5.00;
    muffin1->expdate=202012;

    lqueue_t *lqp= lqopen();
    pthread_t thrd1,thrd2;

    arg_t *lqpArgs=malloc(sizeof(arg_t));
    lqpArgs->lq = lqp;
    lqpArgs->mp = muffin1;

    if(pthread_create(&thrd1,NULL,thread_putFunction,(void*)lqpArgs)!=0){
        printf("FAILURE: lqput did not work\n");
        lqclose(lqp);
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    printf("SUCESS:Thread 1 lqput worked\n");

    muffin_t *muffin2=malloc(sizeof(muffin_t));
    strcpy(muffin1->flavor,"pumpkin");
    muffin1->price=4.00;
    muffin1->expdate=202016;

    arg_t *lqpArgs2=malloc(sizeof(arg_t));
    lqpArgs2->lq = lqp;
    lqpArgs2->mp = muffin2;
    if(pthread_create(&thrd2,NULL,thread_putFunction,(void*)lqpArgs2)!=0){
        printf("FAILURE: lqput did not work\n");
        lqclose(lqp);
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    printf("SUCESS:Thread 2 lqput worked\n");

    //free(muffin1);
    //free(muffin2);
    
    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    free(lqpArgs);
    free(lqpArgs2);
    lqclose(lqp);
    pthread_exit(NULL);
    
    return 0;

}

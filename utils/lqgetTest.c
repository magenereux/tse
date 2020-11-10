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

void* thread_getFunction(void *lqpArgs){
    arg_t *lArg = (arg_t*)lqpArgs;
    
    muffin_t *data = lqget(lArg->lq);
    if (data != NULL) {
        printf("%s element gotten\n",data->flavor);
    }
    free(data);
    
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
    
    arg_t *lqpArg = malloc(sizeof(arg_t));
    lqpArg->lq = lqp;


    if(pthread_create(&thrd1,NULL,thread_getFunction,(void*)lqpArg)!=0){
        printf("FAILURE: thread1 did not create\n");
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS: Thread 1 lqremoved worked\n");
    
    pthread_join(thrd1,NULL);
    free(lqpArg);
    lqclose(lqp);
    
    return 0;

}

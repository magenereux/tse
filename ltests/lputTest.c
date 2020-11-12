#include "queue.h"  
#include "lqueue.h" 
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

void* thread_putFunction(lqueue_t *lqp){
    lqsetdelay(10);
    muffin_t *muffin1=malloc(sizeof(muffin_t));
    strcpy(muffin1->flavor,"blueberry");
    muffin1->price=5.00;
    muffin1->expdate=202012;
    lqput(lqp,muffin1);
    return lqp;
}

void* thread_getFunction(lqueue_t *lqp){
    lqsetdelay(5);
    // won't work, thread 1 will be busy --> will wait until sleep(10) in put is over -> then gets
    muffin_t *data;
    data=lqget(lqp);
    if (data != NULL) {
        printf("%s element gotten\n",data->flavor);
        free(data);
    }
    return lqp;
}

int main (void){

    lqueue_t *lqp= lqopen();
    pthread_t thrd1,thrd2;

    if(pthread_create(&thrd1,NULL,thread_putFunction,lqp)!=0){ 
        printf("FAILURE: thread 1 did not create\n");
        lqclose(lqp);
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    printf("SUCESS:Thread 1 created\n");

    if(pthread_create(&thrd2,NULL,thread_getFunction,lqp)!=0){ 
        printf("FAILURE: thread 2 did not create\n");
        lqclose(lqp);
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    printf("SUCESS:Thread 2 created\n");
    
    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    lqclose(lqp);
    pthread_exit(NULL);
    
    return 0;

}

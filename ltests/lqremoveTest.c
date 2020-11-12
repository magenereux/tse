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

bool search(void* elementp, const void* keyp){
    muffin_t *mp=(muffin_t*)elementp;                                                  
    if (strcmp(mp->flavor,keyp)==0)      
        return true;                                                           
    return false;                                                                   
}

void* thread_putFunction(lqueue_t *lqp){
    lqsetdelay(10);
    muffin_t *muffin1=malloc(sizeof(muffin_t));
    strcpy(muffin1->flavor,"blueberry");
    muffin1->price=5.00;
    muffin1->expdate=202012;
    if(lqput(lqp,muffin1) == 0){
        printf("element put, flavor: %s\n",muffin1->flavor);
    }
    return lqp;
}

void* thread_removeFunction(lqueue_t *lqp){
    lqsetdelay(5);
    muffin_t *data = lqremove(lqp, search,"blueberry");
    if (data != NULL) {
        printf("%s found and removed\n",data->flavor);
    }
    free(data);
    
    return NULL;
}

int main (void){
    pthread_t thrd1,thrd2;

    lqueue_t *lqp= lqopen();

    if(pthread_create(&thrd1,NULL,thread_putFunction,lqp)!=0){
        printf("FAILURE: thread1 did not create\n");
        lqclose(lqp);
        exit(EXIT_FAILURE);
    }
    printf("SUCCESS: Thread 1 lqremoved worked\n");

    if(pthread_create(&thrd2,NULL,thread_removeFunction,lqp)!=0){
        printf("FAILURE: thread2 did not create\n");
        lqclose(lqp);
        exit(EXIT_FAILURE);
    }
    printf("SUCCESS: Thread 2 lqremoved worked\n");
    
    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    lqclose(lqp);
    pthread_exit(NULL);
    
    return 0;

}

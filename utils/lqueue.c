/* lqueue.c --- 
 * 
 * 
 * Author: Marguerite Genereux
 * Created: Sun Nov  7 19:10:32 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "queue.h" 
#include "lqueue.h"                                                        
#include <stdlib.h>                                                             
#include <stdio.h>
#include <pthread.h>  
#include <unistd.h>
#include <errno.h>

// testing: first play around with creating and working with threads
// then make small queues and check that single and double threads have same output
// same for hash
// step 3: then try on crawler and check to run time differences
static int timeDelay = 0;

typedef struct lqueueStruct {                                                                                                                              
  queue_t *q;   
  pthread_mutex_t lock;                                                         
} lqueueStruct_t; 


lqueue_t* lqopen(void) {
    lqueueStruct_t *lqp;
    if ((lqp = (lqueueStruct_t *)malloc(sizeof(lqueueStruct_t)))==NULL){
        return NULL;
    }
    //intialize mutex
    pthread_mutex_init(&(lqp->lock),NULL);
    (lqp->q)= qopen();
    return (lqueue_t*)lqp;
}


void lqclose(lqueue_t *qp) {  
    if (qp!=NULL){
        lqueueStruct_t *lqp = (lqueueStruct_t *)qp;
        pthread_mutex_lock(&(lqp->lock));
        sleep(timeDelay);
        qclose(lqp->q);
        pthread_mutex_unlock(&(lqp->lock));
        pthread_mutex_destroy(&(lqp->lock));
        free(lqp);
    }
}

int32_t lqput(lqueue_t *qp, void *elementp) {
    if (qp==NULL || elementp==NULL){
        return -1;
    }
    lqueueStruct_t *lqp = (lqueueStruct_t *)qp;
    pthread_mutex_lock(&(lqp->lock));
    printf("put locked\n");
    sleep(timeDelay);
    qput(lqp->q,elementp);
    pthread_mutex_unlock(&(lqp->lock));
    printf("put unlocked\n");
    return 0;
}

void* lqget(lqueue_t *qp) { 
    void* first = NULL;
    if (qp!=NULL){
        lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
        pthread_mutex_lock(&(lqp->lock));
        printf("get locked\n");
        sleep(timeDelay);
        first=qget(lqp->q);
        pthread_mutex_unlock(&(lqp->lock));
        printf("get unlocked\n");
    }
    return first;
}

void lqapply(lqueue_t *qp, void (*fn)(void* elementp)) { 
    lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
    pthread_mutex_lock(&(lqp->lock));
    printf("apply locked\n");
    sleep(timeDelay);
    qapply(lqp->q,fn);
    pthread_mutex_unlock(&(lqp->lock));
    printf("apply unlocked\n");
}    

void* lqsearch(lqueue_t *qp,bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {     
    if (qp!=NULL){
        lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
        pthread_mutex_lock(&(lqp->lock));
        printf("search locked\n");
        sleep(timeDelay);
        void* data = qsearch(lqp->q,searchfn,skeyp);
        pthread_mutex_unlock(&(lqp->lock));
        printf("search unlocked\n");
        return data;
    }
    return NULL;
}

void* lqremove(lqueue_t *qp,                                                      
              bool (*searchfn)(void* elementp,const void* keyp),                
              const void* skeyp) {  
    if (qp!=NULL){
        lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
        pthread_mutex_lock(&(lqp->lock));
        sleep(timeDelay);
        void* data = qremove(lqp->q,searchfn,skeyp);
        pthread_mutex_unlock(&(lqp->lock));
        return data;
    }
    return NULL;
    
}

void lqsetdelay(int seconds){
    timeDelay = seconds;
}


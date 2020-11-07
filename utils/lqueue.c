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

typedef struct lqueueStruct {                                                                                                                              
  queue_t *q;   
  pthread_mutex_t lock;                                                         
} lqueueStruct_t; 


lqueue_t* lqopen(void) {
    lqueueStruct_t *lqp;
    if ((lqp = (lqueueStruct_t *)malloc(sizeof(lqueueStruct_t)))==NULL){
        return NULL;
    }
    pthread_mutex_lock(&(lqp->lock));
    (lqp->q)= qopen();
    pthread_mutex_unlock(&(lqp->lock));
    return (lqueue_t*)lqp;
}


void lqclose(lqueue_t *qp) {  
    if (qp!=NULL){
        lqueueStruct_t *lqp = (lqueueStruct_t *)qp;
        pthread_mutex_lock(&(lqp->lock));
        qclose(lqp->q);
        pthread_mutex_unlock(&(lqp->lock));
    }
}


int32_t lqput(lqueue_t *qp, void *elementp) {
    if (qp!=NULL){
        return -1;
    }
    lqueueStruct_t *lqp = (lqueueStruct_t *)qp;
    pthread_mutex_lock(&(lqp->lock));
    qput(lqp->q,elementp);
    pthread_mutex_unlock(&(lqp->lock));
    return 0;
}

void* lqget(lqueue_t *qp) { 
    void* first;
    if (qp!=NULL){
        lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
        pthread_mutex_lock(&(lqp->lock));
        first=qget(lqp->q);
        pthread_mutex_unlock(&(lqp->lock));
    }
    else {
        first=NULL;
    }
    return first;
}

void lqapply(lqueue_t *qp, void (*fn)(void* elementp)) { 
    lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
    pthread_mutex_lock(&(lqp->lock));
    qapply(lqp->q,fn);
    pthread_mutex_unlock(&(lqp->lock));
    }    

void* lqsearch(lqueue_t *qp,bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {           
    lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
    pthread_mutex_lock(&(lqp->lock));
    lqp = (lqueue_t*)qsearch(lqp->q,searchfn,skeyp);
    pthread_mutex_unlock(&(lqp->lock));
    return lqp;
}

void* lqremove(lqueue_t *qp,                                                      
              bool (*searchfn)(void* elementp,const void* keyp),                
              const void* skeyp) {  
    lqueueStruct_t *lqp = (lqueueStruct_t *)qp; 
    pthread_mutex_lock(&(lqp->lock));
    lqp = (lqueue_t*)qremove(lqp->q,searchfn,skeyp);
    pthread_mutex_unlock(&(lqp->lock));
    return lqp;
    
}



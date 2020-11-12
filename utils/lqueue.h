
#pragma once

#include "queue.h"                                                              
#include <stdlib.h>                                                             
#include <stdio.h>
#include <pthread.h>  


typedef void lqueue_t;

lqueue_t* lqopen(void);

void lqclose(lqueue_t *qp);

int32_t lqput(lqueue_t *qp, void *elementp);

void* lqget(lqueue_t *qp);

void lqapply(lqueue_t *qp, void (*fn)(void* elementp));

void* lqsearch(lqueue_t *qp,bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp);

void* lqremove(lqueue_t *qp,                                                      
              bool (*searchfn)(void* elementp,const void* keyp),                
              const void* skeyp);

void lqsetdelay(int timeDelay);

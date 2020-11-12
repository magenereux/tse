#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "lhash.h"
#include <pthread.h>  
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

static int timeDelay = 0;

typedef struct lhashtableStruct {
  hashtable_t* lhtable;
  pthread_mutex_t lock;
} lhashtableStruct_t;

lhashtable_t *lhopen(uint32_t hsize) {
    lhashtableStruct_t *lhtp;
    if ((lhtp = (lhashtableStruct_t *)malloc(sizeof(lhashtableStruct_t)))==NULL)
        return NULL;
    pthread_mutex_init(&(lhtp->lock),NULL);
    (lhtp->lhtable)= hopen(hsize);
    return (lhashtable_t*)lhtp;
}

void lhclose(lhashtable_t *lhtp) {
	if (lhtp!=NULL) {
        lhashtableStruct_t *lhtsp=(lhashtableStruct_t*)lhtp;
        pthread_mutex_lock(&(lhtsp->lock));
        sleep(timeDelay);
        hclose(lhtsp->lhtable);
        pthread_mutex_unlock(&(lhtsp->lock));
        pthread_mutex_destroy(&(lhtsp->lock));
        free(lhtsp);
    }
}

int32_t lhput(lhashtable_t *lhtp, void *ep, const char *key, int keylen) {
    if (lhtp==NULL || ep==NULL){
        return -1;
    }
    lhashtableStruct_t *lhtsp = (lhashtableStruct_t *)lhtp;
    pthread_mutex_lock(&(lhtsp->lock));
    printf("put locked\n");
    sleep(timeDelay);
    hput(lhtsp->lhtable,ep,key,keylen);
    pthread_mutex_unlock(&(lhtsp->lock));
    printf("put unlocked\n");
    return 0;
}

void lhapply(lhashtable_t *lhtp, void (*fn)(void* ep)) {
    if (lhtp!=NULL) {
        lhashtableStruct_t *lhtsp = (lhashtableStruct_t *)lhtp; 
        pthread_mutex_lock(&(lhtsp->lock));
        printf("apply locked\n");
        sleep(timeDelay);
        happly(lhtsp->lhtable,fn);
        pthread_mutex_unlock(&(lhtsp->lock));
        printf("apply unlocked\n");
    }
}

void *lhsearch(lhashtable_t *lhtp,bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key,int32_t keylen) {
    if (lhtp!=NULL){
        lhashtableStruct_t *lhtsp = (lhashtableStruct_t *)lhtp; 
        pthread_mutex_lock(&(lhtsp->lock));
        printf("search locked\n");
        sleep(timeDelay);
        void* data = hsearch(lhtsp->lhtable,searchfn,key,keylen);
        pthread_mutex_unlock(&(lhtsp->lock));
        printf("search unlocked\n");
        return data;
    }
    return NULL;
}

void *lhremove(lhashtable_t *lhtp,bool (*searchfn)(void* elementp, const void* searchkeyp),const char *key,int32_t keylen) {
    if (lhtp!=NULL){
        lhashtableStruct_t *lhtsp = (lhashtableStruct_t *)lhtsp; 
        pthread_mutex_lock(&(lhtsp->lock));
        printf("remove locked\n");
        sleep(timeDelay);
        void* data = hremove(lhtsp->lhtable,searchfn,key,keylen);
        pthread_mutex_unlock(&(lhtsp->lock));
        printf("remove unlocked\n");
        return data;
    }
    return NULL;
}

void lqsetdelay(int seconds){
    timeDelay = seconds;
}
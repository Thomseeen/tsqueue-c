/********************************************************************************
 * cqueue.h
 ********************************************************************************/
#ifndef CQUEUE_H
#define CQUEUE_H

#include <inttypes.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct CQueueItem {
    void* next;
    void* value;
} CQueueItem;

typedef struct CQueueHead {
    CQueueItem* head;
    CQueueItem* tail;
    uint64_t len;
    pthread_mutex_t* lock;
} CQueueHead;

CQueueHead* cqueue_create();
void cqueue_destroy(CQueueHead* handle);
void cqueue_push(CQueueHead* handle, void* value);
void* cqueue_pop(CQueueHead* handle);
uint64_t cqueue_len(CQueueHead* handle);

#endif /* CQUEUE_H */
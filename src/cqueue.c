#include "cqueue.h"

CQueueHead* cqueue_create() {
    CQueueHead* handle = malloc(sizeof(CQueueHead));
    handle->head = NULL;
    handle->tail = NULL;
    handle->len = 0;

    pthread_rwlock_t* lock = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(lock, NULL);
    handle->rwlock = lock;

    return handle;
}

void cqueue_destroy(CQueueHead* handle) {
    pthread_rwlock_destroy(handle->rwlock);
    free(handle->rwlock);
    free(handle);
}

void cqueue_push(CQueueHead* handle, void* value) {
    CQueueItem* new_item_p = malloc(sizeof(CQueueItem));
    new_item_p->value = value;
    new_item_p->next = NULL;

    pthread_rwlock_wrlock(handle->rwlock);
    if (!handle->head) {
        handle->head = new_item_p;
        handle->tail = new_item_p;
        pthread_rwlock_unlock(handle->rwlock);
    } else {
        pthread_rwlock_unlock(handle->rwlock);
        pthread_rwlock_wrlock(handle->rwlock);
        handle->tail->next = new_item_p;
        handle->tail = new_item_p;

        handle->len++;
        pthread_rwlock_unlock(handle->rwlock);
    }
}

void* cqueue_pop(CQueueHead* handle) {
    pthread_rwlock_rdlock(handle->rwlock);

    if (!handle->head) {
        pthread_rwlock_unlock(handle->rwlock);
        return NULL;
    } else {
        pthread_rwlock_unlock(handle->rwlock);
        pthread_rwlock_wrlock(handle->rwlock);

        CQueueItem* item_copy_p = handle->head;
        handle->head = item_copy_p->next;

        void* return_value_p = item_copy_p->value;

        free(item_copy_p);
        handle->len--;

        pthread_rwlock_unlock(handle->rwlock);
        return return_value_p;
    }
}
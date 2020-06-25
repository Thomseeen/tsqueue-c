#include "cqueue.h"

CQueueHead* cqueue_create() {
    CQueueHead* handle = malloc(sizeof(CQueueHead));
    handle->head = NULL;
    handle->tail = NULL;
    handle->len = 0;

    pthread_mutex_t* lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock, NULL);
    handle->lock = lock;

    return handle;
}

void cqueue_destroy(CQueueHead* handle) {
    pthread_mutex_destroy(handle->lock);
    free(handle->lock);
    free(handle);
}

void cqueue_push(CQueueHead* handle, void* value) {
    CQueueItem* new_item_p = malloc(sizeof(CQueueItem));
    new_item_p->value = value;
    new_item_p->next = NULL;

    pthread_mutex_lock(handle->lock);
    if (!handle->head) {
        handle->head = new_item_p;
        handle->tail = new_item_p;
    } else {
        handle->tail->next = new_item_p;
        handle->tail = new_item_p;
    }
    handle->len++;
    pthread_mutex_unlock(handle->lock);
}

void* cqueue_pop(CQueueHead* handle) {
    pthread_mutex_lock(handle->lock);
    void* return_val;

    if (!handle->head) {
        return_val = NULL;
    } else {
        CQueueItem* item_copy_p = handle->head;
        handle->head = item_copy_p->next;

        void* return_value_p = item_copy_p->value;

        free(item_copy_p);
        handle->len--;

        return_val = return_value_p;
    }
    pthread_mutex_unlock(handle->lock);
    return return_val;
}

uint64_t cqueue_len(CQueueHead* handle) {
    pthread_mutex_lock(handle->lock);
    uint64_t return_len = handle->len;
    pthread_mutex_unlock(handle->lock);
    return return_len;
}
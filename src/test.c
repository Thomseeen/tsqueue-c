#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cqueue.h"

void* consumer_thread(void* arg);
void* producer_thread(void* arg);

#define CONSUMER_THREADS_CNT 3

CQueueHead* queue_handle;
pthread_t producer_thread_id;
pthread_t consumer_thread_id[CONSUMER_THREADS_CNT];

int main(int argc, char const* argv[]) {
    queue_handle = cqueue_create();
    printf("New queue with %lu elements\n", cqueue_len(queue_handle));

    pthread_create(&producer_thread_id, NULL, producer_thread, NULL);

    for (int ii = 0; ii < CONSUMER_THREADS_CNT; ii++) {
        pthread_create(&consumer_thread_id[ii], NULL, consumer_thread,
                       (void*)&consumer_thread_id[ii]);
    }

    pthread_join(producer_thread_id, NULL);
    for (int ii = 0; ii < CONSUMER_THREADS_CNT; ii++) {
        pthread_join(consumer_thread_id[ii], NULL);
    }

    cqueue_destroy(queue_handle);

    exit(0);
}

void* consumer_thread(void* arg) {
    int failed_cnt = 0;
    while (1) {
        void* value = cqueue_pop(queue_handle);
        // printf("Thread %lu reads queue len as %lu elements\n", *(pthread_t*)arg,
        //       cqueue_len(queue_handle));
        if (value) {
            printf("Thread %lu got item %d\n", *(pthread_t*)arg, *(int*)value);
        } else {
            failed_cnt++;
            if (failed_cnt > 100) {
                printf("Thread %lu stopped, no more values\n", *(pthread_t*)arg);
                break;
            }
        }
        usleep(100);
    }
    pthread_exit(0);
}

void* producer_thread(void* arg) {
    for (int cnt_val = 0; cnt_val < CONSUMER_THREADS_CNT * 5; cnt_val++) {
        printf("Thread producer reads queue len as %lu elements writing %d\n",
               cqueue_len(queue_handle), cnt_val);
        int* value = malloc(sizeof(int));
        memcpy(value, &cnt_val, sizeof(int));
        cqueue_push(queue_handle, (void*)value);
        usleep(100);
    }
    printf("Thread producer stopped, no more values\n");
    pthread_exit(0);
}

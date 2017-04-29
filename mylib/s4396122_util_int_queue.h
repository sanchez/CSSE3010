#ifndef UTIL_INT_QUEUE_H
#define UTIL_INT_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

struct intQueue {
    struct intQueue *next;
    int data;
};

typedef struct {
    struct intQueue *head;
    struct intQueue *tail;
    int size;
} IntQueue;

IntQueue* s4396122_util_int_queue_create();
int s4396122_util_int_queue_size(IntQueue *q);
void s4396122_util_int_queue_push(IntQueue *q, int d);
int s4396122_util_int_queue_pop(IntQueue *q);
void s4396122_util_int_queue_free(IntQueue *q);

#endif

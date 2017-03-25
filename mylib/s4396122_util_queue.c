#include "s4396122_util_queue.h"

/**
 * Creates the queue for reference later on
 * @return Returns a Queue Object for later applying operations on
 */
Queue* s4396122_util_queue_create() {
    Queue *q = malloc(sizeof(Queue));
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/**
 * Gets the size of the Queue
 * @param  q The Queue for which to read the size from
 * @return   The size of the Queue
 */
int s4396122_util_queue_size(Queue *q) {
    return q->size;
}

/**
 * Pushes an element to the top of the Queue
 * @param q The Queue to push data to
 * @param d The data to be added
 */
void s4396122_util_queue_push(Queue *q, void *d) {
    struct queue *qElement = malloc(sizeof(struct queue));
    qElement->data = d;
    if (q->size) {
        q->tail->next = qElement;
    } else {
        q->head = qElement;
    }
    q->tail = qElement;
    q->size++;
}

/**
 * Pops an element from the bottom of the Queue and returns the data
 * @param q Queue to pop from
 */
void* s4396122_util_queue_pop(Queue *q) {
    if (q->head == NULL) {
        return NULL;
    }
    struct queue *oldHead = q->head;
    void *d = oldHead->data;
    if (oldHead->next != NULL) {
        q->head = oldHead->next;
    }
    free(oldHead);
    q->size--;
    return d;
}
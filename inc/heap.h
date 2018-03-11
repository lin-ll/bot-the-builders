#ifndef HEAP_H
#define HEAP_H

typedef struct {
    int priority;
    int data;
} node_t;

typedef struct {
    node_t *nodes;
    int len;
    int size;
} heap_t;

int isEmpty(heap_t *h);
void push (heap_t *h, int priority, int data);
int pop (heap_t *h);

#endif

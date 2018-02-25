#ifndef HEAP_H
#define HEAP_H

typedef struct {
    int priority;
    char *data;
} node_t;

typedef struct {
    node_t *nodes;
    int len;
    int size;
} heap_t;

int isEmpty(heap_t *h);
void push (heap_t *h, int priority, char *data);
char *pop (heap_t *h);

#endif

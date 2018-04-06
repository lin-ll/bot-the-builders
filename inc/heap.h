/*----------------------------------------------------------------------------*/
/* heap.h 																      */
/* Operates like a stack in C												  */
/*----------------------------------------------------------------------------*/

#ifndef HEAP_H
#define HEAP_H

// define a node in the stack
typedef struct {
    int priority;
    int data;
} node_t;

// define the stack itself
typedef struct {
    node_t *nodes;
    int len;
    int size;
} heap_t;

heap_t *heap_init();

int isEmpty(heap_t *h);
void push (heap_t *h, int priority, int data);
int pop (heap_t *h);

#endif

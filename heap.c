#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

heap_t *heap_init() {
    heap_t *heap;

    heap = (heap_t *) malloc(sizeof(heap_t));

    heap->len = 0;
    heap->size = 0;
    heap->nodes = (node_t *) malloc(0);

    return heap;
}

int isEmpty(heap_t *h) {
    return h->size == 0;
}

void push (heap_t *h, int priority, int data) {
    if (h->len + 1 >= h->size) {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = (node_t *)realloc(h->nodes, h->size * sizeof (node_t));
    }
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && h->nodes[j].priority > priority) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j = j / 2;
    }
    h->nodes[i].priority = priority;
    h->nodes[i].data = data;
    h->len++;
}

int pop (heap_t *h) {
    int i, j, k;
    if (!h->len) {
        return -1;
    }
    int data = h->nodes[1].data;

    h->nodes[1] = h->nodes[h->len];
    int priority = h->nodes[1].priority;

    h->len--;

    i = 1;
    while (1) {
        k = i;
        j = 2 * i;
        if (j <= h->len && h->nodes[j].priority < priority) {
            k = j;
        }
        if (j + 1 <= h->len && h->nodes[j + 1].priority < h->nodes[k].priority) {
            k = j + 1;
        }
        if (k == i) {
            break;
        }
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    h->nodes[i] = h->nodes[h->len + 1];
    return data;
}

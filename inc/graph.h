/*graph.h*/
#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef enum {UNDIRECTED=0,DIRECTED} graph_type_e;

/* Adjacency list node*/
typedef struct adjlist_node
{
    int vertex;                /*Index to adjacency list array*/
    struct adjlist_node *next; /*Pointer to the next node*/
}adjlist_node_t, *adjlist_node_p;

/* Adjacency list */
typedef struct adjlist
{
    int num_members;           /*number of members in the list (for future use)*/
    adjlist_node_t *head;      /*head of the adjacency linked list*/
}adjlist_t, *adjlist_p;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct graph
{
    graph_type_e type;        /*Directed or undirected graph */
    int num_vertices;         /*Number of vertices*/
    adjlist_p adjListArr;     /*Adjacency lists' array*/
}graph_t, *graph_p;

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph_p createGraph(int n, graph_type_e type);

/*Destroys the graph*/
void destroyGraph(graph_p graph);

/* Adds an edge to a graph*/
void addEdge(graph_t *graph, int src, int dest);

/* Returns 1 if edge exists, 0 otherwise */
int hasEdge(graph_t *graph, int src, int dest);

#endif

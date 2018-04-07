/*graph.c*/
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/* Exit function to handle fatal errors*/
__inline void err_exit(char* msg)
{
  printf("[Fatal Error]: %s \nExiting...\n", msg);
  exit(1);
}
/* Function to create an adjacency list node*/
adjlist_node_p createNode(int v)
{
    adjlist_node_p newNode = (adjlist_node_p)malloc(sizeof(adjlist_node_t));
    if(!newNode)
        err_exit("Unable to allocate memory for new node");

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph_p createGraph(int n, graph_type_e type)
{
    int i;
    size_t blah = sizeof(graph_t);
    printf("before create\n");
    graph_p graph = (graph_p)malloc(blah);
    printf("after create\n");
    if(!graph)
        err_exit("Unable to allocate memory for graph");
    graph->num_vertices = n;
    graph->type = type;

    /* Create an array of adjacency lists*/
    graph->adjListArr = (adjlist_p)malloc(n * sizeof(adjlist_t));
    if(!graph->adjListArr)
        err_exit("Unable to allocate memory for adjacency list array");

    for(i = 0; i < n; i++)
    {
        graph->adjListArr[i].head = NULL;
        graph->adjListArr[i].num_members = 0;
    }

    return graph;
}

/*Destroys the graph*/
void destroyGraph(graph_p graph)
{
    if(graph)
    {
        if(graph->adjListArr)
        {
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->num_vertices; v++)
            {
                adjlist_node_p adjListPtr = graph->adjListArr[v].head;
                while (adjListPtr)
                {
                    adjlist_node_p tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->adjListArr);
        }
        /*Free the graph*/
        free(graph);
    }
}

/* Adds an edge to a graph*/
void addEdge(graph_t *graph, int src, int dest)
{
    /* Add an edge from src to dst in the adjacency list*/
    adjlist_node_p newNode = createNode(dest);
    newNode->next = graph->adjListArr[src].head;
    graph->adjListArr[src].head = newNode;
    graph->adjListArr[src].num_members++;

    if(graph->type == UNDIRECTED)
    {
        /* Add an edge from dest to src also*/
        newNode = createNode(src);
        newNode->next = graph->adjListArr[dest].head;
        graph->adjListArr[dest].head = newNode;
        graph->adjListArr[dest].num_members++;
    }
}

/* Returns 1 if edge exists, 0 otherwise */
int hasEdge(graph_t *graph, int src, int dest)
{
    adjlist_node_p curr = graph->adjListArr[src].head;
    while (curr != NULL)
    {
        int check = curr->vertex;
        if (dest == check)
        {
            return 1;
        }
    }

    return 0;
}

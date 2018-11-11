// common implementations for both
// the adjacency matrix and list
// version of the directed graph

#include "directed_graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Prints a pair of nodes connected by an edge.
 * @param from The source node.
 * @param to The destination node.
 */
static void print_pair(graph_node_ptr from, graph_node_ptr to);

void print_graph(const cgraph_ptr graph)
{
    if (graph != NULL) {
        for_each_edge(graph, print_pair);
    }
}

// ------------------------------------- TRANSLATION-UNIT-LOCAL FUNCTIONS ------------------------------------- //

void print_pair(graph_node_ptr from, graph_node_ptr to)
{
    if (from != NULL && to != NULL) {
        printf("( %s ) --> ( %s )\n", from->payload, to->payload);
    }
}

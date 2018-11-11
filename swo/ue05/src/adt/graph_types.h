#ifndef UE05_GRAPH_TYPES_H
#define UE05_GRAPH_TYPES_H

#include <stdlib.h>

/**
 * The data associated with graph nodes.
 */
typedef const char* node_payload;

/**
 * The identifying descriptor of graph nodes.
 */
typedef unsigned node_id;

/**
 * A node within a {@link graph}.
 */
typedef struct {
    /**
     * The unique node descriptor.
     */
    node_id id;

    /**
     * The data associated with a node.
     */
    node_payload payload;
} graph_node;
typedef graph_node *graph_node_ptr;

/**
 * Represents a graph nodes dependencies.
 * (i. e. all nodes that have an outgoing
 * edge landing at said node.)
 */
typedef node_id *dependencies;

/**
 * A node which is aware of its dependencies.
 */
typedef struct {
    /**
     * All information about the node itself.
     */
    graph_node data;

    /**
     * The dependencies of the node.
     */
    dependencies dependency_arr;

    /**
     * Have a guess what this describes.
     */
    size_t nr_of_dependencies;
} conscious_node;

/**
 * A directed graph.
 */
typedef struct graph graph;
typedef struct graph *graph_ptr;
typedef const struct graph *cgraph_ptr;

/**
 * Functions of this type can be used with the graphs
 * for_each_edge function to achieve some sort of
 * iterative behaviour without the need of a separate
 * iterator class for each graph implementation.
 *  Objects of this function type are supposed
 *  to be supplied with the source and destination node
 *  of a directed edge.
 */
typedef void(*graph_edge_consumer)(graph_node_ptr from, graph_node_ptr to);

#endif //!UE05_GRAPH_TYPES_H

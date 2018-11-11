#ifndef UE05_DIRECTED_GRAPH_H
#define UE05_DIRECTED_GRAPH_H

#include <stdlib.h>
#include "graph_types.h"

/**
 * @return A pointer to a newly created graph.
 */
graph_ptr create_graph();

/**
 * Deletes the supplied graph.
 * @param ppgraph The graph to delete.
 */
void delete_graph(graph_ptr *ppgraph);

/**
 * @param pgraph The graph to add a node to.
 * @param payload The payload of the graph node.
 * @return The id of the newly created node.
 * @Note If the supplied graph is NULL, the function
 *        will still return a theoretically valid id
 *        but any usage results in undefined behaviour.
 */
node_id add_graph_node(graph_ptr pgraph, node_payload payload);

/**
 * Removes the node with the specified id and all edges
 * associated with it from the graph.
 * If the node does not know about the node with said id,
 * this function does nothing.
 * @param pgraph The graph to remove the node from.
 * @param id The id of the node to delete.
 */
void remove_graph_node(graph_ptr pgraph, node_id id);

/**
 * Adds a directed edge to the graph, starting from the
 * supplied source node and ending at the destination node.
 * @param pgraph The graph to add the edge to.
 * @param from The source node for the edge.
 * @param to The destination node for the edge.
 */
void add_graph_edge(graph_ptr pgraph, node_id from, node_id to);

/**
 * Removes the edge going from <i>from</i> to <i>to</i>.
 * @param pgraph The graph to remove the edge from.
 * @param from The source node of the edge.
 * @param to The destination node of the edge.
 */
void remove_graph_edge(graph_ptr pgraph, node_id from, node_id to);

/**
 * @param cpgraph The graph to query.
 * @return The number of nodes in the graph.
 */
size_t nr_of_nodes(cgraph_ptr cpgraph);

/**
 * @param pgraph The graph to query.
 * @return The number of edges in the graph.
 */
size_t nr_of_edges(cgraph_ptr pgraph);

/**
 * Prints all relevant nodes. (i.e. nodes
 * that have outgoing or incoming directed
 * edges.)
 * @param cpgraph The graph to print.
 */
void print_graph(cgraph_ptr cpgraph);

/**
 * A simple iterator function which executes the
 * supplied consumer function for each edge in
 * the graph (with the respective edge as
 * argument).
 * @param cpgraph The graph to iterate.
 * @param consume_edge_f The consumer function to use.
 */
void for_each_edge(cgraph_ptr cpgraph, graph_edge_consumer consume_edge_f);

/**
 * @param cpgraph The graph to query.
 * @param id The id of the node to fetch information from.
 * @return The number of outgoing edges.
 */
size_t node_out_degree(cgraph_ptr cpgraph, node_id id);

/**
 * @param cpgraph The graph to query.
 * @param id The id of the node to fetch information from.
 * @return The number of incoming edges.
 */
size_t node_in_degree(cgraph_ptr cpgraph, node_id id);

/**
 * @param cpgraph The graph from which to fetch 'em nodes.
 * @return An array containing all nodes of the graphs
 *          including their respective direct dependencies.
 */
conscious_node *get_conscious_nodes(cgraph_ptr cpgraph);

/**
 * @param cpgraph The graph to query.
 * @param id The id of the node to fetch.
 * @return The node within the supplied graph with the
 *          specified id. If the graph does not contain
 *          such a node, NULL is returned.
 */
graph_node_ptr get_graph_node_by_id(cgraph_ptr cpgraph, node_id id);

#endif //!UE05_DIRECTED_GRAPH_H

// warning: ISO C forbids an empty translation unit
typedef int i_am_a_number_making_the_compiler_happy_because_the_pedantic\
_compile_option_warns_about_empty_translation_units;

#ifndef UE05_USE_LIST

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "../adt/graph_types.h"
#include "../adt/directed_graph.h"
#include "../adt/node_list.h"

/**
 * The type of the elements in the
 * adjacency matrix.
 */
typedef bool adjacenty_mat_type;

/**
 * A matrix representing adjacency relations.
 */
typedef adjacenty_mat_type *adjacency_mat;

struct graph {
    /**
     * A matrix representing relationships
     * between all nodes of the graph.
     */
    adjacency_mat edges;

    /**
     * A single linked list of graph nodes.
     */
    list_node_ptr nodes;

    /**
     * The current dimension of the edge matrix.
     */
    size_t mat_side_len;
};

/**
 * @param pgraph The graph for which to fetch the first
 *              free node it.
 * @return The first available node id for the
 *          supplied graph. If the graphs adjacency matrix
 *          is full, it is resized.
 */
static node_id generate_id(graph_ptr pgraph);

/**
 * A function <i>f : R<sup>2</sup> --> R</i>
 * which maps the 2D indices <i>from</i> and
 * <i>to</i> to a 1D array index.
 * @param side_length The side length of the virtual matrix.
 * @param from The row to target.
 * @param to The column to target.
 * @return The projected 1D array index.
 */
static size_t calc_mat_index(size_t side_length, node_id from, node_id to);

/**
 * Sets the specified edge value for the edge from node
 * <i>from</i> to node <i>to</i>.
 * @param pgraph The graph to set the edge value for.
 * @param from The source node of the edge.
 * @param to The destination node of the edge.
 * @param edge_flag The value to set. <i>true</i> adds the
 *                  edge, <i>false</i> removes it.
 */
static void set_edge_value(graph_ptr pgraph, node_id from, node_id to, bool edge_flag);

/**
 * @param payload The payload of the node.
 * @param id The id of the node.
 * @return A pointer to the newly created graph node.
 */
static graph_node_ptr create_graph_node(node_payload payload, node_id id);

/**
 * Increases the supplied graphs edge matrix by one
 * row and one column.
 * @param pgraph The graph to manipulate.
 */
static void enlarge_matrix(graph_ptr pgraph);

/**
 * @param cpgraph The graph to query.
 * @param id The id to search for.
 * @return True if there is a node with the
 *          specified ID in the supplied graph.
 */
static bool node_exists(cgraph_ptr cpgraph, node_id id);

/**
 * @param cpgraph The graph to query
 * @param source The source node.
 * @param destination The destination node.
 * @return True if there is an edge going from the
 *          supplied source node to the destination node,
 *          False otherwise.
 */
bool node_has_edge_to(cgraph_ptr cpgraph, node_id source, node_id destination);

graph_ptr create_graph()
{
    graph_ptr pgraph = (graph_ptr) malloc(sizeof(graph));
    pgraph->mat_side_len = 0;
    pgraph->edges = NULL;
    pgraph->nodes = NULL;
    return pgraph;
}

void delete_graph(graph_ptr *ppgraph)
{
    if (ppgraph != NULL && *ppgraph != NULL) {
        graph_ptr pgraph = *ppgraph;

        // delete_list only deletes list nodes, not
        // associated data! We have to do that manually
        list_node_ptr curr = pgraph->nodes;
        while (curr != NULL) {
            free((char *) curr->data->payload);
            free(curr->data);
            curr = curr->next;
        }

        // free remaining memory associated with the graph
        free(pgraph->edges);
        delete_list(&(pgraph->nodes));
        free(pgraph);

        *ppgraph = NULL;
    }
}

node_id add_graph_node(graph_ptr pgraph, node_payload payload)
{
    node_id id = 0;
    if (pgraph != NULL) {
        id = generate_id(pgraph);
        graph_node_ptr pnode = create_graph_node(payload, id);
        append_to_list_sorted(&(pgraph->nodes), pnode);
    }
    return id;
}

void remove_graph_node(graph_ptr pgraph, node_id id)
{
    if (pgraph != NULL) {
        list_node_ptr node_to_delete = get_node_by_id(pgraph->nodes, id);
        if (node_to_delete != NULL) { // if it exists
            // remove all edges to the node
            for (unsigned i = 0; i < nr_of_nodes(pgraph); ++i) {
                remove_graph_edge(pgraph, id, i);
                remove_graph_edge(pgraph, i, id);
            }

            list_node_ptr curr = pgraph->nodes;

            // if the node to delete is not the first one
            if (node_to_delete != curr) {
                // go on an epic adventure to find the
                // predecessor to dat boi
                while (curr->next != node_to_delete) {
                    curr = curr->next;
                }

                // unchain node_to_delete
                curr->next = node_to_delete->next;
            } else {
                // uncahin node_to_delete
                pgraph->nodes = node_to_delete->next;
            }

            // "encapsulate" the node so it
            // represents an individual list
            node_to_delete->next = NULL;
            // and delete it
            free((char *) node_to_delete->data->payload);
            free(node_to_delete->data);
            delete_list(&node_to_delete);
        }
    }
}

void add_graph_edge(graph_ptr pgraph, node_id from, node_id to)
{
    set_edge_value(pgraph, from, to, true);
}

void remove_graph_edge(graph_ptr pgraph, node_id from, node_id to)
{
    set_edge_value(pgraph, from, to, false);
}

size_t nr_of_nodes(const cgraph_ptr pgraph)
{
    size_t nr = 0;
    if (pgraph != NULL) {
        nr = get_list_size(pgraph->nodes);
    }
    return nr;
}

size_t nr_of_edges(const cgraph_ptr pgraph)
{
    size_t nr = 0;
    if (pgraph != NULL) {
        size_t mat_arr_length = pgraph->mat_side_len * pgraph->mat_side_len;
        for (size_t i = 0; i < mat_arr_length; ++i) {
            if (pgraph->edges[i]) {
                nr += 1;
            }
        }
    }
    return nr;
}

void for_each_edge(const cgraph_ptr cpgraph, graph_edge_consumer consume_edge_f)
{
    if (cpgraph != NULL && consume_edge_f != NULL) {
        // for each row (source node)
        for (node_id from = 0; from < cpgraph->mat_side_len; ++from) {
            list_node_ptr from_ptr = get_node_by_id(cpgraph->nodes, from);
            // for each column (destination node) of the current row
            for (node_id to = 0; to < cpgraph->mat_side_len; ++to) {
                // if they are connected via an edge
                if (cpgraph->edges[calc_mat_index(cpgraph->mat_side_len, from, to)]) {
                    list_node_ptr to_ptr = get_node_by_id(cpgraph->nodes, to);
                    // consume it
                    consume_edge_f(from_ptr->data, to_ptr->data);
                }
            }
        }
    }
}

size_t node_out_degree(const cgraph_ptr cpgraph, node_id source)
{
    size_t degree = 0;
    if (node_exists(cpgraph, source)) {
        for (node_id i = 0; i < cpgraph->mat_side_len; ++i) {
            // +1 for each edge in the <source>th row
            // because the row _i_ represents outgoing
            // edges to the node with id _i_
            if (node_has_edge_to(cpgraph, source, i)) {
                degree += 1;
            }
        }
    }
    return degree;
}

size_t node_in_degree(const cgraph_ptr cpgraph, node_id destination)
{
    size_t degree = 0;
    // if destination is not out of bounds for the matrix
    if (node_exists(cpgraph, destination)) {
        // +1 for each edge in the <destination>th row
        // because the row _i_ represents outgoing
        // edges to the node with id _i_
        for (node_id i = 0; i < cpgraph->mat_side_len; ++i) {
            if (node_has_edge_to(cpgraph, i, destination)) {
                degree += 1;
            }
        }
    }
    return degree;
}

conscious_node *get_conscious_nodes(const cgraph_ptr cpgraph)
{
    conscious_node *nodes_arr = NULL;
    if (cpgraph != NULL) {
        size_t node_count = nr_of_nodes(cpgraph);
        nodes_arr = (conscious_node *) malloc(sizeof(conscious_node) * node_count);
        for (node_id i = 0; i < node_count; ++i) {
            if (node_exists(cpgraph, i)) {
                conscious_node cn;
                cn.data = *(get_graph_node_by_id(cpgraph, i));
                cn.dependency_arr = (dependencies) malloc(sizeof(node_id) * node_in_degree(cpgraph, i));
                cn.nr_of_dependencies = 0;
                for (node_id dep = 0; dep < cpgraph->mat_side_len; ++dep) {
                    if (node_has_edge_to(cpgraph, dep, i)) {
                        cn.dependency_arr[cn.nr_of_dependencies++] = dep;
                    }
                }
                nodes_arr[i] = cn;
            }
        }
    }
    return nodes_arr;
}

graph_node_ptr get_graph_node_by_id(cgraph_ptr cpgraph, node_id id)
{
    graph_node_ptr pgraph_node = NULL;
    if (cpgraph != NULL) {
        list_node_ptr curr = cpgraph->nodes;
        while (curr != NULL && curr->data->id != id) {
            curr = curr->next;
        }
        if (curr != NULL) {
            pgraph_node = curr->data;
        }
    }
    return pgraph_node;
}

// ------------------------------------- TRANSLATION-UNIT-LOCAL FUNCTIONS ------------------------------------- //

graph_node_ptr create_graph_node(node_payload payload, node_id id)
{
    graph_node_ptr pnode = (graph_node_ptr) malloc(sizeof(graph_node));
    char *str_cpy = (char *) malloc(sizeof(char) * strlen(payload) + 1);
    strcpy(str_cpy, payload);
    pnode->payload = str_cpy;
    pnode->id = id;
    return pnode;
}

node_id generate_id(graph_ptr pgraph)
{
    node_id id = 0;
    // if the graph is empty or there are no nodes
    // return 0!
    if (pgraph != NULL) {

        if (pgraph->edges == NULL) {
            enlarge_matrix(pgraph);
            // we do not need to do anything else here;
            // 0 is the next available index!
        } else {
            // the node list is sorted so if the first
            // node id is not 0 we can immediately return 0.
            // otherwise we have to search for any "ID gaps"
            if (pgraph->nodes->data->id == 0) {

                list_node_ptr curr = pgraph->nodes->next;
                while (curr != NULL && curr->data->id == id + 1) {
                    id += 1; // same as curr = curr->data->id;
                    curr = curr->next;
                }

                // in both cases (curr == NULL and curr != NULL)
                // we have to increment since _id_ still holds
                // the last unavailable id before the one that's
                // available
                id +=1 ;

                if (curr == NULL) {
                    // id would now index our matrix out
                    // of bounds so lets make it BIGGUR
                    enlarge_matrix(pgraph);
                }
            }
        }
    }
    return id;
}

size_t calc_mat_index(size_t side_length, node_id from, node_id to)
{
    return from * side_length + to;
}

void set_edge_value(graph_ptr pgraph, node_id from, node_id to, bool edge_flag)
{
    if (pgraph != NULL) {
        if (node_exists(pgraph, from) && node_exists(pgraph, to)) {
            pgraph->edges[calc_mat_index(pgraph->mat_side_len, from, to)] = edge_flag;
        }
    }
}

void enlarge_matrix(graph_ptr pgraph)
{
    // prevent enlargement if the matrix is not completely
    // STUFFED with IDs
    if (pgraph != NULL && pgraph->mat_side_len == nr_of_nodes(pgraph)) {
        // allocate new matrix
        size_t new_mat_side_len = pgraph->mat_side_len + 1;
        size_t arr_length = new_mat_side_len * new_mat_side_len;
        adjacency_mat new_mat = (adjacency_mat) malloc(sizeof(adjacenty_mat_type) * arr_length);

        if (pgraph->mat_side_len == 0) {
            new_mat[0] = false;
        } else {
            // copy values
            for (node_id from = 0; from < pgraph->mat_side_len; ++from) {

                // write old values to the new matrix
                for (node_id to = 0; to < pgraph->mat_side_len; ++to) {
                    size_t old_index = calc_mat_index(pgraph->mat_side_len, from, to);
                    size_t new_index = calc_mat_index(new_mat_side_len, from, to);
                    new_mat[new_index] = pgraph->edges[old_index];
                }

                // set the edge flags for new nodes to 0

                // graph->mat_side_len is now the last column / row since the
                // new side length is now graph->mat_side_len + 1
                size_t last_row_index = calc_mat_index(new_mat_side_len, (node_id) pgraph->mat_side_len, from);
                size_t last_col_index = calc_mat_index(new_mat_side_len, from, (node_id) pgraph->mat_side_len);
                size_t corner_index = calc_mat_index(new_mat_side_len, (node_id) pgraph->mat_side_len,
                                                     (node_id) pgraph->mat_side_len);
                new_mat[last_row_index] = false;
                new_mat[last_col_index] = false;
                new_mat[corner_index] = false;
            }
        }

        // delete old matrix and deploy new one
        free(pgraph->edges);
        pgraph->edges = new_mat;
        pgraph->mat_side_len += 1;
    }
}

bool node_exists(const cgraph_ptr pgraph, node_id id)
{
    list_node_ptr pgraph_node = NULL;
    if (pgraph != NULL) {
        pgraph_node = get_node_by_id(pgraph->nodes, id);
    }
    return pgraph_node != NULL;
}

bool node_has_edge_to(const cgraph_ptr cpgraph, node_id source, node_id destination)
{
    size_t index = calc_mat_index(cpgraph->mat_side_len, source, destination);
    return cpgraph->edges[index];
}

#endif

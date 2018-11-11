// warning: ISO C forbids an empty translation unit
typedef int i_am_a_number_making_the_compiler_happy_because_the_pedantic\
_compile_option_warns_about_empty_translation_units;

#ifdef UE05_USE_LIST

#include <stdbool.h>
#include <memory.h>
#include "directed_graph.h"
#include "node_list.h"

#define INITIAL_VECTOR_SIZE 5

/**
 * A dynamically growing array of list node pointers.
 */
typedef list_node_ptr *list_node_ptr_vec;

struct graph {
    /**
     * A vector containing all nodes of the graph.
     */
    list_node_ptr_vec nodes;

    /**
     * The current size of the vector.
     */
    size_t vec_size;
};

/**
 * @param payload The payload of the node.
 * @param id The id of the node.
 * @return A pointer to the newly created graph node.
 */
static graph_node_ptr create_graph_node(node_payload payload, node_id id);

/**
 * @param pgraph The graph for which to fetch the first
 *              free node it.
 * @return The first available node id for the
 *          supplied graph. If the graphs adjacency matrix
 *          is full, it is resized.
 */
static node_id generate_id(graph_ptr pgraph);

/**
 * Resizes the supplied graphs node vector
 * by doubling its capacity.
 * @param pgraph The graph of which the node vector
 *                  shall be resized.
 */
static void grow_node_vector(graph_ptr pgraph);

/**
 * @param pgraph The graph to query.
 * @param id The id of the node to search for.
 * @return True if the node exists, false
 *          otherwise.
 */
static bool node_exists(cgraph_ptr pgraph, node_id id);

/**
 * Deletes the node with the specified id.
 * @param pgraph The graph from which to delete the node.
 * @param id The id of the node to delete.
 */
static void delete_graph_node(graph_ptr pgraph, node_id id);

graph_ptr create_graph()
{
    graph_ptr pgraph = (graph_ptr) malloc(sizeof(graph));
    pgraph->nodes = (list_node_ptr_vec) malloc(sizeof(list_node_ptr) * INITIAL_VECTOR_SIZE);
    for (size_t i = 0; i < INITIAL_VECTOR_SIZE; ++i) {
        pgraph->nodes[i] = NULL;
    }
    pgraph->vec_size = INITIAL_VECTOR_SIZE;
    return pgraph;
}

void delete_graph(graph_ptr *ppgraph)
{
    if (ppgraph != NULL && *ppgraph != NULL) {
        graph_ptr pgraph = *ppgraph;
        // if pgraph is not null, pgraph->nodes
        // is guaranteed to not be null because it's
        // a vector which is first allocated during
        // graph creation!
        for (node_id id = 0; id < pgraph->vec_size; ++id) {
            if (node_exists(pgraph, id)) {
                delete_graph_node(pgraph, id);
            }
        }
        free(pgraph->nodes);
        free(pgraph);
        *ppgraph = NULL;
    }
}

void delete_graph_node(graph_ptr pgraph, node_id id)
{
    if (node_exists(pgraph, id)) {
        free((char *) pgraph->nodes[id]->data->payload);
        free(pgraph->nodes[id]->data);
        delete_list(&(pgraph->nodes[id]));
    }
}

node_id add_graph_node(graph_ptr pgraph, node_payload payload)
{
    node_id id = 0;
    if (pgraph != NULL) {
        id = generate_id(pgraph);
        graph_node_ptr pnode = create_graph_node(payload, id);
        // if the next free id is not within the vectors bounds
        if (pgraph->vec_size <= id) {
            grow_node_vector(pgraph);
        }
        // add the graph node to the vector
            // the vector contains lists with their head
            // being the source graph node of an edge
            // and all ancestors representing the destination
            // nodes for edges going out from said source node.
        append_to_list_sorted(&(pgraph->nodes[id]), pnode);
    }
    return id;
}

void remove_graph_node(graph_ptr pgraph, node_id id)
{
    if (node_exists(pgraph, id)) {
        // iterate all nodes and delete edges to the node with the specified id
        for (size_t current_id = 0; current_id < pgraph->vec_size; ++current_id) {
            if (node_exists(pgraph, (node_id) current_id)) {
                // since the node_id is the index into the vector
                remove_graph_edge(pgraph, (node_id) current_id, id);
            }
        }
        delete_graph_node(pgraph, id);
    }
}

void add_graph_edge(graph_ptr pgraph, node_id from, node_id to)
{
    // if both from and to exist
    if (node_exists(pgraph, from) && node_exists(pgraph, to)) {
        // add an entry with TO to the edge-list of FROM
        graph_node_ptr to_ptr = pgraph->nodes[to]->data;
        append_to_list_sorted(&(pgraph->nodes[from]), to_ptr);
    }
}

void remove_graph_edge(graph_ptr pgraph, node_id from, node_id to)
{
    if (node_exists(pgraph, from) && node_exists(pgraph, to)) {
        // traverse list of edges until the node with the desired ID is reached
        list_node_ptr prev_edge = pgraph->nodes[from];
        list_node_ptr curr_edge =prev_edge->next;
        while (curr_edge != NULL && curr_edge->data->id != to) {
            prev_edge = curr_edge;
            curr_edge = curr_edge->next;
        }
        if (curr_edge != NULL) {
            // and delete dat boi
            delete_list_node_after(prev_edge);
        }
    }
}

size_t nr_of_nodes(const cgraph_ptr cpgraph)
{
    size_t nr = 0;
    if (cpgraph != NULL) {
        for (size_t i = 0; i < cpgraph->vec_size; ++i) {
            if (node_exists(cpgraph, (node_id) i)) {
                nr += 1;
            }
        }
    }
    return nr;
}

size_t nr_of_edges(const cgraph_ptr pgraph)
{
    size_t nr = 0;
    if (pgraph != NULL) {
        for (size_t i = 0; i < pgraph->vec_size; ++i) {
            if (node_exists(pgraph, (node_id) i)) {
                // for each node in the vector
                list_node_ptr curr_edge = pgraph->nodes[i]->next;
                while (curr_edge != NULL) {
                    // add +1 to the total sum for each
                    // list node after the head
                    nr += 1;
                    curr_edge = curr_edge->next;
                }
            }
        }
    }
    return nr;
}

void for_each_edge(const cgraph_ptr cpgraph, graph_edge_consumer consume_edge_f)
{
    if (cpgraph != NULL) {
        // for each source node
        for (size_t i = 0; i < cpgraph->vec_size; ++i) {
            list_node_ptr curr_src_node = cpgraph->nodes[i];
            if (curr_src_node != NULL) {
                graph_node_ptr src_graph_node = curr_src_node->data;
                list_node_ptr curr_dest_node = curr_src_node->next;
                // combine it with each of its destination nodes
                while (curr_dest_node != NULL) {
                    // and invoke the callback
                    consume_edge_f(src_graph_node, curr_dest_node->data);
                    curr_dest_node = curr_dest_node->next;
                }
            }
        }
    }
}

size_t node_out_degree(const cgraph_ptr cpgraph, node_id id)
{
    size_t degree = 0;
    if (node_exists(cpgraph, id)) {
        // first node is source node, ancestors are edge destinations
        list_node_ptr edge_list = cpgraph->nodes[id]->next;
        degree = get_list_size(edge_list);
    }
    return degree;
}

size_t node_in_degree(const cgraph_ptr cpgraph, node_id id)
{
    size_t degree = 0;
    // if the destination is valid
    if (node_exists(cpgraph, id)) {
        // for each node in the vector
        for (size_t i = 0; i < cpgraph->vec_size; ++i) {
            if (node_exists(cpgraph, (node_id) i)) {
                // check if its edge list contains
                // the node with the specified id
                list_node_ptr curr_dest_node = cpgraph->nodes[i]->next;
                bool has_edge_to_id = false;
                while (curr_dest_node != NULL && !has_edge_to_id) {
                    has_edge_to_id = curr_dest_node->data->id == id;
                    curr_dest_node = curr_dest_node->next;
                }
                if (has_edge_to_id) {
                    degree += 1;
                }
            }
        }
    }
    return degree;
}

// ------------------------------------- TRANSLATION-UNIT-LOCAL FUNCTIONS ------------------------------------- //

graph_node_ptr create_graph_node(node_payload payload, node_id id)
{
    graph_node_ptr pnode = (graph_node_ptr) malloc(sizeof(graph_node));
    char *str_copy = (char *) malloc(sizeof(char) * strlen(payload) + 1);
    strcpy(str_copy, payload);
    pnode->payload = str_copy;
    pnode->id = id;
    return pnode;
}

node_id generate_id(graph_ptr pgraph)
{
    // if the graph is null, return 0
    node_id id = 0;
    if (pgraph != NULL) {
        // search the vector for the first entry
        // that is not NULL. if the vector is full,
        // return an id outside the bounds of the vector
        while (id < pgraph->vec_size && pgraph->nodes[id] != NULL) {
            id += 1;
        }
    }
    return id;
}

void grow_node_vector(graph_ptr pgraph)
{
    if (pgraph != NULL) {
        size_t new_length = pgraph->vec_size * 2;
        pgraph->nodes = (list_node_ptr_vec) realloc(pgraph->nodes, sizeof(list_node_ptr) * new_length);
        for (size_t i = 0; i < pgraph->vec_size; ++i) {
            // also initialize new items to NULL
            pgraph->nodes[pgraph->vec_size + i] = NULL;
        }
        pgraph->vec_size = new_length;
    }
}

bool node_exists(const cgraph_ptr pgraph, node_id id)
{
    return pgraph != NULL && id < pgraph->vec_size && pgraph->nodes[id] != NULL;
}

#endif

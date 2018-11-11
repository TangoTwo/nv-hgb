#ifndef UE05_NODE_LIST_H
#define UE05_NODE_LIST_H

#include "graph_types.h"
#include <stdlib.h>

/**
 * A node of a single linked list.
 */
typedef struct list_node {
    /**
     * The information stored in the list node.
     */
    graph_node_ptr data;

    /**
     * The next node in the list.
     */
    struct list_node *next;
} list_node;
typedef list_node *list_node_ptr;

/**
 * Recursively deletes a list starting from a given node.
 * Note: this does not delete the data within the list!
 * @param plist The list to delete.
 */
void delete_list(list_node_ptr *plist);

/**
 * Appends the supplied node to the back of the list.
 * @param plist The list to append to.
 * @param data The graph data to add to the appended list node.
 */
void append_to_list_sorted(list_node_ptr *plist, graph_node_ptr data);

/**
 * @param list The list to query.
 * @param id The id of the node to fetch.
 * @return The list node with the specified id.
 */
list_node_ptr get_node_by_id(list_node_ptr list, node_id id);

/**
 * Removes the direct ancestor of <i>plist_node</i>.
 * @param plist_node The predecessor of the node to delete.
 */
void delete_list_node_after(list_node_ptr plist_node);

/**
 * @param list The list to use for evaluation.
 * @return The number of elements in the list.
 */
size_t get_list_size(list_node_ptr list);

#endif //!UE05_NODE_LIST_H

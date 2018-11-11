#include "node_list.h"
#include <stdlib.h>
#include <assert.h>

/**
 * @param data The data to stuff into the list node.
 * @return A pointer to the newly created list node.
 */
static list_node_ptr create_list_node(graph_node_ptr data);

void delete_list(list_node_ptr *plist)
{
    if (plist != NULL) {
        list_node_ptr node = *plist;
        if (node != NULL) {
            list_node_ptr next = node->next;
            free(node);
            // point plist to the next node in the list;
            *plist = next;
            // delete recursively
            delete_list(plist);
        }
        *plist = NULL;
    }
}

void append_to_list_sorted(list_node_ptr *plist, graph_node_ptr data)
{
    if (plist != NULL && data != NULL) {
        list_node_ptr node = create_list_node(data);
        if (*plist == NULL) {
            *plist = node;
        } else {
            list_node_ptr current = *plist;
            while (current->next != NULL && current->next->data->id < data->id) {
                // this should nevur evur heppen
                assert(data->id != current->next->data->id);
                current = current->next;
            }

            // if there is an "ID gap", insert inbetween
            if (current->next != NULL && current->next->data->id > data->id) {
                node->next = current->next;
            }
            current->next = node;
        }
    }
}

list_node_ptr get_node_by_id(list_node_ptr list, node_id id)
{
    if (list == NULL || list->data->id > id) {
        return NULL;
    } else if (list->data->id == id) {
        return list;
    } else {
        return get_node_by_id(list->next, id);
    }
}

void delete_list_node_after(list_node_ptr plist_node)
{
    if (plist_node != NULL && plist_node->next != NULL) {
        list_node_ptr buffer = plist_node->next->next;
        free(plist_node->next);
        plist_node->next = buffer;
    }
}

size_t get_list_size(list_node_ptr list)
{
    size_t size = 0;
    if (list != NULL) {
        size = 1 + get_list_size(list->next);
    }
    return size;
}

// ------------------------------------- TRANSLATION-UNIT-LOCAL FUNCTIONS ------------------------------------- //

list_node_ptr create_list_node(graph_node_ptr data)
{
    list_node_ptr node = (list_node_ptr) malloc(sizeof(list_node));
    node->next = NULL;
    node->data = data;
    return node;
}

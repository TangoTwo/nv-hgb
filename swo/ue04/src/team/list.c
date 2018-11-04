#include "list.h"
#include <malloc.h>

/**
 * @param comp A team composition.
 * @return A pointer to the newly created
 *          list node containing the supplied
 *          team composition.
 */
static list_node *create_list_node(const int comp[]);

/**
 * Deletes a node.
 * @param node The node to delete.
 */
static void delete_list_node(list_node *pnode);

list *create_list()
{
    list *plist = (list *) malloc(sizeof(list));
    plist->last = NULL;
    plist->first = NULL;
    plist->size = 0;
    return plist;
}

void delete_list(list *plist)
{
    if (plist != NULL) {
        clear_list(plist);
        delete_list_handle(plist);
    }
}

void add_team_to_list(list_handle *phandle, const int *comp)
{
    if (phandle != NULL) {
        list_node *pnode = create_list_node(comp);
        if (phandle->first == NULL) {
            phandle->first = pnode;
        } else {
            phandle->last->next = pnode;
        }
        phandle->last = pnode;
        phandle->size += 1;
    }
}

void pop_list(list_handle *phandle)
{
    if (phandle != NULL && phandle->size > 0) {
        if (phandle->size == 1) {
            // if there is only one element
            delete_list_node(phandle->first);
            phandle->first = NULL;
            phandle->last = NULL;
        } else { // phandle->size > 1
            // if there are more elements
            list_node *pnode = phandle->first;
            // pnode->next->next is safe here since
            // phandle->size > 1!
            while (pnode->next->next != NULL) {
                pnode = pnode->next;
            }
            delete_list_node(pnode->next);
            pnode->next = NULL;
            // let last point to the previously
            // second-to-last node
            phandle->last = pnode;
        }
        phandle->size -= 1;
#ifdef PARCEL_WRAPPING_POPS
        // I wanted to use a random
        // tab distance but calling
        // rand() a few thousand (pot.
        // million) times wrecked even
        // my HPC-Machine.
        static int tabs = 0;
        for (int i = 0; i < tabs; ++i) {
            printf("\t");
        }
        printf("*pop*\n");
        if (tabs == 5) {
            tabs = 0;
        } else {
            tabs++;
        }
#endif
    }
}

void clone_list(list_handle *destination, const list_handle *source)
{
    if (source != NULL && destination != NULL) {
        list_node *pnode = source->first;
        while (pnode != NULL) {
            add_team_to_list(destination, pnode->team_composition);
            pnode = pnode->next;
        }
    } else {
        printf("Could not clone list.");
    }
}

list_handle *get_list_handle(list_handle *phandle, list_node *pnode)
{
    list *new_phandle = create_list();
    new_phandle->first = pnode;
    new_phandle->last = phandle->last;

    // calculate new size
    int i = 0;
    list_node *new_pnode = new_phandle->first;
    while (new_pnode != NULL) {
        ++i;
        new_pnode = new_pnode->next;
    }
    new_phandle->size = i;

    return new_phandle;
}

void delete_list_handle(list_handle *phandle)
{
    if (phandle != NULL) {
        free(phandle);
    }
}

void print_list(list_handle *phandle)
{
    if (phandle != NULL) {
        list_node *pnode = phandle->first;
        while (pnode != NULL) {
            int *comp = pnode->team_composition;
            // team size is guaranteed to be 4
            printf("[%d, %d, %d, %d]\n", comp[0], comp[1], comp[2], comp[3]);
            pnode = pnode->next;
        }
    } else {
        printf("(NULL list)\n");
    }
}

void clear_list(list_handle *phandle)
{
    if (phandle != NULL) {
        list_node *pnode = phandle->first;
        while (pnode != NULL) {
            list_node *prev = pnode;
            pnode = pnode->next;
            delete_list_node(prev);
        }
        // reset values
        phandle->first = NULL;
        phandle->last = NULL;
        phandle->size = 0;
    }
}

// ---- PRIVATE FUNCTIONS ---- //

list_node *create_list_node(const int *comp)
{
    list_node *pnode = (list_node *) malloc(sizeof(list_node));
    pnode->next = NULL;
    pnode->team_composition = (int *) malloc(sizeof(int) * TEAM_SIZE);
    for (int i = 0; i < TEAM_SIZE; ++i) {
        pnode->team_composition[i] = comp[i];
    }
    return pnode;
}

void delete_list_node(list_node *pnode)
{
    free(pnode->team_composition);
    pnode->team_composition = NULL;
    free(pnode);
}

#ifndef UE04_LIST_H
#define UE04_LIST_H

#define TEAM_SIZE 4

/**
 * A node containing a team composition.
 */
typedef struct list_node
{
    /**
     * The next node in the list.
     */
    struct list_node *next;

    /**
     * The array of team members.
     */
    int *team_composition;
} list_node;

/**
 * A list of team compositions.
 */
typedef struct list
{
    /**
     * The first team in the list.
     */
    list_node *first;

    /**
     * The last team in the list.
     */
    list_node *last;

    /**
     * The size of the container.
     */
    int size;
} list;

/**
 * A list handle is an anchor at
 * which a list is "grabbed". It
 * can grab a list at any element
 * within the list, meaning that all
 * iterative processes using this handle
 * will start at the aforementioned
 * element.
 */
typedef list list_handle;

/**
 * @return A pointer to an empty list.
 * @note Do not forget to call delete_list after usage.
 */
list *create_list();

/**
 * Removes all elements from the list
 * without deleting the entire handle.
 * @param phandle The list to clear.
 */
void clear_list(list_handle *phandle);

/**
 * Deletes the supplied list and all
 * of its content.
 * @param plist The list to delete.
 */
void delete_list(list *plist);

/**
 * Adds a new team composition to the supplied
 * list of teams.
 * @param phandle The list to append to.
 * @param comp The team composition.
 */
void add_team_to_list(list_handle *phandle, const int *comp);

/**
 * Removes the last team from the list.
 * @param phandle The list to pop().
 */
void pop_list(list_handle *phandle);


 /**
  * Creates a deep copy of the source list and writes it to
  * the destination list.
  * @param destination The list which receives the cloned elements.
  * @param source The source of the clone.
  */
void clone_list(list_handle *destination, const list_handle *source);

/**
 * Creates a new list object with its first element
 * being the supplied list node.
 * @param phandle The list to create a new handle for.
 * @param pnode The node "at which to grab the list".
 * @return A pointer to a list starting with the
 *          supplied <i>node</i>.
 * @note This does not clone the list contents, it just
 *          creates a new anchor. This anchor must be
 *          deleted using {@link delete_list_handle}.
 */
list_handle *get_list_handle(list_handle *phandle, list_node *pnode);

/**
 * Deletes the supplied list handle.
 * @param phandle The handle to delete.
 * @note This does not delete the lists contents.
 */
void delete_list_handle(list_handle *phandle);

/**
 * Prints a list to the standard output.
 * @param phandle The list to print.
 */
void print_list(list_handle *phandle);

#endif //!UE04_LIST_H

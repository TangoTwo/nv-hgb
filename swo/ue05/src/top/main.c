#include <stdio.h>
#include <stdbool.h>
#include "directed_graph.h"
#include "graph_types.h"

#define MAX 100

// Sorting a graph using kahns algorithm
void sort_topologically(graph_ptr pgraph, conscious_node nodes[], bool *error)
{
    /*
     * In order do differentiate between my comments and the
     * outline of kahns algorithm, I put the outline comments
     * at the beginning of a line.
     */

    size_t node_count = nr_of_nodes(pgraph);
    if (error != NULL) {
        *error = false;
    }

//      L ← Empty list that will contain the sorted elements
    conscious_node *L = (conscious_node*) malloc(sizeof(conscious_node) * node_count);
    size_t li = 0;

//      S ← Set of all nodes with no incoming edge
    conscious_node *S = (conscious_node*) malloc(sizeof(conscious_node) * node_count);
    size_t si = 0;

    conscious_node *All = get_conscious_nodes(pgraph);

    // move all nodes without dependencies to S
    for (size_t i = 0; i < node_count; ++i) {
        if (All[i].nr_of_dependencies == 0) {
            S[si++] = All[i];
        }
    }

//    while S is non-empty do
    while (si > 0) {
//          remove a node n from S
        conscious_node n = S[--si];
//          add n to tail of L
        L[li++] = n;
//          for each node m with an edge e from n to m do
        for (size_t potential_m_index = 0; potential_m_index < node_count; ++potential_m_index) {
            conscious_node potential_m = All[potential_m_index];
            size_t m_dep_index = 0;
            while (m_dep_index < potential_m.nr_of_dependencies && potential_m.dependency_arr[m_dep_index] != n.data.id) {
                ++m_dep_index;
            }
            if (m_dep_index < potential_m.nr_of_dependencies) {
                conscious_node m = potential_m;
//                  remove edge e from the graph
                remove_graph_edge(pgraph, n.data.id, m.data.id);
//                  if m has no other incoming edges then
                if (node_in_degree(pgraph, m.data.id) == 0) {
//                  insert m into S
                    S[si++] = m;
                }
            }
        }
    }

    if (nr_of_edges(pgraph) != 0 && error != NULL) {
        *error = true;
    }
    for (size_t cpy = 0; cpy < node_count; ++cpy) {
        nodes[cpy] = L[cpy];
        free(All[cpy].dependency_arr);
    }

    free(All);
    free(L);
    free(S);
}

void print_sort_result(const cgraph_ptr cpgraph, const conscious_node *nodes, bool error)
{
    if (error) {
        printf("Graph was cyclic");
    } else {
        for (size_t i = 0; i < nr_of_nodes(cpgraph); ++i) {
            printf("( %s ), ", nodes[i].data.payload);
        }
        printf("\n");
    }
}

int main()
{
    conscious_node nodes[MAX];
    bool error = false;
    graph_ptr pgraph = create_graph();

    // construct graph
    node_id work = add_graph_node(pgraph, "working out a solution");
    node_id finish = add_graph_node(pgraph, "finishing");
    node_id read = add_graph_node(pgraph, "reading the requirements");
    add_graph_edge(pgraph, read, work);
    add_graph_edge(pgraph, work, finish);

    sort_topologically(pgraph, nodes, &error);
    print_sort_result(pgraph, nodes, error);

    // construct cyclic graph
    graph_ptr cyclic_graph = create_graph();
    node_id a = add_graph_node(cyclic_graph, "A");
    add_graph_edge(cyclic_graph, a, a);

    sort_topologically(cyclic_graph, nodes, &error);
    print_sort_result(cyclic_graph, nodes, error);

    delete_graph(&pgraph);
    delete_graph(&cyclic_graph);
    return 0;
}

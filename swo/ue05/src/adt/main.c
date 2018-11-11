#include "directed_graph.h"
#include "unic.h"

int main()
{
    unic_init();

    // create graph
    graph_ptr pgraph = create_graph();
    unic_ass_true(pgraph != NULL, "Graph creation");
    unic_ass_eq_i((int) nr_of_nodes(pgraph), 0, "No nodes on new graph");
    unic_ass_eq_i((int) nr_of_edges(pgraph), 0, "No edges on new graph");

    //add nodes

    node_id a = add_graph_node(pgraph, "A");
    unic_ass_eq_i((int) nr_of_nodes(pgraph), 1, "New node nr after node insert");
    unic_ass_eq_i((int) nr_of_edges(pgraph), 0, "No edges after node insert");

    node_id b = add_graph_node(pgraph, "B");
    unic_ass_eq_i((int) nr_of_nodes(pgraph), 2, "New node nr after two node inserts");
    unic_ass_eq_i((int) nr_of_edges(pgraph), 0, "No edges after two node inserts");

    node_id c = add_graph_node(pgraph, "C");

    node_id d = add_graph_node(pgraph, "D");

    node_id e = add_graph_node(pgraph, "E");
    unic_ass_eq_i((int) nr_of_nodes(pgraph), 5, "New node nr after all node insert");
    unic_ass_eq_i((int) nr_of_edges(pgraph), 0, "No edges after all node inserts");

    // edges from A

    add_graph_edge(pgraph, a, b);
    unic_ass_eq_i(5, (int) nr_of_nodes(pgraph), "No new node nr after edge insert");
    unic_ass_eq_i(1, (int) nr_of_edges(pgraph), "New edge nr after edge insert");

    add_graph_edge(pgraph, a, c);
    unic_ass_eq_i(5, (int) nr_of_nodes(pgraph), "No new node nr after two edge inserts");
    unic_ass_eq_i(2, (int) nr_of_edges(pgraph), "New edge nr after two edge inserts");

    add_graph_edge(pgraph, a, d);

    add_graph_edge(pgraph, a, e);
    unic_ass_eq_i(5, (int) nr_of_nodes(pgraph), "No new node nr after all edge inserts");
    unic_ass_eq_i(4, (int) nr_of_edges(pgraph), "New edge nr after all edge inserts");

    unic_ass_eq_i(4, (int) node_out_degree(pgraph, a), "Correct outgoing node degree");

    // edges from B
    add_graph_edge(pgraph, b, d);
    unic_ass_eq_i(5, (int) nr_of_nodes(pgraph), "No new node nr after new edge insert from different node");
    unic_ass_eq_i(5, (int) nr_of_edges(pgraph), "New edge nr after new edge insert from different node");

    unic_ass_eq_i(2, (int) node_in_degree(pgraph, d), "Correct incoming node degree");

    // edges from C
    add_graph_edge(pgraph, c, b);
    add_graph_edge(pgraph, c, e);

    // print dis boi once
    print_graph(pgraph);

    // remove an existing edge
    remove_graph_edge(pgraph, b, d);
    unic_ass_eq_i(5, (int) nr_of_nodes(pgraph), "Unchanged number of node after edge removal");
    unic_ass_eq_i(6, (int) nr_of_edges(pgraph), "Decreased number of edges after edge removal");

    // remove none existend edge
    remove_graph_edge(pgraph, b, a);
    unic_ass_eq_i(5, (int) nr_of_nodes(pgraph), "Unchanged number of nodes after nonexistent edge removal");
    unic_ass_eq_i(6, (int) nr_of_edges(pgraph), "Unchanged number of edges after nonexistent edge removal");

    // remove existing node
    remove_graph_node(pgraph, c);
    unic_ass_eq_i(4, (int) nr_of_nodes(pgraph), "Decreased number of nodes after node removal");
    unic_ass_eq_i(3, (int) nr_of_edges(pgraph), "Decreased number of edges after node removal");

    unic_ass_eq_i(3, (int) node_out_degree(pgraph, a), "Correct outgoing node degree after manipulation");
    unic_ass_eq_i(1, (int) node_in_degree(pgraph, e), "Correct incoming node degree after manipulation");

    // remove nonexistent node
    remove_graph_node(pgraph, 8);
    unic_ass_eq_i(4, (int) nr_of_nodes(pgraph), "Unchanged number of nodes after nonexistent node removal");
    unic_ass_eq_i(3, (int) nr_of_edges(pgraph), "Unchanged number of edges after nonexistent node removal");

    // a second print
    // note: print_graph prints only nodes
    // connected by an edge
    print_graph(pgraph);

    delete_graph(&pgraph);
    unic_ass_true(pgraph == NULL, "Graph is null pointer after delete");
    return unic_get_results();
}

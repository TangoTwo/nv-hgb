#include "graph.hpp"

using handle_t = graph_t<char>::handle_t;
using std::string;
using std::cout;
using std::endl;

static void test_basic()
{
    graph_t<char> graph;

    handle_t a_handle = graph.add_vertex(vertex_t<char>('w'));
    handle_t b_handle = graph.add_vertex(vertex_t<char>('a'));
    handle_t c_handle = graph.add_vertex(vertex_t<char>('t'));

    graph.add_edge(a_handle, a_handle, 1);
    graph.add_edge(a_handle, b_handle, 2);
    graph.add_edge(b_handle, c_handle, 10);
    graph.add_edge(c_handle, b_handle, 5);

    cout << graph << endl;
}

static void test_shortest_path()
{
    graph_t<string> graph;

    // add vertices
    handle_t dis_handle          = graph.add_vertex(vertex_t<string>("This"));
    handle_t is_handle           = graph.add_vertex(vertex_t<string>("is"));
    handle_t a_handle            = graph.add_vertex(vertex_t<string>("a"));
    handle_t sentence_handle     = graph.add_vertex(vertex_t<string>("sentence"));
    handle_t distraction1_handle = graph.add_vertex(vertex_t<string>("memes"));
    handle_t distraction2_handle = graph.add_vertex(vertex_t<string>("cookies"));

    // add edges
    graph.add_edge(dis_handle, is_handle, 1);

        // distraction 1, those damn memes
    graph.add_edge(is_handle, distraction1_handle, 1);
    graph.add_edge(is_handle, a_handle, 1);
    graph.add_edge(distraction1_handle, a_handle, 2);

        // distraction 2
    graph.add_edge(a_handle, distraction2_handle, 2);
    graph.add_edge(a_handle, sentence_handle, 1);
    graph.add_edge(distraction2_handle, sentence_handle, 1);

    cout << graph << endl;

    cout << "Shortest path from \""
         << graph.get_vertex(dis_handle)
         << "\" to \""
         << graph.get_vertex(sentence_handle)
         << "\": " << endl;
    auto path = graph.get_shortest_path_between(dis_handle, sentence_handle);
    for (const auto &vertex : path) {
        cout << graph.get_vertex(vertex) << " ";
    }
    cout << "(length: " << graph.shortest_path(dis_handle, sentence_handle) << ")" << endl;
}

int main()
{
    test_basic();
    test_shortest_path();
    return 0;
}
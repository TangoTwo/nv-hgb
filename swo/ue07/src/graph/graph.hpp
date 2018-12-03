#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
#include "vertex.hpp"

template <typename T>
class graph_t
{
public: // typedefs

    using handle_t = unsigned;
    using weight_t = int;

public: // methods

    /**
     * Adds the supplied vertex to the graph.
     * @param vertex The vertex to add.
     * @return A handle to identify the added node.
     */
    graph_t::handle_t add_vertex(vertex_t<T> vertex);

    /**
     * @param handle The handle of the vertex to retrieve
     * @return The vertex associated with a certain handle.
     */
    vertex_t<T> get_vertex(handle_t handle) const;

    /**
     * Adds an edge from the source node denoted by _from_
     * to the destination node denoted by _to_ and applies
     * the specified _weight_ to the edge.
     * @param from The handle for the source node of the edge.
     * @param to The handle for the destination node of the edge.
     * @param weight The weight of the edge.
     */
    void add_edge(handle_t from, handle_t to, weight_t weight);

    /**
     * Prints the graph to the supplied output stream.
     * @param out The output stream to write to.
     * @return The used output stream.
     */
    std::ostream & print(std::ostream &os) const;

    /**
     * @param from The node at which to start searching for paths to _to_.
     * @param to The node denoting the path destination.
     * @return The computed shorted path between the vertices
     *          associated with the handles _from_ and _to_.
     */
    std::vector<handle_t> get_shortest_path_between(handle_t from, handle_t to) const;

    /**
     * @param from The node at which to start searching for paths to _to_.
     * @param to The node denoting the path destination.
     * @return The length of the shortest path from _from_ to _to_.
     * @note This method does not follow the naming scheme since I suppose
     *       I am required to coincide with the exercises interface.
     */
    int shortest_path(handle_t from, handle_t to) const;

    /**
     * @param vertex The vertex of which to retrieve all neighbours.
     * @return All neighbours of _vertex_.
     */
    std::vector<handle_t> get_neighbours_of(handle_t vertex) const;

    /**
     * @param from The source node of the edge.
     * @param to The destination node of the edge.
     * @return The weight of the edge going from _from_ to _to_.
     */
    weight_t get_edge_weight(handle_t from, handle_t to) const;

private: // methods

    /**
     * Asserts that the supplied handle is associated
     * with a node within the graph.
     * @param handle The handle to check.
     * @param msg The message to display if the assertion fails.
     * @throws invalid_argument if the handle does
     *         not denote a node.
     */
    void _assert_valid_handle(handle_t handle, const std::string &msg) const;

private: // members

    /**
     * The adjacency matrix of the graph.
     */
    std::vector<std::vector<weight_t>> _adjacencies;

    /**
     * The nodes with their respective id.
     */
    std::map<handle_t, vertex_t<T>> _vertices;
};

template <typename T>
std::ostream & operator<<(std::ostream &os, const graph_t<T> & graph);

/* IMPLEMENTATIONS */

template <typename T>
auto graph_t<T>::add_vertex(const vertex_t<T> vertex) -> handle_t
{
    handle_t new_handle = 0;
    // find first free id
    while (_vertices.find(new_handle) != std::cend(_vertices)) {
        ++new_handle;
    }

    // if the new id is outside tha matrix' bounds
    if (new_handle >= _adjacencies.size()) {
        // add a new row
        _adjacencies.push_back(std::vector<weight_t>(new_handle+1, 0));

        // add a new column
        for (std::size_t i = 0; i < new_handle + 1; ++i) {
            _adjacencies[i].resize(new_handle+1, 0);
        }
    }

    // insert vertex
    _vertices.insert(std::make_pair(new_handle, vertex));
    return new_handle;
}

template <typename T>
void graph_t<T>::add_edge(const handle_t from, const handle_t to, const weight_t weight)
{
    _assert_valid_handle(from, "Invalid source vertex for a new edge.");
    _assert_valid_handle(to, "Invalid destination vertex for a new edge.");
    _adjacencies[from][to] = weight;
}

template <typename T>
std::ostream & graph_t<T>::print(std::ostream &os) const
{
    // print nodes
    os << "[   Vertices   ]" << std::endl;
    auto it = std::cbegin(_vertices);
    while (it != std::cend(_vertices)) {
        os << (it->second);
        ++it;
        if (it != std::cend(_vertices)) {
            os << ", ";
        }
    }
    os << ";" << std::endl;

    // print edges
    os << "[   Edges   ]" << std::endl;
    for (const auto &from : _vertices) {
        for (const auto &to : _vertices) {
            const auto & curr_weight = get_edge_weight(from.first, to.first);
            if (curr_weight != 0) {
                os << from.second << " -> (" << curr_weight << ") -> " << to.second << std::endl;
            }
        }
    }
    return os;
}

template<typename T>
auto graph_t<T>::get_shortest_path_between(const handle_t from, const handle_t to) const -> std::vector<handle_t>
{
    using std::cbegin;
    using std::cend;

    std::list<handle_t> path;

    _assert_valid_handle(from, "Invalid start vertex");
    _assert_valid_handle(to, "Invalid end vertex");

    // unvisited nodes
    std::vector<handle_t> queue;
    // accumulated (min) distances to a particular node
    std::map<handle_t, weight_t> distances;
    // path - construction helper
    std::map<handle_t, handle_t> previous;

    for (const auto &vertex : _vertices) {
        // add current vertex
        queue.push_back(vertex.first);
        // init distances with "infinity"
        distances.insert(std::make_pair(vertex.first, std::numeric_limits<weight_t>::max()));
        // init previous with garbage values
        previous.insert(std::make_pair(vertex.first, -1)); // I use a signed literal to init a signed value *dabs*
    }
    distances[from] = 0;

    while (!queue.empty()) {
        // find vertex with min distance
        handle_t u = *std::min_element(cbegin(queue), cend(queue), [&distances](const auto &p1, const auto &p2) {
            return distances[p1] < distances[p2];
        });

        if (u == to) {
            break;
        }

        // remove u from Q
        queue.erase(std::find(cbegin(queue), cend(queue), u));

        // find lowest-cost neighbour of u
        for (handle_t vertex : get_neighbours_of(u)) {
            weight_t alt_costs = distances[u] + get_edge_weight(u, vertex);
            if (alt_costs < distances[vertex]) {
                distances[vertex] = alt_costs;
                previous[vertex] = u;
            }
        }
    }

    // fill path list by reverse-iteration
    handle_t u = to;
    auto it = std::front_insert_iterator<std::list<handle_t>>(path);
    if (previous.find(u) != cend(previous) || u == from) {
        while (previous.find(u) != cend(previous)) {
            *it = u;
            u = previous[u];
        }
    }

    return std::vector<handle_t>(cbegin(path), cend(path));
}

template<typename T>
int graph_t<T>::shortest_path(const handle_t from, const handle_t to) const
{
    return static_cast<int>(get_shortest_path_between(from, to).size());
}

template<typename T>
vertex_t<T> graph_t<T>::get_vertex(handle_t handle) const
{
    auto it = _vertices.find(handle);
    if (it == std::cend(_vertices)) {
        throw std::invalid_argument("Invalid vertex handle");
    }
    return it->second; // safe do deref iterator here!
}

template<typename T>
auto graph_t<T>::get_neighbours_of(handle_t vertex) const -> std::vector<handle_t>
{
    std::vector<handle_t> neighbours{};
    for (const auto &to : _vertices) {
        if (get_edge_weight(vertex, to.first) != 0) {
            neighbours.push_back(to.first);
        }
    }
    return neighbours;
}

template<typename T>
auto graph_t<T>::get_edge_weight(const handle_t from, const handle_t to) const -> weight_t
{
    _assert_valid_handle(from, "Invalid start vertex");
    _assert_valid_handle(to, "Invalid end vertex");
    return _adjacencies[from][to];
}

template<typename T>
void graph_t<T>::_assert_valid_handle(const handle_t handle, const std::string &msg) const
{
    if (_vertices.find(handle) == std::cend(_vertices)) {
        throw std::invalid_argument(msg);
    }
}

template <typename T>
std::ostream & operator<<(std::ostream &os, const graph_t<T> & graph)
{
    return graph.print(os);
}
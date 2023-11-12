#ifndef LIST2_GRAPH_H
#define LIST2_GRAPH_H

#include <iostream>
#include <fstream>

#include "Edge.h"

class Graph {
    const std::string name;
    std::vector<std::vector<Edge*>> adj;
    std::vector<std::vector<int>> mst;  // by indexes

    Edge* get_edge(const int from_id, const int to_id);
    void dfs_util(const int start, std::vector<bool>& visited, std::vector<int>& dfs_cycle, const int prev);

public:
    int n;
    std::vector<Vertex*> points;

    explicit Graph(int nodes_num, const std::string& name) : n(nodes_num), name(name) {
        adj = std::vector<std::vector<Edge*>>(n);
    }

    void add_point(Vertex* vertex);

    void add_edge(const int p1, const int p2);

    void primMST(const int start);

    std::vector<int> get_dfs_cycle(const int start_id);

    int local_search_weight(int& steps);

    void invert(std::vector<int> &cycle, const int i, const int j);

    int get_inverted_cycle_weight(std::vector<int> cycle, const int current_weight, const int i, const int j);

    int compute_weight(const std::vector<int> &cycle);

    void experiment1();

    void experiment3();
};

struct CompareEdge {
    bool operator()(const Edge* e1, const Edge* e2) {
        return e1->weight > e2->weight;
    }
};

#endif //LIST2_GRAPH_H

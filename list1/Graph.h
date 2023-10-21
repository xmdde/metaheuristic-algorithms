#ifndef LIST1_GRAPH_H
#define LIST1_GRAPH_H

#include <iostream>
#include <fstream>

#include "Edge.h"

class Graph {
    const std::string name;
    std::vector<std::vector<Edge*>> adj;
    std::vector<std::vector<int>> mst;  // by indexes
public:
    int n;
    std::vector<Vertex*> points;

    explicit Graph(int nodes_num, const std::string& name) : n(nodes_num), name(name) {
        adj = std::vector<std::vector<Edge*>>(n);
    }

    void add_point(Vertex* vertex);
    void add_edge(const int p1, const int p2);
    void primMST(const int start, const bool save_to_file);
    void graph_to_json();
    int get_dfs_weight(const bool save_to_file);
    void dfs_util(const int start, std::vector<bool>& visited, std::vector<int>& dfs_cycle, const int prev);
    Edge* get_edge(const int from_id, const int to_id);
};

struct CompareEdge {
    bool operator()(const Edge* e1, const Edge* e2) {
        return e1->weight > e2->weight;
    }
};

#endif  // LIST1_GRAPH_H

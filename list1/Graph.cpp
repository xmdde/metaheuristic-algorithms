#include "Graph.h"
#include <fstream>
#include <queue>

void Graph::add_point(Vertex* vertex) {
    points.push_back(vertex);
}

void Graph::add_edge(const int p1, const int p2) {
    adj[p1].push_back(new Edge(points[p1], points[p2]));
}

void Graph::primMST(const int start, const bool save_to_file) {  // let's say that's an index
    std::vector<bool> inMST(n, false);
    std::vector<int> key(n, INT_MAX);  // weight (probably unnecessary)
    std::vector<int> parent(n, -1);

    std::priority_queue<Edge*, std::vector<Edge*>, CompareEdge> pq;

    key[start] = 0;
    inMST[start] = true;

    for (Edge* edge : adj[start]) {
        int v = edge->get_finish_id() - 1;
        int weight = edge->weight;
        if (!inMST[v] && weight < key[v]) {
            key[v] = weight;
            parent[v] = start;
            pq.push(edge);
        }
    }

    while (!pq.empty()) {
        int u = pq.top()->get_finish_id() - 1;
        pq.pop();
        inMST[u] = true;

        for (Edge* edge: adj[u]) {
            int v = edge->get_finish_id() - 1;
            int weight = edge->weight;
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push(edge);
            }
        }
    }

    uint64_t mst_weight = 0;

    if (save_to_file) {
        const std::string path = "../output/mst-" + name + ".txt";
        std::ofstream ofs(path, std::ios_base::out);
        ofs << "id1;id2\n";

        for (int i = 1; i < n; ++i) {
            ofs << parent[i] + 1 << ";" << i + 1 << "\n";
            mst_weight += key[i];
        }

        ofs.close();
    } else {
        for (int i = 1; i < n; ++i) {
            mst_weight += key[i];
        }
    }

    std::cout << "minimum spanning tree weight: " << mst_weight << "\n";

    mst = std::vector<std::vector<int>>(n);
    for (int i = 1; i < n; ++i) {
        mst[parent[i]].push_back(i);
    }
}

void Graph::graph_to_json() {
    const std::string path = "../output/graph-" + name + ".json";
    std::ofstream ofs(path, std::ios_base::out);
    ofs << "[";
    for (auto point : points) {
        ofs << "{\"id\":" << point->id << ",\"x\":" << point->get_x()
            << ",\"y\":" << point->get_y() << "},";
    }
    ofs << "]";
    ofs.close();
}

void Graph::dfs_to_txt() {
    if (mst.size() == 0) {
        primMST(0, false);
    }

    std::vector<bool> visited(n, false);
    std::vector<Edge*> dfs_cycle;
    dfs_util(0, visited, dfs_cycle, -1);
    // add the last one!
    std::cout << "\n";
}

void Graph::dfs_util(const int start, std::vector<bool>& visited, std::vector<Edge*>& dfs_cycle, const int prev) {
    visited[start] = true;
    std::cout << start + 1 << ' ';

    if (prev != -1) {
        dfs_cycle.push_back(get_edge(prev + 1, start + 1));
    }

    for (int i : mst[start]) {
        if (!visited[i]) {
            dfs_util(i, visited, dfs_cycle, start);
        }
    }
}

Edge* Graph::get_edge(const int from_id, const int to_id) {
    assert(from_id > 0 && from_id <= n && to_id > 0 && to_id <= n && from_id != to_id);

    int idx;
    if (from_id < to_id) {
        idx = to_id - 2;
    } else {
        idx = to_id - 1;
    }

    return adj[from_id - 1][idx];
}
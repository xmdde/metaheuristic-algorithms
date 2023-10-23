#include "Graph.h"
#include <fstream>
#include <queue>
#include <random>

void Graph::add_point(Vertex* vertex) {
    points.push_back(vertex);
}

void Graph::add_edge(const int p1, const int p2) {
    adj[p1].push_back(new Edge(points[p1], points[p2]));
}

void Graph::primMST(const int start, const bool save_to_file) {  // let's say that's an index
    std::vector<bool> inMST(n, false);
    std::vector<int> key(n, INT_MAX);
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

int Graph::get_dfs_weight(const bool save_to_file) {
    if (mst.empty()) {
        primMST(0, false);
    }

    std::vector<bool> visited(n, false);
    std::vector<int> dfs_cycle;
    dfs_cycle.push_back(1);
    dfs_util(0, visited, dfs_cycle, -1);

    int weight = 0;
    for (int i = 1; i < n; ++i) {
        weight += get_edge(dfs_cycle[i-1], dfs_cycle[i])->weight;
    }
    weight += get_edge(dfs_cycle[n-1], 1)->weight;

    if (save_to_file) {
        const std::string path = "../output/dfs-" + name + ".txt";
        std::ofstream ofs(path, std::ios_base::out);
        ofs << "id\n";

        for (int e : dfs_cycle) {
            ofs << e << "\n";
        }

        ofs.close();
    }

    return weight;
}

void Graph::dfs_util(const int start, std::vector<bool>& visited, std::vector<int>& dfs_cycle, const int prev) {
    visited[start] = true;

    if (prev != -1) {
        dfs_cycle.push_back(start + 1);
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

int Graph::random_cycle_weight() {
    srand(time(nullptr));
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, n);

    std::vector<int> cycle;
    int used[n];
    for (int i = 0; i < n; ++i) {
        used[i] = i + 1;
    }

    for (int i = 0; i < n; ++i) {
        bool inserted = false;
        while (!inserted) {
            int x = floor(dist(mt));
            if (used[x] != -1) {
                cycle.push_back(used[x]);
                used[x] = -1;
                inserted = true;
            }
        }
    }

    int weight = 0;
    for (int i = 1; i < n; ++i) {
        weight += get_edge(cycle[i-1], cycle[i])->weight;
    }
    weight += get_edge(cycle[n-1], cycle[0])->weight;

    return weight;
}

void Graph::stats() {
    uint64_t sum_group_of_10 = 0;  // sum of minimum weights in group of 10
    uint64_t sum_group_of_50 = 0;
    int min10 = INT_MAX;
    int min50 = INT_MAX;
    int min_global =INT_MAX;

    for (int i = 1; i <= 1000; ++i) {
        int weight = random_cycle_weight();

        if (min10 > weight) {
            min10 = weight;
        }
        if (min50 > weight) {
            min50 = weight;
        }
        if (min_global > weight) {
            min_global = weight;
        }

        if (i%10 == 0) {
            sum_group_of_10 += min10;
            min10 = INT_MAX;
        }
        if (i%50 == 0) {
            sum_group_of_50 += min50;
            min50 = INT_MAX;
        }
    }

    std::cout << sum_group_of_10/100 << ';' << sum_group_of_50/20 << ';' << min_global;
}

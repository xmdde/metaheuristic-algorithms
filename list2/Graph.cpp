#include "Graph.h"
#include "Edge.h"

#include <fstream>
#include <queue>
#include <random>

void Graph::add_point(Vertex* vertex) {
    points.push_back(vertex);
}

void Graph::add_edge(const int p1, const int p2) {
    adj[p1].push_back(new Edge(points[p1], points[p2]));
}

void Graph::primMST(const int start) {
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

        for (Edge* edge : adj[u]) {
            int v = edge->get_finish_id() - 1;
            int weight = edge->weight;
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push(edge);
            }
        }
    }

    mst = std::vector<std::vector<int>>(n);
    for (int i = 1; i < n; ++i) {
        mst[parent[i]].push_back(i);
        mst[i].push_back(parent[i]);
    }
}

std::vector<int> Graph::get_dfs_cycle(const int start_id) {
    if (mst.empty()) {
        primMST(0);
    }
    std::vector<bool> visited(n, false);
    std::vector<int> dfs_cycle;  // IDs

    dfs_cycle.push_back(start_id);
    dfs_util(start_id - 1, visited, dfs_cycle, -1);
    return dfs_cycle;
}

std::vector<int> Graph::get_random_cycle() {
    std::vector<int> cycle;  // IDs
    for (int i = 1; i <= n; ++i) {
        cycle.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cycle.begin(), cycle.end(), g);

    return cycle;
}

void Graph::invert(std::vector<int>& cycle, int i, int j) { // i, j - indexes
    while (i < j) {
        std::swap(cycle[i], cycle[j]);
        i++;
        j--;
    }
}

int Graph::compute_weight(const std::vector<int>& cycle) {
    int sum = 0;
    for (int i = 1; i < n; ++i) {
        sum += get_edge(cycle[i-1], cycle[i])->weight;
    }
    sum += get_edge(cycle[n-1], cycle[0])->weight;
    return sum;
}

int Graph::local_search_weight(int& steps) {
    srand(time(nullptr));
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, n);

    const int vertex_index = floor(dist(mt));
    std::vector<int> cycle = get_dfs_cycle(vertex_index + 1);
    int cycle_weight = compute_weight(cycle);

    while (true) {
        std::pair<int, int> indexes;
        int best_nbh_weight = INT_MAX;

        for (int i = 0; i < n; ++i) {
             for (int j = i + 1; j < n; ++j) {
                int nbh_weight = get_inverted_cycle_weight(cycle, cycle_weight, i, j);
                if (nbh_weight < best_nbh_weight) {
                    best_nbh_weight = nbh_weight;
                    indexes.first = i;
                    indexes.second = j;
                }
             }
        }

        if (cycle_weight > best_nbh_weight) {
            invert(cycle, indexes.first, indexes.second);
            cycle_weight = compute_weight(cycle);
            steps++;
            best_nbh_weight = INT_MAX;
        } else {
            return cycle_weight;
        }
    }
}

int Graph::local_search_weight_n_nbhs(int& steps) {
    srand(time(nullptr));
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, n);

    std::vector<int> cycle = get_random_cycle();
    int cycle_weight = compute_weight(cycle);

    while (true) {
        std::pair<int, int> indexes;
        int best_nbh_weight = INT_MAX;
        std::vector<std::pair<int,int>> neighbors;
        draw_nbhs(neighbors);

        for (auto pair : neighbors) {
            int nbh_weight = get_inverted_cycle_weight(cycle, cycle_weight, pair.first, pair.second);
            if (nbh_weight < best_nbh_weight) {
                best_nbh_weight = nbh_weight;
                indexes.first = pair.first;
                indexes.second = pair.second;
            }
        }

        if (cycle_weight > best_nbh_weight) {
            invert(cycle, indexes.first, indexes.second);
            cycle_weight = compute_weight(cycle);
            steps++;
            best_nbh_weight = INT_MAX;
        } else {
            return cycle_weight;
        }
    }
}

int Graph::get_inverted_cycle_weight(std::vector<int> cycle, const int current_weight, const int i, const int j) {
    assert(i < j && i >= 0 && j < n);

    if ((i == 0 && j == n - 1) || i + 2 == j) // poza jednym albo wszystkie
        return current_weight;

    const int imin1 = (i == 0) ? n-1 : i-1;
    const int jplus1 = (j == n-1) ? 0 : j+1;
    int result = current_weight - get_edge(cycle[imin1], cycle[i])->weight - get_edge(cycle[j], cycle[jplus1])->weight
                 + get_edge(cycle[imin1], cycle[j])->weight + get_edge(cycle[i], cycle[jplus1])->weight;
    return result;
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

void Graph::experiment1() {
    uint64_t sum = 0;
    int steps_sum = 0;
    int best_result = INT_MAX;
    int num = sqrt(n);

    for (int i = 0; i < num; ++i) {
        std::cout << i << ' ';
        int result = local_search_weight(steps_sum);
        sum += result;
        if (best_result > result)
            best_result = result;
    }
    std::cout << '\n' << name << ';' << sum/num << ';' << best_result << ';' << steps_sum/num;
}

void Graph::draw_nbhs(std::vector<std::pair<int,int>>& nbhs) {
    for (int i = 0; i < n; i++) {
        srand(time(nullptr));
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0, n);
        int idx1 = floor(dist(mt));
        int idx2 = floor(dist(mt));
        while (idx1 == idx2) {
            idx2 = floor(dist(mt));
        }
        if (idx1 < idx2) {
            nbhs.push_back(std::make_pair(idx1, idx2));
        } else {
            nbhs.push_back(std::make_pair(idx2, idx1));
        }
    }
}

void Graph::experiment3() {
    uint64_t sum = 0;
    int steps_sum = 0;
    int best_result = INT_MAX;
    int num = sqrt(n);

    for (int i = 0; i < num; ++i) {
        int result = local_search_weight_n_nbhs(steps_sum);
        sum += result;
        if (best_result > result)
            best_result = result;
    }
    std::cout << name << ';' << sum/num << ';' << best_result << ';' << steps_sum/num;
}

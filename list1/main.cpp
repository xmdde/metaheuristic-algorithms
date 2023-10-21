#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "Edge.h"
#include "Graph.h"

Graph* generate_from_file(std::ifstream& ifs, const std::string& name) {
    bool flag = false;
    std::string current;
    Graph* g;

    if (ifs.is_open()) {
        int num_of_nodes;
        while (!flag) {
            ifs >> current;
            if (current == "DIMENSION") {
                ifs >> current >> num_of_nodes;
                g = new Graph(num_of_nodes, name);
            } else if (current == "NODE_COORD_SECTION") {
                flag = true;
            }
        }

        int id;
        int x;
        int y;
        for (int i = 0; i < num_of_nodes; i++) {
            ifs >> id >> x >> y;
            g->add_point(new Vertex(id, x, y));
        }

        for (int i = 0; i < g->n; i++) {
            for (int j = i + 1; j < g->n; j++) {
                if (i != j) {
                    g->add_edge(i, j);
                    g->add_edge(j, i);
                }
            }
        }
    }

    ifs.close();
    return g;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "wrong number of arguments";
        return 1;
    }
    const std::string name = argv[1];
    std::ifstream ifs("../data/" + name + ".tsp", std::ios_base::in);

    Graph* g = generate_from_file(ifs, name);
    g->primMST(0, false);
    g->dfs_to_txt();
    return 0;
}

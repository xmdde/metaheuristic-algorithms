#ifndef LIST1_EDGE_H
#define LIST1_EDGE_H

#include "Vertex.h"

class Edge {
    Vertex* from;
    Vertex* to;

public:
    int weight;

    Edge(Vertex* v1, Vertex* v2) : from(v1), to(v2) {
        weight = v1->get_distance(v2);
    }

    const int get_start_id();
    const int get_finish_id();
};

#endif  // LIST1_EDGE_H

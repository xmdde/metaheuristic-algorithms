#ifndef LIST2_VERTEX_H
#define LIST2_VERTEX_H

#include <cmath>

class Vertex {
    const int x;
    const int y;
public:
    const int id;

    Vertex(int id, int x, int y) : id(id), x(x), y(y) {}
    int get_distance(const Vertex* v);

    int get_x() {
        return x;
    }

    int get_y() {
        return y;
    }
};

#endif //LIST2_VERTEX_H

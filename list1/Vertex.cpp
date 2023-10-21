#include "Vertex.h"
#include <cmath>

int Vertex::get_distance(const Vertex *v) {
    double distance = sqrt(pow(x - v->x, 2) + pow(y - v->y, 2));
    return static_cast<int>(distance);
}

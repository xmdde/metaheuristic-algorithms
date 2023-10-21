#include "Edge.h"

const int Edge::get_start_id() {
    return from->id;
}

const int Edge::get_finish_id() {
    return to->id;
}
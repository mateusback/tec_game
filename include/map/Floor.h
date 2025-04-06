#ifndef FLOOR_H
#define FLOOR_H

#include "Room.h"
#include <vector>
namespace Map {
    struct Floor {
        int index;
        std::vector<Room> rooms;
    };
}


#endif

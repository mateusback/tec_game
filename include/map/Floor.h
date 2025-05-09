#ifndef FLOOR_H
#define FLOOR_H

#include "Room.h"
#include <vector>
namespace Map {

    struct RoomInfo {
        int id;
        int x, y;
    };

    struct Floor {
        int index;
        std::vector<RoomInfo> roomInfos; 
        std::vector<Room> rooms;
    };
}


#endif

#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "../map/TileSet.h"
#include "../map/Floor.h"
#include "../map/Room.h"
#include "EntityManager.h"
#include "ItemManager.h"
#include "EnemyManager.h"
#include "../utils/Types.h"
#include "../entities/PlayerBody.h"
#include "../map/RoomState.h"
#include "../utils/GlobalAccess.h"
#include "../entities/Body.h"

namespace Manager {
    
    class RoomManager {
    private:
        Map::Floor floor;
        Map::Room* currentRoom = nullptr;

        SDL_Renderer* renderer;
        EntityManager* entityManager;
        TileSet* tileSet;
        ItemManager* itemManager = nullptr;
        EnemyManager* enemyManager = nullptr;
        Entities::PlayerBody* player = nullptr;
        std::unordered_map<int, Map::RoomState> roomStates;
    
    public:
        RoomManager(SDL_Renderer* renderer,
            EntityManager* entityManager,
            TileSet* tileSet,
            ItemManager* itemManager,
            EnemyManager* enemyManager);
        ~RoomManager();
    
        void update(float deltaTime);

        void connectFloorRooms();
        void loadRequiredAssets(SDL_Renderer* renderer);
        void loadRoom(Map::Room* room);
        void loadRoomByType(Map::ERoomType type);
        void loadTiles(Map::Room* room);
        void loadEntities(Map::Room* room);        

        void updateVirutalRenderer(Map::Room* room);
        void moveToRoomInDirection(EDirection direction);
        void checkAndMovePlayerBetweenRooms();
        bool areAllEnemiesDefeated() const;
        void openDoorsOfCurrentRoom();
        void saveCurrentRoomState();

        const Map::Room* getCurrentRoom() const;
        const Map::Room* getRoomByPosition(int x, int y) const;
        Map::Room* getRoomByPosition(int x, int y);
        const std::vector<Map::Room>& getRooms() const;
        const bool wasRoomVisited(int roomId) const;

        //Procedural Generation
        void generateFloor(int index, int seed);


        Entities::PlayerBody* getPlayer() const { return this->player; }
    };
}


#endif

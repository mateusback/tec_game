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
    
        void loadFloor(int index);
        void loadRoom(Map::Room* room);
        void loadRoomByType(Map::ERoomType type);

        void update(float deltaTime);

        void updateVirutalRenderer(Map::Room* room);
        void loadTiles(Map::Room* room);
        void loadEntities(Map::Room* room);        
        void createPlayerInStartRoom();

        void moveToRoomInDirection(EDirection direction);
        void checkAndMovePlayerBetweenRooms();
        bool areAllEnemiesDefeated() const;
        void openDoorsOfCurrentRoom();
        void saveCurrentRoomState();

        void setEntityPositionByPixels(Entities::Body* entity, Vector2f position);
        void setEntityPositionByTiles(Entities::Body* entity, Vector2f position);

        Map::Room* getCurrentRoom();
        Map::Room* getRoomByPosition(int x, int y);
        const std::vector<Map::Room>& getRooms();

        Entities::PlayerBody* getPlayer() const { return this->player; }

        #pragma region "temp"
        void tempMovePlayer(int cols, int rows);
        #pragma endregion
    };
}


#endif

#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>
#include <memory>
#include "../../include/core/Scene.h"
#include "../../include/entities/PlayerBody.h"
#include "../../include/entities/ItemBody.h"

class GameplayScene : public Core::Scene {
public:
    GameplayScene(SDL_Renderer* renderer);
    void update(float deltaTime) override;
    void handleEvent(const SDL_Event& event) override;
    void render(SDL_Renderer* renderer) override;

private:
    Entities::PlayerBody* player;
    std::list<std::unique_ptr<Entities::ItemBody>> items;
};

#endif

#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>

#include "../../include/core/Scene.h"
#include "../../include/entities/PlayerBody.h"
#include "../../include/entities/ItemBody.h"

class GameplayScene : public Core::Scene {
public:
    GameplayScene(SDL_Renderer* renderer);
    void Update(float deltaTime) override;
    void HandleEvent(const SDL_Event& event) override;
    void Render(SDL_Renderer* renderer) override;

private:
    Entites::PlayerBody* player;
    std::list<ItemBody*> items;
    //TODO - TROCAR PARA unique_ptr
};

#endif

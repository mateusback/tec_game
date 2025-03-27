#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include <list>

#include "../../Core/Interface/Scene.h"
#include "../../Entities/Interface/PlayerBody.h"
#include "../../Entities/Interface/ItemBody.h"

class GameplayScene : public Core::Scene {
public:
    GameplayScene(SDL_Renderer* renderer);
    void Update(float deltaTime) override;
    void HandleEvent(const SDL_Event& event) override;
    void Render(SDL_Renderer* renderer) override;

private:
    Entites::PlayerBody* player;
    std::list<ItemBody*> items;
    //unique_ptr
};

#endif

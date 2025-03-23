#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "../Core/Interface/Scene.h"
#include "../Entities/Interface/PlayerBody.h"

class GameplayScene : public Scene {
public:
    GameplayScene();
    void HandleEvent(const SDL_Event& event) override;
    void Update(float deltaTime) override;
    void Render(SDL_Renderer* renderer) override;

private:
    PlayerBody* player;
};

#endif

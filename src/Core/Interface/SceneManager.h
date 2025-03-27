#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

namespace Core 
{
    class SceneManager 
    {
    public:
        static void SetScene(Scene* newScene);
        static Scene* GetCurrentScene();

    private:
        static Scene* currentScene;
    };
}

#endif

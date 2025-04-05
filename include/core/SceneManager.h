#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

namespace Core 
{
    class SceneManager 
    {
    public:
        static void setScene(Scene* newScene);
        static Scene* getCurrentScene();

    private:
        static Scene* currentScene;
    };
}

#endif

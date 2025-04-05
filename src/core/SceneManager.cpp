#include "../../include/core/SceneManager.h"

namespace Core
{
    Scene* SceneManager::currentScene = nullptr;

    void SceneManager::setScene(Scene* newScene) 
    {
        delete currentScene;
        currentScene = newScene;
    }

    Scene* SceneManager::getCurrentScene() 
    {
        return currentScene;
    }
}


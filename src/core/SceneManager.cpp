#include "../../include/core/SceneManager.h"

namespace Core
{
    Scene* SceneManager::currentScene = nullptr;

    void SceneManager::SetScene(Scene* newScene) 
    {
        delete currentScene;
        currentScene = newScene;
    }

    Scene* SceneManager::GetCurrentScene() 
    {
        return currentScene;
    }
}


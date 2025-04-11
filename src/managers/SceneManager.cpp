#include "../../include/managers/SceneManager.h"

namespace Manager
{
    Core::Scene* SceneManager::currentScene = nullptr;

    void SceneManager::setScene(Core::Scene* newScene) 
    {
        delete currentScene;
        currentScene = newScene;
    }

    Core::Scene* SceneManager::getCurrentScene() 
    {
        return currentScene;
    }
}


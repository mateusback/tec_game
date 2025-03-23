#include "../Interface/SceneManager.h"

Scene* SceneManager::currentScene = nullptr;

void SceneManager::SetScene(Scene* newScene) {
    delete currentScene;
    currentScene = newScene;
}

Scene* SceneManager::GetCurrentScene() {
    return currentScene;
}

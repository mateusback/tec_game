#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "../core/Scene.h"

namespace Manager {
    class SceneManager {
    public:
        static void setScene(Scene* newScene);
        static Scene* getCurrentScene();

    private:
        static Core::Scene* currentScene;
    };
}

#endif

#ifndef SCORE_MANAGER_GLOBAL_H
#define SCORE_MANAGER_GLOBAL_H

#include "ScoreManager.h"

namespace Manager {
    class ScoreManagerGlobal {
    public:
        static ScoreManager* instance;

        static void init() {
            if (!instance)
                instance = new ScoreManager();
        }

        static void destroy() {
            delete instance;
            instance = nullptr;
        }
    };
}

#endif

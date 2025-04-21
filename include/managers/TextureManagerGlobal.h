#ifndef TEXTURE_MANAGER_GLOBAL_H
#define TEXTURE_MANAGER_GLOBAL_H

#include "TextureManager.h"

namespace Manager {
    class TextureManagerGlobal {
    public:
        static TextureManager* instance;

        static void init() {
            if (!instance)
                instance = new TextureManager();
        }

        static void destroy() {
            delete instance;
            instance = nullptr;
        }
    };
}

#endif

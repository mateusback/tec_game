#ifndef AUDIO_MANAGER_GLOBAL_H
#define AUDIO_MANAGER_GLOBAL_H

#include "AudioManager.h"

namespace Manager {
    class AudioManagerGlobal {
    public:
        static AudioManager* instance;

        static void init() {
            if (!instance)
                instance = new AudioManager();
        }

        static void destroy() {
            delete instance;
            instance = nullptr;
        }
    };
}

#endif

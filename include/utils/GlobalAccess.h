#ifndef GLOBAL_ACCESS_H
#define GLOBAL_ACCESS_H

#include "../managers/TextureManagerGlobal.h"
#include "../renders/VirtualRendererGlobal.h"
#include "../managers/AudioManagerGlobal.h"
#include "../managers/ScoreManagerGlobal.h"

inline Manager::TextureManager* textures() {
    return Manager::TextureManagerGlobal::instance;
}

inline Renderer::VirtualRenderer* virtualRenderer() {
    return Renderer::VirtualRendererGlobal::instance;
}

inline Manager::AudioManager* audio() {
    return Manager::AudioManagerGlobal::instance;
}

inline Manager::ScoreManager* score() {
    return Manager::ScoreManagerGlobal::instance;
}

#endif

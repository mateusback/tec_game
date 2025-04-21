#ifndef GLOBAL_ACCESS_H
#define GLOBAL_ACCESS_H

#include "../managers/TextureManagerGlobal.h"
#include "../renders/VirtualRendererGlobal.h"

inline Manager::TextureManager* textures() {
    return Manager::TextureManagerGlobal::instance;
}

inline Renderer::VirtualRenderer* virtualRenderer() {
    return Renderer::VirtualRendererGlobal::instance;
}

#endif

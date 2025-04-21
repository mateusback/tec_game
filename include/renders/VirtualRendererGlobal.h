#ifndef VIRTUAL_RENDERER_GLOBAL_H
#define VIRTUAL_RENDERER_GLOBAL_H

#include "VirtualRenderer.h"
namespace Renderer {
    class VirtualRendererGlobal {
    public:
        static VirtualRenderer* instance;

        static void init(int screenWidth, int screenHeight, int tilesX, int tilesY) {
            if (!instance) {
                instance = new VirtualRenderer(screenWidth, screenHeight, tilesX, tilesY);
            }
        }

        static void destroy() {
            delete instance;
            instance = nullptr;
        }
    };
}
#endif

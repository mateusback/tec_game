#include "../../include/core/FontManager.h"
#include <iostream>

namespace Core {
    std::unordered_map<std::string, TTF_Font*> FontManager::fonts;

    void FontManager::load(const std::string& id, const std::string& path, int size) {
        TTF_Font* font = TTF_OpenFont(path.c_str(), size);
        if (!font) {
            std::cerr << "Erro ao carregar fonte '" << path << "': " << TTF_GetError() << std::endl;
        } else {
            fonts[id] = font;
        }
    }

    TTF_Font* FontManager::get(const std::string& id) {
        return fonts[id];
    }

    void FontManager::clear() {
        for (auto& pair : fonts) {
            TTF_CloseFont(pair.second);
        }
        fonts.clear();
    }
}

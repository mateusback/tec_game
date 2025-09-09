#include "../include/managers/FontManager.h"
#include <stdexcept>
#include <format>

using namespace Mylib;

namespace Manager {
    Mylib::unordered_map_string_key<TTF_Font*> Manager::FontManager::fonts;

    void FontManager::load(std::string_view id, std::string_view path, int size) {
        TTF_Font* font = TTF_OpenFont(std::string(path).c_str(), size);
        if (!font) {
            std::cerr << std::format("Erro ao carregar fonte '{}': {}\n", path, TTF_GetError());
            return;
        }
        fonts[std::string(id)] = font;
    }
    
    TTF_Font* FontManager::get(std::string_view id) {
        auto it = fonts.find(std::string(id));
        if (it != fonts.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    void FontManager::unload(std::string_view id) {
        auto it = fonts.find(std::string(id));
        if (it != fonts.end()) {
            TTF_CloseFont(it->second);
            fonts.erase(it);
        }
    }
    
    void FontManager::clear() {
        for (auto& [key, font] : fonts) {
            TTF_CloseFont(font);
        }
        fonts.clear();
    }
}

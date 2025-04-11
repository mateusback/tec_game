#include "../include/managers/FontManager.h"
#include <stdexcept>
#include <format>

using namespace Manager;
using namespace Mylib;
using namespace Utils;

namespace Manager {
    Mylib::unordered_map_string_key<TTF_Font*> FontManager::fonts;

    void FontManager::load(std::string_view fname, std::string_view path, int size) {
        std::string id = std::format("{}_{}", fname, size);
    
        if (fonts.contains(id)) return;
    
        TTF_Font* font = TTF_OpenFont(std::string(path).c_str(), size);
        if (!font) {
            throw std::runtime_error(std::format("Erro ao carregar fonte '{}': {}", id, TTF_GetError()));
        }
    
        fonts[std::move(id)] = font;
    }

    TTF_Font* FontManager::get(const Descriptor& font) {
        return font.data.get_value<TTF_Font*>();
    }    

    void FontManager::unload(const Descriptor& font) {
        TTF_Font* ptr = font.data.get_value<TTF_Font*>();
        if (!ptr) return;
    
        for (auto it = fonts.begin(); it != fonts.end(); ++it) {
            if (it->second == ptr) {
                TTF_CloseFont(ptr);
                fonts.erase(it);
                return;
            }
        }
    }
    
    void FontManager::clear() {
        for (auto& [key, font] : fonts) {
            TTF_CloseFont(font);
        }
        fonts.clear();
    }
}

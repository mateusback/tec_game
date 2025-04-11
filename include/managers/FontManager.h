#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL2/SDL_ttf.h>
#include <my-lib/utils.h>
#include <string_view>

namespace Manager {
    class FontManager {
    public:
        static void load(std::string_view id, std::string_view path, int size);
        static TTF_Font* get(std::string_view id);
        static void unload(std::string_view id);
        static void clear();

    private:
        static Mylib::unordered_map_string_key<TTF_Font*> fonts;
    };
}

#endif

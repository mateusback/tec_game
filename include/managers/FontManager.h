#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <unordered_map>
#include <string>
#include <SDL2/SDL_ttf.h>

namespace Manager {
    class FontManager {
    public:
        static void load(const std::string& id, const std::string& path, int size);
        static TTF_Font* get(const std::string& id);
        static void clear();
    private:
		//https://github.com/ehmcruz/my-game-lib/blob/main/include/my-game-lib/audio.h
        static std::unordered_map<std::string, TTF_Font*> fonts;
    };
}

#endif

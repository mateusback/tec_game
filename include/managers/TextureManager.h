#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <my-lib/utils.h>
#include <string>

namespace Manager {
    class TextureManager {
    public:
        static void Load(SDL_Renderer* renderer, const std::string& id, const std::string& path);
        static SDL_Texture* Get(const std::string& id);
        static void Clear();
        static void Clear(std::string id);
        
    private:
        static Mylib::unordered_map_string_key<SDL_Texture*> textures;
    };
}
#endif
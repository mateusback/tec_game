#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <my-lib/utils.h>
#include <string>
#include "../utils/Types.h"
namespace Manager {
    class TextureManager {
    public:
        static void Load(SDL_Renderer* renderer, const std::string& id, const std::string& path);
        static SDL_Texture* Get(const std::string& id);
        static void Clear();
        static void Clear(std::string id);
        
        static Vector2i getTextureScale(SDL_Texture* texture){
            int width = 0, height = 0;
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
            return {width, height};
        }
        
    private:
        static Mylib::unordered_map_string_key<SDL_Texture*> textures;
    };
}
#endif
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>

namespace Core 
{
    class TextureManager 
    {
    public:
        static void Load(SDL_Renderer* renderer, const std::string& id, const std::string& path);
        static SDL_Texture* Get(const std::string& id);
        static void Clear();
        static void Clear(std::string id);
        
    private:
        static std::unordered_map<std::string, SDL_Texture*> textures;
    };
}
#endif
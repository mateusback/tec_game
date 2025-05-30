#include "../../include/core/TextureManager.h"

namespace Core
{
    std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;
    
    void TextureManager::Load(SDL_Renderer* renderer, const std::string& id, const std::string& path) {
        SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
        if (!tex) {
            printf("Erro ao carregar textura '%s': %s\n", path.c_str(), IMG_GetError());
        } else {
            textures[id] = tex;
        }
    }
    
    SDL_Texture* TextureManager::Get(const std::string& id) {
        return textures[id];
    }
    
    void TextureManager::Clear() {
        for (auto& pair : textures) {
            SDL_DestroyTexture(pair.second);
        }
        textures.clear();
    }
    
    void TextureManager::Clear(std::string id) {
        auto it = textures.find(id);
        if (it != textures.end()) {
            SDL_DestroyTexture(it->second);
            textures.erase(it);
        }
    }
}
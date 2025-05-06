#include "../../include/renders/HudRenderer.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/managers/FontManager.h"
#include "../../include/renders/TextRenderer.h"
#include "../../include/utils/GlobalAccess.h"

using namespace Renderer;

HudRenderer::HudRenderer(SDL_Renderer* renderer) {
    SDL_Texture* texture = textures()->Get("hudsheet");
    this->hudIcons.load(texture, { 0, 1, 2, 3, 4 });
}

void HudRenderer::render(SDL_Renderer* renderer, const Entities::PlayerBody* player) {
    drawHearts(renderer, player->getHealth(), player->getMaxHealth());
    drawBombs(renderer, player->getBombs());
    drawCoins(renderer, player->getCoins());
    drawScore(renderer);
}

void HudRenderer::drawHearts(SDL_Renderer* renderer, int hp, int maxHp) {
    const int hpPerHeart = 10;
    const int heartsPerRow = 5;

    int totalHearts = (maxHp + hpPerHeart - 1) / hpPerHeart;
    int fullHearts = hp / hpPerHeart;
    bool hasHalfHeart = (hp % hpPerHeart) >= 5;

    float size = virtualRenderer()->normalizeValue(0.25f);
    float spacing = virtualRenderer()->normalizeValue(0.02f);
    float startX = virtualRenderer()->normalizeValue(0.2f);
    float startY = virtualRenderer()->normalizeValue(0.4f);

    for (int i = 0; i < totalHearts; ++i) {
        int iconIndex = 2; 
        if (i < fullHearts) {
            iconIndex = 0; 
        } else if (i == fullHearts && hasHalfHeart) {
            iconIndex = 1;
        }

        int row = i / heartsPerRow;
        int col = i % heartsPerRow;

        const Renderer::Sprite& icon = this->hudIcons.get(iconIndex);
        SDL_Texture* texture = icon.getTexture();
        const SDL_Rect& src = icon.getSourceRect();

        SDL_Rect dst = {
            static_cast<int>(startX + col * (size + spacing)),
            static_cast<int>(startY + row * (size + spacing)),
            static_cast<int>(size),
            static_cast<int>(size)
        };

        SDL_RenderCopy(renderer, texture, &src, &dst);
    }
}

void HudRenderer::drawBombs(SDL_Renderer* renderer, int bombs) {
    const Renderer::Sprite& icon = this->hudIcons.get(3);
    SDL_Texture* texture = icon.getTexture();
    const SDL_Rect& src = icon.getSourceRect();
    float size = virtualRenderer()->normalizeValue(0.2f);
    float startX = virtualRenderer()->normalizeValue(0.2f);
    float startY = virtualRenderer()->normalizeValue(1.2f);

    SDL_Rect dst = {
        static_cast<int>(startX),
        static_cast<int>(startY),
        static_cast<int>(size),
        static_cast<int>(size)
    };

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void HudRenderer::drawCoins(SDL_Renderer* renderer, int coins) {
    const Renderer::Sprite& icon = this->hudIcons.get(4);
    SDL_Texture* texture = icon.getTexture();
    const SDL_Rect& src = icon.getSourceRect();
    float size = virtualRenderer()->normalizeValue(0.2f);
    float startX = virtualRenderer()->normalizeValue(0.2f);
    float startY = virtualRenderer()->normalizeValue(1.2f + 0.2f + 0.02f); 

    SDL_Rect dst = {
        static_cast<int>(startX),
        static_cast<int>(startY),
        static_cast<int>(size),
        static_cast<int>(size)
    };

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void HudRenderer::drawScore(SDL_Renderer* renderer) {
    TTF_Font* font = Manager::FontManager::get("default");
    if (!font) return;

    SDL_Color color = {255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(score()->getScore());

    int x = static_cast<int>(virtualRenderer()->normalizeValue(4.2f));
    int y = static_cast<int>(virtualRenderer()->normalizeValue(0.2f));

    //TODO - TROCAR ISSO DEPOIS
    Core::TextRenderer::render(renderer, font, scoreText, x, y, color);
}

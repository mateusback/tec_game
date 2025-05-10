#include "../../include/handlers/NotificationHandler.h"
#include "../../include/renders/TextRenderer.h"

void NotificationHandler::setFont(TTF_Font* font) {
    this->font = font;
}

void NotificationHandler::show(const std::string& title, const std::string& subtitle) {
    this->title = title;
    this->subtitle = subtitle;
    this->displayTime = duration;
}

void NotificationHandler::update(float deltaTime) {
    if (displayTime > 0.0f)
        displayTime -= deltaTime;
}

void NotificationHandler::render(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    if (displayTime <= 0.0f || font == nullptr)
        return;

    int textW, textH;

    TTF_SizeText(font, title.c_str(), &textW, &textH);
    int titleX = (screenWidth - textW) / 2;
    int titleY = static_cast<int>(screenHeight * 0.15f);

    Core::TextRenderer::render(renderer, font, title, titleX, titleY, {255, 255, 255, 255});

    if (!subtitle.empty()) {
        TTF_SizeText(font, subtitle.c_str(), &textW, &textH);
        int subX = (screenWidth - textW) / 2;
        int subY = titleY + textH + 5;

        Core::TextRenderer::render(renderer, font, subtitle, subX, subY, {200, 200, 200, 255});
    }
}

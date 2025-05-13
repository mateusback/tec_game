#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H

#include <string>
#include <SDL2/SDL_ttf.h>

class SDL_Renderer;

class NotificationHandler {
public:
    void setFont(TTF_Font* font);
    void show(const std::string& title, const std::string& subtitle = "");
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, int screenWidth, int screenHeight);

private:
    std::string title;
    std::string subtitle;
    float displayTime = 0.0f;
    const float duration = 2.5f;

    TTF_Font* font = nullptr;
};

#endif

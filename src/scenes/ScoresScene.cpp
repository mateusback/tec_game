#include "../../include/scenes/ScoresScene.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/FontManager.h"
#include "../../include/managers/SceneManager.h"
#include "../../include/scenes/MenuScene.h"
#include "../../include/renders/TextRenderer.h"
#include <algorithm>

namespace Scenes {

    // Função auxiliar para calcular largura do texto e centralizar
    int ScoresScene::getCenteredX(const char* text, TTF_Font* font) {
        int textWidth, textHeight;
        TTF_SizeText(font, text, &textWidth, &textHeight);
        return (width - textWidth) / 2;
    }

    ScoresScene::ScoresScene(SDL_Renderer* renderer, int width, int height)
        : width(width), height(height)
    {
        Manager::FontManager::load("scores_title", "assets/fonts/Montserrat-Bold.ttf", 42);
        Manager::FontManager::load("scores_text", "assets/fonts/Roboto-Regular.ttf", 24);
        Manager::FontManager::load("scores_small", "assets/fonts/Roboto-Regular.ttf", 18);
        
        titleFont = Manager::FontManager::get("scores_title");
        textFont = Manager::FontManager::get("scores_text");
        smallFont = Manager::FontManager::get("scores_small");

        scores = score()->load();
    }

    ScoresScene::~ScoresScene() {
        Manager::FontManager::unload("scores_title");
        Manager::FontManager::unload("scores_text");
        Manager::FontManager::unload("scores_small");
    }

    void ScoresScene::handleEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                case SDLK_ESCAPE:
                case SDLK_SPACE:
                case SDLK_BACKSPACE:
                    {
                        auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
                        Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
                    }
                    break;
            }
        }
        
        // Também permite sair clicando com o mouse
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            auto renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
            Manager::SceneManager::setScene(new MenuScene(renderer, width, height));
        }
    }

    void ScoresScene::update(float, const Manager::PlayerInput&) {
        // Nada
    }

    void ScoresScene::render(SDL_Renderer* renderer) {
        // Fundo gradiente - roxo escuro para preto
        for (int y = 0; y < height; y++) {
            int r = 25 + (y * 15) / height;  // De 25 a 40
            int g = 5 + (y * 10) / height;   // De 5 a 15
            int b = 35 + (y * 25) / height;  // De 35 a 60
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }

        // Cores temáticas
        SDL_Color gold = {255, 215, 0, 255};
        SDL_Color silver = {192, 192, 192, 255};
        SDL_Color bronze = {205, 127, 50, 255};
        SDL_Color lightBlue = {173, 216, 230, 255};
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color lightGray = {200, 200, 200, 255};
        SDL_Color darkGray = {150, 150, 150, 255};

        if (!titleFont || !textFont || !smallFont) return;

        int centerX = width / 2;
        int startY = height / 8;

        // Título principal
        const char* title = "RANKING DE PONTUAÇÕES";
        Core::TextRenderer::render(renderer, titleFont, title, 
                                 getCenteredX(title, titleFont), startY, gold);

        // Linha decorativa
        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        SDL_RenderDrawLine(renderer, centerX - 200, startY + 60, centerX + 200, startY + 60);

        // Subtítulo
        const char* subtitle = "TOP 10 MELHORES JOGADORES";
        Core::TextRenderer::render(renderer, textFont, subtitle, 
                                 getCenteredX(subtitle, textFont), startY + 90, lightBlue);

        // Cabeçalho da tabela
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect headerRect = {centerX - 250, startY + 130, 500, 35};
        SDL_RenderFillRect(renderer, &headerRect);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderDrawRect(renderer, &headerRect);

        Core::TextRenderer::render(renderer, textFont, "POS.", centerX - 230, startY + 140, white);
        Core::TextRenderer::render(renderer, textFont, "JOGADOR", centerX - 120, startY + 140, white);
        Core::TextRenderer::render(renderer, textFont, "PONTUAÇÃO", centerX + 100, startY + 140, white);

        // Scores com cores diferenciadas
        int currentY = startY + 180;
        size_t maxScores = std::min(scores.size(), size_t(10));

        for (size_t i = 0; i < maxScores; ++i) {
            const auto& entry = scores[i];
            
            // Cor baseada na posição
            SDL_Color positionColor;
            if (i == 0) positionColor = gold;       // 1º lugar - Ouro
            else if (i == 1) positionColor = silver; // 2º lugar - Prata
            else if (i == 2) positionColor = bronze; // 3º lugar - Bronze
            else positionColor = white;              // Demais - Branco

            // Fundo alternado para melhor legibilidade
            if (i % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 100);
                SDL_Rect rowRect = {centerX - 250, currentY - 5, 500, 30};
                SDL_RenderFillRect(renderer, &rowRect);
            }

            // Posição com medalha para top 3
            std::string position;
            if (i == 0) position = "🥇 1º";
            else if (i == 1) position = "🥈 2º";
            else if (i == 2) position = "🥉 3º";
            else position = std::to_string(i + 1) + "º";

            Core::TextRenderer::render(renderer, textFont, position.c_str(), 
                                     centerX - 230, currentY, positionColor);

            // Nome do jogador
            Core::TextRenderer::render(renderer, textFont, entry.name.c_str(), 
                                     centerX - 120, currentY, positionColor);

            // Pontuação
            std::string scoreStr = std::to_string(entry.score);
            Core::TextRenderer::render(renderer, textFont, scoreStr.c_str(), 
                                     centerX + 100, currentY, positionColor);

            currentY += 35;
        }

        // Se não há pontuações
        if (scores.empty()) {
            const char* emptyMsg = "Nenhuma pontuação registrada ainda";
            Core::TextRenderer::render(renderer, textFont, emptyMsg, 
                                     getCenteredX(emptyMsg, textFont), currentY, lightGray);
            currentY += 50;
            
            const char* playMsg = "Jogue para aparecer aqui!";
            Core::TextRenderer::render(renderer, smallFont, playMsg, 
                                     getCenteredX(playMsg, smallFont), currentY, darkGray);
        }

        // Separador
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, centerX - 200, height - 80, centerX + 200, height - 80);

        // Instruções
        const char* instruction = "Pressione ENTER, ESC ou clique para voltar ao menu";
        Core::TextRenderer::render(renderer, smallFont, instruction, 
                                 getCenteredX(instruction, smallFont), height - 50, lightGray);

        // Estatísticas no rodapé
        if (!scores.empty()) {
            std::string statsText = "Total de jogadores: " + std::to_string(scores.size());
            Core::TextRenderer::render(renderer, smallFont, statsText.c_str(), 
                                     getCenteredX(statsText.c_str(), smallFont), height - 25, darkGray);
        }

        SDL_RenderPresent(renderer);
    }

}

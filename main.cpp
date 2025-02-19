#include <SDL.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <string>
#include <vector>

const int WIDTH = 800, HEIGHT = 600;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
std::string text = "Hello";
bool vertical = false;

// Функция для разделения строки на символы UTF-8
std::vector<std::string> splitUTF8(const std::string& str) {
    std::vector<std::string> result;
    size_t i = 0;
    while (i < str.size()) {
        unsigned char c = str[i];
        size_t charLen = 1;

        // Определяем количество байтов для текущего символа
        if ((c & 0x80) == 0) charLen = 1;
        else if ((c & 0xE0) == 0xC0) charLen = 2;
        else if ((c & 0xF0) == 0xE0) charLen = 3;
        else if ((c & 0xF8) == 0xF0) charLen = 4;

        result.push_back(str.substr(i, charLen));
        i += charLen;
    }
    return result;
}

// Функция рендеринга текста с поддержкой кириллицы
void renderText(const std::string& text, bool vertical) {
    SDL_Color color = {255, 255, 255, 255};

    // Отрисовка первого текста горизонтально
    SDL_Surface* textSurface1 = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);

    int w1, h1;
    SDL_QueryTexture(textTexture1, NULL, NULL, &w1, &h1);

    SDL_Rect dst1 = {50, 50, w1, h1}; // Позиция для первого текста

    // Отрисовка символа ":"
    SDL_Surface* colonSurface = TTF_RenderUTF8_Solid(font, ":", color);
    SDL_Texture* colonTexture = SDL_CreateTextureFromSurface(renderer, colonSurface);

    int wColon, hColon;
    SDL_QueryTexture(colonTexture, NULL, NULL, &wColon, &hColon);

    SDL_Rect dstColon = {50 + w1 + 10, 50, wColon, hColon}; // Позиция для ":"

    // Очищаем экран
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Рисуем первый текст
    SDL_RenderCopy(renderer, textTexture1, NULL, &dst1);
    // Рисуем символ ":"
    SDL_RenderCopy(renderer, colonTexture, NULL, &dstColon);

    // Отрисовка второго текста
    if (vertical) {
        // Если вертикально, то каждую букву отрисовываем отдельно
        int x = 50 + w1 + wColon + 20;
        int y = 50;

        // Разбиваем текст на UTF-8 символы и отрисовываем их поочередно
        std::vector<std::string> symbols = splitUTF8(text);
        for (const auto& charStr : symbols) {
            SDL_Surface* charSurface = TTF_RenderUTF8_Solid(font, charStr.c_str(), color);
            SDL_Texture* charTexture = SDL_CreateTextureFromSurface(renderer, charSurface);

            int wChar, hChar;
            SDL_QueryTexture(charTexture, NULL, NULL, &wChar, &hChar);

            SDL_Rect dstChar = {x, y, wChar, hChar};
            SDL_RenderCopy(renderer, charTexture, NULL, &dstChar);

            y += hChar + 5; // Отступ между буквами

            SDL_FreeSurface(charSurface);
            SDL_DestroyTexture(charTexture);
        }
    } else {
        // Если горизонтально, то отрисовываем весь текст как одну строку
        SDL_Surface* textSurface2 = TTF_RenderUTF8_Solid(font, text.c_str(), color);
        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);

        int w2, h2;
        SDL_QueryTexture(textTexture2, NULL, NULL, &w2, &h2);

        SDL_Rect dst2 = {50 + w1 + wColon + 20, 50, w2, h2};
        SDL_RenderCopy(renderer, textTexture2, NULL, &dst2);

        SDL_FreeSurface(textSurface2);
        SDL_DestroyTexture(textTexture2);
    }

    // Обновляем экран
    SDL_RenderPresent(renderer);

    // Освобождаем память
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);
    SDL_FreeSurface(colonSurface);
    SDL_DestroyTexture(colonTexture);
}

// Основной цикл рендеринга
void mainLoop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
            return;
        }
    }
    renderText(text, vertical);
}

// Функция инициализации
extern "C" void setTextAndOrientation(const char* newText, bool isVertical) {
    text = newText;
    vertical = isVertical;
}

// Основная функция
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) return 1;

    window = SDL_CreateWindow("WebAssembly Text Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("arial.ttf", 24);

    emscripten_set_main_loop(mainLoop, 0, 1);

    return 0;
}
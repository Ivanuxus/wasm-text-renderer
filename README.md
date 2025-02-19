# WASM Text Renderer

Это WebAssembly приложение на C++ отображает текст горизонтально или вертикально с использованием SDL2 и SDL_TTF.

## 📌 Описание проекта

Данное приложение рендерит текст на HTML5 canvas с использованием WebAssembly и библиотеки SDL2.

- **Ориентация текста** задается через параметры HTTP GET запроса:
  - `o=h` — горизонтально
  - `o=v` — вертикально
- **Текст** передается через параметр `t`. Поддерживается кириллица и UTF-8.

## 🚀 Функциональные возможности

- Отображение текста в горизонтальной или вертикальной ориентации.
- Поддержка кириллицы и UTF-8 символов.
- Работа в браузере с использованием WebAssembly.
- Обработка шрифтов с использованием SDL_TTF.

## ⚙️ Требования

Перед началом убедитесь, что у вас установлены:

- [Emscripten](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html)
- Python 3 (для запуска локального HTTP-сервера)
- Arial.ttf шрифт (включен в проект)

## 🔧 Установка и компиляция

1. **Установите Emscripten SDK**:

   ```sh
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **Соберите проект**:
   ```sh
   emcc main.cpp -o draw.js -s USE_SDL=2 -s USE_SDL_TTF=2 \
   -s EXPORTED_FUNCTIONS="['_main', '_setTextAndOrientation']" \
   -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
   --preload-file arial.ttf
   ```

## 🌐 Запуск локального HTTP-сервера

1. Откройте терминал и выполните команду:
   ```sh
   python3 -m http.server 8080
   ```
2. Перейдите в браузере по ссылке:
   - Вертикально: [http://localhost:8080/draw.html?o=v&t=Вертикально](http://localhost:8080/draw.html?o=v&t=Вертикально)
   - Горизонтально: [http://localhost:8080/draw.html?o=h&t=Горизонтально](http://localhost:8080/draw.html?o=h&t=Горизонтально)

## 📊 Пример использования

- Откройте в браузере:
- Параметры:
- `o`: Ориентация текста (`v` — вертикально, `h` — горизонтально)
- `t`: Текст для отображения

## 🧩 Архитектура приложения

- **C++ (main.cpp)**: Основная логика отрисовки с использованием SDL2 и SDL_TTF.
- **HTML (draw.html)**: Веб-страница с canvas для отображения текста.

## ❗️ Известные ограничения

- Приложение протестировано в браузерах Chrome и Safari. В других браузерах могут быть ограничения в отображении кириллицы.
- При использовании нестандартных шрифтов необходимо добавить их в проект и указать в коде.

---

## 🔗 Полезные ссылки

- [Emscripten Documentation](https://emscripten.org/docs/)
- [SDL2 Documentation](https://wiki.libsdl.org/FrontPage)
- [SDL_TTF Documentation](https://www.libsdl.org/projects/SDL_ttf/)

---

## ✨ Автор

- **[Ivan Alferov]**

/*
В данном файле хранятся определения классов Screen, Menu, Menu_Start, Menu_Info,
Menu_Exit, Input_Window, в основном определяющих элементы управления окна приложения.
*/

#include "const.h"
#include "record.h"
#include "window_classes.h"

/* Функция для печати сообщения на поверхность:
surface - поверхность, на которой нужно отобразить текст;
message - текст, который нужно отобразить;
font - путь к шрифту;
size - размер шрифта;
color - цвет шрифта;
rect - прямоугольная область на поверхности surface, в которой нужно отобразить текст;
centered - каким образом центрировать rect на surface. */
void print_ttf(SDL_Surface* surface, std::string* message, char* font, int size, SDL_Color color, SDL_Rect* rect, Centered centered)
{
    TTF_Font* ttf_font = TTF_OpenFont(font, size);
    if (ttf_font == nullptr)
    {
        std::cout << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Surface* text_surface = TTF_RenderUTF8_Blended(ttf_font, message->c_str(), color); // поверхность с текстом в ней
    if (centered == x_centered)
    {
        // Если текст нужно поместить посередине по горизонтали
        rect->x = (surface->w - text_surface->w) / 2;
    }
    else if (centered == y_centered)
    {
        // Если текст нужно поместить посередине по вертикали
        rect->y = (surface->h - text_surface->h) / 2;
    }
    else if (centered == xy_centered)
    {
        // Если текст нужно поместить посередине по горизонтали и вертикали
        rect->x = (surface->w - text_surface->w) / 2;
        rect->y = (surface->h - text_surface->h) / 2;
    }
    SDL_BlitSurface(text_surface, NULL, surface, rect);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(ttf_font);
}

// Конструктор класса Screen
Screen::Screen()
{
    x = y = 0;
    height = HEIGHT; // высота экрана
    width = WIDTH; // ширина экрана
    page = welcome_page; // начальная страница, отображаемая на экране
}

// Деструктор класса Screen
Screen::~Screen()
{
    SDL_DestroyTexture(texture_end);
    SDL_DestroyTexture(texture_game);
    SDL_DestroyTexture(texture_info);
    SDL_DestroyTexture(texture_welcome);
}

// Метод класса Screen для отображения нужной текстуры экрана на рендерер
void Screen::apply_texture(SDL_Renderer* renderer, std::vector<Record*>* records)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    switch (page)
    {
    case end_page:
    {
        if (was_created == false)
        {
            // Если текстура для экрана после поражения не была еще создана
            create_end_texture(renderer, records);
            was_created = true;
        }        
        // Отрендерим текстуру
        SDL_RenderCopy(renderer, texture_end, NULL, &rect);
        break;
    }
    case game_page:
    {
        // Нужно показать экран во время игры
        SDL_RenderCopy(renderer, texture_game, NULL, &rect);
        break;
    }

    case info_page:
    {
        // Нужно показать экран при выводе справки об игре
        SDL_RenderCopy(renderer, texture_info, NULL, &rect);
        break;
    }
    case welcome_page:
    {
        // Нужно показать начальную заставку
        SDL_RenderCopy(renderer, texture_welcome, NULL, &rect);
        break;
    }
    }
}

// Метод класса Screen создает текстуру для экрана после поражения
void Screen::create_end_texture(SDL_Renderer* renderer, std::vector<Record*>* records)
{
    // Нужно показать экран при завершении игры
    bool was_shown = false; // параметр отвечает за то, был ли выведен счет только что завершенной игры (true - был)
    Record* now_record = records->at(records->size() - 1); // результат только что завершенной игры
    selection_sort(records); // сортируем вектор с рекордами по убыванию
    SDL_Rect rect;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Заполняем экран одним цветом
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    std::string text = "Game Over!";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, &text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    text = "Highscores:";
    rect.x = 0;
    rect.y += 50;
    char buffer[20];
    print_ttf(surface, &text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    // Мы показываем в списке лучших результатов не больше RECORDS_MAX_NUM штук
    int n = records->size() < RECORDS_MAX_NUM ? records->size() : RECORDS_MAX_NUM;
    for (int i = 0; i < n; i++)
    {
        _itoa_s(i + 1, buffer, 10); // положение в списке
        text = buffer;
        text += ". ";
        text += records->at(i)->name; // имя рекордсмена
        text += " - ";
        _itoa_s(records->at(i)->length, buffer, 10); // результат рекордсмена
        text += buffer;
        rect.x = 0;
        rect.y += 30;
        if (*(records->at(i)) == *now_record)
        {
            print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, BLACK, &rect, x_centered);
            was_shown = true;
        }
        else
        {
            print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
        }
    }
    if (!was_shown)
    {
        // Если результат только что прошедшей игры не был показан
        int i = n;
        while (i < records->size() && *(records->at(i)) != *now_record)
        {
            i++;
        }
        _itoa_s(i + 1, buffer, 10); // положение в списке
        text = buffer;
        text += ". ";
        text += records->at(i)->name; // имя рекордсмена
        text += " - ";
        _itoa_s(records->at(i)->length, buffer, 10); // результат рекордсмена
        text += buffer;
        rect.x = 0;
        rect.y += 30;
        print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, BLACK, &rect, x_centered);
    }
    texture_end = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
}

// Метод класса Screen создает все необходимые текстуры для экрана
void Screen::create_textures(SDL_Renderer* renderer)
{
    std::string text; // текст, который нужно отобразить на экране
    SDL_Rect rect;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Создаем текстуру для отображения при завершении игры
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    text = "Game Over!";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, &text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    texture_end = SDL_CreateTextureFromSurface(renderer, surface);
    // Создаем текстуру для отображения во время игры
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    texture_game = SDL_CreateTextureFromSurface(renderer, surface);
    // Создаем текстуру для отображения при выводе справки об игре
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    text = "Instructions";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, &text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    text = "To control the snake";
    rect.x = 0;
    rect.y = 200;
    print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    text = "use the Up, Down,";
    rect.x = 0;
    rect.y = 230;
    print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    text = "Left, and Right keys.";
    rect.x = 0;
    rect.y = 260;
    print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    texture_info = SDL_CreateTextureFromSurface(renderer, surface);
    // Создаем текстуру для отображения при запуске приложения
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, SCREEN_COLOR.r, SCREEN_COLOR.g, SCREEN_COLOR.b));
    // Выведем необходимый текст на начальную заставку
    text = "Welcome to the Snake game!";
    rect.x = 0;
    rect.y = 150;
    print_ttf(surface, &text, (char*)FONT, FONT_TITLE_SIZE, BLACK, &rect, x_centered);
    text = "Enter your name to start the game:";
    rect.x = 0;
    rect.y = 200;
    print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, x_centered);
    texture_welcome = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Конструктор класса Menu
Menu::Menu()
{
    x = 0;
    y = 0;
    height = MENU_HEIGHT; // высота пункта меню
    width = WIDTH / MENU_ITEM_NUM; // ширина пункта меню
}

// Деструктор класса Menu
Menu::~Menu()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_clicked);
    SDL_DestroyTexture(texture_hovered);
}

// Метод класса Menu для отображения нужной текстуры пункта меню на рендерер
void Menu::apply_texture(SDL_Renderer* renderer)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    // Отрендерим текстуру
    switch (state)
    {
    case clicked:
    {
        // Если по пункту меню кликнули мышкой
        SDL_RenderCopy(renderer, texture_clicked, NULL, &rect);
        break;
    }
    case hovered:
    {
        // Если на пункт меню навели мышку
        SDL_RenderCopy(renderer, texture_hovered, NULL, &rect);
        break;
    }
    case usual:
    {
        // Если пункт меню в обычном состоянии
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        break;
    }
    }
}

// Метод класса Menu создает все необходимые текстуры для пункта меню
void Menu::create_textures(SDL_Renderer* renderer, std::string* text)
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Создаем текстуру для отображения пункта меню в обычном состоянии
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, MENU_USUAL_COLOR.r, MENU_USUAL_COLOR.g, MENU_USUAL_COLOR.b));
    print_ttf(surface, text, (char*)FONT, FONT_MENU_SIZE, DIM_GRAY, &rect, xy_centered);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    // Создаем текстуру для отображения пункта меню, когда по нему кликнули мышкой
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, MENU_CLICKED_COLOR.r, MENU_CLICKED_COLOR.g, MENU_CLICKED_COLOR.b));
    print_ttf(surface, text, (char*)FONT, FONT_MENU_SIZE, WHITE, &rect, xy_centered);
    texture_clicked = SDL_CreateTextureFromSurface(renderer, surface);
    // Создаем текстуру для отображения пункта меню, когда на него навели мышкой
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, MENU_HOVERED_COLOR.r, MENU_HOVERED_COLOR.g, MENU_HOVERED_COLOR.b));
    print_ttf(surface, text, (char*)FONT, FONT_MENU_SIZE, BLACK, &rect, xy_centered);
    texture_hovered = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Метод класса Menu определяет, находится ли точка внутри пункта меню
bool Menu::is_in(Sint32 x, Sint32 y)
{
    if (this->x < x && x < this->x + width && this->y < y && y < this->y + height)
    {
        // Точка с координатами x, y находится внутри пункта меню
        return true;
    }
    return false;
}

// Метод класса Menu задает положение пункта меню
void Menu::set_position(Menu* menu)
{
    x = menu->x + menu->width;
    y = 0;
}

// Конструктор класса Menu_Exit
Menu_Exit::Menu_Exit() : Menu()
{
}

// Метод класса Menu_Exit определяет действие, когда кликнули мышкой по пункту меню Выход
bool Menu_Exit::down(bool*, Screen*, Game_Field*, Snake*, Food*, SDL_Renderer*)
{
    state = clicked; // устанавливаем нажатое состояние пункта меню
    return true; // значение true означает, что нужно выйти из приложения
}

// Конструктор класса Menu_Info
Menu_Info::Menu_Info() : Menu()
{
}

// Метод класса Menu_Info определяет действие, когда кликнули мышкой по пункту меню Справка
bool Menu_Info::down(bool* is_playing, Screen* screen, Game_Field*, Snake*, Food*, SDL_Renderer*)
{
    state = clicked; // устанавливаем нажатое состояние пункта меню
    *is_playing = false; // значение false означает, что нужно завершить игру, если она игралась
    screen->page = info_page; // нужно отобразить на экране приложения страницу со справкой
    return false; // значение false означает, что приложение продолжает работать
}

// Конструктор класса Menu_Start
Menu_Start::Menu_Start() : Menu()
{
}

// Метод класса Menu_Start определяет действие, когда кликнули мышкой по пункту меню Старт
bool Menu_Start::down(bool* is_playing, Screen* screen, Game_Field* game_field, Snake* snake, Food* food, SDL_Renderer* renderer)
{
    state = clicked; // устанавливаем нажатое состояние пункта меню
    *is_playing = true; // значение true означает, что нужно начать игру
    screen->page = game_page; // нужно отобразить на экране приложения страницу с игровым полем
    game_field->create_field(renderer); // создаем игровое поле
    snake->start(game_field); // задаем змейку
    food->set_position(snake, game_field); // задаем положение еды
    return false; // значение false означает, что приложение продолжает работать
}

// Конструктор класса Input_Window для окна ввода
Input_Window::Input_Window()
{
    is_displayed = true; // по умолчанию окно ввода отображается
    state = usual; // окно ввода в обычном состоянии
    height = INPUT_WINDOW_HEIGHT; // высота окна ввода
    width = INPUT_WINDOW_WIDTH; // ширина окна ввода
    x = (WIDTH - INPUT_WINDOW_WIDTH) / 2; // координата x верхнего левого угла окна ввода
    y = INPUT_WINDOW_Y; // координата y верхнего левого угла окна ввода
}

// Метод класса Input_window для отображения нужной текстуры окна ввода на рендерер
void Input_Window::apply_texture(SDL_Renderer* renderer)
{
    if (!is_displayed)
    {
        // Если окно ввода не отображается на экране
        return;
    }
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Отрендерим текстуру
    switch (state)
    {
    case clicked:
    {
        // Если по окну ввода кликнули мышкой
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, INPUT_CLICKED_COLOR.r, INPUT_CLICKED_COLOR.g, INPUT_CLICKED_COLOR.b));
        if (text.size())
        {
            print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, BLACK, &rect, y_centered);
        }
        break;
    }
    case hovered:
    {
        // Если на окно ввода навели мышку
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, INPUT_HOVERED_COLOR.r, INPUT_HOVERED_COLOR.g, INPUT_HOVERED_COLOR.b));
        if (text.size())
        {
            print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, y_centered);
        }
        break;
    }
    case usual:
    {
        // Если окно ввода в обычном состоянии
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, INPUT_USUAL_COLOR.r, INPUT_USUAL_COLOR.g, INPUT_USUAL_COLOR.b));
        if (text.size())
        {
            print_ttf(surface, &text, (char*)FONT, FONT_TEXT_SIZE, DIM_GRAY, &rect, y_centered);
        }
        break;
    }
    }
    rect.x = x;
    rect.y = y;
    rect.h = height;
    rect.w = width;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

// Метод класса Input_Window определяет, находится ли точка внутри окна ввода
bool Input_Window::is_in(Sint32 x, Sint32 y)
{
    if (is_displayed && this->x < x && x < this->x + width && this->y < y && y < this->y + height)
    {
        // Точка с координатами x, y находится внутри окна ввода
        return true;
    }
    return false;
}
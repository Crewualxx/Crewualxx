/*
В файле хранятся определения классов Game_Field, Snake и Food,
в основном определяющих игровой процесс.
*/

#include <iostream>
#include "const.h"
#include "game_classes.h"

// Конструктор класса Game_Field
Game_Field::Game_Field()
{
}

// Метод класса Game_Filed определяет, может ли змейка находиться на игровом поле в заданном положении
bool Game_Field::available_position(Snake* snake)
{
    if (-1 < snake->i_x.at(0) && snake->i_x.at(0) < nx && -1 < snake->i_y.at(0) && snake->i_y.at(0) < ny)
    {
        // Змейка находится в доступном пололжении
        return true;
    }
    return false;
}

// Метод класса Game_Field для отображения текстуры игрового поля на рендерер
void Game_Field::apply_texture(SDL_Renderer* renderer)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width * nx, height * ny, 32, 0, 0, 0, 0);
    // Заполняем игровое поле одним цветом
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, GAME_FIELD_COLOR.r, GAME_FIELD_COLOR.g, GAME_FIELD_COLOR.b));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = height * ny;
    rect.w = width * nx;
    // Отрендерим текстуру
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    // Устанавливаем цвет для рисования линий
    SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, SDL_ALPHA_OPAQUE);
    // Рисуем вертикальные линии на игровом поле
    for (int i = 0; i <= nx; i++)
    {
        SDL_RenderDrawLine(renderer, x + height * i, y, x + width * i, y + height * ny);
    }
    // Рисуем горизонтальные линии на игровом поле
    for (int i = 0; i <= ny; i++)
    {
        SDL_RenderDrawLine(renderer, x, y + height * i, x + width * nx, y + height * i);
    }
    SDL_DestroyTexture(texture);
}

// Метод класса Game_Field создает игровое поле
void Game_Field::create_field(SDL_Renderer* renderer)
{
    nx = NX_MIN + rand() % NX; // количество клеток по горизонтали на игровом поле
    ny = NY_MIN + rand() % NY; // количество клеток по вертикали на игровом поле
    height = width = BLOCK_SIZE; // высота и ширина одной клетки на игровом поле 
    x = (WIDTH - width * nx) / 2; // координата x верхнего левого угла игрового поля
    y = MENU_HEIGHT + (HEIGHT - MENU_HEIGHT - height * ny) / 2; // координата y верхнего левого угла игрового поля
}

// Конструктор класса Snake
Snake::Snake()
{
    height = width = BLOCK_SIZE; // высота и ширина одного блока змейки
}

// Метод класса Snake управляет змейкой на игровом поле
bool Snake::control(Food* food)
{
    move(); // передвигаем змейку
    eat(food); // смотрим, может ли змейка поесть
    return !is_self_cross(); // если змейка пересекла сама себя, возвращается false
}

// Метод класса Snake определяет, занята ли точка на игровом поле змейкой
bool Snake::is_occupied(int i_x_, int i_y_)
{
    for (int i = 0; i < n; i++)
    {
        if (i_x.at(i) == i_x_ && i_y.at(i) == i_y_)
        {
            // Если клетка на игровом поле занята змейкой
            return true;
        }
    }
    // Если клетка на игровом поле свободна
    return false;
}

// Метод класса Snake определяет, пересекается ли змейка сама с собой
bool Snake::is_self_cross()
{
    int x_head = i_x.at(0), y_head = i_y.at(0); // координаты головы змейки
    for (int i = 1; i < n; i++)
    {
        if (x_head == i_x.at(i) && y_head == i_y.at(i))
        {
            // Если змейка пересекается сама с собой, то возвращается true
            return true;
        }
    }
    // Змейка не пересекается сама с собой
    return false;
}

// Метод класса Snake для отображения текстуры змейки на рендерер
void Snake::apply_texture(SDL_Renderer* renderer, Game_Field* game_field)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    int r, g, b;
    r = 255 * n / n_max; // величина оттенка красного цвета блока в RGB
    g = 255 - 255 * (n - 1) / n_max; // величина оттенка зеленого цвета блока в RGB
    b = 255 - 255 * (n - 1) / n_max; // величина оттенка синего цвета блока в RGB
    // Заполняем блок цветом
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect;
    rect.h = height;
    rect.w = width;
    for (int i = 0; i < n; i++)
    {
        rect.x = i_x.at(i) * width + game_field->x;
        rect.y = i_y.at(i) * height + game_field->y;
        // Отрендерим текстуру
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
    SDL_DestroyTexture(texture);
}

// Метод класса Snake изменяет направление движения
void Snake::change_direction(Direction new_direction)
{
    // В параметре direction хранится направление, в котором змейка движется.
    // В параметре changed_direction хранится измененное направление движения.
    // Если игрок изменил направление движения, то новое направление сначала записывается
    // в changed_direction. Когда приходит время передвинуться змейке (вызывается функция move),
    // параметр direction принимает значение changed_direction
    if (n != 1)
    {
        // Если длина змейки больше 1, она не может менять направление движения на противоположное
        if (direction == up && new_direction == down)
        {
            return;
        }
        if (direction == down && new_direction == up)
        {
            return;
        }
        if (direction == left && new_direction == right)
        {
            return;
        }
        if (direction == right && new_direction == left)
        {
            return;
        }
    }
    changed_direction = new_direction;
}

// Метод класса Snake увеличивает длину змейки
void Snake::eat(Food* food)
{
    if (i_x.at(0) == food->i_x && i_y.at(0) == food->i_y)
    {
        // Если координата головы змейки совпала с координатой еды, то увеличиваем длину змейки в направлении движения
        i_x_for_tail = i_x.at(n - 1); // записываем координаты хвоста змейки, в нее на следующем шаге запишем
        i_y_for_tail = i_y.at(n - 1); // дополнительный блок для змейки
        do_eat = true; // змейка поела
        // Еда съедена змейкой
        food->is_eaten = true;
    }
}

// Метод класса Snake передвигает змейку
void Snake::move()
{
    int i_x_prev, i_y_prev; // координаты на игровом поле предыдущего блока змейки
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            direction = changed_direction; // направлению движения змейки задается измененное значения направления
            // Работаем с головой змейки.
            // Координаты головы змейки будут координатами предыдущего блока для второго блока
            i_x_prev = i_x.at(i);
            i_y_prev = i_y.at(i);
            // Голова змейки движется в выбранном направлении
            switch (direction)
            {
            case down:
            {
                i_y.at(i)++;
                break;
            }
            case up:
            {
                i_y.at(i)--;
                break;
            }
            case right:
            {
                i_x.at(i)++;
                break;
            }
            case left:
            {
                i_x.at(i)--;
                break;
            }
            }
        }
        else
        {
            // Остальные блоки змейки перемещаются на место впереди расположенных блоков
            int x_temp = i_x.at(i);
            int y_temp = i_y.at(i);
            i_x.at(i) = i_x_prev;
            i_y.at(i) = i_y_prev;
            i_x_prev = x_temp;
            i_y_prev = y_temp;
        }
    }
    if (do_eat)
    {
        // Если змейка поела на предыдущем шаге, то на этом шаге нужно увеличить длину змейки
        i_x.push_back(i_x_prev);
        i_y.push_back(i_y_prev);
        n++;
        do_eat = false;
    }
}

// Метод класса Snake задает параметры змейки в начале игры
void Snake::start(Game_Field* game_field)
{
    n = 1; // длина змейки
    n_max = game_field->nx * game_field->ny;
    // Единственный блок, из которого состоит змейка, помещается по центру игрового поля
    i_x.clear();
    i_y.clear();
    i_x.push_back(game_field->nx / 2);
    i_y.push_back(game_field->ny / 2);
    direction = changed_direction = left; // змейка движется налево
    do_eat = false; // змейка при старте не поела
}

// Конструктор класса Food
Food::Food()
{
    height = width = BLOCK_SIZE; // высота и ширина блока еды
}

// Метод класса Food определяет положение блока еды
bool Food::set_position(Snake* snake, Game_Field* game_field)
{
    is_eaten = false; // еда не съедена
    if (game_field->nx * game_field->ny == snake->n)
    {
        // Если длина змейки равна количеству клеток на игровом поле
        return false;
    }
    int n = rand() % (game_field->nx * game_field->ny - snake->n); // номер клетки, в которой будет помещена еда
    int free_blocks = -1; // счетчик свободных клеток на карте
    i_x = game_field->nx;
    i_y = game_field->ny;
    for (int i_y_ = 0; i_y_ < game_field->ny; i_y_++)
    {
        for (int i_x_ = 0; i_x_ < game_field->nx; i_x_++)
        {
            if (!snake->is_occupied(i_x_, i_y_))
            {
                // Если блок на игровом поле свободен от змейки
                free_blocks++;
                if (free_blocks == n)
                {
                    // Положение для еды найдено
                    i_x = i_x_;
                    i_y = i_y_;
                    break;
                }
            }
        }
        if (i_x != game_field->nx)
        {
            // Если положение для еды было найдено
            break;
        }
    }
    return true;
}

// Метод класса Food определяет, была ли съедена еда
bool Food::was_eaten(Snake* snake, Game_Field* game_field)
{
    if (is_eaten)
    {
        // Если еда была съедена, нужно поместить еду в новое пололжени
        return set_position(snake, game_field);
    }
    return true;
}

// Метод класса Food для отображения блока еды на рендерер
void Food::apply_texture(SDL_Renderer* renderer, Game_Field* game_field)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Заполняем блок одним цветом
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, GREEN.r, GREEN.g, GREEN.b));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect;
    rect.x = i_x * width + game_field->x;
    rect.y = i_y * height + game_field->y;
    rect.h = height;
    rect.w = width;
    // Отрендерим текстуру
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
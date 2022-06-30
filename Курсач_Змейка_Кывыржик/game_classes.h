/*
В заголовочном файле хранятся объявления классов Game_Field, Snake и Food,
в основном определяющих игровой процесс.
*/

#pragma once
#ifndef _GAME_CLASSES_
#define _GAME_CLASSES_

#include <SDL.h>
#include <vector>

/* Тип перечисления для направления движения змейки:
down - вниз;
left - влево;
right - вправо;
up - вверх. */
enum Direction { down, left, right, up };

// Прототип класса Game_Field
class Game_Field;

// Прототип класса Snake
class Snake;

// Прототип класса Food
class Food;

// Класс Game_Field определяет игровое поле
class Game_Field
{
public:
    int x; // координата x верхнего левого угла игрового поля
    int y; // координата y верхнего левого угла игрового поля
    int height; // высота одной клетки
    int width; // ширина одной клетки
    int nx; // количество клеток на игровом поле по горизонтали
    int ny; // количество клеток на игровом поле по вертикали
    // Конструктор
    Game_Field();
    // Метод определяет, может ли змейка находится в заданном положении на игровом поле
    bool available_position(Snake*);
    // Метод для отображения текстуры игрового поля на рендерер
    void apply_texture(SDL_Renderer*);
    // Метод создает игровое поле
    void create_field(SDL_Renderer*);
};

// Класс Snake определяет объект змейку, которая движется в игре
class Snake
{
public:
    bool do_eat; // если true, то змейка поела и на следующем шаге нужно увеличить ее длину
    int n; // длина змейки
    int n_max; // максимальная возможная длина змейки
    std::vector<int> i_x; // вектор с координатами x блоков змейки на игровом поле
    std::vector<int> i_y; // вектор с координатами y блоков змейки на игровом поле
    int height; // высота одного блока змейки
    int width; // ширина одного блока змейки
    int i_x_for_tail; // координата x хвоста змейки; используется, когда змейка съедает еду
    int i_y_for_tail; // координата y хвоста змейки; используется, когда змейка съедает еду
    Direction direction; // направление движения змейки
    Direction changed_direction; // измененное направление движения змейки
    // Конструктор
    Snake();
    // Метод управляет змейкой
    bool control(Food*);
    // Метод определяет, занята ли точка на игровом поле змейкой
    bool is_occupied(int, int);
    // Метод определяет, пересекается ли змейка сама с собой
    bool is_self_cross();
    // Метод для отображения текстуры змейки на рендерер
    void apply_texture(SDL_Renderer*, Game_Field*);
    // Метод изменяет направление движения
    void change_direction(Direction);
    // Метод увеличивает длину змейки
    void eat(Food*);
    // Метод передвигает змейку
    void move();
    // Метод задает параметры змейки в начале игры
    void start(Game_Field*);
};

// Класс Food определяет объект еду для змейки
class Food
{
public:
    int i_x; // координата x блока еды
    int i_y; // координата y блока еды
    int height; // высота блока еды
    int width; // ширина блока еды
    bool is_eaten; // если true, то еда съедена
    // Конструктор
    Food();
    // Метод определяет положение блока еды
    bool set_position(Snake*, Game_Field*);
    // Метод определяет, была ли съедена еда
    bool was_eaten(Snake*, Game_Field*);
    // Метод для отображения текструры блока еды на рендерер
    void apply_texture(SDL_Renderer*, Game_Field*);
};

#endif // !_GAME_CLASSES_


/*
В заголовочном файле хранятся объявления классов Screen, Menu, Menu_Start, Menu_Info,
Menu_Exit, Input_Window, в основном определяющих элементы управления окна приложения.
*/

#pragma once
#ifndef _WINDOW_CLASSES_
#define _WINDOW_CLASSES_

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <vector>
#include "const.h"
#include "game_classes.h"
#include "record.h"

/* Тип перечисления для задания страницы, которую отображает окно приложения:
welcome_page - начальная страница с приветствием;
game_page - страница во время игры;
info_page - страница со справкой об игре;
end_page - страница при завершении игры (поражении игрока). */
enum Page { welcome_page, game_page, info_page, end_page };

/* Тип перечисления для определения состояния объектов окна приложения (пунктов меню, окна ввода):
usual - обычное;
clicked - по пункту меню кликнули мышкой;
hovered - на пункт меню навели мышкой. */
enum State { usual, clicked, hovered };

/* Тип перечисления для центрирования текста:
not_centered - обычное расположение, текст центрировать не нужно;
x_centered - текст нужно разместить по центру по горизонтали;
y_centered - текст нужно разместить по центру по вертикали;
xy_centered - текст нужно разместить по центру по горизонтали и вертикали. */
enum Centered { not_centered, x_centered, y_centered, xy_centered };

// Прототип класса Screen
class Screen;

// Прототип класса Menu
class Menu;

// Прототип класса Menu_Exit
class Menu_Exit;

// Прототип класса Menu_Info
class Menu_Info;

// Прототип класса Menu_Start
class Menu_Start;

// Прототип класса Input_Window
class Input_Window;

// Класс Screen определяет экран приложения
class Screen
{
public:
    bool was_created; // была ли создана текстура для экрана после поражения в игре
    int x; // координата x верхнего левого угла экрана
    int y; // координата y верхнего левого угла экрана
    int height; // высота экрана
    int width; // ширина экрана
    Page page; // какую страницу экран приложения должен показывать
    SDL_Texture* texture_end; // текстура для экрана после поражения
    SDL_Texture* texture_game; // текстура для экрана во время игры
    SDL_Texture* texture_info; // текстура для экрана при выводе справки об игре
    SDL_Texture* texture_welcome; // текстура для экрана при запуске приложения
    // Конструктор
    Screen();
    // Деструктор
    ~Screen();
    // Метод для отображения нужной текстуры экрана на рендерер
    void apply_texture(SDL_Renderer*, std::vector<Record*>*);
    // Метод создает текстуру для экрана после поражения
    void create_end_texture(SDL_Renderer*, std::vector<Record*>*);
    // Метод создает все необходимые текстуры для экрана
    void create_textures(SDL_Renderer*);
};

// Класс Menu определяет пункт меню
class Menu
{
public:
    int x; // координата x верхнего левого угла пункта меню
    int y; // координата y верхнего леаого угла пунтк меню
    int height; // высота пункта меню
    int width; // ширина пункта меню
    State state; // состояние пункта меню (обычное, на пункт меню навели мышкой или кликнули)
    SDL_Texture* texture; // текстура для рисования пункта меню в обычном состоянии
    SDL_Texture* texture_clicked; // текстура для рисования пункта меню в нажатом состоянии
    SDL_Texture* texture_hovered; // текстура для рисования пункта меню при наведении на него мышкой
    // Конструктор
    Menu();
    // Деструктор
    ~Menu();
    // Метод для отображения нужной текстуры пункта меню на рендерер
    void apply_texture(SDL_Renderer*);
    // Метод создает все необходимые текстуры для пункта меню
    void create_textures(SDL_Renderer*, std::string*);
    // Метод определяет, находится ли точка внутри пункта меню
    bool is_in(Sint32, Sint32);
    // Метод задает положение пункта меню
    void set_position(Menu*);
    // Абстрактный метод определяет действие, когда кликнули мышкой по пункту меню
    virtual bool down(bool*, Screen*, Game_Field*, Snake*, Food*, SDL_Renderer*) = 0;
};

// Класс Menu_Exit определяет пункт меню Выход
class Menu_Exit : public Menu
{
public:
    // Конструктор
    Menu_Exit();
    // Метод определяет действие, когда кликнули мышкой по пункту меню Выход
    bool down(bool*, Screen*, Game_Field*, Snake*, Food*, SDL_Renderer*);
};

// Класс Menu_Info определяет пункт меню Справка
class Menu_Info : public Menu
{
public:
    // Конструктор
    Menu_Info();
    // Метод определяет действие, когда кликнули мышкой по пункту меню Справка
    bool down(bool*, Screen*, Game_Field*, Snake*, Food*, SDL_Renderer*);
};

// Класс Menu_Start определяет пункт меню Старт
class Menu_Start : public Menu
{
public:
    // Конструктор
    Menu_Start();
    // Метод определяет действие, когда кликнули мышкой по пункту меню Старт
    bool down(bool*, Screen*, Game_Field*, Snake*, Food*, SDL_Renderer*);
};

// Класс Окно_Ввода определяет окошко для ввода данных пользователем
class Input_Window
{
public:
    int x; // координата x верхнего левого угла окна ввода
    int y; // координата y верхнего леаого угла окна ввода
    int height; // высота окна ввода
    int width; // ширина окна ввода
    bool is_displayed; // если true, то окно ввода отображается на экране приложения
    std::string text; // текст, введенный пользователем
    std::string input_data; // введенная пользователем информация
    State state; // состояние окна ввода (обычное, на окно ввода навели мышкой или кликнули)
    // Конструктор
    Input_Window();
    // Метод для отображения нужной текстуры окна ввода на рендерер
    void apply_texture(SDL_Renderer*);
    // Метод определяет, находится ли точка внутри окна ввода
    bool is_in(Sint32, Sint32);
};

#endif
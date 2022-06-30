/*
Заголовочный файл с объявлением используемых в приложении констант.
*/

#pragma once
#ifndef _CONST_
#define _CONST_

#include <SDL.h>

extern const char FILE_NAME[]; // имя файла, в который записываются рекорды игры
extern const char FONT[]; // используемый шрифт
extern const int BLOCK_SIZE; // размер одного блока по горизонтали и вертикали на игровом поле
extern const int HEIGHT; // высота окна приложения
extern const int WIDTH; // ширина окна приложения
extern const int FONT_MENU_SIZE; // размер шрифта меню
extern const int FONT_TEXT_SIZE; // размер шрифта обычного текста
extern const int FONT_TITLE_SIZE; // размер шрифта заголовков
extern const int INPUT_WINDOW_HEIGHT; // высота окна ввода
extern const int INPUT_WINDOW_WIDTH; // ширина окна ввода
extern const int INPUT_WINDOW_Y; // координата y верхнего левого угла окна ввода
extern const int MENU_HEIGHT; // высота меню
extern const int MENU_ITEM_NUM; // количество пунктов в меню: Старт, Справка, Выход
extern const int NAME_MAX_LENGTH; // максимальная длина имени, которое может ввести пользователь
extern const int NX; // максимальное количество клеток по горизонтали на игровом поле
extern const int NX_MIN; // минимальное количество клеток по горизонтали на игровом поле
extern const int NY; // максимальное количество клеток по вертикали на игровом поле
extern const int NY_MIN; // минимальное количество клеток по вертикали на игровом поле
extern const int RECORDS_MAX_NUM; // максимальное количество рекордов, выводимых на экран приложения
extern const SDL_Color BLACK; // черный цвет
extern const SDL_Color DIM_GRAY; // темно-серый цвет
extern const SDL_Color GREEN; // зеленой цвет
extern const SDL_Color RED; // красный цвет
extern const SDL_Color WHITE; // белый цвет
extern const SDL_Color GAME_FIELD_COLOR; // цвет игрового поля
extern const SDL_Color INPUT_CLICKED_COLOR; // цвет окна ввода в кликнутом состоянии
extern const SDL_Color INPUT_HOVERED_COLOR; // цвет окна ввода в наведенном состоянии
extern const SDL_Color INPUT_USUAL_COLOR; // цвет окна ввода в обычном состоянии
extern const SDL_Color MENU_CLICKED_COLOR; // цвет пункта меню в кликнутом состоянии
extern const SDL_Color MENU_HOVERED_COLOR; // цвет пункта меню в наведенном состоянии
extern const SDL_Color MENU_USUAL_COLOR; // цвет пункта меню в обычном состоянии
extern const SDL_Color SCREEN_COLOR; // цвет экрана
extern const Uint32 GAME_TIME_INTERVAL; // интервал между вызовами функции game_timer_callback во время игры

#endif // !_CONST_


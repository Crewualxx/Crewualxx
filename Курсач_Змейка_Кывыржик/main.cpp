#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "const.h"
#include "game_classes.h"
#include "window_classes.h"

bool is_playing = false;
std::vector<Record*> all_records; 
Screen screen; 
Menu_Exit menu_exit;
Menu_Info menu_info; 
Menu_Start menu_start;
Input_Window input_window;
Game_Field game_field; 
Snake snake; 
Food food; 

void draw(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer); 
    screen.apply_texture(renderer, &all_records); 
    menu_start.apply_texture(renderer);
    menu_info.apply_texture(renderer);
    menu_exit.apply_texture(renderer);
    input_window.apply_texture(renderer);
    if (is_playing)
    {
        game_field.apply_texture(renderer);
        snake.apply_texture(renderer, &game_field);
        food.apply_texture(renderer, &game_field);
    }
    SDL_RenderPresent(renderer);
}
Uint32 game_timer_callback(Uint32 interval, void* param)
{
    SDL_Renderer* renderer = (SDL_Renderer*)param;
    if (is_playing)
    {
        if (!snake.control(&food) ||
            !game_field.available_position(&snake) ||
            !food.was_eaten(&snake, &game_field))
        {
            is_playing = false;
            screen.page = end_page;
            write_record(&all_records, &input_window.text, snake.n);
            return 0;
        }
        draw(renderer);
        return interval;
    }
    else
    {
        return 0;
    }
}

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(time(0)));
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0)
    {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    screen.create_textures(renderer);
    std::string menu_name = "Start";
    menu_start.create_textures(renderer, &menu_name);
    menu_info.set_position(&menu_start);
    menu_name = "Info";
    menu_info.create_textures(renderer, &menu_name);
    menu_exit.set_position(&menu_info);
    menu_name = "Exit";
    menu_exit.create_textures(renderer, &menu_name);
    draw(renderer);
    std::string name1 = "vany";
    Record record1(&name1, 20);
    name1 = "pety";
    Record record2(&name1, 30);
    name1 = "vany";
    Record record3(&name1, 20);
    bool result_bool = record1 == record2;
    std::cout << record1.name << "==" << record2.name << "=" << result_bool << std::endl;
    result_bool = record1 == record3;
    std::cout << record1.name << "==" << record3.name << "=" << result_bool << std::endl;

    SDL_TimerID my_timer;
    SDL_StopTextInput();
    read_file(&all_records);
    SDL_Event window_event;
    bool quit = false;
    while (!quit)
    {
        if (SDL_PollEvent(&window_event))
        {
            switch (window_event.type)
            {
            case SDL_QUIT:
            {
                quit = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                if (is_playing)
                {
                    switch (window_event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_DOWN:
                    {
                        snake.change_direction(down);
                        break;
                    }
                    case SDL_SCANCODE_LEFT:
                    {
                        snake.change_direction(left);
                        break;
                    }
                    case SDL_SCANCODE_RIGHT:
                    {
                        snake.change_direction(right);
                        break;
                    }
                    case SDL_SCANCODE_UP:
                    {
                        snake.change_direction(up);
                        break;
                    }
                    }
                }
                if (input_window.state == clicked && window_event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && input_window.text.size())
                {
                    input_window.text.pop_back();
                }                
                break;
            }
            case SDL_TEXTINPUT:
            {
                if (input_window.text.size() < NAME_MAX_LENGTH)
                {
                    if (window_event.text.text[0] == ' ')
                    {
                        input_window.text += "_";
                    }
                    else
                    {
                        input_window.text += window_event.text.text;
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if (window_event.button.button == SDL_BUTTON_LEFT)
                {
                    Sint32 x = window_event.button.x;
                    Sint32 y = window_event.button.y;
                    if (menu_start.is_in(x, y))
                    {
                        quit = menu_start.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false;
                        Uint32 time_interval = GAME_TIME_INTERVAL;
                        my_timer = SDL_AddTimer(time_interval, game_timer_callback, renderer);
                        screen.was_created = false;
                    }
                    else if (menu_info.is_in(x, y))
                    {
                        quit = menu_info.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false;
                    }
                    else if (menu_exit.is_in(x, y))
                    {
                        quit = menu_exit.down(&is_playing, &screen, &game_field, &snake, &food, renderer);
                        input_window.is_displayed = false;
                    }
                    if (input_window.is_in(x, y))
                    {
                        input_window.state = clicked;
                        SDL_StartTextInput();
                    }
                    else if (input_window.is_displayed)
                    {
                        input_window.state = usual;
                        SDL_StopTextInput();
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (window_event.button.button == SDL_BUTTON_LEFT)
                {
                    if (menu_start.state == clicked)
                    {
                        menu_start.state = usual;
                    }
                    else if (menu_info.state == clicked)
                    {
                        menu_info.state = usual;
                    }
                    else if (menu_exit.state == clicked)
                    {
                        menu_exit.state = usual;
                    }
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                Sint32 x = window_event.motion.x;
                Sint32 y = window_event.motion.y;
                if (menu_start.state == hovered && !menu_start.is_in(x, y))
                {
                    menu_start.state = usual;
                }
                else if (menu_start.state == !clicked && menu_start.is_in(x, y))
                {
                    menu_start.state = hovered;
                }
                if (menu_info.state == hovered && !menu_info.is_in(x, y))
                {
                    menu_info.state = usual;
                }
                else if (menu_info.state == !clicked && menu_info.is_in(x, y))
                {
                    menu_info.state = hovered;
                }
                if (menu_exit.state == hovered && !menu_exit.is_in(x, y))
                {
                    menu_exit.state = usual;
                }
                else if (menu_exit.state == !clicked && menu_exit.is_in(x, y))
                {
                    menu_exit.state = hovered;
                }
                if (input_window.is_in(x, y) && input_window.state != clicked)
                {
                    input_window.state = hovered;
                }
                else if (!input_window.is_in(x, y) && input_window.state == hovered)
                {
                    input_window.state = usual;
                }
            }
            }
            draw(renderer);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

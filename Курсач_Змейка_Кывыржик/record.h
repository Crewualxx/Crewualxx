/*
В заголовочном файле хранятся объявления класса Record и других самостоятельных функций,
которые служат для записи и чтения списка с результатами игры в двоичный файл.
*/

#pragma once
#ifndef _RECORD_
#define _RECORD_

#include <iostream>
#include <vector>

// Класс Record для работы с записями результатов игры
class Record
{
public:
    static const int name_max_length = 19; // максимальная длина имени игрока
    char name[name_max_length]; // имя игрока
    int length; // длина змейки, достигнутая игроком в игре
    // Конструкторы
    Record();
    Record(std::string*, int);
    Record(Record*);
    // Метод задает имя игрока и длину змейки, полученную игроком
    void set_record(std::string*, int);
    void set_record(Record*);
};
// Переопределение оператора равенства для объектов класса Record
bool operator == (Record, Record);
// Переопределение оператора неравенства для объектов класса Record
bool operator != (Record, Record);

// Функция selection_sort для сортировки рекордов в игре по убыванию. Для сортировки применяется метод выбора
void selection_sort(std::vector<Record*>*);

/* Функция read_file читает двоичный файл с рекордами, помещает записи в объекты
типа Record, которые помещает в вектор */
void read_file(std::vector<Record*>*);

// Функция write_file записывает данные из вектора типа Records в двоичный файл
void write_file(std::vector<Record*>*);

/* Функция write_record записывает результат игрока в список с рекорадми.
records - вектор с рекордами;
name - имя игрока;
length - результат игрока - длина змейки в последней сыгранной игре */
void write_record(std::vector<Record*>* records, std::string* name, int length);

#endif // !_RECORD_

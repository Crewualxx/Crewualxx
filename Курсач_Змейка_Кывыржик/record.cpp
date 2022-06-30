/*
В файле хранятся определения методов класса Record и другие самостоятельные
функции, которые служат для записи и чтения списка с результатами игры в двоичный файл.
*/

#include <fstream>
#include "const.h"
#include "record.h"

// Конструктор класса Record
Record::Record()
{
}

// Конструктор класса Record
Record::Record(std::string* name, int length)
{
    set_record(name, length);
}

// Конструктор класса Record
Record::Record(Record* record)
{
    set_record(record);
}

// Метод класса Record задает имя игрока и длину змейки, полученную игроком
void Record::set_record(std::string* name, int length)
{
    if (name->size() == 0)
    {
        // Если игрок не ввел никакое имя
        (*name) = "anonymous";

    }
    // В атрибут name класса Record мы можем записать имя не длиннее (name_max_length-1)
    // (последний символ на конец строки '\0'). Потому нужно проверить, что короче:
    // длина строки name, переданной в функцию, или (name_max_length-1)
    int n = name->size() < name_max_length - 1 ? name->size() : name_max_length - 1;
    int i;
    for (i = 0; i < n; i++)
    {
        this->name[i] = (*name)[i];
    }
    this->name[i] = '\0'; // добавляем конец строки к имени
    this->length = length;
}

// Метод класса Record задает имя игрока и длину змейки, полученную игроком
void Record::set_record(Record* record)
{
    for (int i = 0; i < name_max_length; i++)
    {
        this->name[i] = record->name[i];
    }
    this->length = record->length;
}

// Переопределение оператора равенства для объектов класса Record
bool operator == (Record r1, Record r2)
{
    if (r1.length == r2.length)
    {
        for (int i = 0; i < Record::name_max_length; i++)
        {
            if (r1.name[i] != r2.name[i])
            {
                // Если хоть один символ в значении атрибута name двух объектов не совпал, то объекты не равны
                return false;
            }
            else if (r1.name[i] == r2.name[i] && r1.name[i] == '\0')
            {
                // Если цикл дошел до символа конца строки в атрибуте name обоих объектов, то объекты равны
                return true;
            }
        }
    }
    // Если значения атрибута length объектов не совпали, то объекты не равны
    return false;
}

// Переопределение оператора неравенства для объектов класса Record
bool operator != (Record r1, Record r2)
{
    if (r1.length == r2.length)
    {
        for (int i = 0; i < Record::name_max_length; i++)
        {
            if (r1.name[i] != r2.name[i])
            {
                // Если хоть один символ в значении атрибута name двух объектов не совпал, то объекты не равны
                return true;
            }
            else if (r1.name[i] == r2.name[i] && r1.name[i] == '\0')
            {
                // Если цикл дошел до символа конца строки в атрибуте name обоих объектов, то объекты равны
                return false;
            }
        }
    }
    // Если значения атрибута length объектов не совпали, то объекты не равны
    return true;
}

// Функция selection_sort для сортировки рекордов в игре по убыванию. Для сортировки применяется метод выбора
void selection_sort(std::vector<Record*>* records)
{
    int size = records->size();
    for (int i = 0; i < size - 1; i++)
    {
        int max_i = i;
        for (int j = i + 1; j < size; j++)
        {
            if (records->at(max_i)->length < records->at(j)->length)
            {
                max_i = j;
            }
        }
        if (max_i != i)
        {
            Record* temp = records->at(i);
            records->at(i) = records->at(max_i);
            records->at(max_i) = temp;
        }
    }
}

/* Функция read_file читает двоичный файл с записями о результатах игры, помещает записи в объекты
типа Record, указатели на которые помещает в вектор records */
void read_file(std::vector<Record*>* records)
{
    std::ifstream file;
    file.open(FILE_NAME, std::ios::binary); // открываем файл с записями для чтения
    if (!file)
    {
        // Если файл не был открыт
        std::cout << "File with records was not opened for reading.\n";
        return;
    }
    file.seekg(0, file.end); // перемещаемся в конец файла
    int n = file.tellg() / sizeof(Record); // количество записей в файле
    file.seekg(0, file.beg); // перемещаемся в начало файла
    int i = 0;
    while (i < n)
    {
        Record* record = new Record; // объект, в который будем читать данные из двоичного файла с рекордами
        file.read((char*)record, sizeof(Record)); // читаем запись в объект record
        records->push_back(record); // помещаем указатель на прочитанные данные в вектор с указателями
        std::cout << i << " - " << records->at(i)->name << " - " << records->at(i)->length << "\n";
        i++;
    }
    file.close(); // закрываем файл
}

// Функция write_file записывает последний элемент из вектора records в двоичный файл
void write_file(std::vector<Record*>* records)
{
    std::ofstream file;
    file.open(FILE_NAME, std::ofstream::app | std::ofstream::binary); // открываем файл с записями о результатах игры для записи
    if (!file)
    {
        // Если файл не был открыт
        std::cout << "File with records was not opened for writing.\n";
        return;
    }
    // Если файл был открыт
    std::cout << "write_file: " << records->at(records->size() - 1)->name << " - " << records->at(records->size() - 1)->length << std::endl;
    file.write((char*)records->at(records->size() - 1), sizeof(Record)); // записываем в файл
    file.close(); // закрываем файл
}

/* Функция write_record записывает результат игрока в список с результатами.
records - вектор с результатами;
name - имя игрока;
length - результат игрока - длина змейки в последней сыгранной игре */
void write_record(std::vector<Record*>* records, std::string* name, int length)
{
    Record* record = new Record(name, length); // создаем запись об игре типа Record
    records->push_back(record); // помещаем запись в вектор с результатами игры
    write_file(records); // записываем последний элемент вектора с результатами в файл
}
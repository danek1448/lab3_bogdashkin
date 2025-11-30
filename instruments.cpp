#include "instruments.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cctype>

using namespace std;

// Функция проверки строки на допустимые символы
bool Proverka_stroki(const string& s) {
    if (s.empty()) return false;  // Пустая строка недопустима

    // Проверяем каждый символ строки
    for (char c : s) {
        // Проверяем латинские буквы
        bool is_latin_letter = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        // Проверяем русские буквы (включая Ё/ё)
        bool is_cyrillic_letter = (c >= 'А' && c <= 'я') || (c == 'Ё' || c == 'ё');
        // Проверяем пробельные символы
        bool is_space = isspace(c);
        // Проверяем допустимые символы пунктуации
        bool is_allowed_punctuation = (c == '-' || c == '\'' || c == ' ' || c == '.');

        // Если символ не соответствует ни одной категории - строка невалидна
        if (!is_latin_letter && !is_cyrillic_letter && !is_space && !is_allowed_punctuation) {
            return false;
        }
    }
    return true;  // Все символы валидны
}

// Функция ввода строки с проверкой
string Vvod_stroki(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;  // Выводим приглашение
        getline(cin, input);  // Считываем всю строку

        if (!input.empty()) {
            return input;  // Возвращаем непустую строку
        }
        else {
            cout << "Ошибка! Название не может быть пустым: ";
        }
    }
}

// Функция проверки ввода вещественного числа в диапазоне
float Proverka_fl(float min_znach, float max_znach) {
    float znachenie;
    while (true) {
        cin >> znachenie;  // Пытаемся считать число

        // Проверяем на ошибку ввода или выход за диапазон
        if (cin.fail() || znachenie < min_znach || znachenie > max_znach) {
            cin.clear();  // Сбрасываем флаги ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер
            cout << "Ошибка. Введите число от " << min_znach << " до " << max_znach << ": ";
        }
        else {
            // Проверяем, что после числа нет лишних символов
            char next_char = cin.peek();
            if (next_char != '\n' && next_char != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Введите только число: ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер от \n
            return znachenie;  // Возвращаем валидное число
        }
    }
}

// Функция проверки ввода целого числа в диапазоне
int Proverka_in(int min_znach, int max_znach) {
    int znachenie;
    while (true) {
        cin >> znachenie;  // Пытаемся считать целое число

        // Проверяем на ошибку ввода или выход за диапазон
        if (cin.fail() || znachenie < min_znach || znachenie > max_znach) {
            cin.clear();  // Сбрасываем флаги ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер
            cout << "Ошибка. Введите число от " << min_znach << " до " << max_znach << ": ";
        }
        else {
            // Проверяем, что после числа нет лишних символов
            char next_char = cin.peek();
            if (next_char != '\n' && next_char != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Введите только число: ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер от \n
            return znachenie;  // Возвращаем валидное число
        }
    }
}

// Функция проверки ввода булевого значения (0 или 1)
bool Proverka_bl() {
    int znachenie;
    while (true) {
        cin >> znachenie;  // Пытаемся считать число

        // Проверяем на ошибку ввода или недопустимое значение
        if (cin.fail() || (znachenie != 0 && znachenie != 1)) {
            cin.clear();  // Сбрасываем флаги ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер
            cout << "Ошибка. Введите 0 или 1: ";
        }
        else {
            // Проверяем, что после числа нет лишних символов
            char next_char = cin.peek();
            if (next_char != '\n' && next_char != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Введите только число (0 или 1): ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер от \n
            return znachenie == 1;  // Возвращаем true если ввели 1, false если 0
        }
    }
}

// Функция для получения названия трубы с предварительной очисткой буфера
string GetPipeName() {
    cin.ignore();  // Игнорируем оставшийся в буфере символ новой строки
    return Vvod_stroki("Введите название: ");  // Возвращаем результат Vvod_stroki
}

// Функция для получения числа в заданном диапазоне
int GetNumberInRange(int min_val, int max_val) {
    return Proverka_in(min_val, max_val);  // Используем базовую функцию проверки
}

// Функция для получения числа с минимальным ограничением
int GetNumberMin(int min_val) {
    // Максимальное значение - максимальное для типа int
    return Proverka_in(min_val, numeric_limits<int>::max());
}
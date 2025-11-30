#include "instruments.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cctype>

using namespace std;

bool Proverka_stroki(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        bool is_letter = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= 'А' && c <= 'я') || (c == 'Ё' || c == 'ё');
        bool is_space = isspace(c);
        bool is_allowed_punctuation = (c == '-' || c == '\'' || c == ' ' || c == '.');
        if (!is_letter && !is_space && !is_allowed_punctuation) {
            return false;
        }
    }
    return true;
}

string Vvod_stroki(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) {
            return input;
        }
        else {
            cout << "Ошибка! Название не может быть пустым: ";
        }
    }
}

float Proverka_fl(float min_znach, float max_znach) {
    float znachenie;
    while (true) {
        cin >> znachenie;
        if (cin.fail() || znachenie < min_znach || znachenie > max_znach) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка. Введите число: ";
        }
        else {
            char next_char = cin.peek();
            if (next_char != '\n' && next_char != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Введите только число: ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return znachenie;
        }
    }
}

int Proverka_in(int min_znach, int max_znach) {
    int znachenie;
    while (true) {
        cin >> znachenie;
        if (cin.fail() || znachenie < min_znach || znachenie > max_znach) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка. Введите число от " << min_znach << " до " << max_znach << ": ";
        }
        else {
            char next_char = cin.peek();
            if (next_char != '\n' && next_char != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Введите только число: ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return znachenie;
        }
    }
}

bool Proverka_bl() {
    int znachenie;
    while (true) {
        cin >> znachenie;
        if (cin.fail() || (znachenie != 0 && znachenie != 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка. Введите 0 или 1: ";
        }
        else {
            char next_char = cin.peek();
            if (next_char != '\n' && next_char != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка! Введите только число (0 или 1): ";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return znachenie == 1;
        }
    }
}

// Исправленные функции - без перегрузки
string GetPipeName() {
    cin.ignore();
    return Vvod_stroki("Введите название: ");
}

int GetNumberInRange(int min_val, int max_val) {
    return Proverka_in(min_val, max_val);
}

int GetNumberMin(int min_val) {
    return Proverka_in(min_val);
}
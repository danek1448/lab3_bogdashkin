#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <string>
#include <vector>
#include <iostream>
#include <limits>

using namespace std;

// Базовые функции проверки ввода
bool Proverka_stroki(const string& s);
string Vvod_stroki(const string& prompt);
float Proverka_fl(float min_znach = 0.0f, float max_znach = 3.402823466e+38f);
int Proverka_in(int min_znach = 0, int max_znach = 2147483647);
bool Proverka_bl();

// Функции для работы с сетью - убираем перегрузку
string GetPipeName();
int GetNumberInRange(int min_val, int max_val);
int GetNumberMin(int min_val);

#endif
#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <string>
#include <vector>
#include "truba.h"
#include "cs.h"

using namespace std;

// функции проверки ввода
bool Proverka_stroki(const string& s);  // проверка строки на допустимые символы
string Vvod_stroki(const string& prompt);  // ввод строки с подсказкой
float Proverka_fl(float min_znach = 0.0f, float max_znach = 3.402823466e+38f);  // ввод дробного числа
int Proverka_in(int min_znach = 0, int max_znach = 2147483647);  // ввод целого числа
bool Proverka_bl();  //  ввод булевого значения 

void Sohranit_dannye(const string& imya_faila, const vector<Truba>& truby, const vector<Kompressornaya_stantsiya>& kompressornye_stantsii);  // сохранение данных в файл
void Zagruzit_dannye(const string& imya_faila, vector<Truba>& truby, vector<Kompressornaya_stantsiya>& kompressornye_stantsii);  // загрузка данных из файла

#endif
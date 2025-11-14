#include "truba.h"
#include "instruments.h"
#include <iostream>
#include <string>

using namespace std;

// конструкторы
Truba::Truba() : name(""), dlina_km(0), diameter_mm(0), remont(false) {}

Truba::Truba(string n, double l, int d, bool r) : name(n), dlina_km(l), diameter_mm(d), remont(r) {}

// получаем данные о трубе
string Truba::getName() const { return name; }
double Truba::getDlina() const { return dlina_km; }
int Truba::getDiameter() const { return diameter_mm; }
bool Truba::isRemont() const { return remont; }
void Truba::setRemont(bool r) { remont = r; }
void Truba::setName(const string& newName) { name = newName; }

// вывод инф о трубе 
void Truba::print() const {
    if (name.empty()) {
        cout << "Труба не добавлена" << endl;
        return;
    }
    cout << *this;
}

void Truba::read() {
    cin >> *this;
}

ostream& operator<<(ostream& out, const Truba& t) {
    if (t.name.empty()) {
        out << "Труба не добавлена" << endl;
        return out;
    }

    out << "Труба " << endl
        << "Название: " << t.name << endl
        << "Длина: " << t.dlina_km << " км" << endl
        << "Диаметр: " << t.diameter_mm << " мм" << endl
        << "Статус: " << (t.remont ? "В ремонте" : "Работает") << endl
        << endl;
    return out;
}

istream& operator>>(istream& in, Truba& t) {
    t.name = Vvod_stroki("Введите название трубы: ");

    cout << "Введите длину трубы(км): ";
    t.dlina_km = Proverka_fl(0.1f);

    cout << "Введите диаметр трубы(мм): ";
    t.diameter_mm = Proverka_in(1);

    cout << "Введите статус трубы (0 - работает, 1 - в ремонте): ";
    t.remont = Proverka_bl();

    return in;
}
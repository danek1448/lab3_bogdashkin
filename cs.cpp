#include "cs.h"
#include "instruments.h"
#include <iostream>
#include <string>

using namespace std;

// инициализируем
Kompressornaya_stantsiya::Kompressornaya_stantsiya() : name(""), kol_cehov(0), vrabote(0), klass_stancii(0.0) {}

Kompressornaya_stantsiya::Kompressornaya_stantsiya(string n, int cehi, int rabotayushchie, double klass)
    : name(n), kol_cehov(cehi), vrabote(rabotayushchie), klass_stancii(klass) {
}

// получаем данные о кс
string Kompressornaya_stantsiya::getName() const { return name; }
int Kompressornaya_stantsiya::getKolCehov() const { return kol_cehov; }
int Kompressornaya_stantsiya::getVrabote() const { return vrabote; }
double Kompressornaya_stantsiya::getKlass() const { return klass_stancii; }

// расчет процента незадействованных цехов
double Kompressornaya_stantsiya::getProcentNeispolzovannyh() const {
    return (kol_cehov - vrabote) * 100.0 / kol_cehov;
}

// запуск одного цеха
void Kompressornaya_stantsiya::zapustit_ceh() {
    if (vrabote < kol_cehov) {
        vrabote++;
        cout << "Цех запущен!" << endl;
    }
    else {
        cout << "Все цехи уже работают!" << endl;
    }
}

// остановка одного цеха
void Kompressornaya_stantsiya::ostanovit_ceh() {
    if (vrabote > 0) {
        vrabote--;
        cout << "Цех остановлен!" << endl;
    }
    else {
        cout << "Все цехи уже остановлены!" << endl;
    }
}

// установка нового названия станции
void Kompressornaya_stantsiya::setName(const string& newName) { name = newName; }


void Kompressornaya_stantsiya::print() const {
    cout << *this;
}

void Kompressornaya_stantsiya::read() {
    cin >> *this;
}

ostream& operator<<(ostream& out, const Kompressornaya_stantsiya& ks) {
    if (ks.name.empty()) {
        out << "Компрессорная станция не добавлена" << endl;
        return out;
    }

    out << "Компрессорная станция " << endl
        << "Название: " << ks.name << endl
        << "Всего цехов: " << ks.kol_cehov << endl
        << "Работающих цехов: " << ks.vrabote << endl
        << "Класс станции: " << ks.klass_stancii << endl
        << endl;
    return out;
}

istream& operator>>(istream& in, Kompressornaya_stantsiya& ks) {
    ks.name = Vvod_stroki("Введите название КС: ");

    cout << "Введите общее количество цехов: ";
    ks.kol_cehov = Proverka_in(1);

    cout << "Введите количество работающих цехов: ";
    ks.vrabote = Proverka_in(0, ks.kol_cehov);

    cout << "Введите класс станции (от 0 до 5): ";
    ks.klass_stancii = Proverka_fl(0.0f, 5.0f);

    return in;
}
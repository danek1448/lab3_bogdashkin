#ifndef TRUBA_H
#define TRUBA_H
#include <string>

using namespace std;

class Truba {
private:
    string name;        // название трубы
    double dlina_km;    // длина трубы в километрах
    int diameter_mm;    // диаметр трубы в миллиметрах
    bool remont;        // статус ремонта 

public:
    // конструкторы
    Truba();
    Truba(string n, double l, int d, bool r);

    string getName() const;     // получить название трубы
    double getDlina() const;    // получить длину трубы
    int getDiameter() const;    // получить диаметр трубы
    bool isRemont() const;      // получить статус ремонта

    void setRemont(bool r);                // установить статус ремонта
    void setName(const string& newName);   // установить новое название трубы

    void print() const;  // вывести инфу о трубе 
    void read();         // ввести данные о трубе

    friend ostream& operator<<(ostream& out, const Truba& t);
    friend istream& operator>>(istream& in, Truba& t);
};

#endif
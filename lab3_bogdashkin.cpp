#include <iostream>
#include <windows.h>
#include "sistema.h"
#include "logirovanie.h"

using namespace std;

void Pokazat_menu() {
    cout << "\nМеню" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить компрессорную станцию" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать компрессорную станцию" << endl;
    cout << "6. Поиск труб" << endl;
    cout << "7. Поиск компрессорных станций" << endl;
    cout << "8. Пакетное редактирование труб" << endl;
    cout << "9. Сохранить данные" << endl;
    cout << "10. Загрузить данные" << endl;
    cout << "11. Соединить КС" << endl;
    cout << "12. Удалить соединение" << endl;
    cout << "13. Показать сеть" << endl;
    cout << "14. Топологическая сортировка" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    Logirovanie::log("Программа запущена");

    Sistema sistema;

    while (true) {
        Pokazat_menu();
        int vibor;
        cin >> vibor;

        switch (vibor) {
        case 1: sistema.Dobavit_trubu(); break;
        case 2: sistema.Dobavit_KS(); break;
        case 3: sistema.Pokazat_vse_obekty(); break;
        case 4: sistema.Redaktirovat_trubu(); break;
        case 5: sistema.Redaktirovat_KS(); break;
        case 6: sistema.Poisk_trub(); break;
        case 7: sistema.Poisk_KS(); break;
        case 8: sistema.Paketnoe_redaktirovanie_trub(); break;
        case 9: sistema.Sohranit_dannye(); break;
        case 10: sistema.Zagruzit_dannye(); break;
        case 11: sistema.Soedinit_KS(); break;
        case 12: sistema.Udalit_soedinenie(); break;
        case 13: sistema.Pokazat_set(); break;
        case 14: sistema.Topologicheskaya_sortirovka(); break;
        case 0:
            Logirovanie::log("Выход из программы");
            cout << "Выход из программы!" << endl;
            return 0;
        default:
            cout << "Неверный выбор!" << endl;
            break;
        }
    }

    return 0;
}
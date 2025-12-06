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
    cout << "6. Удалить трубу" << endl;
    cout << "7. Удалить КС" << endl;
    cout << "8. Поиск труб" << endl;
    cout << "9. Поиск компрессорных станций" << endl;
    cout << "10. Пакетное редактирование труб" << endl;
    cout << "11. Сохранить данные" << endl;
    cout << "12. Загрузить данные" << endl;
    cout << "13. Соединить КС" << endl;
    cout << "14. Показать соединения" << endl;
    cout << "15. Удалить соединение" << endl;
    cout << "16. Топологическая сортировка" << endl;
    cout << "17. Показать газосеть" << endl;
    cout << "18. Расчет максимального потока" << endl;
    cout << "19. Поиск кратчайшего пути" << endl;
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
        case 1: sistema.Dobavit_trubu();
            break;
        case 2: sistema.Dobavit_KS();
            break;
        case 3: sistema.Pokazat_vse_obekty();
            break;
        case 4: sistema.Redaktirovat_trubu();
            break;
        case 5: sistema.Redaktirovat_KS();
            break;
        case 6: sistema.Udalit_trubu();
            break;
        case 7: sistema.Udalit_KS();
            break;
        case 8: sistema.Poisk_trub();
            break;
        case 9: sistema.Poisk_KS();
            break;
        case 10: sistema.Paketnoe_redaktirovanie_trub();
            break;
        case 11: sistema.Sohranit_dannye();
            break;
        case 12: sistema.Zagruzit_dannye();
            break;
        case 13: sistema.Soedinit_KS();
            break;
        case 14: sistema.Pokazat_soedineniya();
            break;
        case 15: sistema.Udalit_soedinenie();
            break;
        case 16: sistema.Topologicheskaya_sortirovka();
            break;
        case 17: sistema.Pokazat_gazoset();
            break;
        case 18: sistema.CalculateMaxFlow();
            break;
        case 19: sistema.FindShortestPath();
            break;
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
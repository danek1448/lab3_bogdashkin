#include "sistema.h"
#include "instruments.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
using namespace std;

Sistema::Sistema() : sleduyushiy_id_truba(1), sleduyushiy_id_ks(1) {}

void Sistema::Dobavit_trubu() {
    Truba novaya_truba;
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cin >> novaya_truba;
    truby[sleduyushiy_id_truba++] = novaya_truba;
    Logirovanie::log("Добавлена труба с ID: " + to_string(sleduyushiy_id_truba - 1));
    cout << "Труба успешно добавлена с ID: " << sleduyushiy_id_truba - 1 << endl;
}

void Sistema::Dobavit_KS() {
    Kompressornaya_stantsiya novaya_ks;
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cin >> novaya_ks;
    kompressornye_stantsii[sleduyushiy_id_ks++] = novaya_ks;
    Logirovanie::log("Добавлена КС с ID: " + to_string(sleduyushiy_id_ks - 1));
    cout << "Компрессорная станция успешно добавлена с ID: " << sleduyushiy_id_ks - 1 << endl;
}

void Sistema::Pokazat_vse_obekty() const {
    cout << "\nТРУБЫ" << endl;
    if (truby.empty()) {
        cout << "Трубы отсутствуют" << endl;
    }
    else {
        for (const auto& pair : truby) {
            cout << "ID: " << pair.first << endl;
            cout << pair.second;
        }
    }

    cout << "\nКОМПРЕССОРНЫЕ СТАНЦИИ" << endl;
    if (kompressornye_stantsii.empty()) {
        cout << "Компрессорные станции отсутствуют" << endl;
    }
    else {
        for (const auto& pair : kompressornye_stantsii) {
            cout << "ID: " << pair.first << endl;
            cout << pair.second;
        }
    }
    Logirovanie::log("Просмотр всех объектов");
}

void Sistema::Redaktirovat_trubu() {
    if (truby.empty()) {
        cout << "Нет доступных труб для редактирования" << endl;
        return;
    }

    cout << "Доступные трубы (ID): ";
    for (const auto& pair : truby) cout << pair.first << " ";
    cout << "\nВведите ID трубы для редактирования: ";

    int id; cin >> id;
    if (truby.find(id) == truby.end()) {
        cout << "Труба с указанным ID не найдена!" << endl;
        return;
    }

    cout << "Текущий статус: " << (truby[id].isRemont() ? "В ремонте" : "Работает") << endl;
    cout << "Введите новый статус (1 - в ремонте, 0 - работает): ";
    int status; cin >> status;
    bool noviy_status = (status == 1);

    truby[id].setRemont(noviy_status);
    Logirovanie::log("Изменен статус трубы ID: " + to_string(id) + " на: " + (noviy_status ? "в ремонте" : "работает"));
    cout << "Статус трубы успешно изменен!" << endl;
}

void Sistema::Redaktirovat_KS() {
    if (kompressornye_stantsii.empty()) {
        cout << "Нет доступных компрессорных станций для редактирования" << endl;
        return;
    }

    cout << "Доступные КС (ID): ";
    for (const auto& pair : kompressornye_stantsii) cout << pair.first << " ";
    cout << "\nВведите ID КС для редактирования: ";

    int id; cin >> id;
    if (kompressornye_stantsii.find(id) == kompressornye_stantsii.end()) {
        cout << "КС с указанным ID не найдена!" << endl;
        return;
    }

    cout << "1. Запустить цех" << endl;
    cout << "2. Остановить цех" << endl;
    cout << "Выберите действие: ";

    int vibor; cin >> vibor;
    if (vibor == 1) {
        kompressornye_stantsii[id].zapustit_ceh();
        Logirovanie::log("Запущен цех на КС ID: " + to_string(id));
    }
    else if (vibor == 2) {
        kompressornye_stantsii[id].ostanovit_ceh();
        Logirovanie::log("Остановлен цех на КС ID: " + to_string(id));
    }
    else {
        cout << "Неверный выбор!" << endl;
    }
}

void Sistema::Poisk_trub() {
    if (truby.empty()) {
        cout << "Нет доступных труб для поиска" << endl;
        return;
    }

    cout << "Критерии поиска труб:" << endl;
    cout << "1. По названию" << endl;
    cout << "2. По статусу ремонта" << endl;
    cout << "Выберите критерий: ";

    int kriteriy; cin >> kriteriy;
    vector<int> naydennye_id;

    if (kriteriy == 1) {
        cout << "Введите название для поиска: ";
        cin.ignore();
        string iskomoe_nazvanie;
        getline(cin, iskomoe_nazvanie);

        for (const auto& pair : truby) {
            if (pair.second.getName().find(iskomoe_nazvanie) != string::npos) {
                naydennye_id.push_back(pair.first);
            }
        }
    }
    else if (kriteriy == 2) {
        cout << "Введите статус ремонта (1 - в ремонте, 0 - работает): ";
        int status; cin >> status;
        bool iskomy_status = (status == 1);

        for (const auto& pair : truby) {
            if (pair.second.isRemont() == iskomy_status) {
                naydennye_id.push_back(pair.first);
            }
        }
    }
    else {
        cout << "Неверный критерий!" << endl;
        return;
    }

    if (naydennye_id.empty()) {
        cout << "Трубы по заданному критерию не найдены" << endl;
        return;
    }

    cout << "\nНайденные трубы:" << endl;
    for (int id : naydennye_id) {
        cout << "ID: " << id << endl;
        cout << truby[id];
    }
    Logirovanie::log("Выполнен поиск труб. Найдено: " + to_string(naydennye_id.size()) + " объектов");
}

void Sistema::Poisk_KS() {
    if (kompressornye_stantsii.empty()) {
        cout << "Нет доступных компрессорных станций для поиска" << endl;
        return;
    }

    cout << "Критерии поиска компрессорных станций:" << endl;
    cout << "1. По названию" << endl;
    cout << "2. По проценту незадействованных цехов" << endl;
    cout << "Выберите критерий: ";

    int kriteriy; cin >> kriteriy;
    vector<int> naydennye_id;

    if (kriteriy == 1) {
        cout << "Введите название для поиска: ";
        cin.ignore();
        string iskomoe_nazvanie;
        getline(cin, iskomoe_nazvanie);

        for (const auto& pair : kompressornye_stantsii) {
            if (pair.second.getName().find(iskomoe_nazvanie) != string::npos) {
                naydennye_id.push_back(pair.first);
            }
        }
    }
    else if (kriteriy == 2) {
        cout << "Введите минимальный процент незадействованных цехов: ";
        double min_procent; cin >> min_procent;

        for (const auto& pair : kompressornye_stantsii) {
            if (pair.second.getProcentNeispolzovannyh() >= min_procent) {
                naydennye_id.push_back(pair.first);
            }
        }
    }
    else {
        cout << "Неверный критерий!" << endl;
        return;
    }

    if (naydennye_id.empty()) {
        cout << "Компрессорные станции по заданному критерию не найдены" << endl;
        return;
    }

    cout << "\nНайденные компрессорные станции:" << endl;
    for (int id : naydennye_id) {
        cout << "ID: " << id << endl;
        cout << kompressornye_stantsii[id];
        cout << "Процент незадействованных цехов: " << kompressornye_stantsii[id].getProcentNeispolzovannyh() << "%" << endl;
    }
    Logirovanie::log("Выполнен поиск КС. Найдено: " + to_string(naydennye_id.size()) + " объектов");
}

void Sistema::Paketnoe_redaktirovanie_trub() {
    if (truby.empty()) {
        cout << "Нет доступных труб для пакетного редактирования" << endl;
        return;
    }

    vector<int> vozmozhnye_id;
    cout << "Выберите трубы для пакетного редактирования:" << endl;
    cout << "1. Все трубы" << endl;
    cout << "2. Только трубы в ремонте" << endl;
    cout << "3. Только работающие трубы" << endl;
    cout << "Выберите опцию: ";

    int opciya; cin >> opciya;

    for (const auto& pair : truby) {
        if (opciya == 1 || (opciya == 2 && pair.second.isRemont()) || (opciya == 3 && !pair.second.isRemont())) {
            vozmozhnye_id.push_back(pair.first);
        }
    }

    if (vozmozhnye_id.empty()) {
        cout << "Нет труб, соответствующих выбранному критерию" << endl;
        return;
    }

    vector<int> vybrannye_id;
    cout << "\nДоступные трубы для редактирования:" << endl;
    for (int id : vozmozhnye_id) {
        cout << "ID: " << id << " - " << truby[id].getName()
            << " (" << (truby[id].isRemont() ? "в ремонте" : "работает") << ")" << endl;
    }

    cout << "Введите ID труб для редактирования (по одному, -1 для завершения): " << endl;
    while (true) {
        int id; cin >> id;
        if (id == -1) break;

        if (find(vozmozhnye_id.begin(), vozmozhnye_id.end(), id) != vozmozhnye_id.end()) {
            vybrannye_id.push_back(id);
            cout << "Труба " << id << " добавлена в список редактирования" << endl;
        }
        else {
            cout << "Труба с ID " << id << " не найдена в доступном списке!" << endl;
        }
    }

    if (vybrannye_id.empty()) {
        cout << "Не выбрано ни одной трубы для редактирования" << endl;
        return;
    }

    cout << "\nДействия:" << endl;
    cout << "1. Изменить статус ремонта" << endl;
    cout << "2. Удалить выбранные трубы" << endl;
    cout << "Выберите действие: ";

    int deystvie; cin >> deystvie;

    if (deystvie == 1) {
        cout << "Введите новый статус (1 - в ремонте, 0 - работает): ";
        int status; cin >> status;
        bool noviy_status = (status == 1);

        for (int id : vybrannye_id) {
            truby[id].setRemont(noviy_status);
        }
        Logirovanie::log("Пакетное изменение статуса " + to_string(vybrannye_id.size()) + " труб на: " + (noviy_status ? "в ремонте" : "работает"));
        cout << "Статус " << vybrannye_id.size() << " труб успешно изменен!" << endl;
    }
    else if (deystvie == 2) {
        for (int id : vybrannye_id) {
            truby.erase(id);
        }
        Logirovanie::log("Пакетное удаление " + to_string(vybrannye_id.size()) + " труб");
        cout << "Удалено " << vybrannye_id.size() << " труб!" << endl;
    }
    else {
        cout << "Неверное действие!" << endl;
    }
}

void Sistema::Sohranit_dannye() {
    cout << "Введите имя файла для сохранения: ";
    cin.ignore();
    string imya_faila;
    getline(cin, imya_faila);

    vector<Truba> vektor_trub;
    for (const auto& pair : truby) vektor_trub.push_back(pair.second);

    vector<Kompressornaya_stantsiya> vektor_ks;
    for (const auto& pair : kompressornye_stantsii) vektor_ks.push_back(pair.second);

    Logirovanie::log("Сохранение данных в файл: " + imya_faila);
}

void Sistema::Zagruzit_dannye() {
    cout << "Введите имя файла для загрузки: ";
    cin.ignore();
    string imya_faila;
    getline(cin, imya_faila);

    vector<Truba> vektor_trub;
    vector<Kompressornaya_stantsiya> vektor_ks;

    truby.clear();
    kompressornye_stantsii.clear();
    sleduyushiy_id_truba = 1;
    sleduyushiy_id_ks = 1;

    for (const auto& truba : vektor_trub) truby[sleduyushiy_id_truba++] = truba;
    for (const auto& ks : vektor_ks) kompressornye_stantsii[sleduyushiy_id_ks++] = ks;

    Logirovanie::log("Загрузка данных из файла: " + imya_faila);
}

void Sistema::Soedinit_KS() {
    if (kompressornye_stantsii.size() < 2) {
        cout << "Для соединения нужно как минимум 2 КС!" << endl;
        return;
    }

    cout << "Доступные КС (ID): ";
    for (const auto& pair : kompressornye_stantsii)
        cout << pair.first << " ";
    cout << endl;

    cout << "Введите ID КС входа: ";
    int input_id = Proverka_in(1);
    if (kompressornye_stantsii.find(input_id) == kompressornye_stantsii.end()) {
        cout << "КС с ID " << input_id << " не найдена!" << endl;
        return;
    }

    cout << "Введите ID КС выхода: ";
    int output_id = Proverka_in(1);
    if (kompressornye_stantsii.find(output_id) == kompressornye_stantsii.end()) {
        cout << "КС с ID " << output_id << " не найдена!" << endl;
        return;
    }

    cout << "Введите диаметр трубы (500, 700, 1000, 1400): ";
    int diameter = Proverka_in(500);

    int pipe_id = gazoset.find_available_pipe(truby, diameter);

    if (pipe_id == -1) {
        cout << "Свободной трубы с диаметром " << diameter << " мм не найдено." << endl;
        cout << "Создать новую трубу? (1 - да, 0 - нет): ";
        int choice = Proverka_in(0, 1);

        if (choice == 1) {
            Dobavit_trubu();
            pipe_id = sleduyushiy_id_truba - 1; 
        }
        else {
            return;
        }
    }

    if (gazoset.add_connection(pipe_id, input_id, output_id, diameter)) {
        Logirovanie::log("Создано соединение: КС " + to_string(input_id) +
            " -> КС " + to_string(output_id) + " (труба " +
            to_string(pipe_id) + ")");
    }
}

void Sistema::Udalit_soedinenie() {
    if (gazoset.get_connections().empty()) {
        cout << "Нет соединений для удаления!" << endl;
        return;
    }

    cout << "Введите ID КС входа: ";
    int input_id = Proverka_in(1);
    cout << "Введите ID КС выхода: ";
    int output_id = Proverka_in(1);

    if (gazoset.remove_connection(input_id, output_id)) {
        Logirovanie::log("Удалено соединение: КС " + to_string(input_id) +
            " -> КС " + to_string(output_id));
    }
}

void Sistema::Pokazat_set() {
    const vector<Connection>& connections = gazoset.get_connections();

    if (connections.empty()) {
        cout << "Газосеть не содержит соединений!" << endl;
        return;
    }

    cout << "\nГАЗОСЕТЬ" << endl;
    cout << "Всего соединений: " << connections.size() << endl;

    for (auto it = connections.begin(); it != connections.end(); ++it) {
        const Connection& conn = *it;
        cout << "КС " << conn.input_cs_id << " -> КС " << conn.output_cs_id
            << " (труба " << conn.pipe_id << ", диаметр " << conn.diameter << " мм)" << endl;
    }
}

void Sistema::Topologicheskaya_sortirovka() {
    if (kompressornye_stantsii.empty()) {
        cout << "Нет компрессорных станций для сорта!" << endl;
        return;
    }

    vector<int> sorted = gazoset.topological_sort(kompressornye_stantsii);

    if (sorted.empty()) {
        cout << "Топологическая сорт невозможна из-за циклов в графе!" << endl;
        return;
    }

    cout << "\nТопологическая сортировка КС:" << endl;

    for (size_t i = 0; i < sorted.size(); ++i) {
        int cs_id = sorted[i];
        const auto& ks = kompressornye_stantsii.at(cs_id);
        cout << i + 1 << ". КС " << cs_id << " - " << ks.getName() << endl;
    }

    Logirovanie::log("Выполнена топологическая сорт " +
        to_string(sorted.size()) + " КС");
}
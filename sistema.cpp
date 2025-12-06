#include "sistema.h"
#include "instruments.h"
#include "soedinenie.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <fstream>
#include <unordered_set>

using namespace std;

Sistema::Sistema() : sleduyushiy_id_truba(1), sleduyushiy_id_ks(1) {}

void Sistema::Dobavit_trubu() {
    Truba novaya_truba;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите данные для новой трубы:" << endl;
    cin >> novaya_truba;
    gazoset.addPipe(novaya_truba, sleduyushiy_id_truba);
    Logirovanie::log("Добавлена труба с ID: " + to_string(sleduyushiy_id_truba));
    cout << "Труба успешно добавлена с ID: " << sleduyushiy_id_truba++ << endl;
}

void Sistema::Dobavit_KS() {
    Kompressornaya_stantsiya novaya_ks;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Введите данные для новой КС:" << endl;
    cin >> novaya_ks;
    gazoset.addCS(novaya_ks, sleduyushiy_id_ks);
    Logirovanie::log("Добавлена КС с ID: " + to_string(sleduyushiy_id_ks));
    cout << "Компрессорная станция успешно добавлена с ID: " << sleduyushiy_id_ks++ << endl;
}

void Sistema::Pokazat_vse_obekty() const {
    cout << gazoset;
    Logirovanie::log("Просмотр всех объектов");
}

void Sistema::Pokazat_gazoset() const {
    cout << gazoset;
}

void Sistema::Redaktirovat_trubu() {
    auto& pipes = gazoset.getPipes();
    if (pipes.empty()) {
        cout << "Нет доступных труб для редактирования" << endl;
        return;
    }

    cout << "Доступные трубы (ID): ";
    for (const auto& pair : pipes) cout << pair.first << " ";
    cout << "\nВведите ID трубы для редактирования: ";

    int id = Proverka_in(1);
    if (pipes.find(id) == pipes.end()) {
        cout << "Труба с указанным ID не найдена!" << endl;
        return;
    }

    if (Link::IsPipeUsed(id, gazoset.getConnections())) {
        cout << "Внимание: эта труба используется в соединении!" << endl;
        cout << "Вы можете изменить только статус ремонта." << endl;
    }

    cout << "Текущий статус: " << (pipes[id].isRemont() ? "В ремонте" : "Работает") << endl;
    cout << "Введите новый статус (1 - в ремонте, 0 - работает): ";
    int status = Proverka_in(0, 1);

    pipes[id].setRemont(status == 1);
    Logirovanie::log("Изменен статус трубы ID: " + to_string(id) + " на: " + (status == 1 ? "в ремонте" : "работает"));
    cout << "Статус трубы успешно изменен!" << endl;
}

void Sistema::Redaktirovat_KS() {
    auto& cs_dict = gazoset.getCS();
    if (cs_dict.empty()) {
        cout << "Нет доступных компрессорных станций для редактирования" << endl;
        return;
    }

    cout << "Доступные КС (ID): ";
    for (const auto& pair : cs_dict) cout << pair.first << " ";
    cout << "\nВведите ID КС для редактирования: ";

    int id = Proverka_in(1);
    if (cs_dict.find(id) == cs_dict.end()) {
        cout << "КС с указанным ID не найдена!" << endl;
        return;
    }

    cout << "Текущее состояние КС " << id << ":" << endl;
    cout << cs_dict[id];

    cout << "1. Запустить цех" << endl;
    cout << "2. Остановить цех" << endl;
    cout << "Выберите действие: ";
    int vibor = Proverka_in(1, 2);

    if (vibor == 1) {
        cs_dict[id].zapustit_ceh();
        Logirovanie::log("Запущен цех на КС ID: " + to_string(id));
        cout << "Цех запущен! Теперь работает " << cs_dict[id].getVrabote() << " цехов" << endl;
    }
    else if (vibor == 2) {
        cs_dict[id].ostanovit_ceh();
        Logirovanie::log("Остановлен цех на КС ID: " + to_string(id));
        cout << "Цех остановлен! Теперь работает " << cs_dict[id].getVrabote() << " цехов" << endl;
    }
}

void Sistema::Udalit_trubu() {
    auto& pipes = gazoset.getPipes();
    if (pipes.empty()) {
        cout << "Нет доступных труб для удаления" << endl;
        return;
    }

    cout << "Доступные трубы (ID): ";
    for (const auto& pair : pipes) cout << pair.first << " ";
    cout << "\nВведите ID трубы для удаления: ";

    int id = Proverka_in(1);

    if (Link::IsPipeUsed(id, gazoset.getConnections())) {
        cout << "Ошибка: труба используется в соединении!" << endl;
        cout << "Сначала удалите соединение, затем удалите трубу." << endl;
        return;
    }

    gazoset.delete_pipe(id);
    Logirovanie::log("Удалена труба ID: " + to_string(id));
}

void Sistema::Udalit_KS() {
    auto& cs_dict = gazoset.getCS();
    if (cs_dict.empty()) {
        cout << "Нет доступных КС для удаления" << endl;
        return;
    }

    cout << "Доступные КС (ID): ";
    for (const auto& pair : cs_dict) cout << pair.first << " ";
    cout << "\nВведите ID КС для удаления: ";

    int id = Proverka_in(1);
    gazoset.delete_CS(id);
    Logirovanie::log("Удалена КС ID: " + to_string(id));
}

void Sistema::Poisk_trub() {
    gazoset.PipesFiltering();
    Logirovanie::log("Выполнен поиск труб");
}

void Sistema::Poisk_KS() {
    gazoset.CSFiltering();
    Logirovanie::log("Выполнен поиск КС");
}

void Sistema::Paketnoe_redaktirovanie_trub() {
    auto& pipes = gazoset.getPipes();
    if (pipes.empty()) {
        cout << "Нет доступных труб для пакетного редактирования" << endl;
        return;
    }

    cout << "Выберите трубы для пакетного редактирования:" << endl;
    cout << "1. Все трубы" << endl;
    cout << "2. Только трубы в ремонте" << endl;
    cout << "3. Только работающие трубы" << endl;
    cout << "Выберите опцию: ";

    int opciya = Proverka_in(1, 3);
    unordered_set<int> vybrannye_id;

    for (const auto& pair : pipes) {
        if (opciya == 1 ||
            (opciya == 2 && pair.second.isRemont()) ||
            (opciya == 3 && !pair.second.isRemont())) {
            vybrannye_id.insert(pair.first);
        }
    }

    if (vybrannye_id.empty()) {
        cout << "Нет труб, соответствующих выбранному критерию" << endl;
        return;
    }

    int used_pipes_count = 0;
    for (int id : vybrannye_id) {
        if (Link::IsPipeUsed(id, gazoset.getConnections())) {
            used_pipes_count++;
        }
    }

    if (used_pipes_count > 0) {
        cout << "Внимание: " << used_pipes_count << " труб используются в соединениях." << endl;
        cout << "Их статус ремонта будет изменен, но соединения останутся активными." << endl;
    }

    cout << "Выбранные трубы (ID): ";
    for (int id : vybrannye_id) cout << id << " ";
    cout << endl;

    cout << "Введите новый статус ремонта (1 - в ремонте, 0 - работает): ";
    int status = Proverka_in(0, 1);

    gazoset.PacketEditPipe(vybrannye_id, status == 1);
    Logirovanie::log("Пакетное изменение статуса " + to_string(vybrannye_id.size()) + " труб");
}

void Sistema::Soedinit_KS() {
    Link new_connection;
    if (new_connection.CreateLink(gazoset.getPipes(), gazoset.getCS(), gazoset.getConnections())) {
        gazoset.addConnection(new_connection);
        Logirovanie::log("Создано соединение: КС " + to_string(new_connection.CS_inlet) +
            " -> КС " + to_string(new_connection.CS_outlet) +
            " (труба " + to_string(new_connection.pipeline) + ")");
        cout << "Соединение успешно добавлено в газосеть!" << endl;
    }
    else {
        cout << "Создание соединения отменено или завершилось ошибкой." << endl;
    }
}

void Sistema::Pokazat_soedineniya() const {
    const auto& connections = gazoset.getConnections();
    if (connections.empty()) {
        cout << "Нет активных соединений." << endl;
    }
    else {
        cout << "\nАКТИВНЫЕ СОЕДИНЕНИЯ" << endl;
        for (const auto& conn : connections) {
            cout << "КС " << conn.CS_inlet << " -> КС " << conn.CS_outlet
                << " (труба " << conn.pipeline << ")" << endl;
        }
        cout << "Всего соединений: " << connections.size() << endl;
    }
}

void Sistema::Udalit_soedinenie() {
    auto& connections = gazoset.getConnections();
    if (connections.empty()) {
        cout << "Нет соединений для удаления!" << endl;
        return;
    }

    cout << "Текущие соединения:" << endl;
    Pokazat_soedineniya();

    cout << "Введите ID входной КС: ";
    int input_id = Proverka_in(0);
    cout << "Введите ID выходной КС: ";
    int output_id = Proverka_in(0);

    gazoset.delete_connection(input_id, output_id);
    Logirovanie::log("Удалено соединение: КС " + to_string(input_id) +
        " -> КС " + to_string(output_id));
}

void Sistema::Topologicheskaya_sortirovka() {
    const auto& cs_dict = gazoset.getCS();
    const auto& connections = gazoset.getConnections();

    if (cs_dict.empty()) {
        cout << "Нет компрессорных станций для сортировки!" << endl;
        return;
    }

    if (connections.empty()) {
        cout << "Нет соединений для построения графа!" << endl;
        return;
    }

    cout << "\nТОПОЛОГИЧЕСКАЯ СОРТИРОВКА" << endl;
    TopSort(cs_dict, connections);
    Logirovanie::log("Выполнена топологическая сортировка");
}


void Sistema::CalculateMaxFlow() {
    const auto& connections = gazoset.getConnections();
    const auto& pipes = gazoset.getPipes();
    const auto& cs_dict = gazoset.getCS();

    if (connections.empty()) {
        cout << "Нет соединений для расчета потока!" << endl;
        return;
    }

    cout << "\nРАСЧЕТ МАКСИМАЛЬНОГО ПОТОКА" << endl;

    if (cs_dict.empty()) {
        cout << "Нет компрессорных станций!" << endl;
        return;
    }

    cout << "Доступные КС (ID): ";
    for (const auto& cs : cs_dict) {
        cout << cs.first << " ";
    }
    cout << endl;

    cout << "Введите ID начальной КС (источник): ";
    int source = Proverka_in(0);

    cout << "Введите ID конечной КС (сток): ";
    int sink = Proverka_in(0);

    if (cs_dict.find(source) == cs_dict.end()) {
        cout << "Ошибка: КС " << source << " не существует!" << endl;
        return;
    }

    if (cs_dict.find(sink) == cs_dict.end()) {
        cout << "Ошибка: КС " << sink << " не существует!" << endl;
        return;
    }

    if (source == sink) {
        cout << "Ошибка: источник и сток не могут быть одинаковыми!" << endl;
        return;
    }

    double max_flow = CalculateMaxxFlow(cs_dict, connections, pipes, source, sink);

    cout << "\nРЕЗУЛЬТАТЫ" << endl;
    cout << "Источник: КС " << source << endl;
    cout << "Сток: КС " << sink << endl;
    cout << "Максимальный поток: " << max_flow << " единиц" << endl;

    if (max_flow == 0.0) {
        cout << "\nПримечание: поток равен 0." << endl;
        cout << "Возможные причины:" << endl;
        cout << "1. Нет пути между указанными КС" << endl;
        cout << "2. Все трубы на пути находятся в ремонте" << endl;
        cout << "3. Нет соединений между этими КС" << endl;
    }
    else {
        cout << "\nРасчет выполнен успешно!" << endl;

        cout << "\nАктивные соединения в сети:" << endl;
        int active_connections = 0;
        for (const auto& conn : connections) {
            auto pipe_it = pipes.find(conn.pipeline);
            if (pipe_it != pipes.end()) {
                const Truba& pipe = pipe_it->second;
                double capacity = CalculatePipeCapacity(pipe);
                if (capacity > 0) {
                    cout << "  КС " << conn.CS_inlet << " -> КС " << conn.CS_outlet
                        << " (Производительность: " << capacity << " ед.)" << endl;
                    active_connections++;
                }
            }
        }
        if (active_connections == 0) {
            cout << "  Нет активных соединений" << endl;
        }
    }

    Logirovanie::log("Расчет максимального потока: КС " + to_string(source) +
        " -> КС " + to_string(sink) + " = " + to_string(max_flow) + " единиц");
}

void Sistema::FindShortestPath() {
    const auto& connections = gazoset.getConnections();
    const auto& pipes = gazoset.getPipes();
    const auto& cs_dict = gazoset.getCS();

    if (connections.empty()) {
        cout << "Нет соединений для поиска пути!" << endl;
        return;
    }

    if (cs_dict.size() < 2) {
        cout << "Для поиска пути нужно как минимум 2 КС!" << endl;
        return;
    }

    cout << "\nПОИСК КРАТЧАЙШЕГО ПУТИ" << endl;

    cout << "Доступные КС (ID): ";
    for (const auto& cs : cs_dict) {
        cout << cs.first << " ";
    }
    cout << endl;

    cout << "Введите ID начальной КС: ";
    int start = Proverka_in(0);

    cout << "Введите ID конечной КС: ";
    int end = Proverka_in(0);

    if (cs_dict.find(start) == cs_dict.end()) {
        cout << "Ошибка: КС " << start << " не существует!" << endl;
        return;
    }

    if (cs_dict.find(end) == cs_dict.end()) {
        cout << "Ошибка: КС " << end << " не существует!" << endl;
        return;
    }

    if (start == end) {
        cout << "Начальная и конечная КС одинаковы!" << endl;
        cout << "Путь: КС " << start << endl;
        cout << "Длина пути: 0 км" << endl;
        return;
    }

    vector<int> path = FindShortestPathDijkstra(cs_dict, connections, pipes, start, end);

    cout << "\nРЕЗУЛЬТАТЫ" << endl;

    if (path.empty()) {
        cout << "Путь между КС " << start << " и КС " << end << " не найден!" << endl;
        cout << "\nВозможные причины:" << endl;
        cout << "1. Нет соединений между этими КС" << endl;
        cout << "2. Все трубы на возможных путях находятся в ремонте" << endl;

        cout << "\nДоступные соединения из КС " << start << ":" << endl;
        bool has_connections = false;
        for (const auto& conn : connections) {
            if (conn.CS_inlet == start) {
                has_connections = true;
                auto pipe_it = pipes.find(conn.pipeline);
                if (pipe_it != pipes.end()) {
                    const Truba& pipe = pipe_it->second;
                    cout << "  -> КС " << conn.CS_outlet
                        << " (Длина: " << pipe.getDlina() << " км, Статус: "
                        << (pipe.isRemont() ? "В ремонте" : "Работает") << ")" << endl;
                }
            }
        }
        if (!has_connections) {
            cout << "  Нет исходящих соединений" << endl;
        }
    }
    else {
        cout << "Найден кратчайший путь: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << "КС " << path[i];
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;

        double total_length = 0.0;
        cout << "\nДетали пути:" << endl;

        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i];
            int to = path[i + 1];

            bool found = false;
            for (const auto& conn : connections) {
                if (conn.CS_inlet == from && conn.CS_outlet == to) {
                    auto pipe_it = pipes.find(conn.pipeline);
                    if (pipe_it != pipes.end()) {
                        const Truba& pipe = pipe_it->second;
                        double length = pipe.getDlina();
                        total_length += length;

                        cout << "  КС " << from << " -> КС " << to
                            << " (Труба: " << pipe.getName()
                            << ", Длина: " << length << " км"
                            << ", Диаметр: " << pipe.getDiameter() << " мм"
                            << ", Статус: " << (pipe.isRemont() ? "В ремонте" : "Работает")
                            << ")" << endl;

                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                cout << "  КС " << from << " -> КС " << to << ": соединение не найдено" << endl;
            }
        }

        cout << "\nИтоговая информация:" << endl;
        cout << "  Общая длина пути: " << total_length << " км" << endl;
        cout << "  Количество КС на пути: " << path.size() << endl;
        cout << "  Количество переходов: " << (path.size() - 1) << endl;
    }

    Logirovanie::log("Поиск кратчайшего пути: КС " + to_string(start) +
        " -> КС " + to_string(end) +
        (path.empty() ? " (путь не найден)" : " (путь найден)"));
}

void Sistema::Sohranit_dannye() {
    cout << "Введите имя файла для сохранения: ";
    cin.ignore();
    string imya_faila;
    getline(cin, imya_faila);

    ofstream outfile(imya_faila);
    if (!outfile.is_open()) {
        cout << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    const auto& pipes = gazoset.getPipes();
    for (const auto& pair : pipes) {
        outfile << "PIPE " << pair.first << " "
            << pair.second.getName() << " "
            << pair.second.getDlina() << " "
            << pair.second.getDiameter() << " "
            << (pair.second.isRemont() ? 1 : 0) << endl;
    }

    const auto& cs_dict = gazoset.getCS();
    for (const auto& pair : cs_dict) {
        outfile << "CS " << pair.first << " "
            << pair.second.getName() << " "
            << pair.second.getKolCehov() << " "
            << pair.second.getVrabote() << " "
            << pair.second.getKlass() << endl;
    }

    const auto& connections = gazoset.getConnections();
    for (const auto& conn : connections) {
        outfile << "LINK " << conn.pipeline << " "
            << conn.CS_inlet << " " << conn.CS_outlet << endl;
    }

    outfile.close();
    Logirovanie::log("Сохранение данных в файл: " + imya_faila);
    cout << "Данные успешно сохранены в файл " << imya_faila << endl;
}

void Sistema::Zagruzit_dannye() {
    cout << "Введите имя файла для загрузки: ";
    cin.ignore();
    string imya_faila;
    getline(cin, imya_faila);

    ifstream infile(imya_faila);
    if (!infile.is_open()) {
        cout << "Файл не существует или недоступен!" << endl;
        return;
    }

    gazoset = Gazoset();
    sleduyushiy_id_truba = 1;
    sleduyushiy_id_ks = 1;

    string tip;
    while (infile >> tip) {
        if (tip == "PIPE") {
            int id;
            string name;
            double dlina;
            int diameter;
            int remont;

            infile >> id >> name >> dlina >> diameter >> remont;
            Truba truba(name, dlina, diameter, remont == 1);
            gazoset.addPipe(truba, id);
            if (id >= sleduyushiy_id_truba) {
                sleduyushiy_id_truba = id + 1;
            }

        }
        else if (tip == "CS") {
            int id;
            string name;
            int kol_cehov, vrabote;
            double klass;

            infile >> id >> name >> kol_cehov >> vrabote >> klass;
            Kompressornaya_stantsiya ks(name, kol_cehov, vrabote, klass);
            gazoset.addCS(ks, id);
            if (id >= sleduyushiy_id_ks) {
                sleduyushiy_id_ks = id + 1;
            }

        }
        else if (tip == "LINK") {
            int pipe_id, cs_in, cs_out;
            infile >> pipe_id >> cs_in >> cs_out;
            Link conn;
            conn.pipeline = pipe_id;
            conn.CS_inlet = cs_in;
            conn.CS_outlet = cs_out;
            gazoset.addConnection(conn);
        }
    }

    infile.close();
    Logirovanie::log("Загрузка данных из файла: " + imya_faila);
    cout << "Данные успешно загружены из файла " << imya_faila << endl;
    cout << "Загружено: " << gazoset.getPipes().size() << " труб, "
        << gazoset.getCS().size() << " КС, "
        << gazoset.getConnections().size() << " соединений" << endl;
}
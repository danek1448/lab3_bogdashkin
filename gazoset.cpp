#include "gazoset.h"      
#include "instruments.h"  
#include <iostream>       
#include <algorithm>      

using namespace std;

void Gazoset::delete_pipe(int id) {
    if (Pipeline.find(id) != Pipeline.end()) {
        deletePipeConnection(id, connections);
        Pipeline.erase(id);
        cout << "Труба " << id << " удалена!" << endl;
    }
    else {
        cout << "Труба с ID " << id << " не найдена!" << endl;
    }
}

void Gazoset::delete_CS(int id) {
    if (CS_system.find(id) != CS_system.end()) {
        deleteCSConnections(id, connections);
        CS_system.erase(id);
        cout << "КС " << id << " удалена!" << endl;
    }
    else {
        cout << "КС с ID " << id << " не найдена!" << endl;
    }
}

void Gazoset::delete_connection(int input_cs, int output_cs) {
    bool found = false;
    for (auto it = connections.begin(); it != connections.end(); ) {
        if (it->CS_inlet == input_cs && it->CS_outlet == output_cs) {
            it = connections.erase(it);
            found = true;
        }
        else {
            ++it;
        }
    }

    if (found) {
        cout << "Соединение КС " << input_cs << " -> КС " << output_cs << " удалено!" << endl;
    }
    else {
        cout << "Соединение не найдено!" << endl;
    }
}

void Gazoset::PipesFiltering() {
    cout << "Выберите критерий фильтрации труб:" << endl
        << "1. По названию" << endl
        << "2. По статусу ремонта" << endl;

    int choice = GetNumberInRange(1, 2);
    switch (choice) {
    case 1: {
        cout << "Введите название для поиска: ";
        string name = GetPipeName();
        auto filtered = FindPipesByFilter(Pipeline, CheckByName, name);
        cout << "Найденные трубы:" << endl;
        for (const auto& pipe : filtered) {
            cout << "ID: " << pipe.first << " - " << pipe.second.getName()
                << " (диаметр: " << pipe.second.getDiameter() << " мм, "
                << (pipe.second.isRemont() ? "в ремонте" : "работает") << ")" << endl;
        }
        break;
    }
    case 2: {
        cout << "Введите статус ремонта (1 - в ремонте, 0 - работает): ";
        bool status = GetNumberInRange(0, 1);
        auto filtered = FindPipesByFilter(Pipeline, CheckByMaintenanceStatus, status);
        cout << "Найденные трубы:" << endl;
        for (const auto& pipe : filtered) {
            cout << "ID: " << pipe.first << " - " << pipe.second.getName()
                << " (" << (pipe.second.isRemont() ? "в ремонте" : "работает") << ")" << endl;
        }
        break;
    }
    }
}

void Gazoset::CSFiltering() {
    cout << "Выберите критерий фильтрации КС:" << endl
        << "1. По названию" << endl
        << "2. По проценту незадействованных цехов" << endl;

    int choice = GetNumberInRange(1, 2);
    switch (choice) {
    case 1: {
        cout << "Введите название для поиска: ";
        string name = GetPipeName();
        auto filtered = FindCSByFilter(CS_system, CheckByName, name);
        cout << "Найденные КС:" << endl;
        for (const auto& cs : filtered) {
            cout << "ID: " << cs.first << " - " << cs.second.getName()
                << " (цехов: " << cs.second.getKolCehov() << ", работает: "
                << cs.second.getVrabote() << ")" << endl;
        }
        break;
    }
    case 2: {
        cout << "Введите процент незадействованных цехов: ";
        int percent = GetNumberInRange(0, 100);
        cout << "Выберите тип сравнения:" << endl
            << "1. Равно" << endl
            << "2. Больше или равно" << endl
            << "3. Меньше или равно" << endl;

        int comp_choice = GetNumberInRange(1, 3);
        unordered_map<int, Kompressornaya_stantsiya> filtered;

        switch (comp_choice) {
        case 1:
            filtered = FindCSByFilter(CS_system, CheckByEqual, percent);
            break;
        case 2:
            filtered = FindCSByFilter(CS_system, CheckByMore, percent);
            break;
        case 3:
            filtered = FindCSByFilter(CS_system, CheckByLess, percent);
            break;
        }

        cout << "Найденные КС:" << endl;
        for (const auto& cs : filtered) {
            cout << "ID: " << cs.first << " - " << cs.second.getName()
                << " (" << cs.second.getProcentNeispolzovannyh() << "% незадействованных)" << endl;
        }
        break;
    }
    }
}

void Gazoset::PacketEditPipe(const unordered_set<int>& IDs, bool state) {
    int count = 0;
    for (int id : IDs) {
        if (Pipeline.find(id) != Pipeline.end()) {
            Pipeline[id].setRemont(state);
            count++;
        }
    }
    cout << "Статус ремонта обновлен для " << count << " труб" << endl;
}

void Gazoset::PacketEditCS(const unordered_set<int>& IDs) {
    int count = 0;
    for (int id : IDs) {
        if (CS_system.find(id) != CS_system.end()) {
            cout << "КС " << id << ": " << CS_system[id].getName()
                << " (цехов: " << CS_system[id].getKolCehov()
                << ", работает: " << CS_system[id].getVrabote() << ")" << endl;
            count++;
        }
    }
    cout << "Обработано " << count << " КС" << endl;
}

ostream& operator << (ostream& out, const Gazoset& gts) {
    out << "\nГАЗОТРАНСПОРТНАЯ СИСТЕМА" << endl;

    out << "\nТрубы (" << gts.Pipeline.size() << "):" << endl;
    if (gts.Pipeline.empty()) {
        out << "Трубы отсутствуют" << endl;
    }
    else {
        for (const auto& pipe : gts.Pipeline) {
            out << "ID: " << pipe.first << " - " << pipe.second.getName()
                << " (длина: " << pipe.second.getDlina() << " км, диаметр: "
                << pipe.second.getDiameter() << " мм, "
                << (pipe.second.isRemont() ? "в ремонте" : "работает") << ")" << endl;
        }
    }

    out << "\nКС (" << gts.CS_system.size() << "):" << endl;
    if (gts.CS_system.empty()) {
        out << "КС отсутствуют" << endl;
    }
    else {
        for (const auto& cs : gts.CS_system) {
            out << "ID: " << cs.first << " - " << cs.second.getName()
                << " (цехов: " << cs.second.getKolCehov() << ", работает: "
                << cs.second.getVrabote() << ", класс: " << cs.second.getKlass() << ")" << endl;
        }
    }

    out << "\nСоединения (" << gts.connections.size() << "):" << endl;
    if (gts.connections.empty()) {
        out << "Соединения отсутствуют" << endl;
    }
    else {
        for (const auto& conn : gts.connections) {
            out << "КС " << conn.CS_inlet << " -> КС " << conn.CS_outlet
                << " (труба " << conn.pipeline << ")" << endl;
        }
    }

    return out;
}
#include "soedinenie.h"
#include "instruments.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <limits>

using namespace std;

bool CheckByDiameter(const Truba& truba, int parameter) {
    return truba.getDiameter() == parameter;
}

bool CheckByAvailability(const Truba& truba, bool parameter) {
    return truba.isRemont() == parameter;
}

bool CheckByName(const Truba& truba, string parameter) {
    return truba.getName().find(parameter) != string::npos;
}

bool CheckByMaintenanceStatus(const Truba& truba, bool parameter) {
    return truba.isRemont() == parameter;
}

bool CheckByName(const Kompressornaya_stantsiya& CS, string parameter) {
    return CS.getName().find(parameter) != string::npos;
}

bool CheckByEqual(const Kompressornaya_stantsiya& CS, int percent) {
    return CS.getProcentNeispolzovannyh() == percent;
}

bool CheckByMore(const Kompressornaya_stantsiya& CS, int percent) {
    return CS.getProcentNeispolzovannyh() >= percent;
}

bool CheckByLess(const Kompressornaya_stantsiya& CS, int percent) {
    return CS.getProcentNeispolzovannyh() <= percent;
}

bool Link::IsPipeUsed(int pipe_id, const vector<Link>& connections) {
    for (const auto& conn : connections) {
        if (conn.pipeline == pipe_id) {
            return true;
        }
    }
    return false;
}

bool Link::CreateLink(unordered_map<int, Truba>& pipes,
    unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
    const vector<Link>& existing_connections) {
    if (cs_dict.size() < 2) {
        cout << "Для соединения нужно как минимум 2 КС!" << endl;
        return false;
    }

    while (!CheckAnExistence(CS_inlet, cs_dict)) {
        cout << "Введите ID входной компрессорной станции: ";
        CS_inlet = GetNumberMin(0);
        if (!CheckAnExistence(CS_inlet, cs_dict)) {
            cout << "КС с таким ID не найдена!" << endl;
        }
    }

    while (!CheckAnExistence(CS_outlet, cs_dict) || (CS_inlet == CS_outlet)) {
        cout << "Введите ID выходной компрессорной станции: ";
        CS_outlet = GetNumberMin(0);
        if (!CheckAnExistence(CS_outlet, cs_dict)) {
            cout << "КС с таким ID не найдена!" << endl;
        }
        if (CS_inlet == CS_outlet) {
            cout << "Входная и выходная КС не могут быть одинаковыми!" << endl;
        }
    }

    int diameter;
    cout << "Введите диаметр трубы (500, 700, 1000, 1400): ";
    diameter = GetNumberMin(1);

    vector<int> allowed_diameters = { 500, 700, 1000, 1400 };
    if (find(allowed_diameters.begin(), allowed_diameters.end(), diameter) == allowed_diameters.end()) {
        cout << "Ошибка: диаметр " << diameter << " мм не поддерживается!" << endl;
        return false;
    }

    unordered_map<int, Truba> suitable_pipes = FindPipesByFilter(pipes, CheckByDiameter, diameter);
    unordered_map<int, Truba> available_pipes = FindPipesByFilter(suitable_pipes, CheckByAvailability, false);

    unordered_map<int, Truba> free_pipes;
    for (const auto& pipe : available_pipes) {
        if (!IsPipeUsed(pipe.first, existing_connections)) {
            free_pipes[pipe.first] = pipe.second;
        }
    }

    if (free_pipes.empty()) {
        cout << "Нет доступных труб с диаметром " << diameter << " мм." << endl;
        cout << "Создать новую трубу? (1 - да, 0 - нет): ";
        int choice = GetNumberInRange(0, 1);

        if (choice == 1) {
            Truba new_pipe;
            cout << "Введите данные для новой трубы:" << endl;
            cin >> new_pipe;
            int new_id = static_cast<int>(pipes.size()) + 1;
            pipes[new_id] = new_pipe;
            pipeline = new_id;
            cout << "Создана новая труба с ID: " << new_id << endl;
        }
        else {
            return false;
        }
    }
    else {
        cout << "Доступные трубы:" << endl;
        for (const auto& pipe : free_pipes) {
            cout << "ID: " << pipe.first << " - " << pipe.second.getName()
                << " (длина: " << pipe.second.getDlina() << " км, диаметр: "
                << pipe.second.getDiameter() << " мм)" << endl;
        }

        cout << "Выберите ID трубы: ";
        while (!CheckAnExistence(pipeline, free_pipes)) {
            pipeline = GetNumberMin(0);
            if (!CheckAnExistence(pipeline, free_pipes)) {
                cout << "Выберите ID из доступных труб!" << endl;
            }
        }
    }

    cout << "Соединение создано: КС " << CS_inlet << " -> КС " << CS_outlet
        << " (труба " << pipeline << ", диаметр " << diameter << " мм)" << endl;
    return true;
}

void TopSort(const unordered_map<int, Kompressornaya_stantsiya>& CS,
    const vector<Link>& connections) {
    vector<int> sorted_cs;
    unordered_set<int> processed_cs;
    bool cycle = false;

    for (const auto& cs : CS) {
        if (processed_cs.find(cs.first) == processed_cs.end()) {
            unordered_set<int> gray_stations;
            dfs(cs.first, sorted_cs, processed_cs, connections, gray_stations, cycle);
        }
    }

    if (cycle) {
        cout << "Топологическая сортировка невозможна - обнаружен цикл!" << endl;
        return;
    }

    reverse(sorted_cs.begin(), sorted_cs.end());
    cout << "Топологическая сортировка КС: ";
    for (size_t i = 0; i < sorted_cs.size(); ++i) {
        cout << sorted_cs[i];
        if (i != sorted_cs.size() - 1) cout << " -> ";
    }
    cout << endl;
}

void dfs(int station, vector<int>& order, unordered_set<int>& visited,
    const vector<Link>& connections, unordered_set<int>& gray_stations, bool& flag) {
    if (flag) return;

    gray_stations.insert(station);

    for (const auto& conn : connections) {
        if (conn.CS_inlet == station && visited.find(conn.CS_outlet) == visited.end()) {
            if (gray_stations.find(conn.CS_outlet) != gray_stations.end()) {
                flag = true;
                return;
            }
            else {
                dfs(conn.CS_outlet, order, visited, connections, gray_stations, flag);
            }
        }
    }

    visited.insert(station);
    order.push_back(station);
    gray_stations.erase(station);
}

void deletePipeConnection(int id, vector<Link>& connections) {
    for (auto it = connections.begin(); it != connections.end(); ) {
        if (it->pipeline == id) {
            it = connections.erase(it);
        }
        else {
            ++it;
        }
    }
}

void deleteCSConnections(int id, vector<Link>& connections) {
    for (auto it = connections.begin(); it != connections.end(); ) {
        if (it->CS_inlet == id || it->CS_outlet == id) {
            it = connections.erase(it);
        }
        else {
            ++it;
        }
    }
}

ostream& operator<<(ostream& out, const Link& link) {
    out << "Соединение: КС " << link.CS_inlet << " -> КС " << link.CS_outlet
        << " (труба " << link.pipeline << ")" << endl;
    return out;
}

ostream& operator<<(ostream& out, const vector<Link>& system) {
    if (system.empty()) {
        out << "Нет активных соединений." << endl;
    }
    else {
        for (const auto& conn : system) {
            out << conn;
        }
    }
    return out;
}

ofstream& operator<<(ofstream& out, const Link& link) {
    out << "LINK" << endl
        << link.CS_inlet << endl
        << link.CS_outlet << endl
        << link.pipeline << endl;
    return out;
}

ofstream& operator<<(ofstream& out, const vector<Link>& system) {
    for (const auto& conn : system) {
        out << conn;
    }
    return out;
}

ifstream& operator>>(ifstream& in, Link& connection) {
    in >> connection.CS_inlet >> connection.CS_outlet >> connection.pipeline;
    return in;
}
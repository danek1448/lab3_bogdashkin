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
#include <queue>
#include <cmath>

using namespace std;

// Функции фильтрации труб
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

// Функции фильтрации КС
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

// Шаблонные функции фильтрации
template<typename T>
unordered_map<int, Truba> FindPipesByFilter(const unordered_map<int, Truba>& Pipeline,
    bool(*filter)(const Truba&, T), T parameter) {

    unordered_map<int, Truba> result;
    for (const auto& pipe_pair : Pipeline) {
        if (filter(pipe_pair.second, parameter)) {
            result[pipe_pair.first] = pipe_pair.second;
        }
    }
    return result;
}

template<typename T>
unordered_map<int, Kompressornaya_stantsiya> FindCSByFilter(const unordered_map<int, Kompressornaya_stantsiya>& CS_system,
    bool(*filter)(const Kompressornaya_stantsiya&, T), T parameter) {

    unordered_map<int, Kompressornaya_stantsiya> result;
    for (const auto& cs_pair : CS_system) {
        if (filter(cs_pair.second, parameter)) {
            result[cs_pair.first] = cs_pair.second;
        }
    }
    return result;
}

// Явные инстанциации шаблонов
template unordered_map<int, Truba> FindPipesByFilter<string>(const unordered_map<int, Truba>&, bool(*)(const Truba&, string), string);
template unordered_map<int, Truba> FindPipesByFilter<bool>(const unordered_map<int, Truba>&, bool(*)(const Truba&, bool), bool);
template unordered_map<int, Truba> FindPipesByFilter<int>(const unordered_map<int, Truba>&, bool(*)(const Truba&, int), int);

template unordered_map<int, Kompressornaya_stantsiya> FindCSByFilter<string>(const unordered_map<int, Kompressornaya_stantsiya>&, bool(*)(const Kompressornaya_stantsiya&, string), string);
template unordered_map<int, Kompressornaya_stantsiya> FindCSByFilter<int>(const unordered_map<int, Kompressornaya_stantsiya>&, bool(*)(const Kompressornaya_stantsiya&, int), int);

// Методы Link
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

// Топологическая сортировка
void dfs(int station, vector<int>& order, unordered_set<int>& visited,
    const vector<Link>& connections, unordered_set<int>& gray_stations, bool& flag);

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

double CalculatePipeCapacity(const Truba& pipe) {
    if (pipe.isRemont()) {
        return 0.0;
    }

    double diameter_m = pipe.getDiameter() / 1000.0;
    double length_km = pipe.getDlina();
    double length_m = length_km * 1000.0;

    if (length_m == 0) return 0.0;

    double capacity = sqrt(pow(diameter_m, 5) / length_m) * 1000.0;
    return capacity;
}

double CalculatePipeWeight(const Truba& pipe) {
    if (pipe.isRemont()) {
        return numeric_limits<double>::infinity();
    }
    return pipe.getDlina();
}

vector<int> FindShortestPathDijkstra(
    const unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
    const vector<Link>& connections,
    const unordered_map<int, Truba>& pipes,
    int start, int end) {

    vector<int> path;

    if (cs_dict.find(start) == cs_dict.end() || cs_dict.find(end) == cs_dict.end()) {
        return path;
    }

    unordered_map<int, vector<pair<int, double>>> graph;
    for (const auto& conn : connections) {
        auto pipe_it = pipes.find(conn.pipeline);
        if (pipe_it != pipes.end()) {
            double weight = CalculatePipeWeight(pipe_it->second);
            graph[conn.CS_inlet].push_back(make_pair(conn.CS_outlet, weight));
        }
    }

    unordered_map<int, double> dist;
    unordered_map<int, int> prev;

    for (const auto& cs : cs_dict) {
        dist[cs.first] = numeric_limits<double>::max();
    }
    dist[start] = 0.0;

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push(make_pair(0.0, start));

    while (!pq.empty()) {
        double current_dist = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();

        if (current_dist > dist[current_node]) continue;

        if (current_node == end) break;

        auto it = graph.find(current_node);
        if (it != graph.end()) {
            for (const auto& neighbor : it->second) {
                int next_node = neighbor.first;
                double edge_weight = neighbor.second;

                double new_dist = current_dist + edge_weight;
                if (new_dist < dist[next_node]) {
                    dist[next_node] = new_dist;
                    prev[next_node] = current_node;
                    pq.push(make_pair(new_dist, next_node));
                }
            }
        }
    }

    if (dist[end] == numeric_limits<double>::max()) {
        return path;
    }

    for (int current = end; current != start; current = prev[current]) {
        path.push_back(current);
    }
    path.push_back(start);

    reverse(path.begin(), path.end());

    return path;
}

double CalculateMaxxFlow(
    const unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
    const vector<Link>& connections,
    const unordered_map<int, Truba>& pipes,
    int source, int sink) {

    if (cs_dict.find(source) == cs_dict.end() || cs_dict.find(sink) == cs_dict.end()) {
        return 0.0;
    }

    unordered_map<int, vector<pair<int, double>>> graph;
    for (const auto& conn : connections) {
        auto pipe_it = pipes.find(conn.pipeline);
        if (pipe_it != pipes.end()) {
            double capacity = CalculatePipeCapacity(pipe_it->second);
            graph[conn.CS_inlet].push_back(make_pair(conn.CS_outlet, capacity));
        }
    }

    unordered_map<int, int> parent;
    queue<int> q;
    q.push(source);
    parent[source] = -1;
    bool found = false;

    while (!q.empty() && !found) {
        int u = q.front();
        q.pop();

        auto it = graph.find(u);
        if (it != graph.end()) {
            for (const auto& edge : it->second) {
                int v = edge.first;
                double cap = edge.second;

                if (parent.find(v) == parent.end() && cap > 0) {
                    parent[v] = u;
                    if (v == sink) {
                        found = true;
                        break;
                    }
                    q.push(v);
                }
            }
        }
    }

    if (!found) {
        return 0.0;
    }

    double min_capacity = numeric_limits<double>::max();
    for (int v = sink; v != source; v = parent[v]) {
        int u = parent[v];

        auto it = graph.find(u);
        if (it != graph.end()) {
            for (const auto& edge : it->second) {
                if (edge.first == v) {
                    min_capacity = min(min_capacity, edge.second);
                    break;
                }
            }
        }
    }

    return (min_capacity == numeric_limits<double>::max()) ? 0.0 : min_capacity;
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
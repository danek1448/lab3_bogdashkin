#include "soedinenie.h"
#include <iostream>

using namespace std;

bool Gazoset::add_connection(int pipe_id, int input_cs_id, int output_cs_id, int diameter) {
    if (find(available_diameters.begin(), available_diameters.end(), diameter) == available_diameters.end()) {
        cout << "Ошибка: диаметр " << diameter << " мм не поддерживается!" << endl;
        return false;
    }

    if (input_cs_id == output_cs_id) {
        cout << "Ошибка: вход и выход не могут быть одинаковыми!" << endl;
        return false;
    }

    for (const auto& conn : connections) {
        if (conn.input_cs_id == input_cs_id && conn.output_cs_id == output_cs_id) {
            cout << "Ошибка: соединение между КС " << input_cs_id << " и КС " << output_cs_id << " уже существует!" << endl;
            return false;
        }
    }

    connections.emplace_back(pipe_id, input_cs_id, output_cs_id, diameter);
    cout << "Соединение добавлено: КС " << input_cs_id << " -> КС " << output_cs_id
        << " (труба " << pipe_id << ", диаметр " << diameter << " мм)" << endl;
    return true;
}

bool Gazoset::remove_connection(int input_cs_id, int output_cs_id) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->input_cs_id == input_cs_id && it->output_cs_id == output_cs_id) {
            connections.erase(it);
            cout << "Соединение удалено: КС " << input_cs_id << " -> КС " << output_cs_id << endl;
            return true;
        }
    }
    cout << "Соединение не найдено!" << endl;
    return false;
}

bool Gazoset::remove_pipe(int pipe_id) {
    bool found = false;
    for (auto it = connections.begin(); it != connections.end(); ) {
        if (it->pipe_id == pipe_id) {
            it = connections.erase(it);
            found = true;
        }
        else {
            ++it;
        }
    }
    if (found) {
        cout << "Труба " << pipe_id << " удалена из всех соединений!" << endl;
    }
    return found;
}

int Gazoset::find_available_pipe(const unordered_map<int, Truba>& pipes, int diameter) {
    for (const auto& pipe_pair : pipes) {
        const Truba& pipe = pipe_pair.second;
        bool is_used = false;

        for (const auto& conn : connections) {
            if (conn.pipe_id == pipe_pair.first) {
                is_used = true;
                break;
            }
        }

        if (!is_used && pipe.getDiameter() == diameter && !pipe.isRemont()) {
            return pipe_pair.first;
        }
    }
    return -1; 
}

vector<int> Gazoset::topological_sort(const unordered_map<int, Kompressornaya_stantsiya>& cs_dict) {
    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> in_degree;

    for (const auto& cs_pair : cs_dict) {
        int cs_id = cs_pair.first;
        graph[cs_id] = vector<int>();
        in_degree[cs_id] = 0;
    }

    for (const auto& conn : connections) {
        graph[conn.input_cs_id].push_back(conn.output_cs_id);
        in_degree[conn.output_cs_id]++;
    }

    queue<int> q;
    vector<int> result;

    for (const auto& cs_pair : cs_dict) {
        if (in_degree[cs_pair.first] == 0) {
            q.push(cs_pair.first);
        }
    }

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);

        for (int neighbor : graph[current]) {
            in_degree[neighbor]--;
            if (in_degree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    if (result.size() != cs_dict.size()) {
        cout << "Обнаружен цикл в графе! Топологическая сортировка невозможна." << endl;
        return vector<int>();
    }

    return result;
}
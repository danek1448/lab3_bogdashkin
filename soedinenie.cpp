#include "soedinenie.h"
#include <iostream>

using namespace std;

bool Gazoset::add_connection(int pipe_id, int cs_id, bool is_input) {
    for (const auto& conn : connections) {
        if (conn.pipe_id == pipe_id && conn.cs_id == cs_id) {
            cout << "Ошибка: соединение между трубой " << pipe_id
                << " и КС " << cs_id << " уже существует!" << endl;
            return false;
        }
    }

    connections.emplace_back(pipe_id, cs_id, is_input);
    cout << "Соединение добавлено: "
        << (is_input ? "КС " : "КС ") << cs_id
        << " -> " << (is_input ? "вход трубы " : "выход трубы ") << pipe_id << endl;
    return true;
}

bool Gazoset::remove_connection(int pipe_id, int cs_id) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->pipe_id == pipe_id && it->cs_id == cs_id) {
            connections.erase(it);
            cout << "Соединение удалено: труба " << pipe_id << " - КС " << cs_id << endl;
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

bool Gazoset::remove_cs(int cs_id) {
    bool found = false;
    for (auto it = connections.begin(); it != connections.end(); ) {
        if (it->cs_id == cs_id) {
            it = connections.erase(it);
            found = true;
        }
        else {
            ++it;
        }
    }
    if (found) {
        cout << "КС " << cs_id << " удалена из всех соединений!" << endl;
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

vector<Connection> Gazoset::get_connections_for_pipe(int pipe_id) const {
    vector<Connection> result;
    for (const auto& conn : connections) {
        if (conn.pipe_id == pipe_id) {
            result.push_back(conn);
        }
    }
    return result;
}

vector<Connection> Gazoset::get_connections_for_cs(int cs_id) const {
    vector<Connection> result;
    for (const auto& conn : connections) {
        if (conn.cs_id == cs_id) {
            result.push_back(conn);
        }
    }
    return result;
}

unordered_map<int, vector<int>> Gazoset::get_graph() const {
    unordered_map<int, vector<int>> graph;

    unordered_map<int, vector<int>> pipe_connections;

    for (const auto& conn : connections) {
        if (conn.is_input) {
            for (const auto& output_conn : connections) {
                if (output_conn.pipe_id == conn.pipe_id && !output_conn.is_input) {
                    graph[conn.cs_id].push_back(output_conn.cs_id);
                }
            }
        }
    }

    return graph;
}

vector<int> Gazoset::topological_sort(const unordered_map<int, Kompressornaya_stantsiya>& cs_dict) {
    unordered_map<int, vector<int>> graph = get_graph();
    unordered_map<int, int> in_degree;

    for (const auto& cs_pair : cs_dict) {
        int cs_id = cs_pair.first;
        in_degree[cs_id] = 0;
    }

    for (const auto& node : graph) {
        for (int neighbor : node.second) {
            in_degree[neighbor]++;
        }
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
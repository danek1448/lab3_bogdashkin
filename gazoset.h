#ifndef GAZOSET_H
#define GAZOSET_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include "truba.h"
#include "cs.h"
#include "soedinenie.h"
#include "instruments.h"

using namespace std;

class Gazoset {
private:
    unordered_map<int, Truba> Pipeline;
    unordered_map<int, Kompressornaya_stantsiya> CS_system;
    vector<Link> connections;

public:
    Gazoset() = default;

    unordered_map<int, Truba>& getPipes() { return Pipeline; }
    unordered_map<int, Kompressornaya_stantsiya>& getCS() { return CS_system; }
    vector<Link>& getConnections() { return connections; }

    const unordered_map<int, Truba>& getPipes() const { return Pipeline; }
    const unordered_map<int, Kompressornaya_stantsiya>& getCS() const { return CS_system; }
    const vector<Link>& getConnections() const { return connections; }

    void addPipe(const Truba& pipe, int id) { Pipeline[id] = pipe; }
    void addCS(const Kompressornaya_stantsiya& cs, int id) { CS_system[id] = cs; }
    void addConnection(const Link& connection) { connections.push_back(connection); }

    void delete_pipe(int id);
    void delete_CS(int id);
    void delete_connection(int input_cs, int output_cs);

    void PipesFiltering();
    void CSFiltering();

    void PacketEditPipe(const unordered_set<int>& IDs, bool state);
    void PacketEditCS(const unordered_set<int>& IDs);

    friend ostream& operator << (ostream& out, const Gazoset& gts);
};

template<class T>
unordered_set<int> GetIds(const unordered_map<int, T>& System) {
    unordered_set<int> IDs;
    if (System.empty()) {
        cout << "Нет доступных объектов!" << endl;
        return IDs;
    }

    cout << "Выберите опцию:" << endl
        << "1. Выбрать вручную" << endl
        << "2. Выбрать все" << endl;

    int choice = GetNumberInRange(1, 2);

    switch (choice) {
    case 1: {
        while (true) {
            cout << "Введите ID объекта (-1 для завершения): ";
            int id = GetNumberMin(-1);
            if (id == -1) break;

            if (System.find(id) != System.end()) {
                IDs.insert(id);
                cout << "Объект " << id << " добавлен в выборку" << endl;
            }
            else {
                cout << "Объект с ID " << id << " не найден!" << endl;
            }
        }
        break;
    }
    case 2: {
        for (const auto& item : System) {
            IDs.insert(item.first);
        }
        cout << "Выбрано всех объектов: " << IDs.size() << endl;
        break;
    }
    }

    return IDs;
}

#endif
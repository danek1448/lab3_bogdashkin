#ifndef SOEDINENIE_H
#define SOEDINENIE_H

#include "cs.h"
#include "truba.h"
#include "instruments.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <stack>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

struct Link
{
    int pipeline = -1;
    int CS_inlet = -1;
    int CS_outlet = -1;

    bool CreateLink(unordered_map<int, Truba>& pipes,
        unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
        const vector<Link>& existing_connections); 

    friend void TopSort(const unordered_map<int, Kompressornaya_stantsiya>& CS,
        const vector<Link>& connections);

    friend void dfs(int station, vector<int>& order, unordered_set<int>& visited,
        const vector<Link>& connections, unordered_set<int>& gray_stations, bool& flag);

    friend void deletePipeConnection(int id, vector<Link>& connections);
    friend void deleteCSConnections(int id, vector<Link>& connections);

    friend ostream& operator << (ostream& out, const Link& link);
    friend ostream& operator << (ostream& out, const vector<Link>& system);
    friend ofstream& operator << (ofstream& out, const Link& link);
    friend ofstream& operator << (ofstream& out, const vector<Link>& system);
    friend ifstream& operator >> (ifstream& in, Link& connection);

    template <typename T>
    bool CheckAnExistence(int id, const unordered_map<int, T>& elements) {
        return elements.find(id) != elements.end();
    }

    static bool IsPipeUsed(int pipe_id, const vector<Link>& connections);
};

bool CheckByDiameter(const Truba& truba, int parameter);
bool CheckByAvailability(const Truba& truba, bool parameter);
bool CheckByName(const Truba& truba, string parameter);
bool CheckByMaintenanceStatus(const Truba& truba, bool parameter);
bool CheckByName(const Kompressornaya_stantsiya& CS, string parameter);
bool CheckByEqual(const Kompressornaya_stantsiya& CS, int percent);
bool CheckByMore(const Kompressornaya_stantsiya& CS, int percent);
bool CheckByLess(const Kompressornaya_stantsiya& CS, int percent);

template<typename T>
using FilterPipes = bool(*)(const Truba& truba, T parameter);

template<typename T>
unordered_map<int, Truba> FindPipesByFilter(const unordered_map<int, Truba>& Pipeline,
    FilterPipes<T> filter, T parameter) {
    unordered_map<int, Truba> result;
    for (const auto& s : Pipeline) {
        if (filter(s.second, parameter)) {
            result[s.first] = s.second;
        }
    }
    return result;
}

template<typename T>
using FilterCS = bool(*)(const Kompressornaya_stantsiya& CS, T parameter);

template<typename T>
unordered_map<int, Kompressornaya_stantsiya> FindCSByFilter(const unordered_map<int, Kompressornaya_stantsiya>& CS_system,
    FilterCS<T> filter, T parameter) {
    unordered_map<int, Kompressornaya_stantsiya> result;
    for (const auto& s : CS_system) {
        if (filter(s.second, parameter)) {
            result[s.first] = s.second;
        }
    }
    return result;
}

#endif
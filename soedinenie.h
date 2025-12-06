#ifndef SOEDINENIE_H
#define SOEDINENIE_H

#include "cs.h"
#include "truba.h"
#include "instruments.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <queue>
#include <limits>
#include <cmath>
#include <functional>

using namespace std;

struct Link
{
    int pipeline = -1;
    int CS_inlet = -1;
    int CS_outlet = -1;

    bool CreateLink(unordered_map<int, Truba>& pipes,
        unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
        const vector<Link>& existing_connections);

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
unordered_map<int, Truba> FindPipesByFilter(const unordered_map<int, Truba>& Pipeline,
    bool(*filter)(const Truba&, T), T parameter);

template<typename T>
unordered_map<int, Kompressornaya_stantsiya> FindCSByFilter(const unordered_map<int, Kompressornaya_stantsiya>& CS_system,
    bool(*filter)(const Kompressornaya_stantsiya&, T), T parameter);

void deletePipeConnection(int id, vector<Link>& connections);
void deleteCSConnections(int id, vector<Link>& connections);

void TopSort(const unordered_map<int, Kompressornaya_stantsiya>& CS,
    const vector<Link>& connections);

double CalculatePipeCapacity(const Truba& pipe);
double CalculatePipeWeight(const Truba& pipe);

vector<int> FindShortestPathDijkstra(
    const unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
    const vector<Link>& connections,
    const unordered_map<int, Truba>& pipes,
    int start, int end);

double CalculateMaxxFlow(
    const unordered_map<int, Kompressornaya_stantsiya>& cs_dict,
    const vector<Link>& connections,
    const unordered_map<int, Truba>& pipes,
    int source, int sink);

ostream& operator<<(ostream& out, const Link& link);
ostream& operator<<(ostream& out, const vector<Link>& system);
ofstream& operator<<(ofstream& out, const Link& link);
ofstream& operator<<(ofstream& out, const vector<Link>& system);
ifstream& operator>>(ifstream& in, Link& connection);

#endif
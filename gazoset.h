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

#endif
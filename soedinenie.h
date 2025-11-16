#ifndef SOEDINENIE_H
#define SOEDINENIE_H

#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iostream>
#include "truba.h"
#include "cs.h"

using namespace std;

struct Connection {
    int pipe_id;
    int cs_id;           
    bool is_input;      

    Connection(int p_id, int cs_id, bool input)
        : pipe_id(p_id), cs_id(cs_id), is_input(input) {
    }
};

class Gazoset {
private:
    vector<Connection> connections;
    vector<int> available_diameters = { 500, 700, 1000, 1400 };

public:
    bool add_connection(int pipe_id, int cs_id, bool is_input);

    bool remove_connection(int pipe_id, int cs_id);

    bool remove_pipe(int pipe_id);

    bool remove_cs(int cs_id);

    vector<int> topological_sort(const unordered_map<int, Kompressornaya_stantsiya>& cs_dict);

    int find_available_pipe(const unordered_map<int, Truba>& pipes, int diameter);

    vector<Connection> get_connections_for_pipe(int pipe_id) const;

    vector<Connection> get_connections_for_cs(int cs_id) const;

    const vector<Connection>& get_all_connections() const { return connections; }

    unordered_map<int, vector<int>> get_graph() const;

    void clear_connections() { connections.clear(); }
};

#endif
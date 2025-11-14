#ifndef SOEDINENIE_H
#define SOEDINENIE_H

#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "truba.h"
#include "cs.h"
#include <iostream>

using namespace std;

struct Connection {
    int pipe_id;
    int input_cs_id;
    int output_cs_id;
    int diameter;

    Connection(int p_id, int in_id, int out_id, int diam)
        : pipe_id(p_id), input_cs_id(in_id), output_cs_id(out_id), diameter(diam) {
    }
};

class Gazoset {
private:
    vector<Connection> connections;
    vector<int> available_diameters = { 500, 700, 1000, 1400 };

public:
    bool add_connection(int pipe_id, int input_cs_id, int output_cs_id, int diameter);

    bool remove_connection(int input_cs_id, int output_cs_id);

    bool remove_pipe(int pipe_id);

    vector<int> topological_sort(const unordered_map<int, Kompressornaya_stantsiya>& cs_dict);

    int find_available_pipe(const unordered_map<int, Truba>& pipes, int diameter);

    const vector<Connection>& get_connections() const { return connections; }

    void clear_connections() { connections.clear(); }
};

#endif
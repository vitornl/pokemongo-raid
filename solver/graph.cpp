#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include "graph.h"

using namespace std;

Graph initialize_graph(string name, string vertices_file, string distances_file) {

    Graph g;
    ifstream fvertices (vertices_file);
    ifstream fdistances (distances_file);
    string line;

    char delimiter = ' ';
    
    g.name = name;
    
    // vertices file
    if (fvertices.is_open()) {
        int c_line = 0;
        while (getline(fvertices, line)) {
            if(c_line == 0 || c_line == 2) {
                c_line++;
            } else if(c_line == 1) {
                g.n_vertices = atoi(line.c_str());
                c_line++;
            } else {
                istringstream ss(line);
                string location;
                int i = 0;
                while(getline(ss, location, delimiter)) {
                    if(i == 1) {
                        g.vertices.push_back(location);
                        break;
                    }
                    i++;
                }
            }
        }
        fvertices.close();
    }

    // distances file
    if (fdistances.is_open()) {
        int c_line = 0;
        while (getline(fdistances, line)) {
            if(c_line == 0) {
                c_line++;
            } else {
                istringstream ss(line);
                string value;
                vector<float> row;
                while(getline(ss, value, delimiter)) {
                    row.push_back(atof(value.c_str()));
                }
                g.weights.push_back(row);
            }
        }
        fdistances.close();
    }

    // initialize visited vectors;
    for(int i = 0; i < g.n_vertices; i++)
        g.v_vertices.push_back(0);
    for(int i = 0; i < g.n_vertices; i++)
        for(int j = 0; j < g.n_vertices; j++) {
            if(i == j) g.v_edges.push_back(-1);
            else g.v_edges.push_back(0);
        }

    return g;
}

int idx_m2v(int C, int l, int c) {
    return C*l + c;
}

vector<int> idx_m2v(int C, int p) {
    vector<int> out;
    out.push_back((int)(p/C));
    out.push_back((int)(p%C));

    return out;
}
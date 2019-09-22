#include <vector>
#include <string>

using namespace std;

typedef struct _graph {
    int n_vertices;
    vector<string> vertices;
    vector<vector<float> > weights;
    vector<int> v_vertices;
    vector<int> v_edges;
} Graph;

Graph initialize_graph(string vertices_file, string distances_file);

int idx_m2v(int C, int l, int c);

vector<int> idx_m2v(int C, int p);

// float spent_time(Graph g);
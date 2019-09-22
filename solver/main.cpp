#include <iostream>
#include "heuristics.h"

void print_answer(Graph g, Answer a) {
    cout << "### PATH:\n";
    for(int i = 0; i < a.path.size(); i++) {
        cout << a.path[i] << ' ' << g.vertices[a.path[i]] << '\n';
    }
    cout << "### SPENT TIME:" << a.time_spent << '\n';
    cout << "### OBJECTIVE PONCTUATION:" << a.objective_ponctuation << '\n';
}

int main(void) {
    Graph g;
    Answer a;

    // cout << "\n\t# Random Multistart #\n";
    // g = initialize_graph("data/vertices.lhp", "data/d_time.lhp");
    // a = random_multistart(g, 60, 10);
    // print_answer(g, a);
    
    cout << "\n\t# Greedy #\n";
    g = initialize_graph("data/vertices.lhp", "data/d_time.lhp");
    a = greedy(g, 120);
    print_answer(g, a);
    
    cout << "\n\t# Adaptative Greedy #\n";
    g = initialize_graph("data/vertices.lhp", "data/d_time.lhp");
    a = adaptative_greedy(g, 120);
    print_answer(g, a);

    return 0;
}
#include <ctime>
#include <cstdlib>
#include <time.h>
#include "heuristics.h"
#include <iostream>
#include <fstream>

// OBJECTIVE FUNCTION

float objective_function(Graph g) {
    float max_weight = 0;
    
    // find max weights
    for(int i = 0; i < g.n_vertices; i++)
        for(int j = 0; j < g.n_vertices; j++)
            if(g.weights[i][j] > max_weight)
                max_weight = g.weights[i][j];
    

    // weights sum
    float weights_sum = 0;
    for(int i = 0; i < g.n_vertices; i++)
        for(int j = 0; j < g.n_vertices; j++)
            weights_sum += g.v_edges[idx_m2v(g.n_vertices, i, j)] * (g.weights[i][j]/max_weight);
    

    // vertices sum
    int vertices_sum = 0;
    for(int i = 0; i < g.n_vertices; i++)
        vertices_sum += g.v_vertices[i];

    return (2*vertices_sum - weights_sum);
}

float time_spent_function(Graph g) {
    float sum;
    for(int i = 0; i < g.n_vertices; i++)
        for(int j = 0; j < g.n_vertices; j++)
            sum += g.v_edges[idx_m2v(g.n_vertices, i, j)] * g.weights[i][j];

    return sum;

}

void print_answer(Graph g, Answer a) {
    cout << "### PATH:\n";
    for(int i = 0; i < a.path.size(); i++) {
        cout << a.path[i] << ' ' << g.vertices[a.path[i]] << '\n';
    }
    cout << "### SPENT TIME:" << a.time_spent << '\n';
    cout << "### OBJECTIVE PONCTUATION:" << a.objective_ponctuation << '\n';
}

// GREEDY
int _greedy_choice(Graph g, int vertex) {
    int idx;
    float min = -1;
    for(int i = 0; i < g.n_vertices; i++) {
        // short distance
        if((min == -1 && g.v_edges[idx_m2v(g.n_vertices, vertex, i)] == 0) ||
            (g.weights[vertex][i] < min && g.v_vertices[i] == 0 && g.v_edges[idx_m2v(g.n_vertices, vertex, i)] == 0)) {
            min = g.weights[vertex][i];
            idx = i;
        }
    }

    if(min == -1) {
        cout << "DEU MERDA\n";
        exit(1);
    }

    return idx;
}

Answer greedy(Graph g, int T_MAX) {

    Answer a;

    // int v0 = 17;
    int v0 = (rand() % g.n_vertices);
    
    g.v_vertices[v0] = 1;
    a.path.push_back(v0);
    a.time_spent = time_spent_function(g);
    while(a.time_spent <= T_MAX) {
        //cout << v0 << '\n';
        int next_v = _greedy_choice(g, v0);
        
        g.v_vertices[next_v] = 1;
        
        g.v_edges[idx_m2v(g.n_vertices, v0, next_v)] = 1;
        g.v_edges[idx_m2v(g.n_vertices, next_v, v0)] = 1;
        
        a.path.push_back(next_v);
        a.time_spent = time_spent_function(g);
        v0 = next_v;
    }

    a.objective_ponctuation = objective_function(g);

    return a;
}

// adaptive GREEDY
int _adaptive_greedy_choice(Graph g, int vertex) {
    int idx = -1;
    float max = objective_function(g);

    for(int i = 0; i < g.n_vertices; i++) {
        if(g.v_vertices[i]) continue;
        // simulate walk
        g.v_vertices[i] = 1;
        g.v_edges[idx_m2v(g.n_vertices, vertex, i)] = 1;
        g.v_edges[idx_m2v(g.n_vertices, i, vertex)] = 1;

        float simulated_ponctuation = objective_function(g);

        // reverse walk
        g.v_vertices[i] = 0;
        g.v_edges[idx_m2v(g.n_vertices, vertex, i)] = 0;
        g.v_edges[idx_m2v(g.n_vertices, i, vertex)] = 0;

        if(simulated_ponctuation > max) {
            idx = i;
            max = simulated_ponctuation;
        }
    }

    if(idx == -1) {
        cout << "DEU MERDA\n";
        exit(1);
    }

    return idx;
}

Answer adaptive_greedy(Graph g, int T_MAX) {

    Answer a;

    // int v0 = 17;
    int v0 = (rand() % g.n_vertices);
    
    g.v_vertices[v0] = 1;
    a.path.push_back(v0);
    a.time_spent = time_spent_function(g);
    while(a.time_spent <= T_MAX) {
        // cout << v0 << '\n';
        int next_v = _adaptive_greedy_choice(g, v0);
        
        g.v_vertices[next_v] = 1;
        
        g.v_edges[idx_m2v(g.n_vertices, v0, next_v)] = 1;
        g.v_edges[idx_m2v(g.n_vertices, next_v, v0)] = 1;
        
        a.path.push_back(next_v);
        a.time_spent = time_spent_function(g);
        v0 = next_v;
    }

    a.objective_ponctuation = objective_function(g);

    return a;
}

// RANDOM MULTISTART
Answer _random_solution(Graph g, int T_MAX) {
    Answer a;

    int v0 = (rand() % g.n_vertices);

    g.v_vertices[v0] = 1;
    a.path.push_back(v0);
    a.time_spent = time_spent_function(g);
    while(a.time_spent <= T_MAX) {
        
        // random neighbor
        int next_v = (rand() % g.n_vertices);
        while(next_v == v0)
            next_v = (rand() % g.n_vertices);
        
        g.v_vertices[next_v] = 1;
        
        g.v_edges[idx_m2v(g.n_vertices, v0, next_v)] = 1;
        g.v_edges[idx_m2v(g.n_vertices, next_v, v0)] = 1;
        
        a.path.push_back(next_v);
        a.time_spent = time_spent_function(g);
        v0 = next_v;
    }
    a.objective_ponctuation = objective_function(g);

    return a;
}

Answer random_multistart(Graph g, int T_MAX, int stopping_criterion) {

    int i = 0;
    Answer best;
    // ofstream outfile;
    // outfile.open("random-multistart.txt");
    
    Answer a = _random_solution(g, T_MAX);
    best.path = a.path;
    best.time_spent = a.time_spent;
    best.objective_ponctuation = a.objective_ponctuation;

    while(i < stopping_criterion) {
        a = _random_solution(g, T_MAX);

        // outfile << a.objective_ponctuation << ',' << a.time_spent;
        // for(int j = 0; j < a.path.size(); j++)
        //     outfile << ',' << a.path[j];
        // outfile << '\n';
                
        if(a.objective_ponctuation > best.objective_ponctuation) {
            best.path = a.path;
            best.time_spent = a.time_spent;
            best.objective_ponctuation = a.objective_ponctuation;
        }

        i++;
    }

    return best;
}
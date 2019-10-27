#include <ctime>
#include <cstdlib>
#include <time.h>
#include <vector>
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

void print_answer(Graph g, Answer a, bool SHORT) {
    if(SHORT) {
        cout << "### PATH SIZE:" << a.path.size() << '\n';
        cout << "### SPENT TIME:" << a.time_spent << '\n';
        return;
    }
    
    cout << "### PATH:\n";
    for(int i = 0; i < a.path.size(); i++) {
        cout << a.path[i] << ' ' << g.vertices[a.path[i]] << '\n';
    }
    cout << "### PATH SIZE:" << a.path.size() << '\n';
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
        cout << "DEU RUIM\n";
        exit(1);
    }

    return idx;
}

Answer greedy(Graph g, int T_MAX, int v0) {

    Answer a;

    // int v0 = 17;
    v0 = (v0 != -1) ? v0 : (rand() % g.n_vertices);
    
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
    a.final_graph = g;

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
        cout << "DEU RUIM\n";
        exit(1);
    }

    return idx;
}

Answer adaptive_greedy(Graph g, int T_MAX, int v0) {

    Answer a;

    // int v0 = 17;
    v0 = (v0 != -1) ? v0 : (rand() % g.n_vertices);
    
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
    a.final_graph = g;

    return a;
}

// RANDOM MULTISTART
Answer _random_solution(Graph g, int T_MAX, int v0) {
    Answer a;

    v0 = (v0 != -1) ? v0 : (rand() % g.n_vertices);
    
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
    a.final_graph = g;
    
    return a;
}

Answer random_multistart(Graph g, int T_MAX, int stopping_criterion, int v0) {

    int i = 0;
    Answer best;
    // ofstream outfile;
    // outfile.open("random-multistart.txt");
    
    Answer a = _random_solution(g, T_MAX, v0);
    best.path = a.path;
    best.time_spent = a.time_spent;
    best.objective_ponctuation = a.objective_ponctuation;
    best.final_graph = a.final_graph;

    while(i < stopping_criterion) {
        a = _random_solution(g, T_MAX, v0);

        // outfile << a.objective_ponctuation << ',' << a.time_spent;
        // for(int j = 0; j < a.path.size(); j++)
        //     outfile << ',' << a.path[j];
        // outfile << '\n';
                
        if(a.objective_ponctuation > best.objective_ponctuation) {
            best.path = a.path;
            best.time_spent = a.time_spent;
            best.objective_ponctuation = a.objective_ponctuation;
            best.final_graph = a.final_graph;
        }

        i++;
    }
    return best;
}

// SEMI_GREEDY
int _semi_greedy_choice(Graph g, int vertex, float alpha) {
    int idx;
    float min = -1, max = -1;
    for(int i = 0; i < g.n_vertices; i++) {
        // short distance with probability
        if(min == -1 && max == -1 && g.v_edges[idx_m2v(g.n_vertices, vertex, i)] == 0) {
            min = g.weights[vertex][i];
            max = g.weights[vertex][i];
        }
        else if(g.weights[vertex][i] < min && g.v_vertices[i] == 0 && g.v_edges[idx_m2v(g.n_vertices, vertex, i)] == 0) {
            min = g.weights[vertex][i];
        }
        else if(g.weights[vertex][i] > max && g.v_vertices[i] == 0 && g.v_edges[idx_m2v(g.n_vertices, vertex, i)] == 0) {
            max = g.weights[vertex][i];
        }
    }

    if(min == -1 || max == -1) {
        cout << "DEU RUIM\n";
        exit(1);
    }

    float c_min, c_max;
    c_min = min;
    c_max = c_min + alpha*max;
    
    vector<int> ci;
    for(int i = 0; i < g.n_vertices; i++) {
        if((g.weights[vertex][i] >= c_min || g.weights[vertex][i] <= c_max) && g.v_edges[idx_m2v(g.n_vertices, vertex, i)] == 0) {
            ci.push_back(i);
        }
    }
    if(ci.size() == 0) {
        cout << "DEU RUIM\n";
        exit(1);
    }
    return ci[(rand() % ci.size())];
}

Answer semi_greedy(Graph g, int T_MAX, float alpha, int v0) {

    Answer a;

    // int v0 = 17;
    v0 = (v0 != -1) ? v0 : (rand() % g.n_vertices);
    
    g.v_vertices[v0] = 1;
    a.path.push_back(v0);
    a.time_spent = time_spent_function(g);
    while(a.time_spent <= T_MAX) {
        //cout << v0 << '\n';
        int next_v = _semi_greedy_choice(g, alpha, v0);
        
        g.v_vertices[next_v] = 1;
        
        g.v_edges[idx_m2v(g.n_vertices, v0, next_v)] = 1;
        g.v_edges[idx_m2v(g.n_vertices, next_v, v0)] = 1;
        
        a.path.push_back(next_v);
        a.time_spent = time_spent_function(g);
        v0 = next_v;
    }

    a.objective_ponctuation = objective_function(g);
    a.final_graph = g;

    return a;
}

// LOCAL SEARCH
Answer _fill_solution(Answer local_search_solution, int TMAX) {
    Answer new_lss = local_search_solution;
    while(new_lss.time_spent <= TMAX) {
        int min_walk = 100000000;
        int last_vertex = new_lss.path.back(), next_vertex = -1;
        for(int i = 0; i < new_lss.final_graph.n_vertices; i++) {
            if(find(new_lss.path.begin(), new_lss.path.end(), i) != new_lss.path.end()) continue;
            if(new_lss.final_graph.weights[last_vertex][i] < min_walk) {
                min_walk = new_lss.final_graph.weights[last_vertex][i];
                next_vertex = i;
            }
        }
        new_lss.final_graph.v_edges[idx_m2v(new_lss.final_graph.n_vertices, last_vertex, next_vertex)] = 1;
        new_lss.final_graph.v_edges[idx_m2v(new_lss.final_graph.n_vertices, next_vertex, last_vertex)] = 1;
        new_lss.final_graph.v_vertices[next_vertex] = 1;
        new_lss.path.push_back(next_vertex);
        new_lss.time_spent = time_spent_function(new_lss.final_graph);
    }

    return new_lss;
}

Answer first_improving_add_edges(Answer initial_solution) {
    Answer a = initial_solution;

    for(int vertex = 0; vertex < a.path.size()-1; vertex++) {
        int v0 = vertex, v1 = vertex+1;
        int improvement = 1;
        while(improvement) {
            improvement = 0;
            int vo = a.path[v0], vd = a.path[v1];
            Graph simulation_graph = a.final_graph;

            float max_weight = simulation_graph.weights[vo][vd];

            for(int i = 0; i < simulation_graph.n_vertices; i++) {
                if(find(a.path.begin(), a.path.end(), i) != a.path.end()) continue;
                float new_weights = simulation_graph.weights[vo][i] + simulation_graph.weights[i][vd];

                if(new_weights <= max_weight) {
                    // undo worst walk
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, vd)] = 0;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vd, vo)] = 0;

                    // new walk
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, i)] = 1;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vo)] = 1;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vd)] = 1;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vd, i)] = 1;
                    simulation_graph.v_vertices[i] = 1;
                    
                    a.path.insert(a.path.begin()+v1, i);
                    
                    a.time_spent = time_spent_function(simulation_graph);
                    a.objective_ponctuation = objective_function(simulation_graph);
                    a.final_graph = simulation_graph;
                    improvement = 1;
                    break;
                }
            }
        }
    }

    return a;
}

Answer first_improving_reducing_time(Answer initial_solution, int TMAX) {
    Answer a = initial_solution;

    for(int vertex = 0; vertex < a.path.size()-2; vertex++) {
        int v0 = vertex, v1 = vertex+1, v2 = vertex+2;
        int improvement = 1;
        while(improvement) {
            improvement = 0;
            int vo = a.path[v0], vi = a.path[v1], vf = a.path[v2];
            Graph simulation_graph = a.final_graph;
            
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, vi)] = 0;
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vi, vo)] = 0;
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vi, vf)] = 0;
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vf, vi)] = 0;
            simulation_graph.v_vertices[vi] = 0;
            
            for(int i = vertex; i < a.final_graph.n_vertices; i++) {
                if(i == vo || i == vi || i == vf) continue;
                
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, i)] = 1;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vo)] = 1;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vf)] = 1;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vf, i)] = 1;
                simulation_graph.v_vertices[i] = 1;

                if(objective_function(simulation_graph) > a.objective_ponctuation) {
                //if(time_spent_function(simulation_graph) < a.time_spent) {
                    a.path[v0] = vo;
                    a.path[v1] = i;
                    a.path[v2] = vf;
                    a.time_spent = time_spent_function(simulation_graph);
                    a.objective_ponctuation = objective_function(simulation_graph);
                    a.final_graph = simulation_graph;
                    improvement = 1;
                    break;
                }

                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, i)] = 0;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vo)] = 0;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vf)] = 0;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vf, i)] = 0;
                simulation_graph.v_vertices[i] = 0;
            }
        }
    }

    return _fill_solution(a, TMAX);
}

Answer best_improving_add_edges(Answer initial_solution) {
    Answer a = initial_solution;

    for(int vertex = 0; vertex < a.path.size()-1; vertex++) {
        int v0 = vertex, v1 = vertex+1;
        int improvement = 1;
        while(improvement) {
            improvement = 0;

            Answer tmp_best;
            tmp_best.objective_ponctuation = -1;
            tmp_best.time_spent = 100000000;

            int vo = a.path[v0], vd = a.path[v1];
            Graph simulation_graph = a.final_graph;

            float max_weight = simulation_graph.weights[vo][vd];

            for(int i = 0; i < simulation_graph.n_vertices; i++) {
                if(find(a.path.begin(), a.path.end(), i) != a.path.end()) continue;
                float new_weights = simulation_graph.weights[vo][i] + simulation_graph.weights[i][vd];

                if(new_weights <= max_weight) {
                    // undo worst walk
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, vd)] = 0;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vd, vo)] = 0;

                    // new walk
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, i)] = 1;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vo)] = 1;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vd)] = 1;
                    simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vd, i)] = 1;
                    simulation_graph.v_vertices[i] = 1;
                    
                    tmp_best.path = a.path;
                    tmp_best.path.insert(tmp_best.path.begin()+v1, i);
                    
                    tmp_best.time_spent = time_spent_function(simulation_graph);
                    tmp_best.objective_ponctuation = objective_function(simulation_graph);
                    tmp_best.final_graph = simulation_graph;
                }
            }

            if(tmp_best.objective_ponctuation > a.objective_ponctuation) {
                a.path = tmp_best.path;
                a.time_spent = tmp_best.time_spent;
                a.objective_ponctuation = tmp_best.objective_ponctuation;
                a.final_graph = tmp_best.final_graph;
                improvement = 1;
            }
        }
    }

    return a;
}

Answer best_improving_reducing_time(Answer initial_solution, int TMAX) {
    Answer a = initial_solution;

    for(int vertex = 0; vertex < a.path.size()-2; vertex++) {
        int v0 = vertex, v1 = vertex+1, v2 = vertex+2;
        int improvement = 1;
        while(improvement) {
            improvement = 0;
            
            Answer tmp_best;
            tmp_best.objective_ponctuation = -1;
            tmp_best.time_spent = 100000000;

            int vo = a.path[v0], vi = a.path[v1], vf = a.path[v2];
            Graph simulation_graph = a.final_graph;
            
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, vi)] = 0;
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vi, vo)] = 0;
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vi, vf)] = 0;
            simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vf, vi)] = 0;
            
            for(int i = vertex; i < a.final_graph.n_vertices; i++) {
                if(i == vo || i == vf) continue;
                
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, i)] = 1;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vo)] = 1;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vf)] = 1;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vf, i)] = 1;

                if(objective_function(simulation_graph) > tmp_best.objective_ponctuation) {
                // if(time_spent_function(simulation_graph) < tmp_best.time_spent) {
                    tmp_best.path = a.path;
                    tmp_best.path[v0] = vo;
                    tmp_best.path[v1] = i;
                    tmp_best.path[v2] = vf;
                    tmp_best.time_spent = time_spent_function(simulation_graph);
                    tmp_best.objective_ponctuation = objective_function(simulation_graph);
                    tmp_best.final_graph = simulation_graph;
                }

                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vo, i)] = 0;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vo)] = 0;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, i, vf)] = 0;
                simulation_graph.v_edges[idx_m2v(simulation_graph.n_vertices, vf, i)] = 0;
            }

            if(tmp_best.objective_ponctuation > a.objective_ponctuation) {
            // if(tmp_best.time_spent < a.time_spent) {
                a.path = tmp_best.path;
                a.time_spent = tmp_best.time_spent;
                a.objective_ponctuation = tmp_best.objective_ponctuation;
                a.final_graph = tmp_best.final_graph;
                improvement = 1;
            }
        }
    }

    return _fill_solution(a, TMAX);
}
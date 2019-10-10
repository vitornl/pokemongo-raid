#include "graph.h"

typedef struct _answer {
    vector<int> path;
    float time_spent;
    float objective_ponctuation;
    Graph final_graph;
} Answer;

float objective_function(Graph g);

float time_spent_function(Graph g);

void print_answer(Graph g, Answer a);

Answer greedy(Graph g, int T_MAX, int v0);

Answer adaptive_greedy(Graph g, int T_MAX, int v0);

Answer random_multistart(Graph g, int T_MAX, int stopping_criterion, int v0);

Answer semi_greedy(Graph g, int T_MAX, float alpha, int v0);

Answer first_improving_add_edges(Answer initial_solution);

Answer first_improving_reducing_time(Answer initial_solution, int TMAX);

Answer best_improving_add_edges(Answer initial_solution);

Answer best_improving_reducing_time(Answer initial_solution, int TMAX);
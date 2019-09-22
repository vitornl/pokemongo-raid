#include "graph.h"

typedef struct _answer {
    vector<int> path;
    float time_spent;
    float objective_ponctuation;
} Answer;

float objective_function(Graph g);

float time_spent_function(Graph g);

Answer greedy(Graph g, int T_MAX);

Answer adaptative_greedy(Graph g, int T_MAX);

Answer random_multistart(Graph g, int T_MAX, int stopping_criterion);
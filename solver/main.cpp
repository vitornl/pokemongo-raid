#include <iostream>
#include <fstream>
#include <string>

#include "heuristics.h"
#include <mach/mach_time.h>
#include <chrono>

static int64_t get_monotonic_time (void) {
    static mach_timebase_info_data_t timebase_info;
    if (timebase_info.denom == 0) {
        mach_timebase_info (&timebase_info);
        
        if (timebase_info.numer % 1000 == 0)
            timebase_info.numer /= 1000;
        else
            timebase_info.denom *= 1000;
    
        if (timebase_info.denom % timebase_info.numer == 0) {
            timebase_info.denom /= timebase_info.numer;
            timebase_info.numer = 1;
        }
        else {
            mach_timebase_info (&timebase_info);
            assert ("should not get here");
        }
    }

    return mach_absolute_time () / timebase_info.denom;
}

int main(void) {
    Graph g;
    Answer a;
    ofstream outfile;
    int64_t micros;
    double time_taken;

    int MAX_ITERATION = 100;
    int MAX_TIME = 3600;

    srand(time(NULL));
    //g = initialize_graph("data/vertices_icarai.lhp", "data/d_time_icarai.lhp");
    g = initialize_graph("data/vertices_niteroi.lhp", "data/d_time_niteroi.lhp");

    cout << "\n\t# Random Multistart #\n";

    // for(int v0 = 0; v0 < g.n_vertices; v0++) {
    //     cout << v0 << '\n';
    //     outfile.open("output/random-multistart-" + to_string(v0) + ".txt");
    //     if (outfile.is_open()) {
    //         for(int i = 0; i < MAX_ITERATION; i++) {
    //             a = random_multistart(g, MAX_TIME, 0, v0);
    //             //print_answer(g, a);
    //             outfile << a.objective_ponctuation << ',' << a.time_spent;
    //             for(int j = 0; j < a.path.size(); j++)
    //                 outfile << ',' << a.path[j];
    //             outfile << '\n';
    //         }
    //     }
    //     outfile.close();
    // }

    micros = get_monotonic_time();
    for(int i = 0; i < MAX_ITERATION; i++) {
        a = random_multistart(g, MAX_TIME, 0, -1);
    }
    micros = get_monotonic_time() - micros;
    cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';
    
    cout << "\n\t# Greedy #\n";
    
    // for(int v0 = 0; v0 < g.n_vertices; v0++) {
    //     outfile.open("output/greedy-" + to_string(v0) + ".txt");
    //     if (outfile.is_open()) {
    //         for(int i = 0; i < MAX_ITERATION; i++) {
    //             a = greedy(g, MAX_TIME, v0);
    //             //print_answer(g, a);
    //             outfile << a.objective_ponctuation << ',' << a.time_spent;
    //             for(int j = 0; j < a.path.size(); j++)
    //                 outfile << ',' << a.path[j];
    //             outfile << '\n';
    //         }
    //     }
    //     outfile.close();
    // }
    
    micros = get_monotonic_time();
    for(int i = 0; i < MAX_ITERATION; i++) {
        a = greedy(g, MAX_TIME, -1);
    }
    micros = get_monotonic_time() - micros;
    cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';

    cout << "\n\t# Adaptive Greedy #\n";
    
    // for(int v0 = 0; v0 < g.n_vertices; v0++) {
    //     outfile.open("output/daptative-greedy-" + to_string(v0) + ".txt");
    //     if (outfile.is_open()) {
    //         for(int i = 0; i < MAX_ITERATION; i++) {
    //             a = adaptive_greedy(g, MAX_TIME, v0);
    //             //print_answer(g, a);
    //             outfile << a.objective_ponctuation << ',' << a.time_spent;
    //             for(int j = 0; j < a.path.size(); j++)
    //                 outfile << ',' << a.path[j];
    //             outfile << '\n';
    //         }
    //     }
    // outfile.close();
    // }

    micros = get_monotonic_time();
    for(int i = 0; i < MAX_ITERATION; i++) {
        a = adaptive_greedy(g, MAX_TIME, -1);
    }
    micros = get_monotonic_time() - micros;
    cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';

    cout << "\n\t# Semi-Greedy #\n";

    // for(int k = 0; k < 3; k++) {
    //     float alpha = 0.25 * (k+1);
    //     for(int v0 = 0; v0 < g.n_vertices; v0++) {
    //         cout << alpha << '|' << v0 << '\n';
    //         outfile.open("output/semi-greedy-" + to_string(k) + '-' + to_string(v0) + ".txt");
    //         if (outfile.is_open()) {
    //             for(int i = 0; i < MAX_ITERATION; i++) {
    //                 a = semi_greedy(g, MAX_TIME, alpha, v0);
    //                 //print_answer(g, a);
    //                 outfile << a.objective_ponctuation << ',' << a.time_spent;
    //                 for(int j = 0; j < a.path.size(); j++)
    //                     outfile << ',' << a.path[j];
    //                 outfile << '\n';
    //             }
    //         }
    //         outfile.close();
    //     }
    // }

    micros = get_monotonic_time();
    for(int i = 0; i < MAX_ITERATION; i++) {
        a = semi_greedy(g, MAX_TIME, 0.5, -1);
    }
    micros = get_monotonic_time() - micros;
    cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';

    return 0;
}
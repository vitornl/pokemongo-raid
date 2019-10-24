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

Answer grasp(Graph g, int MAX_TIME, float alpha, int initial_vertex, int iterations, int target) {
    ofstream outfile;
    outfile.open("output/grasp-" + to_string(alpha) + "-first.txt");
    outfile << MAX_TIME << ',' << alpha << ',' << initial_vertex << ',' << iterations << ',' << target << '\n';

    Answer a, a1;
    int64_t micros;
    double time_taken;
    
    for(int i = 0; i < iterations; i++) {
        srand(i);
        //a = random_multistart(g, MAX_TIME, 0, v0);
        //a = greedy(g, MAX_TIME, v0);
        //a = adaptive_greedy(g, MAX_TIME, v0);
        
        // micros = get_monotonic_time();
        // do {
        //     a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
        //     a = first_improving_reducing_time(a, MAX_TIME);
        //     //a = best_improving_reducing_time(a, MAX_TIME);
        //     time_taken = get_monotonic_time() - micros;
        // } while(a.path.size() < target && time_taken < 10000000);
        // cout << i << '|' << a.path.size() << '|' << time_taken << '\n';
        // outfile << i << ',' << a.path.size() << ',' << time_taken << '\n';

        a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
        a1 = first_improving_reducing_time(a, MAX_TIME);
        //a1 = best_improving_reducing_time(a, MAX_TIME);
        outfile << a.objective_ponctuation << ',' << a.time_spent << ',';
        outfile << a1.objective_ponctuation << ',' << a1.time_spent;
        for(int j = 0; j < a1.path.size(); j++)
            outfile << ',' << a1.path[j];
        outfile << '\n';
    }

    return a;
}

int main(void) {
    Graph g;
    Answer a;
    ofstream outfile;
    int64_t micros;
    double time_taken;

    int MAX_ITERATION = 100;
    int MAX_TIME = 60;//3600

    //srand(1);
    g = initialize_graph("data/vertices_icarai.lhp", "data/d_time_icarai.lhp");
    //g = initialize_graph("data/vertices_niteroi.lhp", "data/d_time_niteroi.lhp");

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

    // micros = get_monotonic_time();
    // for(int i = 0; i < MAX_ITERATION; i++) {
    //     a = random_multistart(g, MAX_TIME, 0, -1);
    // }
    // micros = get_monotonic_time() - micros;
    // cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';
    
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
    
    // micros = get_monotonic_time();
    // for(int i = 0; i < MAX_ITERATION; i++) {
    //     a = greedy(g, MAX_TIME, -1);
    // }
    // micros = get_monotonic_time() - micros;
    // cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';

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

    // micros = get_monotonic_time();
    // for(int i = 0; i < MAX_ITERATION; i++) {
    //     a = adaptive_greedy(g, MAX_TIME, -1);
    // }
    // micros = get_monotonic_time() - micros;
    // cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';

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

    // micros = get_monotonic_time();
    // for(int i = 0; i < MAX_ITERATION; i++) {
    //     a = semi_greedy(g, MAX_TIME, 0.5, -1);
    // }
    // micros = get_monotonic_time() - micros;
    // cout << "Time: " << (double) micros / 1000000 << " micros" << '\n';

    // cout << "\n\t# Local-Search #\n";

    // float p_r = 0;
    // float p_e = 0;
    // float p_t = 0;
    // float m_r = 0;
    // float m_e = 0;
    // float m_t = 0;
    // float t_e = 0;
    // float t_t = 0;

    // for(int i = 0; i < 500; i++) {
    //     int v0 = 0;
    //     //a = random_multistart(g, MAX_TIME, 0, v0);
    //     //a = greedy(g, MAX_TIME, v0);
    //     //a = adaptive_greedy(g, MAX_TIME, v0);
    //     a = semi_greedy(g, MAX_TIME, 0.75, v0);
    //     p_r += a.path.size();
    //     m_r = a.path.size() > m_r ? a.path.size() : m_r;

    //     // print_answer(g, a);
    //     // cout << "---\n";
    //     micros = get_monotonic_time();
    //     Answer a1 = best_improving_add_edges(a);
    //     p_e += a1.path.size();
    //     m_e = a1.path.size() > m_e ? a1.path.size() : m_e;
    //     t_e += get_monotonic_time() - micros;

    //     // print_answer(g, a1);
    //     // cout << "---\n";
    //     micros = get_monotonic_time();
    //     a1 = best_improving_reducing_time(a, MAX_TIME);
    //     p_t += a1.path.size();
    //     m_t = a1.path.size() > m_t ? a1.path.size() : m_t;
    //     t_t += get_monotonic_time() - micros;
    //     // print_answer(g, a1);
    //     // cout << "---\n";
    //     // a1 = best_improving(a, MAX_TIME);
    //     // print_answer(g, a1);
    // }
    // cout << p_r/500 << '\n';
    // cout << p_e/500 << '\n';
    // cout << p_t/500 << '\n';
    // cout << "---\n";
    // cout << m_r << '\n';
    // cout << m_e << '\n';
    // cout << m_t << '\n';
    // cout << "---\n";
    // cout << t_e/500 << '\n';
    // cout << t_t/500 << '\n';
    grasp(g, MAX_TIME, 0.75, 0, 500, 13);

    return 0;
}
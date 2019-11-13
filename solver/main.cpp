#include <iostream>
#include <fstream>
#include <string>

#include "heuristics.h"
#include <mach/mach_time.h>
#include <chrono>

#include "stdio.h"
#include "stdlib.h"

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

Answer _random_multistart(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, int initial_vertex) {
    cout << "\n\t# Random Multistart (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << to_string(initial_vertex) << ")#\n";
    
    Answer a, best;
    ofstream outfile;
    int64_t micros, ttime=0;

    best.path.push_back(-1);

    if(to_file) {
        outfile.open("output/random_multistart-" + g.name + '-' + to_string(initial_vertex) + ".txt");
    }

    for(int i = 0; i < MAX_ITERATIONS; i++) {
        micros = get_monotonic_time();
        a = random_multistart(g, MAX_TIME, 0, initial_vertex);
        micros = get_monotonic_time() - micros;
        ttime += micros;
        if(to_file) {
            outfile << a.objective_ponctuation << ',' << a.time_spent;
            for(int j = 0; j < a.path.size(); j++)
                outfile << ',' << a.path[j];
            outfile << '\n';
        }
        if(best.path.size() < a.path.size()) best = a;
        else if(best.path.size() == a.path.size()) if(a.time_spent < best.time_spent) best = a;
    }
    if(to_file) outfile.close();

    cout << "Time: " << (double) ttime/MAX_ITERATIONS << " micros" << '\n';

    return best;
}

Answer _greedy(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, int initial_vertex) {
    cout << "\n\t# Greedy (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << to_string(initial_vertex) << ")#\n";
    
    Answer a, best;
    ofstream outfile;
    int64_t micros, ttime=0;

    best.path.push_back(-1);

    if(to_file) {
        outfile.open("output/greedy-" + g.name + '-' + to_string(initial_vertex) + ".txt");
    }

    for(int i = 0; i < MAX_ITERATIONS; i++) {
        micros = get_monotonic_time();
        a = greedy(g, MAX_TIME, initial_vertex);
        micros = get_monotonic_time() - micros;
        ttime += micros;
        if(to_file) {
            outfile << a.objective_ponctuation << ',' << a.time_spent;
            for(int j = 0; j < a.path.size(); j++)
                outfile << ',' << a.path[j];
            outfile << '\n';
        }
        if(best.path.size() < a.path.size()) best = a;
        else if(best.path.size() == a.path.size()) if(a.time_spent < best.time_spent) best = a;
    }
    if(to_file) outfile.close();

    cout << "Time: " << (double) ttime/MAX_ITERATIONS << " micros" << '\n';

    return best;
}

Answer _adaptative_greedy(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, int initial_vertex) {
    cout << "\n\t# Adaptive Greedy (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << to_string(initial_vertex) << ")#\n";
    
    Answer a, best;
    ofstream outfile;
    int64_t micros, ttime=0;

    best.path.push_back(-1);

    if(to_file) {
        outfile.open("output/adaptive_greedy-" + g.name + '-' + to_string(initial_vertex) + ".txt");
    }

    for(int i = 0; i < MAX_ITERATIONS; i++) {
        micros = get_monotonic_time();
        a = adaptive_greedy(g, MAX_TIME, initial_vertex);
        micros = get_monotonic_time() - micros;
        ttime += micros;
        if(to_file) {
            outfile << a.objective_ponctuation << ',' << a.time_spent;
            for(int j = 0; j < a.path.size(); j++)
                outfile << ',' << a.path[j];
            outfile << '\n';
        }
        if(best.path.size() < a.path.size()) best = a;
        else if(best.path.size() == a.path.size()) if(a.time_spent < best.time_spent) best = a;
    }
    if(to_file) outfile.close();

    cout << "Time: " << (double) ttime/MAX_ITERATIONS << " micros" << '\n';

    return best;
}

Answer _semi_greedy(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, int initial_vertex, float alpha) {
    cout << "\n\t# Semi-Greedy (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << to_string(initial_vertex)
         << ',' << to_string(alpha) << ")#\n";
    
    Answer a, best;
    ofstream outfile;
    int64_t micros, ttime=0;

    best.path.push_back(-1);

    if(to_file) {
        outfile.open("output/semi_greedy-" + g.name + '-' + to_string(initial_vertex)  + '-' + to_string(alpha) + ".txt");
    }

    for(int i = 0; i < MAX_ITERATIONS; i++) {
        micros = get_monotonic_time();
        a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
        micros = get_monotonic_time() - micros;
        ttime += micros;
        if(to_file) {
            outfile << a.objective_ponctuation << ',' << a.time_spent;
            for(int j = 0; j < a.path.size(); j++)
                outfile << ',' << a.path[j];
            outfile << '\n';
        }
        if(best.path.size() < a.path.size()) best = a;
        else if(best.path.size() == a.path.size()) if(a.time_spent < best.time_spent) best = a;
    }
    if(to_file) outfile.close();

    cout << "Time: " << (double) ttime/MAX_ITERATIONS << " micros" << '\n';

    return best;
}

Answer _local_search(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, Answer a, char improve, char strategy) {

    cout << "\n\t# Local Search (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << improve << ',' << strategy << ")#\n";

    Answer ls_a;
    ofstream outfile;
    int64_t micros, ttime=0;

    if(to_file) {
        outfile.open("output/local_search-" + g.name + '-' + improve  + '-' + strategy + ".txt");
    }

    for(int i = 0; i < MAX_ITERATIONS; i++) {
        micros = get_monotonic_time();
        if(improve == 'f') {
            if(strategy == 'e') ls_a = first_improving_add_edges(a);
            else ls_a = first_improving_reducing_time(a, MAX_TIME);
        }
        else {
            if(strategy == 'e') ls_a = best_improving_add_edges(a);
            else ls_a = best_improving_reducing_time(a, MAX_TIME);
        }
        micros = get_monotonic_time() - micros;
        ttime += micros;
    }
    
    if(to_file) {
        outfile << ls_a.objective_ponctuation << ',' << ls_a.time_spent;
        for(int j = 0; j < ls_a.path.size(); j++)
            outfile << ',' << ls_a.path[j];
        outfile << '\n';
        outfile.close();
    }

    cout << "Time: " << (double) ttime/MAX_ITERATIONS << " micros" << '\n';
    return ls_a;
}

Answer _grasp(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, int initial_vertex, int target, float alpha) {
    cout << "\n\t# GRASP (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << to_string(initial_vertex)
         << ',' << to_string(target) << ")#\n";

    ofstream outfile;
    int64_t micros, ttime=0;

    if(to_file) {
        outfile.open("output/grasp-" + to_string(target) + "-first.txt");
        outfile << MAX_TIME << ',' << alpha << ',' << initial_vertex << ',' << MAX_ITERATIONS << ',' << target << '\n';
    }
    
    Answer a, a1;
    
    srand(0);
    Answer best = best_improving_reducing_time(semi_greedy(g, MAX_TIME, alpha, initial_vertex), MAX_TIME);
    for(int i = 1; i < MAX_ITERATIONS; i++) {
        srand(i);
        // a = random_multistart(g, MAX_TIME, 0, v0);
        // a = greedy(g, MAX_TIME, v0);
        // a = adaptive_greedy(g, MAX_TIME, v0);
        
        // micros = get_monotonic_time();
        // do {
        //     a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
        //     a = first_improving_reducing_time(a, MAX_TIME);
        //     //a = best_improving_reducing_time(a, MAX_TIME);
        //     time_taken = get_monotonic_time() - micros;
        // } while(a.path.size() < target && time_taken < 10000000);
        // cout << i << '|' << a.path.size() << '|' << time_taken << '\n';
        // outfile << i << ',' << a.path.size() << ',' << time_taken << '\n';

        micros = get_monotonic_time();
        a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
        // a1 = first_improving_reducing_time(a, MAX_TIME);
        a = best_improving_reducing_time(a, MAX_TIME);
        ttime += get_monotonic_time() - micros;

        if(best.path.size() < a.path.size()) best = a;
        else if(best.path.size() == a.path.size() && a.time_spent < best.time_spent) best = a;

    }

    if(to_file) {
        outfile << a.objective_ponctuation << ',' << a.time_spent << ',';
        outfile << a1.objective_ponctuation << ',' << a1.time_spent;
        for(int j = 0; j < a1.path.size(); j++)
            outfile << ',' << a1.path[j];
        outfile << '\n';
    }

    cout << fixed << "Time: " << (double) ttime << " micros" << '\n';

    return best;
}

Answer _foward_pr(Answer a, Answer sline) {
    
    return a;
}

Answer _backward_pr(Answer a, Answer sline) {
    
    return a;
}

vector<Answer> _update_eliteset(Answer a, vector<Answer> elites, int ELITES_MAX) {
    if(elites.size() < ELITES_MAX) {
        if(elites.size() == 0) elites.push_back(a);
        else {
            int delta_min = 999999;
            for(int i = 0; i < elites.size(); i++) {
                int delta = 0;
                for(int j = 0; j < a.path.size(); j ++) {
                    vector<int>::iterator it = find(elites[i].path.begin(), elites[i].path.end(), a.path[j]);
                    if(it != elites[i].path.end()) delta++;
                }
                if(delta < delta_min) delta_min = delta;
            }
            if(delta_min > 0) elites.push_back(a);
        }
    }
    else {
        int min_path = 999999;
        for(int i = 0; i < elites.size(); i++) {
            if(min_path > elites[i].path.size())
                min_path = elites[i].path.size();
        }
        int delta_min = 999999;
        for(int i = 0; i < elites.size(); i++) {
            int delta = 0;
            for(int j = 0; j < a.path.size(); j ++) {
                vector<int>::iterator it = find(elites[i].path.begin(), elites[i].path.end(), a.path[j]);
                if(it != elites[i].path.end()) delta++;
            }
            if(delta < delta_min) delta_min = delta;
        }
        if(a.path.size() > min_path && delta_min > 0) {
            int delta_min = 999999;
            int position = -1;
            for(int i = 0; i < elites.size(); i++) {
                if(elites[i].path.size() >= a.path.size()) continue;
                int delta = 0;
                for(int j = 0; j < a.path.size(); j ++) {
                    vector<int>::iterator it = find(elites[i].path.begin(), elites[i].path.end(), a.path[j]);
                    if(it != elites[i].path.end()) delta++;
                }
                if(delta < delta_min) {
                    delta_min = delta;
                    position = i;
                }
            }
            elites[position] = a;
        }
    }

    return elites;
}

Answer _grasp_pr(Graph g, int MAX_TIME, int MAX_ITERATIONS, bool to_file, int initial_vertex, int target, float alpha, int ELITES_MAX) {
    cout << "\n\t# GRASP+PR (" << g.name << ',' << to_string(MAX_TIME)
         << ',' << to_string(MAX_ITERATIONS) << ',' << to_string(initial_vertex)
         << ',' << to_string(target) << ")#\n";

    ofstream outfile;
    int64_t micros, ttime=0;

    if(to_file) {
        outfile.open("output/grasp_pr-bw-" + to_string(target) + "-best.txt");
        outfile << MAX_TIME << ',' << alpha << ',' << initial_vertex << ',' << MAX_ITERATIONS << ',' << target << '\n';
    }
    
    vector<Answer> elites;
    Answer a, a1;
    
    srand(0);
    for(int i = 0; i < MAX_ITERATIONS; i++) {
        srand(i);
        // a = random_multistart(g, MAX_TIME, 0, v0);
        // a = greedy(g, MAX_TIME, v0);
        // a = adaptive_greedy(g, MAX_TIME, v0);
        
        micros = get_monotonic_time();
        do {
            a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
            a = best_improving_reducing_time(a, MAX_TIME);
            if(elites.size() > 0) {
                Answer sline = elites[rand()%elites.size()];
                a = _backward_pr(a, sline);
            }
            elites = _update_eliteset(a, elites, ELITES_MAX);
            ttime = get_monotonic_time() - micros;
        } while(a.path.size() < target && ttime < 10000000);
        cout << i << '|' << a.path.size() << '|' << ttime << '\n';
        outfile << i << ',' << a.path.size() << ',' << ttime << '\n';

        // micros = get_monotonic_time();
        
        // a = semi_greedy(g, MAX_TIME, alpha, initial_vertex);
        // a = best_improving_reducing_time(a, MAX_TIME);
        
        // if(elites.size() > 0) {
        //     Answer sline = elites[rand()%elites.size()];
        //     //a = _foward_pr(a, sline);
        // }
        // elites = _update_eliteset(a, elites, ELITES_MAX);

        // ttime += get_monotonic_time() - micros;


    }

    if(to_file) {
        outfile << a.objective_ponctuation << ',' << a.time_spent << ',';
        outfile << a1.objective_ponctuation << ',' << a1.time_spent;
        for(int j = 0; j < a1.path.size(); j++)
            outfile << ',' << a1.path[j];
        outfile << '\n';
    }

    micros = get_monotonic_time();

    Answer best = elites[0];
    for(int i = 1; i < elites.size(); i++) {
        if(best.path.size() < elites[i].path.size()) best = elites[i];
        else if(best.path.size() == elites[i].path.size() && elites[i].time_spent < best.time_spent) best = elites[i];
    }

    ttime += get_monotonic_time() - micros;

    cout << fixed << "Time: " << (double) ttime << " micros" << '\n';

    return best;
}

int main(void) {
    Graph g;
    Answer a;
    ofstream outfile;
    int64_t micros;
    double time_taken;

    int MAX_ITERATIONS = 500;
    int MAX_TIME = 60;//3600
    int INITIAL_VERTEX = 0;
    int ELITES_MAX = 10;
    
    srand(1);

    g = initialize_graph("icarai", "data/vertices_icarai.lhp", "data/d_time_icarai.lhp");
    //g = initialize_graph("niteroi", "data/vertices_niteroi.lhp", "data/d_time_niteroi.lhp");
    
    int i = 3; { //for(int i = 0; i < g.n_vertices; i++) {
        INITIAL_VERTEX = i;
        // a = _random_multistart(g, MAX_TIME, MAX_ITERATIONS, false, INITIAL_VERTEX);
        // print_answer(g, a, true);

        // Answer ak = _greedy(g, MAX_TIME, MAX_ITERATIONS, false, INITIAL_VERTEX);
        // print_answer(g, ak, true);
        
        // a = _adaptative_greedy(g, MAX_TIME, MAX_ITERATIONS, false, INITIAL_VERTEX);
        // print_answer(g, a, true);
        
        // for(int k = 0; k < 3; k++) {
        //     float alpha = 0.25 * (k+1);
        //     a = _semi_greedy(g, MAX_TIME, MAX_ITERATIONS, false, INITIAL_VERTEX, alpha);
        //     print_answer(g, a, true);
        // }

        // Answer a1 = _local_search(g, MAX_TIME, MAX_ITERATIONS, false, ak, 'f', 'e');
        // print_answer(g, a1, true);
        // a1 = _local_search(g, MAX_TIME, MAX_ITERATIONS, false, ak, 'f', 't');
        // print_answer(g, a1, true);
        // a1 = _local_search(g, MAX_TIME, MAX_ITERATIONS, false, ak, 'b', 'e');
        // print_answer(g, a1, true);
        // a1 = _local_search(g, MAX_TIME, MAX_ITERATIONS, false, ak, 'b', 't');
        // print_answer(g, a1, true);
        
        // Answer a1 = _grasp(g, MAX_TIME, MAX_ITERATIONS, false, INITIAL_VERTEX, -1, 0.75);
        Answer a1 = _grasp_pr(g, MAX_TIME, MAX_ITERATIONS, true, INITIAL_VERTEX, 13, 0.75, ELITES_MAX);
        print_answer(g, a1, true);
    }

    // cout << "\n\t# Local-Search #\n";
    {
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
    }

    // _grasp(g, MAX_TIME, 500, FALSE, 0, 13, 0.75);

    return 0;
}
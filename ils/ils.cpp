#include "../operation.h"
#include "ils.h"
#include "utils.h"
#include "local_search.h"
#include "perturbation.h"
#include "../image_generation/gantt_chart_generation.h"
#include <bits/stdc++.h>

using namespace std;

Matrix<int> iterated_local_search(Matrix<int> initial_solution, double time_limit_seconds) { // following the pseudocode
    using Clock = chrono::steady_clock;
    auto start_time = Clock::now();
    
    cout << "=== Starting Iterated Local Search ===" << endl;
    
    // step 1: s := Construção_Gulosa()
    Matrix<int> s = initial_solution;
    int s_makespan = calculate_makespan(s);
    
    // step 2: s* := Busca_Local(N(.), s)
    cout << "Performing initial local search..." << endl;
    Matrix<int> s_star = local_search(s, s_makespan);
    int s_star_makespan = calculate_makespan(s_star);
    
    cout << "Initial solution makespan: " << s_makespan << endl;
    cout << "After initial local search: " << s_star_makespan << endl;
    
    // generate initial Gantt chart
    generate_gantt_chart(s_star, 0, s_star_makespan, "ils");
    
    int iteration = 0;
    int perturbation_strength = 1;
    int stagnation_counter = 0;
    const int max_stagnation = 5; // Increase perturbation strength after this many iterations without improvement
    
    // step 3: check for stop condition
    while (true) {
    auto now = Clock::now();
    if (std::chrono::duration<double>(now - start_time).count() > time_limit_seconds) {
        cout << "Time limit reached. Stopping ILS." << endl;
        break;
    }

    iteration++;
    cout << "\n--- ILS Iteration " << iteration << " ---" << endl;
    cout << "Current best makespan: " << s_star_makespan << endl;

    // trying to generate valid configurations
    const int max_tries = 10000;
    int tries = 0;
    Matrix<int> s_perturbed;
    int s_perturbed_makespan;

    do {
        s_perturbed = perturbation(s_star, perturbation_strength);
        s_perturbed_makespan = calculate_makespan(s_perturbed);
        tries++;
        if (s_perturbed_makespan == INT_MAX) {
            cout << "Perturbation generated invalid schedule (cycle detected), retrying (" << tries << "/" << max_tries << ")..." << endl;
        }
    } while (s_perturbed_makespan == INT_MAX && tries < max_tries);

    if (s_perturbed_makespan == INT_MAX) {
        cout << "Failed to generate valid perturbation after " << max_tries << " tries. Stopping ILS." << endl;
        break;
    }

    cout << "After perturbation: " << s_perturbed_makespan << endl;

    if (iteration <= 5) {
        generate_gantt_chart(s_perturbed, iteration * 10, s_perturbed_makespan, "ils_perturbed");
    }

    Matrix<int> s_prime = local_search(s_perturbed, s_perturbed_makespan);
    int s_prime_makespan = calculate_makespan(s_prime);

    cout << "After local search: " << s_prime_makespan << endl;

    if (iteration <= 5) {
        generate_gantt_chart(s_prime, iteration * 10 + 1, s_prime_makespan, "ils_improved");
    }

    if (s_prime_makespan < s_star_makespan) {
        s_star = s_prime;
        s_star_makespan = s_prime_makespan;
        stagnation_counter = 0;
        perturbation_strength = 1;

        cout << "*** NEW BEST SOLUTION FOUND! Makespan: " << s_star_makespan << " ***" << endl;
        generate_gantt_chart(s_star, iteration, s_star_makespan, "ils_best");
    } else {
        cout << "No improvement found." << endl;
        stagnation_counter++;
    }

    cout << "Iteration " << iteration << " completed. Best makespan so far: " << s_star_makespan << endl;
}
    
    cout << "\n=== ILS Completed ===" << endl;
    cout << "Total iterations: " << iteration << endl;
    cout << "Final best makespan: " << s_star_makespan << endl;
    
    return s_star;
}

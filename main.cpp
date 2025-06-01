#include <iostream>
#include <chrono>
#include "./file_management/instance.h"
#include "./ils/utils.h"
#include "./ils/ils.h"
#include "./ils/local_search.h"
#include "./ils/perturbation.h"
#include "./ils/greedy_construction.h"
#include "./image_generation/gantt_chart_generation.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <instance_file>" << endl;
        return 1;
    }

    string instance_file = argv[1];

    if (!read_instance_from_file(instance_file)) {
        cerr << "Failed to load instance. Exiting." << endl;
        return 1;
    }

    print_instance();

    double time_limit = 30.0; // in seconds

    cout << "Constructing initial solution with greedy heuristic..." << endl;
    auto start = chrono::high_resolution_clock::now();
    Matrix<int> initial_solution = greedy_schedule_constructor();
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> greedy_duration = end - start;
    int initial_makespan = calculate_makespan(initial_solution);

    cout << "Initial greedy solution makespan: " << initial_makespan << endl;
    cout << "Greedy construction time: " << greedy_duration.count() << "s" << endl;

    // Run ILS
    Matrix<int> best_solution = iterated_local_search(initial_solution, time_limit);
    int final_makespan = calculate_makespan(best_solution);

    // Final Gantt chart
    generate_gantt_chart(best_solution, 999, final_makespan, "final");

    cout << "\nFinal best makespan after ILS: " << final_makespan << endl;

    return 0;
}

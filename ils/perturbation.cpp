#include "../operation.h"
#include "utils.h"
#include <bits/stdc++.h>
#include "perturbation.h"

using namespace std;

Matrix<int> perturbation(const Matrix<int>& current_schedule, int perturbation_strength){ // the perturbation strength makes it suitable to perform a VNS if required

    cout << "Applying perturbation with strength " << perturbation_strength << "." << endl;
    
    // get current machine orders
    vector<vector<pair<int, int>>> machine_orders = get_true_machine_processing_orders(current_schedule);
    
    // perform multiple random non-consecutive swaps. For ILS we will set perturbation_strength to 1.
    for (int swap_count = 0; swap_count < perturbation_strength; ++swap_count) {

        // select a random machine with at least 3 operations (to ensure non-consecutive swaps)
        vector<int> eligible_machines;
        for (int m = 0; m < num_machines; ++m) {
            if (machine_orders[m].size() >= 3) {
                eligible_machines.push_back(m);
            }
        }
        
        if (eligible_machines.empty()) {
            cout << "  No eligible machines for non-consecutive swap" << endl;
            continue;
        }
        
        // select random machine
        uniform_int_distribution<int> machine_dist(0, eligible_machines.size() - 1);
        int selected_machine = eligible_machines[machine_dist(gen)];
        
        // select two non-consecutive positions
        int ops_count = machine_orders[selected_machine].size();
        uniform_int_distribution<int> pos_dist(0, ops_count - 1);
        
        int pos1 = pos_dist(gen);
        int pos2;
        
        // ensure pos2 is not consecutive to pos1
        do {
            pos2 = pos_dist(gen);
        } while (abs(pos1 - pos2) <= 1 || pos1 == pos2);
        
        if (pos1 > pos2) swap(pos1, pos2);
        
        cout << "  Swapping positions " << pos1 << " and " << pos2 << " on machine " << selected_machine << endl;
        
        // perform the swap
        swap(machine_orders[selected_machine][pos1], machine_orders[selected_machine][pos2]);
    }
    
    // reconstruct schedule with perturbed machine orders
    return reconstruct_schedule_from_machine_orders(machine_orders);
}

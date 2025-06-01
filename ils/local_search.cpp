#include "../operation.h"
#include "utils.h"
#include "local_search.h"
#include <bits/stdc++.h>

using namespace std;


Matrix<int> local_search(const Matrix<int>& current_schedule_start_times, int current_makespan, bool use_first_improvement){ // we will perform the swap movement on adjacent operations in a machine 
                                                                                                // provided that the channges dont affect the feasibility of a solution
    
    Matrix<int> best_schedule = current_schedule_start_times;
    int best_makespan_val = current_makespan;
    bool improved = false; 

    // compute the job operation-to-machine assignments for easier reference

    vector<vector<int>> job_op_machine_assignments(num_jobs, vector<int>(num_machines));
    for (int j = 0; j < num_jobs; ++j) {
        for (int op = 0; op < num_machines; ++op) {
            job_op_machine_assignments[j][op] = jobs[j][op].machine;
        }
    }

    // cache machine orders to avoid recalculation
    vector<vector<pair<int, int>>> current_machine_true_orders =
        get_true_machine_processing_orders(current_schedule_start_times);

    // process machines in the order of highest load first (paper requisite)
    vector<pair<int, int>> machine_loads;
    for (int m = 0; m < num_machines; ++m) {
        int load = 0;
        for (int j = 0; j < num_jobs; ++j) {
            for (int op = 0; op < num_machines; ++op) {
                if (jobs[j][op].machine == m) {
                    load += jobs[j][op].duration;
                }
            }
        }
        machine_loads.push_back({load, m});
    }
    sort(machine_loads.begin(), machine_loads.end(), greater<pair<int, int>>());
    
    // loop through machines in order of load
    for (const auto& load_machine_pair : machine_loads) {
        int m_swapped_idx = load_machine_pair.second;
        
        if (current_machine_true_orders[m_swapped_idx].size() < 2) {
            continue;
        }

        for (size_t i = 0; i < current_machine_true_orders[m_swapped_idx].size() - 1; ++i) {

            // create new order by swapping consecutive operations
            vector<pair<int, int>> new_order_on_m = current_machine_true_orders[m_swapped_idx];
            
            // get the job IDs and operation sequence IDs for the swapped operations
            int job_id1 = new_order_on_m[i].first;
            int op_seq1 = new_order_on_m[i].second;
            int job_id2 = new_order_on_m[i+1].first;
            int op_seq2 = new_order_on_m[i+1].second;
            
            // operations must be from different jobs to be swappable
            if (job_id1 == job_id2) {
                continue;
            }
            
            // perform the swap
            swap(new_order_on_m[i], new_order_on_m[i + 1]);

            // create machine orders for reconstruction
            vector<vector<pair<int, int>>> machine_processing_order = current_machine_true_orders;
            machine_processing_order[m_swapped_idx] = new_order_on_m;
            
            // reconstruct schedule
            Matrix<int> temp_schedule = reconstruct_schedule_from_machine_orders(machine_processing_order);
            
            // check if reconstruction was successful
            bool valid_schedule = true;
            for (int j = 0; j < num_jobs && valid_schedule; ++j) {

                for (int op = 0; op < num_machines && valid_schedule; ++op) {

                    if (temp_schedule[j][op] == -1) {

                        valid_schedule = false;

                    }

                }

            }
            
            if (!valid_schedule) continue;
            
            // Evaluate new schedule
            int new_makespan = calculate_makespan(temp_schedule);
            
            if (new_makespan < best_makespan_val) {

                best_makespan_val = new_makespan;
                best_schedule = temp_schedule;
                improved = true;
                
                // First improvement strategy
                if (use_first_improvement) {
                    return best_schedule;
                }

            }

        }

    }

    return best_schedule;
}
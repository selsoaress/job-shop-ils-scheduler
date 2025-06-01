#include "../operation.h"
#include "greedy_construction.h"
#include <bits/stdc++.h>

using namespace std;

Matrix<int> greedy_schedule_constructor(){

        Matrix<int> start_times(num_jobs, vector<int>(num_machines, -1)); // where we will store our times through the execution

        // SIMULATION VARIABLES:
        vector<int> machine_available(num_machines, 0); // when the machine is ready for use
        vector<int> job_available(num_jobs, 0);
        vector<int> next_operation(num_jobs, 0);

        int currently_scheduled = 0;
        int total_operations = num_jobs * num_machines; // in case of shorter production schedules for certain jobs, you might use durations as 0

        // CONSTRUCTION:

        while(currently_scheduled < total_operations){
            int selected_job = -1;
            int selected_machine = -1;
            int min_duration = INT_MAX;

            // we choose using the SPT criterium

            for(int j = 0; j< num_jobs; ++j){
                int op_index = next_operation[j];

                if(op_index >= num_machines) continue; // schedule ended for this job

                Operation& op = jobs[j][op_index];

                int ready_time = max(machine_available[op.machine], job_available[j]);

                if (op.duration < min_duration && job_available[j] <= ready_time && machine_available[op.machine] <= ready_time) { // if time constraints for next machine are satisfied, select job j

                    selected_job = j;
                    selected_machine = op.machine;
                    min_duration = op.duration;

                }   
            }

            // if no operation is ready to be applied, we advance the time:

            if (selected_job == -1) {

                int earliest = INT_MAX;

                    for (int j = 0; j < num_jobs; ++j) {
                        if (next_operation[j] < num_machines) {
                            earliest = min(earliest, job_available[j]);
                        }
                    }

                    for (int m = 0; m < num_machines; ++m) {
                        earliest = min(earliest, machine_available[m]);
                    }

                    for (int m = 0; m < num_machines; ++m) {
                        machine_available[m] = max(machine_available[m], earliest);
                    }

                    for (int j = 0; j < num_jobs; ++j) {
                        job_available[j] = max(job_available[j], earliest);
                    }

                continue;
        }

        // we can execute the selected operation if there is any:

        int op_index = next_operation[selected_job];
        Operation& op = jobs[selected_job][op_index];

            int start_time = max(machine_available[selected_machine], job_available[selected_job]);
        int end_time = start_time + op.duration;

        start_times[selected_job][op_index] = start_time;
        machine_available[selected_machine] = end_time;
        job_available[selected_job] = end_time;
        next_operation[selected_job]++;
        currently_scheduled++;

        }

        return start_times;

}
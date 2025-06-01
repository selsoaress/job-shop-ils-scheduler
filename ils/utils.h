#ifndef UTILS_H
#define UTILS_H

#include "../operation.h"
#include <bits/stdc++.h>

using namespace std;

// auxiliary structure to perform ordering
struct OpInfoForMachineSort {
    int job_id;
    int op_sequence_id;
    int start_time;
    int original_pos_in_machine_list;

    bool operator<(const OpInfoForMachineSort& other) const;
};

// utilitary functions:
int calculate_makespan(const Matrix<int>& schedule);

vector<vector<pair<int, int>>> get_true_machine_processing_orders(
    const Matrix<int>& schedule_start_times);

Matrix<int> reconstruct_schedule_from_machine_orders(
    const vector<vector<pair<int, int>>>& machine_orders);

#endif // UTILS_H

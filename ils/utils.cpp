#include "../operation.h"
#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

// Verifica se o schedule tem algum tempo inválido (-1)
bool is_valid_schedule(const Matrix<int>& schedule) {
    for (const auto& job_row : schedule) {
        for (int start_time : job_row) {
            if (start_time == -1) {
                return false;
            }
        }
    }
    return true;
}

bool OpInfoForMachineSort::operator<(const OpInfoForMachineSort& other) const {
    // Exemplo de comparação por start_time
    return start_time < other.start_time;
}

int calculate_makespan(const Matrix<int>& schedule){
    if (!is_valid_schedule(schedule)) {
        // Schedule inválido, retorna valor alto para evitar escolher
        return INT_MAX;
    }

    int makespan = 0;
    for(int j = 0; j < num_jobs; ++j){
        int last_op_end = 0;
        for(int m = 0; m < num_machines; ++m){
            Operation op = jobs[j][m];
            int op_start = schedule[j][m];
            int op_end = op_start + op.duration;
            last_op_end = max(last_op_end, op_end);
        }
        makespan = max(makespan, last_op_end);
    }
    return makespan;
}

vector<vector<pair<int, int>>> get_true_machine_processing_orders( // to generate vectors for each machine with the correct execution times and orders
    const Matrix<int>& schedule_start_times) {

    vector<vector<OpInfoForMachineSort>> ops_on_each_machine(num_machines);

    for (int j = 0; j < num_jobs; ++j) {
        for (int m_op_seq = 0; m_op_seq < num_machines; ++m_op_seq) {
            if (schedule_start_times[j][m_op_seq] != -1) {
                const auto& op_data = jobs[j][m_op_seq];
                ops_on_each_machine[op_data.machine].push_back({
                    j,
                    m_op_seq,
                    schedule_start_times[j][m_op_seq],
                    (int)ops_on_each_machine[op_data.machine].size()
                });
            }
        }
    }

    vector<vector<pair<int, int>>> final_machine_orders(num_machines);
    for (int m_idx = 0; m_idx < num_machines; ++m_idx) {
        sort(ops_on_each_machine[m_idx].begin(), ops_on_each_machine[m_idx].end());
        for (const auto& sorted_op_info : ops_on_each_machine[m_idx]) {
            final_machine_orders[m_idx].push_back({sorted_op_info.job_id, sorted_op_info.op_sequence_id});
        }
    }

    return final_machine_orders;
}

/*

Theoretical Explanation:

We can represent the order of the operations based on a DAG (Directed Acyclic Graph). We perform
the topological sort on the graph to order the schedules into the machines and check for cycles
for correctness.



*/


Matrix<int> reconstruct_schedule_from_machine_orders(const vector<vector<pair<int, int>>>& machine_orders) {
    Matrix<int> new_schedule(num_jobs, vector<int>(num_machines, -1));
    
    // Montar lista de máquinas de cada operação do job
    vector<vector<int>> job_op_machine_assignments(num_jobs, vector<int>(num_machines));
    for (int j = 0; j < num_jobs; ++j) {
        for (int op = 0; op < num_machines; ++op) {
            job_op_machine_assignments[j][op] = jobs[j][op].machine;
        }
    }
    
    // Criar grafo de precedências
    vector<vector<int>> adj_list(num_jobs * num_machines);
    vector<int> in_degree(num_jobs * num_machines, 0);
    
    // Precedência dentro do job (ordem sequencial)
    for (int j = 0; j < num_jobs; ++j) {
        for (int op = 1; op < num_machines; ++op) {
            int from_node = j * num_machines + (op - 1);
            int to_node = j * num_machines + op;
            adj_list[from_node].push_back(to_node);
            in_degree[to_node]++;
        }
    }
    
    // Precedência entre operações da mesma máquina
    for (int m = 0; m < num_machines; ++m) {
        for (size_t idx = 1; idx < machine_orders[m].size(); ++idx) {
            int prev_job = machine_orders[m][idx-1].first;
            int prev_op = machine_orders[m][idx-1].second;
            int curr_job = machine_orders[m][idx].first;
            int curr_op = machine_orders[m][idx].second;
            
            int from_node = prev_job * num_machines + prev_op;
            int to_node = curr_job * num_machines + curr_op;
            
            adj_list[from_node].push_back(to_node);
            in_degree[to_node]++;
        }
    }
    
    // Ordenação topológica
    queue<int> q;
    vector<int> sorted_nodes;
    for (int node = 0; node < num_jobs * num_machines; ++node) {
        if (in_degree[node] == 0) {
            q.push(node);
        }
    }
    
    while (!q.empty()) {
        int curr_node = q.front();
        q.pop();
        sorted_nodes.push_back(curr_node);
        for (int nxt : adj_list[curr_node]) {
            in_degree[nxt]--;
            if (in_degree[nxt] == 0) q.push(nxt);
        }
    }
    
    if ((int)sorted_nodes.size() != num_jobs * num_machines) {
        cout << "ERROR: Cycle detected in dependency graph during reconstruct_schedule_from_machine_orders." << endl;
        return Matrix<int>(num_jobs, vector<int>(num_machines, -1)); // Schedule inválido
    }
    
    // Horários de fim das máquinas
    unordered_map<int, int> machine_finish_times;
    
    // Preencher o schedule na ordem topológica
    for (int node : sorted_nodes) {
        int job_id = node / num_machines;
        int op_seq = node % num_machines;
        int machine = job_op_machine_assignments[job_id][op_seq];
        
        int job_ready_time = 0;
        if (op_seq > 0) {
            job_ready_time = new_schedule[job_id][op_seq - 1] + jobs[job_id][op_seq - 1].duration;
        }
        
        int machine_ready_time = machine_finish_times[machine];
        int start_time = max(job_ready_time, machine_ready_time);
        
        new_schedule[job_id][op_seq] = start_time;
        machine_finish_times[machine] = start_time + jobs[job_id][op_seq].duration;
    }
    
    return new_schedule;
}
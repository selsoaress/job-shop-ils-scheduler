#include <bits/stdc++.h>
#include "../operation.h"
#include "instance.h"

using namespace std;

// Function to read instance from file
bool read_instance_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    
    // Read number of jobs and machines
    if (!getline(file, line)) {
        cerr << "Error: Could not read first line (jobs and machines)" << endl;
        return false;
    }
    
    istringstream iss(line);
    if (!(iss >> num_jobs >> num_machines)) {
        cerr << "Error: Invalid format for jobs and machines" << endl;
        return false;
    }
    
    cout << "Reading instance: " << num_jobs << " jobs, " << num_machines << " machines" << endl;
    
    // Resize jobs matrix
    jobs.assign(num_jobs, vector<Operation>(num_machines));
    
    // Calculate max execution time
    // max_exec_time =
    
    // Read job data
    for (int job = 0; job < num_jobs; ++job) {
        if (!getline(file, line)) {
            cerr << "Error: Could not read job " << job << endl;
            return false;
        }
        
        istringstream job_stream(line);
        vector<int> job_data;
        int value;
        
        // Read all values for this job
        while (job_stream >> value) {
            job_data.push_back(value);
        }
        
        // Check if we have the right number of values (machine, duration pairs)
        if (job_data.size() != 2 * num_machines) {
            cerr << "Error: Job " << job << " has " << job_data.size() 
                 << " values, expected " << 2 * num_machines << endl;
            return false;
        }
        
        // Parse machine-duration pairs
        for (int op = 0; op < num_machines; ++op) {
            int machine_id = job_data[2 * op];
            int duration = job_data[2 * op + 1];
            
            jobs[job][op].machine = machine_id;
            jobs[job][op].duration = duration;
            jobs[job][op].job_id = job;
            
            // Update max execution time if needed
           // max_exec_time = max(max_exec_time, duration * num_jobs * num_machines);
        }
    }
    
    file.close();
    cout << "Instance loaded successfully!" << endl;
    return true;
}

// Function to print instance (for verification)
void print_instance() {
    cout << "\n=== Instance Details ===" << endl;
    cout << "Jobs: " << num_jobs << ", Machines: " << num_machines << endl;
    
    for (int job = 0; job < num_jobs; ++job) {
        cout << "Job " << job << ": ";
        for (int op = 0; op < num_machines; ++op) {
            cout << "(M" << jobs[job][op].machine 
                 << ",T" << jobs[job][op].duration << ") ";
        }
        cout << endl;
    }
    cout << "========================\n" << endl;
}
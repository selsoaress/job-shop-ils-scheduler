#pragma once
#include <vector>
#include <random>

using namespace std;

template<typename T>
using Matrix = std::vector<std::vector<T>>;

struct Operation {
    int machine;
    int duration;
    int job_id;
};

// Declarações globais (apenas "extern")
extern Matrix<Operation> jobs;
extern int num_jobs;
extern int num_machines;

extern random_device rd;
extern mt19937 gen;

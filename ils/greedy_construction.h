#ifndef GREEDY_SCHEDULER_H
#define GREEDY_SCHEDULER_H

#include <vector>
#include "../operation.h"

// forward declaration of Matrix template 
template<typename T>
using Matrix = std::vector<std::vector<T>>;

// external declarations for global variables
extern Matrix<Operation> jobs;
extern int num_jobs;
extern int num_machines;

// function declaration
Matrix<int> greedy_schedule_constructor();

#endif // GREEDY_SCHEDULER_H
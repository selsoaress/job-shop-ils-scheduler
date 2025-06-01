#ifndef GANTT_CHART_GENERATION_H
#define GANTT_CHART_GENERATION_H

#include <string>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void generate_gantt_chart(const Matrix<int>& schedule, int iteration, int makespan, const std::string& prefix = "gantt");

extern int num_jobs;
extern int num_machines;
extern Matrix<Operation> jobs;

#endif // GANTT_CHART_GENERATION_H

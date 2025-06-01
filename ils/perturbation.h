#ifndef PERTURBATION_H
#define PERTURBATION_H

#include "../operation.h"
#include <vector>

// applies non-consecutive swap in the machines following a bottleneck criterium
// can be adapted to perform a VNS by changing the perturbation_strength value
Matrix<int> perturbation(const Matrix<int>& current_schedule, int perturbation_strength = 1);

#endif // PERTURBATION_H

#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "../operation.h"
#include <vector>

// executes the local search by performing swaps on the machines
// returns a new schedule potentially improved
Matrix<int> local_search(const Matrix<int>& current_schedule_start_times,
                         int current_makespan,
                         bool use_first_improvement = true);

#endif // LOCAL_SEARCH_H

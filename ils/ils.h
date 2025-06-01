#ifndef ILS_H
#define ILS_H

#include <vector>
#include "../operation.h"

using MatrixInt = std::vector<std::vector<int>>;

MatrixInt iterated_local_search(Matrix<int> initial_solution, double time_limit_seconds);

#endif // ILS_H
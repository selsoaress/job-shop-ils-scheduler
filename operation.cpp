#include "operation.h"

using namespace std;

Matrix<Operation> jobs;
int num_jobs;
int num_machines;

random_device rd;
mt19937 gen(rd());
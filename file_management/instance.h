#ifndef INSTANCE_READER_H
#define INSTANCE_READER_H

#include <bits/stdc++.h>
#include "../operation.h"

// External declarations for global variables
extern int num_jobs;
extern int num_machines;
extern std::vector<std::vector<Operation>> jobs;

// Function declarations
bool read_instance_from_file(const std::string& filename);
void print_instance();

#endif // INSTANCE_READER_H
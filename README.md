## Job Shop Scheduling Problem (JSSP)

This project addresses the **Job Shop Scheduling Problem (JSSP)**, a classic combinatorial optimization challenge. The goal is to determine the execution sequence of operations on limited machines, respecting both technological and capacity constraints.

Each job consists of a fixed sequence of operations. Each operation must be processed on a specific machine, has a known processing time, and is non-preemptive (cannot be interrupted). Each machine can process only one operation at a time. The primary objective is to minimize the total completion time, also known as the **makespan**.

---

## Graph Modeling

The problem is modeled using a **Directed Acyclic Graph (DAG)**. Each operation is represented by a node, and the temporal dependencies between operations are represented by directed edges.

There are two types of dependencies in the graph:
1. **Technological Constraints**: These impose a fixed order of operations within the same job. These dependencies are mandatory and independent of the chosen schedule.
2. **Machine Conflicts**: Operations that share the same machine cannot be executed simultaneously, requiring a decision on their processing order. These decisions define the orientation of the "machine edges" and characterize a specific solution.

For a schedule to be feasible, the resulting graph must be acyclic. The presence of cycles indicates a temporal inconsistency, rendering the solution invalid.

---

## Solution Evaluation

Once the graph is constructed, the makespan is determined by the **critical path**—the longest path in the DAG considering the processing times of the operations. This value is used as the primary fitness criterion for evaluating solutions.

---

## Initial Construction

The initial solution is built using a greedy heuristic based on the **SPT (Shortest Processing Time)** rule. On each machine, available operations are ordered according to the shortest processing time while respecting job precedence constraints. This step ensures the generation of an initial feasible schedule.

---

## Local Search

The local search utilizes a basic move: the **swap of adjacent operations on the same machine**. After a swap, adjustments are made along the job's production chain to ensure that the operation sequence is preserved and the graph remains acyclic.

---

## Perturbation

The perturbation stage involves swapping **non-consecutive operations on the same machine**. This move introduces more significant changes to the schedule, aiming to escape local optima. Following perturbation, the schedule is readjusted to restore feasibility.

---

## Approach Overview

In summary, the approach adopted in this project follows these steps:

* Initial greedy construction via the **SPT** rule;
* Exploration of the solution space through **machine swaps**;
* Adjustments to guarantee **graph feasibility**;
* Solution evaluation via the **critical path**.

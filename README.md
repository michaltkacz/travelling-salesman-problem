# Travelling Salesman Problem
> Travelling Salesman Problem C++ implementation.

## Table of contents
- [Travelling Salesman Problem](#travelling-salesman-problem)
  - [Table of contents](#table-of-contents)
  - [General info](#general-info)
  - [Setup and run](#setup-and-run)
  - [Issues](#issues)
  - [Status](#status)
  - [Contact](#contact)

## General info
The main goal of this project was to implement and compare efficiency of algorithms fidning Travelling Salesman Problem solutions, using following programming methods:
* Exact algorithms (always find the optimal solution):
  * Bruteforce,
  * Dynamic programming (Held–Karp algorithm),
  * Branch and bound;
* Heuristic approach (found soultion is not guranteed to be optimal):
  * Simulated annealing,
  * Tabu search,
  * Ant colony optimization.

Each program on launch loads [config.ini](./data/config.ini) and then executes tests. Each test result is saved to output file. Tests results are shown in [tests_results.xlsx](./tests_results.xlsx).

## Setup and run
Each folder contains individual project. Download project and compile it with C++ compiler.

[graphs](./data/graphs) folder and [config.ini](./data/config.ini) file must be placed in the same folder as the program executable file. Structure of data files:
```
graph_size   <-- number of graph nodes, integer value.
0 x x x ...  \
x 0 x x      |
x x 0 x      |- adjacency matrix, integer values, 0 on diagonal.
x x x 0      |
...          /
```

Structue of  [config.ini](./data/config.ini) file:
```
path_and_filename1.extension number_of_test_repeats correct_tsp_solution correct_tsp_solution_path
path_and_filename2.extension number_of_test_repeats correct_tsp_solution correct_tsp_solution_path
...
output_filename.csv
```

## Issues
No major issues are known.

## Status
This project is finished and no longer developed.

## Contact
Created by [@michaltkacz](https://github.com/michaltkacz) - feel free to contact me!
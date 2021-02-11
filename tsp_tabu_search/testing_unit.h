#pragma once
#include <string>
#include <vector>

#include "tabu_list.h"
#include "graph.h"
#include <random>


using namespace std;

class testing_unit
{
public:
	testing_unit(string input_file_name, int number_of_repeats, int optimal_path_cost, vector<int> optimal_path_nodes);
 	~testing_unit();

	void run_tests();
	void save_tests_results(ofstream& fout);

private:
	graph* graph_;

	string input_file_name_;
	int number_of_repeats_;

	struct path
	{
		vector<int> nodes = {};
		int cost = 0;
		int mval = 0;
		int att_i = 0;
		int att_j = 0;
	};

	path optimal_path_;
	path solution_path_;

	struct save_data
	{
		vector<double> times = {};
		vector<path> solutions = {};
		vector<double> errors = {};
	};
	save_data save_data_;

	tabu_list tabu_ = { 0 };

	void load_graph(string file_name);

	double random01();
	int random_int(int a, int b);
	int myrandom(int i);

	path run_tabu();
	path aspiration_plus(tabu_list tabu, path x0, int best_cost);
	std::vector<path> find_n_solutions(path p);

	path random_start_solution();
	path greedy_start_solution();

	void print_test_result(int i);

	int calculate_path_cost(vector<int> nodes);
	double calculate_path_cost_error(int cost);

};


#pragma once
#include <string>
#include <vector>

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

	path* optimal_path_;
	path* solution_path_;

	struct save_data
	{
		vector<double> times = {};
		vector<path*> solutions = {};
		vector<double> errors = {};
	} save_data_;

	path* tsp_aco(double alpha, double beta, double ro, int m, int CC);

	void print_test_result(int i);

	int calculate_path_cost(vector<int> nodes);
	double calculate_path_cost_error(int cost);

};


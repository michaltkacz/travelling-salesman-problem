#pragma once
#include <string>
#include <vector>

#include "graph.h"


using namespace std;

class testing_unit
{
public:
	testing_unit(string input_file_name, int number_of_repeats, int optimal_path_cost, vector<int> optimal_path_nodes);
 	~testing_unit();
	
	void run_tests();
	void save_data(ofstream& fout);

private:
	graph* graph_;
	int graph_size_;

	string input_file_name_;
	int number_of_repeats_;

	vector<double> times_;
	
	struct path
	{
		vector<int> nodes;
		int cost = 0;
	};

	path optimal_path_;
	path solution_path_;

	int final_cost_;
	int* final_path_;
	bool* visited_nodes_;
	
	int find_first_min_edge_cost_from_n(int n);
	int find_second_min_edge_cost_from_n(int n);

	void bb();
	void bb_recursion(int current_bound, int current_weight, int level, int* current_path);

	int bound(int n);
};


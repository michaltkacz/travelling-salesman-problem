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

	int visited_all_;
	int** duplicates_;
	int duplicates_y_;
	int duplicates_x_;

	int tsp_solution(int already_visited, int position);
};


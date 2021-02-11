#pragma once
#include <string>
#include <vector>

#include "graph.h"


using namespace std;

class testing_unit
{
public:
	testing_unit(string input_file_name, int number_of_repeats, int optimal_solution, vector<int> optimal_path);
 	~testing_unit();
	
	void run_tests();
	void save_data(ofstream& fout);

private:
	string input_file_name_;
	int number_of_repeats_;
	int optimal_solution_;
	vector<int> optimal_path_;

	graph* graph_;
	int solution_;
	vector<int> path_;
	vector<double> times_;

	void bruteforce();
};


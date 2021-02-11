#pragma once
#include <fstream>
#include <string>

#include "path.h"

class graph
{
public:
	graph(int size);
	~graph();

	void set_value_at(int x, int y, int value);
	int get_value_at(int x, int y);

	int get_size();

	void print();

	void load_from_file(std::ifstream& fin);
	static graph* load_graph(std::string file_name);

	int calculate_path_cost(path* p);

	void reset_tau();

	void add_tau_at(int i, int j, double t);
	double get_tau_at(int i, int j);

	void evaporate(int x, int y, double ro);

private:
	int size_;
	int** matrix_;
	double** tau_;

};


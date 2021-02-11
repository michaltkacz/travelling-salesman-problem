#pragma once
#include <fstream>

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

private:
	int size_;
	int** matrix_;

};


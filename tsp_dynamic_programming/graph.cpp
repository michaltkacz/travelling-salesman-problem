#include "graph.h"
#include <iostream>
#include <iomanip>

graph::graph(int size)
{
	size_ = size;
	matrix_ = new int* [size_];
	for (int i = 0; i < size_; i++)
	{
		matrix_[i] = new int[size_];
	}
}

graph::~graph()
{
	for (int i = 0; i < size_; i++)
	{
		delete[] matrix_[i];
	}
	delete[] matrix_;
	matrix_ = nullptr;
}

void graph::set_value_at(int x, int y, int value)
{
	matrix_[y][x] = value;
}

int graph::get_value_at(int x, int y)
{
	return matrix_[y][x];
}

int graph::get_size()
{
	return size_;
}

void graph::print()
{
	int last_node_to_print = 0;
	int i = 0;
	int j = 0;

	while (last_node_to_print != size_)
	{
		last_node_to_print += 30;
		if (last_node_to_print > size_)
		{
			last_node_to_print = size_;
		}
		std::cout << "i\\o";
		for (i; i < last_node_to_print; i++)
		{
			std::cout << std::setw(4) << i;
		}
		std::cout << std::endl;

		int l = j;
		for (int k = 0; k < size_; k++)
		{
			std::cout << std::setw(3) << k;

			for (j = l; j < i; j++)
			{
				std::cout << std::setw(4) << matrix_[k][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void graph::load_from_file(std::ifstream& fin)
{
	int value = 0;
	for (int i = 0; i < size_; i++) 
	{
		for (int j = 0; j < size_; j++)
		{
			fin >> value;
			set_value_at(i, j, value);
		}
	}
}

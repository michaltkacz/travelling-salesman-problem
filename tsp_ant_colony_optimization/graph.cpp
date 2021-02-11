#include "graph.h"
#include <iostream>
#include <iomanip>

graph::graph(int size)
{
	size_ = size;
	matrix_ = new int* [size_];
	tau_ = new double* [size_];
	for (int i = 0; i < size_; i++)
	{
		matrix_[i] = new int[size_];
		tau_[i] = new double[size_];
	}
}

graph::~graph()
{
	for (int i = 0; i < size_; i++)
	{
		delete[] matrix_[i];
		delete[] tau_[i];
	}
	delete[] matrix_;
	delete[] tau_;
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
	reset_tau();
}

graph* graph::load_graph(std::string file_name)
{
	std::ifstream fin(file_name);
	graph* g = nullptr;
	if (fin.is_open())
	{
		int graph_size;
		fin >> graph_size;

		g = new graph(graph_size);
		g->load_from_file(fin);
	}
	fin.close();

	return g;
}

int graph::calculate_path_cost(path* p)
{
	int cost = 0;
	std::vector<int> nodes = p->get_nodes();
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		int index_a = nodes[i];
		int index_b = nodes[i + 1];

		cost += get_value_at(index_a, index_b);
	}
	p->set_cost(cost);
	return cost;
}

void graph::reset_tau()
{
	int m = size_;
	int c_nn = 0;
	for (int i = 1; i < size_; i++)
	{
		c_nn += matrix_[i - 1][i];
	}
	c_nn += matrix_[size_-1][0];

	for (int y = 0; y < size_; y++)
	{
		for (int x = 0; x < size_; x++)
		{
			tau_[y][x] = double(m) / c_nn;
		}
	}
}

void graph::add_tau_at(int i, int j, double t)
{
	tau_[i][j] += t;
}

double graph::get_tau_at(int i, int j)
{
	return tau_[i][j];
}

void graph::evaporate(int x, int y, double ro)
{
	tau_[y][x] *= ro;
}

#include "path.h"

path::path()
{
	nodes_ = {};
	cost_ = 0;
}

path::path(std::vector<int> n)
{
	nodes_ = n;
	cost_ = 0;
}

path::path(std::vector<int> n, int c)
{
	nodes_ = n;
	cost_ = c;
}

std::vector<int> path::get_nodes()
{
	return nodes_;
}

void path::set_nodes(std::vector<int> n)
{
	nodes_ = n;
}

int path::get_cost()
{
	return cost_;
}

void path::set_cost(int c)
{
	cost_ = c;
}

void path::add_node(int n)
{
	nodes_.push_back(n);
}



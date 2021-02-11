#pragma once
#include <vector>

class path
{
public:
	path();
	path(std::vector<int> n);
	path(std::vector<int> n, int c);
	path(const path& p) : nodes_(p.nodes_), cost_(p.cost_) {};

	std::vector<int> get_nodes();
	void set_nodes(std::vector<int> n);

	int get_cost();
	void set_cost(int c);

	void add_node(int n);

	path& operator= (const path& other)
	{
		nodes_.assign(other.nodes_.begin(), other.nodes_.end());
		cost_ = other.cost_;
		return *this;
	}

private:
	std::vector<int> nodes_;
	int cost_;

};


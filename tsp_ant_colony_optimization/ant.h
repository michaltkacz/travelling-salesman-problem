#pragma once
#include <vector>
#include "graph.h"

class ant
{
public:
	ant(double alpha, double beta, int start_node, double q_cycle, int graph_size);

	void next_step(graph* g);
	void next_step_back_to_start(graph* g);
	void cas(graph* g, int path_cost);
	void reset(int graph_size);
	std::vector<int> get_path();

private:
	double alpha_;
	double beta_;

	int start_node_;
	std::vector<int> path_;
	std::vector<int> omega_;
	double q_cycle_;

};


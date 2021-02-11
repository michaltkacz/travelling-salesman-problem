#include "ant.h"
#include "my_rand.h"
#include <iostream>


ant::ant(double alpha, double beta, int start_node, double q_cycle, int graph_size)
{
	alpha_ = alpha;
	beta_ = beta;
	start_node_ = start_node;
	q_cycle_ = q_cycle;
	reset(graph_size);

}

void ant::next_step(graph* g)
{
	int last_node = path_.back();
	double denominator = 0.0;

	for (auto& c : omega_)
	{
		double tau_ic = g->get_tau_at(last_node, c);
		double ni_ic = 1.0 / g->get_value_at(last_node, c);
		denominator += powf(tau_ic, alpha_) * powf(ni_ic, beta_);
	}

	double highest_probability = 0.0;
	std::vector<int>::iterator next_node_to_move = omega_.begin();
	bool moved = false;

	for(std::vector<int>::iterator next_node = omega_.begin(); next_node != omega_.end(); next_node++)
	{
		double tau_ij = g->get_tau_at(last_node, *next_node);
		double ni_ij = 1.0 / g->get_value_at(last_node, *next_node);
		double nominator = powf(tau_ij, alpha_) * powf(ni_ij, beta_);
		double move_probability = nominator / denominator;
		if (move_probability > highest_probability)
		{
			highest_probability = move_probability;
			next_node_to_move = next_node;

			if (my_rand::random01() <= highest_probability)
			{
				moved = true;
				path_.push_back(*next_node_to_move);
				omega_.erase(next_node_to_move);
				break;
			}
		}
	}

	if (!moved)
	{
		path_.push_back(*next_node_to_move);
		omega_.erase(next_node_to_move);
	}
}

void ant::next_step_back_to_start(graph* g)
{
	path_.push_back(start_node_);
}

void ant::cas(graph* g, int path_cost)
{
	double q_deposit = q_cycle_ / path_cost;
	for (int i = 1; i < path_.size(); i++)
	{
		int index_a = path_[i-1];
		int index_b = path_[i];
		g->add_tau_at(index_a, index_b, q_deposit);
	}
	int index_a = path_.back();
	int index_b = path_[0];
	g->add_tau_at(index_a, index_b, q_deposit);
}

void ant::reset(int graph_size)
{
	path_.clear();
	omega_.clear();

	path_.push_back(start_node_);
	for (int i = 0; i < graph_size; i++)
	{
		if (i != start_node_)
		{
			omega_.push_back(i);
		}
	}
}

std::vector<int> ant::get_path()
{
	return path_;
}

#include "testing_unit.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "timer.h"

testing_unit::testing_unit(string input_file_name, int number_of_repeats, int optimal_path_cost,
	vector<int> optimal_path_nodes)
{
	input_file_name_ = input_file_name;
	number_of_repeats_ = number_of_repeats;
	optimal_path_.cost = optimal_path_cost;
	optimal_path_.nodes = optimal_path_nodes;

	load_graph(input_file_name);
}

testing_unit::~testing_unit()
{
	delete graph_;
}

void testing_unit::run_tests()
{
	cout << input_file_name_ << endl;

	timer t{};
	for (int i = 0; i < number_of_repeats_; i++)
	{
		solution_path_.cost = 0;
		solution_path_.nodes.clear();

		t.start();
		solution_path_ = run_sa();
		t.stop();

		save_data_.times.push_back(t.elapsed_time());
		save_data_.solutions.push_back(solution_path_);
		save_data_.errors.push_back(calculate_path_cost_error(solution_path_.cost));

		print_test_result(i);
	}
	cout << endl;
}

double testing_unit::random01()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	return dis(gen);
}

double testing_unit::random_int(int a, int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(a, b);

	return dist(gen);
}

testing_unit::path testing_unit::run_sa()
{
	// Choose start path.
	path solution = greedy_start_solution();
	solution.cost = calculate_path_cost(solution.nodes);

	// Currently the best solution, is starting solution.
	path best_solution = solution;

	// Initial temperature
	double temperature_start = solution.cost * 0.5;
	double temperature = temperature_start;

	// Number of internal iterations
	//int int_iterations_n = static_cast<int>(pow(graph_->get_size(), 2));
	int int_iterations_n = 10 * graph_->get_size();

	// Number of exeternal iterations
	int ext_iterations_n = 3000;

	// Run algorithm ext_iterations_n times.
	for (int loop = 0; loop < ext_iterations_n; loop++)
	{
		// For int_iterations_n times try to improove.
		for (int i = 0; i < int_iterations_n; i++)
		{
			// Find close solution.
			path close_solution = find_close_solution(solution);
			// Save it, if it is the best so far.
			if (close_solution.cost < best_solution.cost)
			{
				best_solution = close_solution;
			}

			// Calculate cost difference.
			int cost_delta = close_solution.cost - solution.cost;
			// If close solution is better, use it.
			if (cost_delta < 0)
			{
				solution = close_solution;
			} // Use it with some probability, even if it is worse.
			else if (random01() < exp(-cost_delta / temperature))
			{
				solution = close_solution;
			}
		}

		// Cool down temeprature.
		temperature = geom_cool_down(temperature);
		//temperature = cauchy_cool_down(temperature_start, 1.0, 3.0, loop);
		// If temeprature is 0, then finish.
		if (temperature <= 0)
		{
			break;
		}
	}

	// Return best found solution.
	return best_solution;
}

testing_unit::path testing_unit::random_start_solution()
{
	path p;
	p.nodes.push_back(0);
	for (int i = 1; i < graph_->get_size(); i++)
	{
		p.nodes.push_back(i);
	}
	p.nodes.push_back(0);

	std::random_shuffle(p.nodes.begin() + 1, p.nodes.end() - 1);
	p.cost = calculate_path_cost(p.nodes);
	return p;
}

testing_unit::path testing_unit::greedy_start_solution()
{
	bool* in_path = new bool[graph_->get_size()];

	in_path[0] = true;
	for (int i = 1; i < graph_->get_size(); i++)
	{
		in_path[i] = false;
	}

	path p;
	p.nodes.push_back(0);
	for (int i = 1; i < graph_->get_size(); i++)
	{
		int min_node_id = -1;
		int min_node_cost = INT_MAX;
		for (int j = 1; j < graph_->get_size(); j++)
		{
			if (in_path[j] == true)
				continue;

			int cost = graph_->get_value_at(i - 1, j);
			if(cost < min_node_cost)
			{
				min_node_id = j;
				min_node_cost = cost;
			}
		}
		if (min_node_id != -1)
		{
			p.nodes.push_back(min_node_id);
			in_path[min_node_id] = true;
		}
	}
	p.nodes.push_back(0);
	p.cost = calculate_path_cost(p.nodes);

	delete[] in_path;

	return p;
}

testing_unit::path testing_unit::find_close_solution(path p)
{
	// Get random indices.
	int i = random_int(1, p.nodes.size() - 2);
	int j = random_int(1, p.nodes.size() - 2);

	while (i == j)
	{
		j = random_int(1, p.nodes.size() - 2);
	}

	// Path inverse.
	path path_inverse = p;
	if (i <= j)
	{
		std::reverse(path_inverse.nodes.begin() + i, path_inverse.nodes.begin() + j + 1);
	}
	else
	{
		std::reverse(path_inverse.nodes.begin() + j, path_inverse.nodes.begin() + i + 1);
	}
	path_inverse.cost = calculate_path_cost(path_inverse.nodes);

	// Path insert.
	path path_insert = p;
	int node = path_insert.nodes[j];
	path_insert.nodes.erase(path_insert.nodes.begin() + j);
	path_insert.nodes.insert(path_insert.nodes.begin() + i, node);

	// Path swap.
	path path_swap = p;
	std::swap(path_swap.nodes[i], path_swap.nodes[j]);
	path_swap.cost = calculate_path_cost(path_swap.nodes);

	// Return path with min cost.
	int min_cost = min(min(path_inverse.cost, path_insert.cost), path_swap.cost);
	if (path_inverse.cost == min_cost)
	{
		return path_inverse;
	}
	else if (path_insert.cost == min_cost)
	{
		return path_inverse;
	}
	return path_swap;
}

double testing_unit::geom_cool_down(double t)
{
	return 0.99 * t;
}

double testing_unit::cauchy_cool_down(double tz, double a, double b, int k)
{
	return tz / (a + b * k);
}

void testing_unit::print_test_result(int i)
{
	cout << "[" << i + 1 << "/" << number_of_repeats_ << "]";

	cout << "   Optimal path cost: " << optimal_path_.cost << endl;
	cout << "\tOptimal path nodes: ";
	if (optimal_path_.nodes.empty() || optimal_path_.nodes[0] == -1) cout << "NoData";
	else for (auto n : optimal_path_.nodes) cout << n << " ";
	cout << endl;

	cout << "\tSolution path cost: " << solution_path_.cost << endl;
	cout << "\tSolution path nodes: ";
	if (solution_path_.nodes.empty()) cout << "NoData";
	else for (auto n : solution_path_.nodes) cout << n << " ";
	cout << endl;

	cout << "\tSolution path cost error: ";
	cout << fixed << setprecision(3) << save_data_.errors.back() * 100.0 << "%" << endl;

	cout << "\tTime: ";
	cout << fixed << setprecision(9) << save_data_.times.back() << endl << endl;
}

void testing_unit::save_tests_results(ofstream& fout)
{
	fout << input_file_name_ << "\t";
	fout << number_of_repeats_ << "\t";
	fout << optimal_path_.cost << "\t";

	if (optimal_path_.nodes.empty()) fout << "NoData.";
	else for (auto n : optimal_path_.nodes) fout << n << " ";
	fout << endl;

	for (int i = 0; i < save_data_.times.size(); i++)
	{
		fout << fixed << setprecision(9) << save_data_.times[i];
		fout << "\t";
		fout << save_data_.solutions[i].cost;
		fout << "\t";
		fout << fixed << setprecision(2) << save_data_.errors[i] * 100.0;
		fout << "\t";
		for (auto n : save_data_.solutions[i].nodes)
		{
			fout << n << " ";
		}
		fout << endl;
	}
	fout << endl;
}

void testing_unit::load_graph(string file_name)
{
	ifstream fin(file_name);
	if (fin.is_open())
	{
		int graph_size;
		fin >> graph_size;

		graph_ = new graph(graph_size);
		graph_->load_from_file(fin);
	}
	fin.close();
}

int testing_unit::calculate_path_cost(vector<int> nodes)
{
	int cost = 0;
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		int index_a = nodes[i];
		int index_b = nodes[i + 1];

		cost += graph_->get_value_at(index_a, index_b);
	}
	return cost;
}

double testing_unit::calculate_path_cost_error(int cost)
{
	// If optimal solution path cost is not given.
	if (optimal_path_.cost == 0)
	{
		return -1;
	}
	return (static_cast<double>(cost) - optimal_path_.cost) / optimal_path_.cost;
}



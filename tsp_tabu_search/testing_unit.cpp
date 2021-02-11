#include "testing_unit.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "timer.h"
#include <list>


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
		solution_path_ = run_tabu();
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

int testing_unit::random_int(int a, int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(a, b);

	return dist(gen);
}

int testing_unit::myrandom(int i)
{
	return std::rand() % i;
}

testing_unit::path testing_unit::run_tabu()
{
	tabu_list tabu = { graph_->get_size() / 4 };

	path x_0 = random_start_solution();
	path x_opt = x_0;

	int ctr = 0;
	int max_iterations = (int)pow(graph_->get_size(), 2);

	high_resolution_clock::time_point start_time = high_resolution_clock::now();
	for (int i = 0; i < max_iterations; i++)
	{
		ctr++;
		x_0 = aspiration_plus(tabu, x_0, x_opt.cost);
		if (x_0.cost < x_opt.cost)
		{
			x_opt = x_0;
			ctr = 0;
		}

		tabu.add(x_0.att_i, x_0.att_j, graph_->get_size()/3);
		//tabu.add(x_0.nodes, graph_->get_size()/3);
		tabu.update_cadences_and_delete_elements();

		if (ctr >= graph_->get_size())
		{
			ctr = 0;
			x_0 = random_start_solution();
			if (x_0.cost < x_opt.cost)
			{
				x_opt = x_0;
			}
		}

		if (duration_cast<duration<double>>(high_resolution_clock::now() - start_time).count() > 120.0)
		{
			break;
		}
	}
	return x_opt;
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

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(p.nodes.begin() + 1, p.nodes.end() - 1, std::default_random_engine(seed));
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
			if (cost < min_node_cost)
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

std::vector<testing_unit::path> testing_unit::find_n_solutions(path p)
{
	std::vector<path> paths = {};

	for (int i = 1; i < p.nodes.size() - 3; i++)
	{
		for (int j = i + 1; j < p.nodes.size() - 2; j++)
		{
			path path_swap = p;
			std::swap(path_swap.nodes[i], path_swap.nodes[j]);
			path_swap.cost = calculate_path_cost(path_swap.nodes);
			path_swap.att_i = i;
			path_swap.att_j = j;
			paths.push_back(path_swap);
		}
	}

	return paths;
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

testing_unit::path testing_unit::aspiration_plus(tabu_list tabu, path x0, int best_cost)
{
	std::vector<path> n_solutions = find_n_solutions(x0);
	for (auto& x : n_solutions)
	{
		x.mval = x.cost - x0.cost;
	}

	std::sort(n_solutions.begin(), n_solutions.end(), [](const path& left, const path& right)
	{
		return (left.mval < right.mval);
	});


	for (auto& x : n_solutions)
	{
		if (x.mval < 0)
		{
			if (!tabu.contains(x.att_i, x.att_j) || x.cost < best_cost)
			{
				return x;
				break;
			}
		}
		else
		{
			return x0;
		}
	}

	return x0;
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



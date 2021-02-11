#include "testing_unit.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "timer.h"
#include <list>
#include "ant.h"
#include "my_rand.h"


testing_unit::testing_unit(string input_file_name, int number_of_repeats, int optimal_path_cost,
	vector<int> optimal_path_nodes)
{
	input_file_name_ = input_file_name;
	number_of_repeats_ = number_of_repeats;
	optimal_path_ = new path(optimal_path_nodes, optimal_path_cost);
	graph_ = graph::load_graph(input_file_name);
}

testing_unit::~testing_unit()
{
	delete graph_;
	delete optimal_path_;
	delete solution_path_;
}

void testing_unit::run_tests()
{
	cout << input_file_name_ << endl;

	timer t{};
	for (int i = 0; i < number_of_repeats_; i++)
	{
		delete solution_path_;
		t.start();
		solution_path_ = tsp_aco(1.0, 2.2, 0.5, graph_->get_size(), 2000);
		t.stop();

		save_data_.times.push_back(t.elapsed_time());
		save_data_.solutions.push_back(solution_path_);
		save_data_.errors.push_back(calculate_path_cost_error(solution_path_->get_cost()));

		print_test_result(i);
	}
	cout << endl;
}

path* testing_unit::tsp_aco(double alpha, double beta, double ro, int m, int CC)
{
	std::vector<ant> ants;
	for (int i = 0; i < m; i++)
	{
		int ant_start_node = my_rand::random_int(0, graph_->get_size()-1);
		ants.push_back(ant(alpha, beta, ant_start_node, 10000, graph_->get_size()));
	}

	path* best_path = nullptr;

	// Liczba iteracji algorytmu
	high_resolution_clock::time_point start_time = high_resolution_clock::now();
	for (int iterations = 0; iterations < CC; iterations++)
	{
		// PrzejdŸ ca³¹ œcie¿kê
		for (int i = 0; i < graph_->get_size()-1; i++)
		{
			// Ka¿da mrówka robi krok
			for (auto& a : ants)
			{
				a.next_step(graph_);
			}
		}

		// Ka¿da mrówka...
		for (auto& a : ants)
		{
			// odwiedzi³a wszystkie wierzcho³ki i wraca na start
			a.next_step_back_to_start(graph_);
			path* a_path = new path(a.get_path());
			graph_->calculate_path_cost(a_path);

			a.cas(graph_, a_path->get_cost());

			// je¿eli scie¿ka tej mrówki jest najlepsza, to zapisz j¹
			if (best_path == nullptr)
			{
				best_path = a_path;
			}
			else if(a_path->get_cost() < best_path->get_cost())
			{
				delete best_path;
				best_path = a_path;
			}

			a.reset(graph_->get_size());
			//delete a_path;
		}

		// Zaktualizuj wartoœci feromonu
		for (int y = 0; y < graph_->get_size(); y++)
		{
			for (int x = 0; x < graph_->get_size(); x++)
			{
				graph_->evaporate(x, y, ro);
			}
		}

		// Zakoncz po uplywie czasu
		if (duration_cast<duration<double>>(high_resolution_clock::now() - start_time).count() > 60.0)
		{
			break;
		}
	}

	return best_path;
}

//testing_unit::path testing_unit::random_start_solution()
//{
//	path p;
//	p.nodes.push_back(0);
//	for (int i = 1; i < graph_->get_size(); i++)
//	{
//		p.nodes.push_back(i);
//	}
//	p.nodes.push_back(0);
//
//	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//	std::shuffle(p.nodes.begin() + 1, p.nodes.end() - 1, std::default_random_engine(seed));
//	p.cost = calculate_path_cost(p.nodes);
//	return p;
//}

//testing_unit::path testing_unit::greedy_start_solution()
//{
//	bool* in_path = new bool[graph_->get_size()];
//
//	in_path[0] = true;
//	for (int i = 1; i < graph_->get_size(); i++)
//	{
//		in_path[i] = false;
//	}
//
//	path p;
//	p.nodes.push_back(0);
//	for (int i = 1; i < graph_->get_size(); i++)
//	{
//		int min_node_id = -1;
//		int min_node_cost = INT_MAX;
//		for (int j = 1; j < graph_->get_size(); j++)
//		{
//			if (in_path[j] == true)
//				continue;
//
//			int cost = graph_->get_value_at(i - 1, j);
//			if (cost < min_node_cost)
//			{
//				min_node_id = j;
//				min_node_cost = cost;
//			}
//		}
//		if (min_node_id != -1)
//		{
//			p.nodes.push_back(min_node_id);
//			in_path[min_node_id] = true;
//		}
//	}
//	p.nodes.push_back(0);
//	p.cost = calculate_path_cost(p.nodes);
//
//	delete[] in_path;
//
//	return p;
//}

void testing_unit::print_test_result(int i)
{
	cout << "[" << i + 1 << "/" << number_of_repeats_ << "]";

	cout << "   Optimal path cost: " << optimal_path_->get_cost() << endl;
	cout << "\tOptimal path nodes: ";
	if (optimal_path_->get_nodes().empty() || optimal_path_->get_nodes()[0] == -1) cout << "NoData";
	else for (auto n : optimal_path_->get_nodes()) cout << n << " ";
	cout << endl;

	cout << "\tSolution path cost: " << solution_path_->get_cost() << endl;
	cout << "\tSolution path nodes: ";
	if (solution_path_->get_nodes().empty()) cout << "NoData";
	else for (auto n : solution_path_->get_nodes()) cout << n << " ";
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
	fout << optimal_path_->get_cost() << "\t";

	if (optimal_path_->get_nodes().empty()) fout << "NoData.";
	else for (auto n : optimal_path_->get_nodes()) fout << n << " ";
	fout << endl;

	for (int i = 0; i < save_data_.times.size(); i++)
	{
		fout << fixed << setprecision(9) << save_data_.times[i];
		fout << "\t";
		fout << save_data_.solutions[i]->get_cost();
		fout << "\t";
		fout << fixed << setprecision(2) << save_data_.errors[i] * 100.0;
		fout << "\t";
		for (auto n : save_data_.solutions[i]->get_nodes())
		{
			fout << n << " ";
		}
		fout << endl;
	}
	fout << endl;
}

double testing_unit::calculate_path_cost_error(int cost)
{
	// If optimal solution path cost is not given.
	int optimal_cost = optimal_path_->get_cost();
	if (optimal_cost == 0)
	{
		return -1;
	}
	return (static_cast<double>(cost) - optimal_cost) / optimal_cost;
}



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
	optimal_path_.cost= optimal_path_cost;
	optimal_path_.nodes = optimal_path_nodes;


	ifstream fin(input_file_name);
	if (fin.is_open()) {
		int graph_size;
		fin >> graph_size;

		graph_ = new graph(graph_size);
		graph_->load_from_file(fin);
	}
	fin.close();

	// ustawia maske na 1...11
	visited_all_ = (1 << graph_->get_size()) - 1;

	// wymiary dla tablicy duplikatow
	duplicates_y_ = int(pow(2, graph_->get_size()));
	duplicates_x_ = graph_->get_size();

	// tablica duplikatow wypleniona wartosciami "-1"
	duplicates_ = new int* [duplicates_y_];
	for (int i = 0; i < duplicates_y_; i++)
	{
		duplicates_[i] = new int[duplicates_x_];
		for (int j = 0; j < duplicates_x_; j++)
		{
			duplicates_[i][j] = -1;
		}
	}
}

testing_unit::~testing_unit()
{
	delete graph_;

	for (int i = 0; i < duplicates_y_; i++)
	{
		delete[] duplicates_[i];
	}
	delete[] duplicates_;
}

void testing_unit::run_tests()
{
	cout << input_file_name_ << endl;

	timer t{};
	for (int i = 0; i < number_of_repeats_; i++)
	{
		solution_path_.cost = 0;
		solution_path_.nodes.clear();

		for (int i = 0; i < duplicates_y_; i++)
		{
			for (int j = 0; j < duplicates_x_; j++)
			{
				duplicates_[i][j] = -1;
			}
		}
		
		t.start();
		solution_path_.cost = tsp_solution(1, 0);
		t.stop();
		times_.push_back(t.elapsed_time());

		cout << "[" << i + 1 << "/" << number_of_repeats_ << "]";
		
		cout << ". Optimal path cost: " << optimal_path_.cost;
		cout << ". Optimal path nodes: ";
		if(optimal_path_.nodes.empty() || optimal_path_.nodes[0] == -1) cout << "NoData";
		else for (auto n : optimal_path_.nodes) cout << n << " ";
		cout << endl;

		cout << "\tSolution path cost: " << solution_path_.cost;
		cout << ". Solution path nodes: ";
		if (optimal_path_.nodes.empty()) cout << "NoData";
		else for (auto n : optimal_path_.nodes) cout << n << " ";
		cout << endl;
		
		cout << ". Time: ";
		cout << fixed << setprecision(9) << times_.back() << endl;
	}
	cout << endl;

}

void testing_unit::save_data(ofstream& fout)
{
	fout << input_file_name_ << " ";
	fout << number_of_repeats_ << " ";
	fout << solution_path_.cost << " ";
	
	if(optimal_path_.nodes.empty()) fout << "NoData.";
	else for (auto n : optimal_path_.nodes) fout << n << " ";
	fout << endl;

	for (auto time : times_)
	{
		fout << fixed << setprecision(9) << time << endl;
	}

}

int testing_unit::tsp_solution(int already_visited, int position)
{
	// START

	// Jezeli dotrze do konca sciezki, zwraca odleglosc miedzy
	// ostatnim wierzcholkiem sciezki (obecnie rozpatrywanego),
	// a wierzcholkiem poczatkowym.
	if (already_visited == visited_all_)
	{
		return graph_->get_value_at(position, 0);
	}

	// Zapobiega obliczaniu sciezek juz sprawdzonych.
	// Kluczowy element optymalizacji algorytmu.
	// Optymalizacja czasowa kosztem zlozonosci pamieciowej.
	if (duplicates_[already_visited][position] != -1)
	{
		return duplicates_[already_visited][position];
	}

	// Zmienna pomocnicza do wyznaczenia œcie¿ki o najmniejszym koszcie.
	int min_cost = INT_MAX;
	
	// Dla kazdego wierzcholka grafu:
	for (int node = 0; node < graph_->get_size(); node++)
	{
		// Jezeli wierzcholek w sciezce nie zostal jeszcze odwiedzony:
		if ((already_visited & (1 << node)) == 0)
		{
			// Oblicz nowy koszt sciezki do tego wierzcholka.
			// Nowy koszt, to koszt przejscia z ostatniego wierzcholka (obecnie rozpatrywanego)
			// dotychczasowej sciezki do obecnie sprawdzanego wierzcholka plus koszt sciezki
			// od wierzcholka koncowego, do obecnie rozpatrywanego wierzcholka.
			int new_cost = graph_->get_value_at(position, node) + tsp_solution(already_visited | (1 << node), node);

			// Zapisz koszt najtanszej sciezki.
			min_cost = min(min_cost, new_cost);
		}
	}

	// Zachowaj w tablicy koszt najtanszej sciezki.
	duplicates_[already_visited][position] = min_cost;

	// Zwroc koszt sciezki.
	return min_cost;

	// END
}

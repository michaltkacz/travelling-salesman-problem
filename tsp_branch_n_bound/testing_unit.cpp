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


	ifstream fin(input_file_name);
	if (fin.is_open()) {
		fin >> graph_size_;

		graph_ = new graph(graph_size_);
		graph_->load_from_file(fin);
	}
	fin.close();

	final_cost_ = INT_MAX;
	final_path_ = new int[graph_size_ + 1];
	visited_nodes_ = new bool[graph_size_];
}

testing_unit::~testing_unit()
{
	delete graph_;

	delete[] final_path_;
	delete[] visited_nodes_;
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
		bb();
		t.stop();
		times_.push_back(t.elapsed_time());

		solution_path_.cost = final_cost_;
		for (int i = 0; i < graph_size_ + 1; i++)
		{
			solution_path_.nodes.push_back(final_path_[i]);
		}

		cout << "[" << i + 1 << "/" << number_of_repeats_ << "]";

		cout << ". Optimal path cost: " << optimal_path_.cost;
		cout << ". Optimal path nodes: ";
		if (optimal_path_.nodes.empty() || optimal_path_.nodes[0] == -1) cout << "NoData";
		else for (auto n : optimal_path_.nodes) cout << n << " ";
		cout << endl;

		cout << "\tSolution path cost: " << solution_path_.cost;
		cout << ". Solution path nodes: ";
		if (solution_path_.nodes.empty()) cout << "NoData";
		else for (auto n : solution_path_.nodes) cout << n << " ";
		cout << endl;

		cout << "\tTime: ";
		cout << fixed << setprecision(9) << times_.back() << endl;

	}
	cout << endl;

}

int testing_unit::find_first_min_edge_cost_from_n(int n)
{
	int min_cost = INT_MAX;
	for (int i = 0; i < graph_size_; i++)
	{
		if (graph_->get_value_at(i, n) < min_cost && i != n)
		{
			min_cost = graph_->get_value_at(i, n);
		}
	}
	return min_cost;
}
int testing_unit::find_second_min_edge_cost_from_n(int n)
{
	int first_min_cost = INT_MAX;
	int second_min_cost = INT_MAX;

	for (int i = 0; i < graph_size_; i++)
	{
		if (n == i) continue;

		if (graph_->get_value_at(i, n) <= first_min_cost)
		{
			second_min_cost = first_min_cost;
			first_min_cost = graph_->get_value_at(i, n);
		}
		else if (graph_->get_value_at(i, n) <= second_min_cost && graph_->get_value_at(i, n) != first_min_cost)
		{
			second_min_cost = graph_->get_value_at(i, n);
		}
	}
	return second_min_cost;
}


void testing_unit::bb()
{
	// Ustaw koszt sciezki na "INF".
	final_cost_ = INT_MAX;

	// ¯aden wierzcho³ek nie zosta³ jeszcze odwiedzony.
	for (int i = 0; i < graph_size_; i++)
	{
		visited_nodes_[i] = false;
	}

	// Oblicz pocz¹tkow¹ granicê.
	int current_bound = 0;
	for (int i = 0; i < graph_size_; i++)
	{
		current_bound += find_first_min_edge_cost_from_n(i);
		current_bound += find_second_min_edge_cost_from_n(i);
	}
	current_bound = (current_bound & 1) ? current_bound / 2 + 1 : current_bound / 2;
	
	// Zmienna do zapisywania obecnej œcie¿ki.
	int* current_path = new int[graph_size_ + 1];
	for (int i = 0; i < graph_size_ + 1; i++)
	{
		current_path[i] = -1;
	}

	// Œcie¿ka rozpoczyna siê od wierzcho³ka 0.
	visited_nodes_[0] = true;
	current_path[0] = 0;

	// Wywo³anie funkcji rekurencyjnej wyznaczaj¹cej œcie¿kê.
	bb_recursion(current_bound, 0, 1, current_path);

	delete[] current_path;
}

void testing_unit::bb_recursion(int current_bound, int current_weight, int level, int* current_path)
{
	// Osi¹gniêto ostatni wierzcho³ek.
	if (level == graph_size_)
	{
		// KrawêdŸ od ostatniego wierzcho³ka do pocz¹tkowego.
		int x = current_path[0];
		int y = current_path[level - 1];
		if (x != y && graph_->get_value_at(x, y) >= 0)
		{
			// Pe³ny koszt œcie¿ki.
			int current_cost = current_weight + graph_->get_value_at(x, y);

			// Je¿eli koszt tej œcie¿ki jest lepszy od obecnego kosztu
			// to zapisz tê œcie¿kê, jako najlepsz¹.
			if (current_cost < final_cost_)
			{
				final_cost_ = current_cost;
				for (int i = 0; i < graph_size_; i++)
				{
					final_path_[i] = current_path[i];
				}
				final_path_[graph_size_] = current_path[0];
			}
		}
		return;
	}

	// Wierzcho³ek w œrodku œcie¿ki.
	for (int i = 0; i < graph_size_; i++)
	{
		// n jest obecnie rozpatrywanym wierzcho³kiem rodzicem.
		// Dla tego wierzcho³ka sprawdzane s¹ dzieci.
		int n = current_path[level - 1];
		if (i != n && graph_->get_value_at(i, n) >= 0 && visited_nodes_[i] == false)
		{
			// Zapisz obecn¹ granicê.
			int temp = current_bound;
			// Oblicz koszt œcie¿ki do dziecka.
			current_weight += graph_->get_value_at(i, n);

			// Oblicz now¹ granicê.
			if (level == 1)
			{
				current_bound -= ((find_second_min_edge_cost_from_n(n) + find_second_min_edge_cost_from_n(i)) / 2);
			}
			else
			{
				current_bound -= ((find_first_min_edge_cost_from_n(n) + find_second_min_edge_cost_from_n(i)) / 2);
			}
				


			// Je¿eli wierzcho³ek dziecko jest obiecuj¹cy
			// to eksplorujemy go dalej.
			if (current_bound + current_weight < final_cost_)
			{
				current_path[level] = i;
				visited_nodes_[i] = true;

				// Wywo³anie funkcji rekurencyjnej wyznaczaj¹cej dalsz¹ œcie¿kê.
				bb_recursion(current_bound, current_weight, level + 1, current_path);
			}
			
			// Je¿eli nie jest obiecuj¹cy, to cofamy zmiany.
			current_weight -= graph_->get_value_at(i, n);
			current_bound = temp;

			// Resetujemy tablicê wierzcho³ków odwiedzonych.
			for (int k = 0; k < graph_size_; k++)
			{
				visited_nodes_[k] = false;
			}

			// Ustawienie wierzcho³ków odwiedzonych.
			for (int j = 0; j < level; j++)
			{
				visited_nodes_[current_path[j]] = true;
			}
		}
	}

}

void testing_unit::save_data(ofstream& fout)
{
	fout << input_file_name_ << " ";
	fout << number_of_repeats_ << " ";
	fout << solution_path_.cost << " ";

	if (solution_path_.nodes.empty()) fout << "NoData.";
	else for (auto n : solution_path_.nodes) fout << n << " ";
	fout << endl;

	for (auto time : times_)
	{
		fout << fixed << setprecision(9) << time << endl;
	}

}


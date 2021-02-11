#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>

#include "graph.h"
#include "testing_unit.h"

using namespace std;

vector<string> load_config_lines(string config_file_name);
testing_unit create_testing_unit(string unit_parameters);

int main()
{
	vector<string> config_lines;
	try
	{
		config_lines = load_config_lines("config.ini");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		exit(0);
	}

	string output_file_name = config_lines.back();
	ofstream output_stream(output_file_name);

	if(!output_stream.is_open())
	{
		cout << "Output file cannot be opened!" << endl;
		exit(0);
	}

	for (int i = 0; i < config_lines.size() - 1; i++)
	{
		try
		{
			testing_unit tu = create_testing_unit(config_lines[i]);
			tu.run_tests();
			tu.save_tests_results(output_stream);
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
			output_stream.close();
			exit(0);
		}
	}

	output_stream.close();

	cout << "Done. Data saved to file. Press" << endl;
	system("pause");
}

vector<string> load_config_lines(string config_file_name)
{
	vector<string> config_lines;

	ifstream config_fin(config_file_name);
	if (!config_fin.is_open())
	{
		throw exception("Config file not found!");
	}

	while (!config_fin.eof())
	{
		string line;
		getline(config_fin, line);
		config_lines.push_back(line);
	}
	config_fin.close();

	if (config_lines.empty())
	{
		throw exception("Config not loaded!");
	}

	return config_lines;
}

testing_unit create_testing_unit(string unit_parameters)
{
	istringstream iss(unit_parameters);
	vector<string> config_line_split((istream_iterator<string>(iss)), istream_iterator<string>());

	if (!(config_line_split.size() > 3)) // Filename, number of repeats, solution. Rest is optimial path.
	{
		throw exception("Data broken!");
	}

	//string file_name = "graphs/" + config_line_split[0];
	string file_name = config_line_split[0];
	int number_of_repeats = atoi(config_line_split[1].c_str());
	int expected_solution = atoi(config_line_split[2].c_str());
	vector<int> expected_path;
	for (int i = 3; i < config_line_split.size(); i++)
	{
		expected_path.push_back(atoi(config_line_split[i].c_str()));
	}

	return testing_unit(file_name, number_of_repeats, expected_solution, expected_path);
}

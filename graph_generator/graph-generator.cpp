#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


using namespace std;

int main()
{
	srand(time(NULL));

	ofstream config_fout("testdata/config.ini");
	if (!config_fout.is_open())
	{
		std::cout << "Config out error!" << endl;
		exit(0);
	}

	int smallest_instance = 5;
	int biggest_instance = 32;
	const int min_val = 1;
	const int max_val = 100;

	for (int i = smallest_instance; i < biggest_instance + 1; i++)
	{
		string file_name = "testdata/test" + to_string(i) + ".txt";
		ofstream graph_fout(file_name);

		if (!graph_fout.is_open())
		{
			std::cout << "File out error!" << endl;
			config_fout.close();
			exit(0);
		}

		config_fout << file_name << " 1 -1 -1" << endl;
		graph_fout << i << endl;

		for (int y = 0; y < i; y++)
		{
			for (int x = 0; x < i; x++)
			{
				if (x == y)
				{
					graph_fout << fixed << setw(4) << 0 << " ";
				}
				else
				{
					graph_fout << fixed << setw(4) << min_val + rand() % (max_val - min_val + 1) << " ";
				}
			}
			graph_fout << endl;
		}

		graph_fout.close();
	}

	config_fout << "output.csv" << endl;
	config_fout.close();

	std::cout << "Graphs generated and saved to files." << endl;
}
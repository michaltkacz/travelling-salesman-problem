#pragma once
#include <vector>
class tabu_list
{

public:

	tabu_list(int n);
	bool contains(int i, int j);
	//bool contains(std::vector<int> v);
	void add(int i, int j, int c);
	//void try_add(int i, int j, int c);
	//void add(std::vector<int> v, int c);
	void clear();
	void update_cadences_and_delete_elements();

	void print();

private:
	struct tabu_list_item
	{
		int i = 0;
		int j = 0;
		int c = 0;
	};

	//struct tabu_list_item
	//{
	//	std::vector<int> v = {  };
	//	int c = 0;
	//};
	int max_tabu_size_ = 0;

	std::vector<tabu_list_item> tabu_list_ = {};




};


#include "tabu_list.h"
#include <iostream>

tabu_list::tabu_list(int n)
{
	max_tabu_size_ = n;
}

bool tabu_list::contains(int i, int j)
{
	for (auto& item : tabu_list_)
	{
		if ((item.i == i && item.j == j) || (item.i == j && item.j == i))
			return true;
	}
	return false;
}

//bool tabu_list::contains(std::vector<int> v)
//{
//	for (auto& item : tabu_list_)
//	{
//		bool same = false;
//		for (int i = 0; i < item.v.size(); i++)
//		{
//			if (item.v[i] != v[i])
//			{
//				same = false;
//				break;
//			}
//			same = true;
//		}
//		if (same)
//		{
//			return true;
//		}
//	}
//	return false;
//}

//void tabu_list::add(std::vector<int> v, int c)
//{
//	if (tabu_list_.size() >= max_tabu_size_)
//	{
//		tabu_list_.erase(tabu_list_.begin());
//	}
//	tabu_list_.push_back(tabu_list_item{ v, c });
//}

void tabu_list::add(int i, int j, int c)
{
	if (tabu_list_.size() >= max_tabu_size_)
	{
		tabu_list_.erase(tabu_list_.begin());
	}
	tabu_list_.push_back(tabu_list_item{ i,j,c });
}

//void tabu_list::try_add(int i, int j, int c)
//{
//	if (tabu_list_.size() < max_tabu_size_)
//	{
//		tabu_list_.push_back(tabu_list_item{ i,j,c });
//
//	}
//}

void tabu_list::clear()
{
	tabu_list_.clear();
}

void tabu_list::update_cadences_and_delete_elements()
{
	for (int i= tabu_list_.size()-1; i>=0; i--)
	{
		tabu_list_[i].c--;
		if (tabu_list_[i].c <= 0)
		{
			tabu_list_.erase(tabu_list_.begin() + i);
		}
	}
}

void tabu_list::print()
{
	std::cout << tabu_list_.size() << std::endl;
}

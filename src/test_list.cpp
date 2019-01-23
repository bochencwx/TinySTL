#include <iostream>

#include "../src/tinySTL_list.h"

using namespace TinySTL;

template<typename InputIterator>
void print(InputIterator first, InputIterator last)
{
	while (first != last)
	{
		std::cout << *first << "\t";
		++first;
	}
	std::cout << std::endl;
}

template<typename InputIterator>
void printList(InputIterator first, InputIterator last)
{
	while (first != last)
	{
		std::cout << *first << "\t";
		++first;
	}
	std::cout << std::endl;
}

int main()
{
	list<int> lisint;
	lisint.push_back(1);
	lisint.push_back(2);
	lisint.push_back(3);
	lisint.push_back(4);
	lisint.push_back(5);
	lisint.push_back(6);
	printList(lisint.begin(), lisint.end());

	list<int> other;
	other.push_back(101);
	other.push_back(102);
	other.push_back(103);
	other.push_back(104);
	other.push_back(105);
	other.push_back(106);
	print(other.begin(), other.end());

	lisint.splice(lisint.begin(), other);
	print(lisint.begin(), lisint.end());
	print(other.begin(), other.end());

	return 0;
}
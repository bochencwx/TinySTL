#include "../src/tinySTL_iterator.h"
#include <iostream>
#include <typeinfo>
using namespace TinySTL;

// ����һ��������
template<typename T>
class tinyIter :public iterator<random_access_iterator_tag, T>
{

};

// ����һ������
template<typename T>
class tiny
{
public:
	typedef tinyIter<T> iterator;
};

int main()
{
	tiny<int> vec;
	tiny<int>::iterator iter;
	struct random_access_iterator_tag test_random_access_iterator_tag;
	std::cout << typeid(iterator_category(iter)).name() << std::endl;
	std::cout << typeid(test_random_access_iterator_tag).name() << std::endl;
	return 0;

}
/*

#include "../src/tinySTL_alloc.h"

#include <iostream>

using namespace TinySTL;

void callback()
{
	std::cout << "fail..." << std::endl;
}

int main()
{
	__malloc_alloc::set_malloc_handler(callback);
	for (;;)
	{
		simple_alloc<int, alloc>::allocate(1000);
	}
	return 0;
}

*/
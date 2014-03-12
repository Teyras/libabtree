#include <iostream>
#include <chrono>
#include <map>
#include <algorithm>
#include <cstdlib>

#include "abtree.hpp"

template <typename F>
double measure_time(F f)
{
	auto tb = std::chrono::steady_clock::now();
	f();
	auto te = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>( te - tb).count() / 1000000.0;
}

void print_result (std::string label, double t)
{
	std::cout << label << ": " << t << "s" << std::endl;
}

template <typename T, typename C>
void run_test (C & container, const std::vector<T> & data)
{
	double t;
	
	t = measure_time([&container, &data] () {
		for (T i: data) {
			container.insert(std::make_pair(i, true));
		}
	});
	print_result("Insert", t);
	
	t = measure_time([&container, &data] () {
		for (T i: data) {
			container.find(i);
		}
	});
	print_result("Find", t);
	
	t = measure_time([&container] () {
		for (auto it = container.begin(); it != container.end(); ++it) {
			
		}
	});
	print_result("Traversal", t);
	
	t = measure_time([&container, &data] () {
		for (T i: data) {
			container.erase(i);
		}
	});
	print_result("Delete", t);
}

template <typename T>
void test_tree (size_t a, size_t b, const std::vector<T> & data)
{
	std::cout << "* (" << a << ", " << b << ") Tree" << std::endl;
	abtree<T, bool> tree(a, b);
	run_test<T>(tree, data);
}

int main (int argc, char ** argv)
{
	std::vector<int> int_data(1024 * 1024);
	std::generate(int_data.begin(), int_data.end(), std::rand);
	
	test_tree<int>(2, 3, int_data);
	
	test_tree<int>(2, 4, int_data);
	
	test_tree<int>(3, 5, int_data);
	
	std::cout << "* Map" << std::endl;
	std::map<int, bool> map;
	run_test<int>(map, int_data);
	
	return 0;
}
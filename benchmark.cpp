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

int main (int argc, char ** argv)
{
	std::vector<int> int_data(1024 * 1024);
	std::generate(int_data.begin(), int_data.end(), std::rand);
	
	std::cout << "* Map" << std::endl;
	std::map<int, bool> map;
	run_test<int>(map, int_data);
	
	std::cout << "* (2, 3) Tree" << std::endl;
	abtree<int, bool> tree(2, 3);
	run_test<int>(tree, int_data);
	
	return 0;
}
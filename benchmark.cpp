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

void print_result (double t1, double t2)
{
	std::cout << "Map: " << t1 << std::endl;
	std::cout << "Tree: " << t2 << std::endl;
	std::cout << std::endl;
}

template <typename T>
void run_test (const std::vector<T> & data)
{
	std::map<T, bool> map;
	abtree<T, bool> tree(2, 4);
	double t1, t2;
	
	std::cout << "* Testing insert" << std::endl;
	
	t1 = measure_time([&map, &data] () {
		for (T i: data) {
			map.insert(std::make_pair(i, true));
		}
	});
	t2 = measure_time([&tree, &data] () {
		for (T i: data) {
			tree.insert(std::make_pair(i, true));
		}
	});
	
	print_result(t1, t2);
	
	std::cout << "* Testing find" << std::endl;
	
	t1 = measure_time([&map, &data] () {
		for (T i: data) {
			map.find(i);
		}
	});
	t2 = measure_time([&tree, &data] () {
		for (T i: data) {
			tree.find(i);
		}
	});
	
	print_result(t1, t2);
	
	std::cout << "* Testing delete" << std::endl;
	
	t1 = measure_time([&map, &data] () {
		for (T i: data) {
			map.erase(i);
		}
	});
	t2 = measure_time([&tree, &data] () {
		for (T i: data) {
			tree.erase(i);
		}
	});
	
	print_result(t1, t2);
}

int main (int argc, char ** argv)
{
	std::vector<int> int_data(1024 * 1024);
	std::generate(int_data.begin(), int_data.end(), std::rand);
	run_test<int>(int_data);
	
	return 0;
}
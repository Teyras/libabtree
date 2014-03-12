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

template <typename T>
void test_set (const std::vector<T> & data)
{
	test_tree<T>(2, 3, data);
	
	test_tree<T>(2, 4, data);
	
	test_tree<T>(3, 5, data);
	
	std::cout << "* Map" << std::endl;
	std::map<T, bool> map;
	run_test<T>(map, data);
}

int main (int argc, char ** argv)
{
	std::vector<int> int_data(1024 * 1024);
	std::generate(int_data.begin(), int_data.end(), std::rand);
	
	std::cout << "== Testing int ==" << std::endl;
	test_set<int>(int_data);
	
	auto rand_string = [] () {
		size_t len = 20;
		std::string str(len, 0);
		char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
		for (size_t i = 0; i < len; i++) {
			str[i] = chars[std::rand() % sizeof(chars)];
		}
		return str;
	};
	
	std::vector<std::string> string_data(1024 * 1024);
	std::generate(string_data.begin(), string_data.end(), rand_string);
	
	std::cout << "== Testing string ==" << std::endl;
	test_set<std::string>(string_data);
	
	return 0;
}
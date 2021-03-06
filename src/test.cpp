#include <iostream>
#include <vector>
#include <algorithm>
#include "abtree.hpp"

void msg (std::string text)
{
	std::cout << "* " << text.c_str() << std::endl;
}

void report (bool value)
{
	if (value) {
		std::cout << "OK" << std::endl;
	} else {
		std::cout << "FAIL" << std::endl;
	}
}

bool check_order (abtree<int, std::string>::iterator it, std::vector<int> & keys)
{
	for (int key: keys) {
		if (key != it->first) {
			return false;
		}
		++it;
	}
	return true;
}

int main (int argc, char ** argv)
{
	abtree<int, std::string> tree(2, 3);
	abtree<int, std::string>::iterator it;
	bool status;
	
	int key_data[] = {
		9, 54, 68, 27, 18, 98, 74, 44, 1, 73, 
		67, 75, 93, 92, 76, 69, 78, 4, 32, 52, 
		81, 97, 71, 99, 87, 43, 53, 57, 30, 85, 
		45, 95, 82, 63, 26, 65, 20, 28, 31, 94
	};
	
	std::vector<int> keys(key_data, key_data + sizeof(key_data) / sizeof(int));
	
	msg("Checking insert & traversal order");
	status = true;
	for (int key: keys) {
		auto it = tree.insert(std::make_pair(key, std::string("foo")));
		status = status && it->first == key;
	}
	
	std::sort(begin(keys), end(keys));
	it = tree.begin();
	
	report(status && check_order(it, keys));
	
	msg("Checking reverse traversal order");
	status = true;
	it = tree.end();
	auto it2 = keys.end();
	
	while (it != tree.begin()) {
		--it;
		--it2;
		status = status && it->first == *it2;
	}
	report(status);
	
	msg("Trying to find something");
	status = true;
	for (int key: keys) {
		it = tree.find(key);
		status = status && key == it->first;
	}
	report(status);
	
	msg("Trying to insert something that's already in the tree");
	int key = key_data[sizeof(key_data) / (2 * sizeof(int))];
	tree.insert(std::make_pair(key, std::string("bar")));
	it = tree.find(key);
	report(it->second == std::string("bar"));
	
	msg("Trying to find a bogus key");
	it = tree.find(666);
	report(it == tree.end());
	
	msg("Testing lower bound");
	status = true;
	for (int i = 0; i <= (--tree.end())->first; i++) {
		if (!(tree.lower_bound(i)->first == *std::lower_bound(keys.begin(), keys.end(), i))) {
			status = false;
			break;
		}
	}
	report(status);
	
	msg("Testing upper bound");
	status = true;
	for (int i = 0; i <= (--tree.end())->first; i++) {
		auto tree_upper = tree.upper_bound(i);
		auto keys_upper = std::upper_bound(keys.begin(), keys.end(), i);
		if (tree_upper == tree.end()) {
			if (keys_upper != keys.end()) {
				status = false;	
			}
		} else if (!(tree_upper->first == *keys_upper)) {
			status = false;
			break;
		}
	}
	report(status);
	
	msg("Erasing a key");
	tree.erase(key);
	keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
	it = tree.find(key);
	report(it == tree.end() && check_order(tree.begin(), keys));
	
	msg("Erasing keys from root");
	status = true;
	for (int i = 1; i <= 4; i++) {
		key = tree.get_root_key();
		tree.erase(key);
		keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
		it = tree.find(key);
		status = status && (it == tree.end() && check_order(tree.begin(), keys));
	}
	report(status);
	
	msg("Erasing keys from the start");
	status = true;
	for (int i = 1; i <= 4; i++) {
		key = tree.begin()->first;
		tree.erase(key);
		keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
		it = tree.find(key);
		status = status && (it == tree.end() && check_order(tree.begin(), keys));
	}
	report(status);
	
	msg("Deleting the whole thing");
	for (int key: key_data) {
		keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
		tree.erase(key);
		if (!check_order(tree.begin(), keys)) {
			break;
		}
	}
	report(tree.size() == 0);
	
	return 0;
}

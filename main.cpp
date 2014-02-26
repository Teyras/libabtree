#include <iostream>
#include <vector>
#include <algorithm>
#include "abtree.h"

void msg (std::string text)
{
	std::cout << "* " << text << std::endl;
}

void report (bool value)
{
	if (value) {
		std::cout << "OK" << std::endl;
	} else {
		std::cout << "FAIL" << std::endl;
	}
}

bool check_order (abtree/*<int, std::string>*/::iterator it, std::vector<int> & keys)
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
	abtree/*<int, std::string>*/ tree(2, 3);
	abtree/*<int, std::string>*/::iterator it = tree.begin();
	
	int key_data[] = {
		9, 54, 68, 27, 18, 98, 74, 44, 1, 73, 
		67, 75, 93, 92, 76, 69, 78, 4, 32, 52, 
		81, 97, 71, 99, 87, 43, 53, 57, 30, 85, 
		45, 95, 82, 63, 26, 65, 20, 28, 31, 94
	};
	
	std::vector<int> keys(key_data, key_data + sizeof(key_data) / sizeof(int));
	
	msg("Checking insert & traversal order");
	
	for (int key: keys) {
		tree.insert(std::make_pair(key, std::string("foo")));
	}
	
	std::sort(begin(keys), end(keys));
	it = tree.begin();
	
	report(check_order(it, keys));
	
	msg("Trying to find something");
	int key = key_data[sizeof(key_data) / (2 * sizeof(int))];
	it = tree.find(key);
	report(key == it->first);
	
	msg("Trying to insert something that's already in the tree");
	tree.insert(std::make_pair(key, std::string("bar")));
	it = tree.find(key);
	report(it->second == std::string("bar"));
	
	msg("Trying to find a bogus key");
	it = tree.find(666);
	report(it == tree.end());
	
	msg("Erasing a key");
	tree.erase(key);
	keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
	it = tree.find(key);
	report(it == tree.end() && check_order(tree.begin(), keys));
	
	msg("Erasing keys from root");
	for (int i = 1; i <= 4; i++) {
		key = tree.get_root_key();
		tree.erase(key);
		keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
		it = tree.find(key);
		report(it == tree.end() && check_order(tree.begin(), keys));
	}
	
	return 0;
}

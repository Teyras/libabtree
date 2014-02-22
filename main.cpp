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

int main (int argc, char ** argv)
{
	abtree/*<int, std::string>*/ tree(2, 3);
	abtree::iterator it = tree.begin();
	
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
	bool ok = true;
	
	for (int key: keys) {
		if (key != it->first) {
			ok = false;
			break;
		}
		++it;
	}
	
	report(ok);
	
	msg("Trying to find something");
	
	int key = key_data[sizeof(key_data) / (2 * sizeof(int))];
	it = tree.find(key);
	report(key == it->first);
	
	msg("Trying to find a bogus key");
	it = tree.find(666);
	report(it == tree.end());
	
	return 0;
}

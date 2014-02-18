#include <iostream>
#include "abtree.h"

int main(int argc, char ** argv) {
	abtree/*<int, std::string>*/ tree(2, 3);
	abtree::iterator it = tree.begin();
	
	int keys[] = {
		9, 54, 68, 27, 18, 98, 74, 44, 1, 73, 
		67, 75, 93, 92, 76, 69, 78, 4, 32, 52, 
		81, 97, 71, 99, 87, 43, 53, 57, 30, 85, 
		45, 95, 82, 63, 26, 65, 20, 28, 31, 94
	};
	
	for (size_t i = 0; i < sizeof(keys) / sizeof(int); i++) {
		tree.insert(std::make_pair(keys[i], std::string("foo")));
	}
	
	std::cout << tree.size() << std::endl;
	
	it = tree.begin();
	for (size_t i = 0; i < 3; i++) {
		std::cout << it->first << " " << it->second << std::endl;
		++it;
	}
	
	return 0;
}

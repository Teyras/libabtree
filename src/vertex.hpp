#ifndef _ABTREE_VERTEX_HPP_
#define _ABTREE_VERTEX_HPP_

#include <iostream>

template <typename TKey, typename TVal>
class abtree;

/**
 * A vertex of an (a, b)-tree
 */
template <typename TKey, typename TVal>
struct abtree_vertex
{
	abtree_vertex * parent;
	size_t item_count;
	std::pair<const TKey, TVal> ** items;
	abtree_vertex ** children;
	
	~abtree_vertex ()
	{
		delete[] items;
		delete[] children;
	}
	
	/**
	 * Returns the index of the first item whose key is larger than or equal than given key
	 * (in other words, it tells us which child should be searched next).
	 * @param key the key to search for
	 * @return the index of the desired key
	 */
	size_t search (const TKey & key) const
	{
		size_t i, step;
		size_t first = 0;
		size_t count = item_count;
		
		while (count > 0) {
			i = first + (step = count / 2);
			if (items[i]->first < key) {
				first = i + 1;
				count -= step + 1;
			} else {
				count = step;
			}
		}
		
		return first;
	}
	
private:
	friend class abtree<TKey, TVal>;
	
	/**
	 * The default constructor. Allocates memory for given amount of items and children.
	 * One more place in memory is allocated to simplify splitting.
	 * Only the abtree container is able to conctruct a vertex.
	 * @param max_children specifies the maximum amount of children
	 */
	abtree_vertex (size_t max_children): parent(nullptr), item_count(0)
	{
		items = new std::pair<const TKey, TVal> * [max_children];
		children = new abtree_vertex<TKey, TVal> * [max_children + 1];
		
		for (size_t i = 0; i < max_children; i++) {
			items[i] = nullptr;
		}
		for (size_t i = 0; i <= max_children; i++) {
			children[i] = nullptr;
		}
	}
};

#endif
#ifndef _ABTREE_VERTEX_HPP_
#define _ABTREE_VERTEX_HPP_

#include <iostream>

template <typename TKey, typename TVal>
struct item
{
	std::pair<const TKey, TVal> pair;
	item (std::pair<const TKey, TVal> pair): pair(pair) {}
	const TKey key ()
	{
		return pair.first;
	}
	TVal & val ()
	{
		return pair.second;
	}
};

template <typename TKey, typename TVal>
struct abtree_vertex
{
	abtree_vertex * parent;
	size_t item_count;
	item<TKey, TVal> ** items;
	abtree_vertex ** children;
	
	abtree_vertex (size_t max_children): parent(nullptr), item_count(0)
	{
		items = new item<TKey, TVal> * [max_children];
		children = new abtree_vertex<TKey, TVal> * [max_children + 1];
		
		for (size_t i = 0; i < max_children; i++) {
			items[i] = nullptr;
		}
		for (size_t i = 0; i <= max_children; i++) {
			children[i] = nullptr;
		}
	}
	
	~abtree_vertex ()
	{
		delete[] items;
		delete[] children;
	}
	
	size_t search (TKey key) const
	{
		size_t i, step;
		size_t first = 0;
		size_t count = item_count;
		
		while (count > 0) {
			i = first + (step = count / 2);
			if (items[i]->key() < key) {
				first = i + 1;
				count -= step + 1;
			} else {
				count = step;
			}
		}
		
		return first;
	}
};

#endif
#ifndef _ABTREE_VERTEX_HPP_
#define _ABTREE_VERTEX_HPP_

#include <iostream>

template <typename TKey, typename TVal>
struct item
{
	std::pair<TKey, TVal> pair;
	item (std::pair<TKey, TVal> pair): pair(pair) {}
	TKey key ()
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
	size_t item_count;
	item<TKey, TVal> ** items;
	abtree_vertex ** children;
	abtree_vertex * parent;
	
	abtree_vertex (size_t max_children): parent(nullptr), item_count(0)
	{
		items = new item<TKey, TVal> * [max_children];
		children = new abtree_vertex<TKey, TVal> * [max_children + 1];
	}
	
	~abtree_vertex ()
	{
		delete[] items;
		delete[] children;
	}
	
	size_t search (TKey key) const
	{
		size_t i;
		for (i = 0; i < item_count && key > items[i]->key(); i++);
		return i;
	}
};

#endif
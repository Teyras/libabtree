#ifndef _ABTREE_H_
#define _ABTREE_H_

#include <iostream>

typedef int TKey;
typedef std::string TVal;

/*template <typename TKey, typename TVal> */class abtree 
{
private:
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
	
	struct vertex
	{
		size_t item_count;
		item ** items;
		vertex ** children;
		vertex * parent;
		
		vertex (size_t max_children): parent(nullptr), item_count(0)
		{
			items = new item * [max_children];
			children = new vertex * [max_children + 1];
		}
	};
	
	vertex * root;
	const size_t a, b;
	size_t size_;
	
	size_t search (const vertex * vertex, TKey key) const
	{
		size_t i;
		for (i = 0; i < vertex->item_count && key > vertex->items[i]->key(); i++);
		return i;
	}
	
	void split_vertex (vertex * cursor)
	{
		size_t middle = (b / 2);
		vertex * new_vertex = new vertex(b);
		item * median = cursor->items[middle];
		cursor->items[middle] = nullptr;
		cursor->item_count--;
		
		for (size_t i = middle + 1; i <= b; i++) {
			new_vertex->children[i - (middle + 1)] = cursor->children[i];
			cursor->children[i] = nullptr;
		}
		for (size_t i = middle + 1; i < b; i++) {
			new_vertex->items[i - (middle + 1)] = cursor->items[i];
			new_vertex->item_count++;
			cursor->items[i] = nullptr;
			cursor->item_count--;
		}
		
		if (cursor->parent == nullptr) {
			root = new vertex(b);
			root->children[0] = cursor;
			root->items[0] = median;
			root->children[1] = new_vertex;
			root->item_count++;
			cursor->parent = root;
			new_vertex->parent = root;
			return;
		}
		
		size_t pos = search(cursor->parent, median->key());
		for (size_t i = cursor->item_count; i > pos; i--) {
			if (i < cursor->item_count) {
				cursor->parent->items[i] = cursor->parent->items[i - 1];
			}
			cursor->parent->children[i] = cursor->parent->children[i - 1];
		}
		
		cursor->parent->items[pos] = median;
		cursor->parent->children[pos] = new_vertex;
		cursor->parent->item_count++;
		
		if (cursor->parent->item_count == b) {
			split_vertex(cursor->parent);
		}
	}
public:
	class iterator
	{
	public:
		iterator (abtree * tree, abtree::vertex * current_vertex, size_t position):
			tree_(tree), vertex_(current_vertex), position_(position), end_(false)
		{}
		
		iterator & operator++ ()
		{
			if (vertex_->children[0] != nullptr) {
				while (vertex_->children[0] != nullptr) {
					vertex_ = vertex_->children[position_ + 1];
					position_ = 0;
				}
			} else {
				position_++;
				if (vertex_->children[0] == nullptr) {
					position_++;
				}
				while (position_ > vertex_->item_count) {
					if (vertex_->parent == nullptr) {
						end_ = true;
						break;
					}
					position_ = tree_->search(vertex_->parent, vertex_->items[0]->key());
					vertex_ = vertex_->parent;
				}
			}
			
			return *this;
		}
		
		std::pair<TKey, TVal> & operator* ()
		{
			return vertex_->items[position_]->pair;
		}
		
		std::pair<TKey, TVal> * operator-> ()
		{
			return &vertex_->items[position_]->pair;
		}
		
		bool operator== (const iterator & it)
		{
			return (end_ && it.end_) || (
				tree_ == it.tree_ &&
				position_ == it.position_ &&
				vertex_ == it.vertex_
			);
		}
	private:
		abtree * tree_;
		abtree::vertex * vertex_;
		size_t position_;
		bool end_;
	};
	
	abtree (size_t a, size_t b): a(a), b(b), size_(0)
	{
		root = new vertex(b);
	}
	
	~abtree ()
	{
		
	}
	
	iterator begin ()
	{
		auto cursor = root;
		while (cursor->children[0] != nullptr) {
			cursor = cursor->children[0];
		}
		return iterator(this, cursor, 0);
	}
	
	iterator find (const TKey & key, vertex * cursor = nullptr)
	{
		if (cursor == nullptr) {
			cursor = root;
		}
		size_t i = search(cursor, key);
		if (i < cursor->item_count && cursor->items[i]->key() == key) {
			return iterator(this, cursor, i);
		}
		if (cursor->children[i] == nullptr) {
			return iterator(this, cursor, i);
		}
		return find(key, cursor->children[i]);
	}
	
	void insert (std::pair<TKey, TVal> pair)
	{
		auto new_item = new item(pair);
		auto cursor = root;
		while (true) {
			size_t i = search(cursor, new_item->key());
			if (i < cursor->item_count && cursor->items[i]->key() == new_item->key()) {
				cursor->items[i]->pair.second = new_item->val();
				return;
			}
			if (cursor->children[i] == nullptr) {
				break;
			}
			cursor = cursor->children[i];
		}
		
		size_t i = search(cursor, new_item->key());
		if (i < cursor->item_count) {
			for (size_t j = cursor->item_count; j > i && j > 0; j--) {
				cursor->items[j] = cursor->items[j - 1];
			}
		}
		
		cursor->items[i] = new_item;
		cursor->item_count++;
		
		if (cursor->item_count == b) {
			split_vertex(cursor);
		}
		
		size_++;
	}
	
	void erase (TKey key)
	{
		
	}
	
	size_t size ()
	{
		return size_;
	}
};

#endif
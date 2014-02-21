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
		
		for (size_t i = middle + 1; i < b; i++) {
			new_vertex->items[i - (middle + 1)] = cursor->items[i];
			new_vertex->item_count++;
			cursor->items[i] = nullptr;
			cursor->item_count--;
		}
		for (size_t i = middle + 1; i < b + 1; i++) {
			if (cursor->children[i] != nullptr) {
				new_vertex->children[i - (middle + 1)] = cursor->children[i];
				cursor->children[i]->parent = new_vertex;
			}
			cursor->children[i] = nullptr;
		}
		
		auto parent = cursor->parent;
		
		if (parent == nullptr) {
			root = new vertex(b);
			root->children[0] = cursor;
			root->items[0] = median;
			root->children[1] = new_vertex;
			root->item_count++;
			cursor->parent = root;
			new_vertex->parent = root;
			return;
		}
		
		size_t pos = search(parent, median->key());
		for (size_t i = parent->item_count; i > pos; i--) {
			parent->items[i] = parent->items[i - 1];
		}
		for (size_t i = parent->item_count + 1; i > pos + 1; i--) {
			parent->children[i] = parent->children[i - 1];
		}
		
		parent->items[pos] = median;
		parent->children[pos + 1] = new_vertex;
		parent->item_count++;
		
		new_vertex->parent = parent;
		
		if (parent->item_count == b) {
			split_vertex(parent);
		}
	}
public:
	class iterator
	{
	public:
		iterator (abtree * tree, abtree::vertex * current_vertex, size_t position):
			tree_(tree), vertex_(current_vertex), position_(position)
		{}
		
		iterator & operator++ ()
		{
			bool descending = false;
			if (vertex_->children[0] != nullptr) {
				while (vertex_->children[0] != nullptr) {
					if (position_ < vertex_->item_count) {
						if (!descending) {
							vertex_ = vertex_->children[position_ + 1];
							descending = true;
						} else {
							vertex_ = vertex_->children[0];
						}
						position_ = 0;
						
					} else {
						break;
					}
				}
			} else {
				position_++;
				while (position_ >= vertex_->item_count) {
					if (vertex_->parent == nullptr) {
						break; // incrementing end
					} else {
						position_ = tree_->search(vertex_->parent, vertex_->items[0]->key());
						vertex_ = vertex_->parent;	
					}
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
			return (
				tree_ == it.tree_ &&
				position_ == it.position_ &&
				vertex_ == it.vertex_
			);
		}
		
		bool operator!= (const iterator & it)
		{
			return !operator==(it);
		}
	private:
		abtree * tree_;
		abtree::vertex * vertex_;
		size_t position_;
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
	
	iterator end ()
	{
		return iterator(this, root, root->item_count);
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
			return end();
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
			for (size_t j = cursor->item_count; j > i; j--) {
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
	
	void dump (vertex * cursor = nullptr, size_t indent = 0) const
	{
		if (cursor == nullptr) {
			cursor = root;
		}
		size_t i;
		for (i = 0; i < cursor->item_count; i++) {
			if (cursor->children[i] != nullptr) {
				dump(cursor->children[i], indent + 1);
			}
			for (size_t j = 0; j < 4 * indent; j++) {
				std::cout << " ";
			}
			std::cout << "*Key: " << cursor->items[i]->key() << " @" << cursor << std::endl;
			
		}
		if (cursor->children[i] != nullptr) {
			dump(cursor->children[cursor->item_count], indent + 1);
		}
	}
};

#endif
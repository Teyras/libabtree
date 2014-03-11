#ifndef _ABTREE_HPP_
#define _ABTREE_HPP_

#include <iostream>
#include <queue>
#include "vertex.hpp"
#include "iterator.hpp"

template <typename TKey, typename TVal>
class abtree {
private:
	typedef abtree_vertex<TKey, TVal> vertex;
	
	vertex * root;
	const size_t a, b;
	size_t size_;
	
	void split_vertex (vertex * cursor)
	{
		size_t middle = (b / 2);
		vertex * new_vertex = new vertex(b);
		auto median = cursor->items[middle];
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
		
		if (cursor == root) {
			root = new vertex(b);
			root->children[0] = cursor;
			root->items[0] = median;
			root->children[1] = new_vertex;
			root->item_count++;
			cursor->parent = root;
			new_vertex->parent = root;
			return;
		}
		
		auto parent = cursor->parent;
		
		size_t pos = parent->search(median->key());
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
	
	vertex * refill_vertex (vertex * parent, size_t i)
	{
		auto cursor = parent->children[i];
		if (i > 0) {
			auto neighbour = cursor->parent->children[i - 1];
			if (neighbour->item_count >= a) {
				for (size_t j = cursor->item_count; j > 0; j--) {
					cursor->items[j] = cursor->items[j - 1];
				}
				for (size_t j = cursor->item_count + 1; j > 0; j--) {
					cursor->children[j] = cursor->children[j - 1];
				}
				cursor->items[0] = cursor->parent->items[i - 1];
				cursor->children[0] = neighbour->children[neighbour->item_count];
				if (cursor->children[0] != nullptr) {
					cursor->children[0]->parent = cursor;
				}
				cursor->item_count++;
				cursor->parent->items[i - 1] = neighbour->items[neighbour->item_count - 1];
				neighbour->items[neighbour->item_count - 1] = nullptr;
				neighbour->children[neighbour->item_count] = nullptr;
				neighbour->item_count--;
				return cursor;
			} else {
				merge_vertices(neighbour, cursor, i - 1);
				return neighbour;
			}
		} else {
			auto neighbour = cursor->parent->children[1];
			if (neighbour->item_count >= a) {
				cursor->items[cursor->item_count] = cursor->parent->items[0];
				cursor->children[cursor->item_count + 1] = neighbour->children[0];
				if (neighbour->children[0] != nullptr) {
					neighbour->children[0]->parent = cursor;
				}
				cursor->item_count++;
				cursor->parent->items[0] = neighbour->items[0];
				for (size_t j = 0; j < neighbour->item_count - 1; j++) {
					neighbour->items[j] = neighbour->items[j + 1];
				}
				for (size_t j = 0; j < neighbour->item_count; j++) {
					neighbour->children[j] = neighbour->children[j + 1];
				}
				neighbour->items[neighbour->item_count - 1] = nullptr;
				neighbour->children[neighbour->item_count] = nullptr;
				neighbour->item_count--;
			} else {
				merge_vertices(cursor, neighbour, 0);
			}
			return cursor;
		}
	}
	
	void merge_vertices (vertex * left, vertex * right, size_t key_pos)
	{
		size_t pos;
		if (left->parent != root) {
			pos = left->parent->parent->search(left->parent->items[key_pos]->key());
		}
		
		left->items[left->item_count] = right->parent->items[key_pos];
		left->item_count++;
		for (size_t j = 0; j < right->item_count; j++) {
			left->items[left->item_count] = right->items[j];
			left->children[left->item_count] = right->children[j];
			if (right->children[j] != nullptr) {
				right->children[j]->parent = left;
			}
			left->item_count++;
		}
		left->children[left->item_count] = right->children[right->item_count];
		if (left->children[left->item_count] != nullptr) {
			left->children[left->item_count]->parent = left;
		}
		
		for (size_t j = key_pos + 1; j < left->parent->item_count; j++) {
			left->parent->items[j - 1] = left->parent->items[j];
			left->parent->children[j] = left->parent->children[j + 1];
		}
		left->parent->item_count--;
		
		if (left->parent != root && left->parent->item_count < a - 1) {
			refill_vertex(left->parent->parent, pos);
		} else if (left->parent == root && left->parent->item_count == 0) {
			root = left;
			delete left->parent;
			left->parent = nullptr;
		}
		delete right;
	}
public:
	typedef abtree_iterator<TKey, TVal> iterator;
	typedef abtree_iterator<TKey, const TVal> const_iterator;
	typedef TKey key_type;
	typedef TVal mapped_type;
	typedef std::pair<key_type, mapped_type> value_type;
	
	abtree (size_t a, size_t b): a(a), b(b), size_(0)
	{
		root = new vertex(b);
	}
	
	~abtree ()
	{
		std::queue<vertex *> queue;
		queue.push(root);
		
		while (queue.size() > 0) {
			vertex * cursor = queue.front();
			for (size_t i = 0; i < cursor->item_count; i++) {
				delete cursor->items[i];
			}
			for (size_t i = 0; i <= cursor->item_count; i++) {
				if (cursor->children[i] != nullptr) {
					queue.push(cursor->children[i]);
				}
			}
			delete cursor;
			queue.pop();
		}
	}
	
	iterator begin ()
	{
		auto cursor = root;
		while (cursor->children[0] != nullptr) {
			cursor = cursor->children[0];
		}
		return iterator(cursor, 0);
	}
	
	iterator end ()
	{
		return iterator(root, root->item_count);
	}
	
	template<typename iterator>
	iterator do_find (const TKey & key, vertex * cursor = nullptr)
	{
		if (cursor == nullptr) {
			cursor = root;
		}
		size_t i = cursor->search(key);
		if (i < cursor->item_count && cursor->items[i]->key() == key) {
			return iterator(cursor, i);
		}
		if (cursor->children[i] == nullptr) {
			return end();
		}
		return do_find<iterator>(key, cursor->children[i]);
	}
	
	iterator find (const TKey & key)
	{
		return do_find<iterator>(key);
	}
	
	const_iterator find (const TKey & key) const
	{
		return do_find<const_iterator>(key);
	}
	
	TVal & at (const TKey & key)
	{
		return find(key)->second;
	}
	
	iterator lower_bound (const key_type & key)
	{
		auto cursor = root;
		size_t i = cursor->search(key);
		std::pair<vertex *, size_t> back = std::make_pair(root, root->item_count);
		
		while (cursor->children[0] != nullptr) {
			if (i < cursor->item_count) {
				if (cursor->items[i]->key() == key) {
					return iterator(cursor, i);
				}
				back = std::make_pair(cursor, i);
			}
			cursor = cursor->children[i];
			i = cursor->search(key);
		}
		
		if (i == cursor->item_count) {
			cursor = back.first;
			i = back.second;
		}
		
		return iterator(cursor, i);
	}
	
	iterator insert (value_type pair)
	{
		auto new_item = new item<TKey, TVal>(pair);
		auto cursor = root;
		while (true) {
			size_t i = cursor->search(new_item->key());
			if (i < cursor->item_count && cursor->items[i]->key() == new_item->key()) {
				delete cursor->items[i];
				cursor->items[i] = new_item;
				return iterator(cursor, i);
			}
			if (cursor->children[i] == nullptr) {
				break;
			}
			cursor = cursor->children[i];
		}
		
		size_t i = cursor->search(new_item->key());
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
		return find(pair.first);
	}
	
	void erase (TKey key)
	{
		auto cursor = root;
		size_t i, pos;
		while (true) {
			i = cursor->search(key);
			if (i < cursor->item_count && cursor->items[i]->key() == key) {
				break;
			}
			if (cursor->children[0] == nullptr) {
				return;
			}
			cursor = cursor->children[i];
		}
		
		delete cursor->items[i];
		
		if (cursor->children[0] != nullptr) {
			auto cursor_leaf = cursor->children[i];
			while (cursor_leaf->children[0] != nullptr) {
				cursor_leaf = cursor_leaf->children[cursor_leaf->item_count];
			}
			cursor->items[i] = cursor_leaf->items[cursor_leaf->item_count - 1];
			cursor = cursor_leaf;
			pos = cursor_leaf->parent->search(cursor->items[cursor->item_count -1]->key());
		} else {
			for (size_t j = i; j < cursor->item_count - 1; j++) {
				cursor->items[j] = cursor->items[j + 1];
			}
			
			if (cursor != root) {
				pos = cursor->parent->search(key);
			}
		}
		
		cursor->item_count--;
		
		if (cursor != root && cursor->item_count < a - 1) {
			cursor = refill_vertex(cursor->parent, pos);
		}
		
// 		cursor->items[cursor->item_count - 1] = nullptr;
		size_--;
	}
	
	size_t size ()
	{
		return size_;
	}
	
	bool empty ()
	{
		return size_ == 0;
	}
	
	// DEBUG
	
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
			std::cout << "*Key: " << cursor->items[i]->key() << std::endl;
			
		}
		if (cursor->children[i] != nullptr) {
			dump(cursor->children[cursor->item_count], indent + 1);
		}
	}
	
	TKey get_root_key ()
	{
		return root->items[0]->key();
	}
};

#endif
#ifndef _ABTREE_ITERATOR_HPP_
#define _ABTREE_ITERATOR_HPP_

#include <iterator>
#include "vertex.hpp"

template <typename TKey, typename TVal>
class abtree;

template <typename TKey, typename TVal>
class abtree_iterator: public std::iterator<std::bidirectional_iterator_tag, std::pair<TKey, TVal> > {
public:
	typedef abtree_vertex<TKey, TVal> vertex;
	
	abtree_iterator ()
	{}
	
	abtree_iterator & operator++ ()
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
					position_ = vertex_->parent->search(vertex_->items[0]->key());
					vertex_ = vertex_->parent;
				}
			}
		}
		
		return *this;
	}
	
	abtree_iterator operator++ (int)
	{
		auto old = *this;
		this->operator++();
		return old;
	}
	
	abtree_iterator & operator-- ()
	{
		bool descending = false;
		if (vertex_->children[0] != nullptr) {
			while (vertex_->children[0] != nullptr) {
				if (!descending) {
					vertex_ = vertex_->children[position_];
					descending = true;
				} else {
					vertex_ = vertex_->children[vertex_->item_count];
				}
				position_ = vertex_->item_count - 1;
			}
		} else {
			while (position_ == 0) {
				if (vertex_->parent == nullptr) {
					break; // decrementing start
				} else {
					position_ = vertex_->parent->search(vertex_->items[0]->key());
					vertex_ = vertex_->parent;
				}
			}
			position_--;
		}
		
		return *this;
	}
	
	abtree_iterator operator-- (int)
	{
		auto old = *this;
		this->operator--();
		return old;
	}
	
	std::pair<TKey, TVal> & operator* ()
	{
		return vertex_->items[position_]->pair;
	}
	
	std::pair<TKey, TVal> * operator-> ()
	{
		return &vertex_->items[position_]->pair;
	}
	
	bool operator== (const abtree_iterator & it)
	{
		return (
			position_ == it.position_ &&
			vertex_ == it.vertex_
		);
	}
	
	bool operator!= (const abtree_iterator & it)
	{
		return !operator==(it);
	}
private:
	friend class abtree<TKey, TVal>;
	
	abtree_iterator (vertex * current_vertex, size_t position): vertex_(current_vertex), position_(position)
	{}
	
	vertex * vertex_;
	size_t position_;
};

#endif
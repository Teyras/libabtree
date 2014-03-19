#ifndef _ABTREE_ITERATOR_HPP_
#define _ABTREE_ITERATOR_HPP_

#include <iterator>
#include "vertex.hpp"

template <typename TKey, typename TVal>
class abtree;

/**
 * An (a, b)-tree iterator
 */
template <typename TKey, typename TVal>
class abtree_iterator: public std::iterator<std::bidirectional_iterator_tag, std::pair<TKey, TVal> > {
public:
	typedef abtree_vertex<TKey, TVal> vertex;
	
	/**
	 * Parameterless constructor (used only for variable declarations)
	 */
	abtree_iterator ()
	{}
	
	/**
	 * Move the iterator one item forward (prefix version)
	 */
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
					position_ = vertex_->parent->search(vertex_->items[0]->first);
					vertex_ = vertex_->parent;
				}
			}
		}
		
		return *this;
	}
	
	/**
	 * Move the iterator one item forward (postfix version)
	 */
	abtree_iterator operator++ (int)
	{
		auto old = *this;
		this->operator++();
		return old;
	}
	
	/**
	 * Move the iterator one item backward (prefix version)
	 */
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
					position_ = vertex_->parent->search(vertex_->items[0]->first);
					vertex_ = vertex_->parent;
				}
			}
			position_--;
		}
		
		return *this;
	}
	
	/**
	 * Move the iterator one item backward (postfix version)
	 */
	abtree_iterator operator-- (int)
	{
		auto old = *this;
		this->operator--();
		return old;
	}
	
	std::pair<const TKey, TVal> & operator* ()
	{
		return *(vertex_->items[position_]);
	}
	
	std::pair<const TKey, TVal> * operator-> ()
	{
		return vertex_->items[position_];
	}
	
	/**
	 * Two iterators are considered equal when they point to the same vertex and position
	 */
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
	
	operator abtree_iterator<TKey, TVal const> () const;
private:
	friend class abtree<TKey, TVal>;
	
	/**
	 * Construct an iterator pointing to given position in given vertex
	 * (Only the abtree container can construct an iterator this way)
	 * @param current_vertex the vertex the iterator should point to
	 * @param position the position of the item the iterator should point to
	 */
	abtree_iterator (vertex * current_vertex, size_t position): vertex_(current_vertex), position_(position)
	{}
	
	vertex * vertex_;
	size_t position_;
};

#endif
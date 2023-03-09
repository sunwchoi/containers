#ifndef ITERATOR_HPP
# define ITERATOR_HPP

#include "iterator_trait.hpp"

namespace ft
{
template <class node>
class const_tree_iterator
{
public:
	typedef typename node::value_type value_type;
	typedef const value_type* pointer; 
	typedef const value_type& reference;
	typedef ptrdiff_t difference_type;
	typedef bidirectional_iterator_tag	iterator_category;

protected:
	typedef node* node_pointer;
	node_pointer	_p;

public:
	//OCCF
	const_tree_iterator() : _p(NULL) {}
	const_tree_iterator(const const_tree_iterator& it) : _p(it._p) {}
	const_tree_iterator& operator=(const const_tree_iterator& it) { _p = it._p; return *this;}
	~const_tree_iterator() {}
	const_tree_iterator(const node_pointer& p) : _p(p) {}

	//dereference
	reference	operator*() const { return _p->val; }
	pointer		operator->() const { return &(operator*()); }
	node_pointer	base() {return this->_p; }
	//incre, decrement
	const_tree_iterator&	operator++()
	{
		if (_p->right)
		{
			_p = _p->right;
			while (_p->left) 
				_p = _p->left;
		}
		else
		{
			while (_p->parent)
			{
				if (_p == _p->parent->left)
				{
					_p = _p->parent;
					break ;
				}
				_p = _p->parent;
			}
		}
		return *this;
	}
	const_tree_iterator&	operator--()
	{
		if (_p->left)
		{
			_p = _p->left;
			while (_p->right)
				_p = _p->right;
		}
		else
		{
			while (_p->parent)
			{
				if (_p == _p->parent->right)
				{
					_p = _p->parent;
					break;
				}
				_p = _p->parent;
			}
		}
		return *this;
	}
	const_tree_iterator	operator++(int) { const_tree_iterator tmp = *this; ++(*this); return tmp; }
	const_tree_iterator	operator--(int) { const_tree_iterator tmp = *this; --(*this); return tmp; }

	friend bool operator==(const const_tree_iterator<node> &lhs, const const_tree_iterator<node> &rhs) { return lhs._p == rhs._p; }
	friend bool operator!=(const const_tree_iterator<node> &lhs, const const_tree_iterator<node> &rhs) { return lhs._p != rhs._p; }
};

template <class node>
class tree_iterator :public const_tree_iterator<node>
{
public:
	typedef typename node::value_type value_type;
	typedef value_type* pointer; 
	typedef value_type& reference;
	typedef ptrdiff_t difference_type;
	typedef bidirectional_iterator_tag	iterator_category;

public:
	//OCCF
	tree_iterator() { this->_p = NULL; }
	tree_iterator(const tree_iterator& it) { this->_p = it._p; }
	tree_iterator& operator=(const tree_iterator& it) { this->_p = it._p; return *this;}
	~tree_iterator() {}
	tree_iterator(const typename const_tree_iterator<node>::node_pointer& p) { this->_p = p; }

	//dereference
	reference	operator*() const { return this->_p->val; }
	pointer		operator->() const { return &(operator*()); };
	//node_pointer	base() {return this->_p; }
	//incre, decrement
	tree_iterator&	operator++()
	{
		if (this->_p->right)
		{
			this->_p = this->_p->right;
			while (this->_p->left) 
				this->_p = this->_p->left;
		}
		else
		{
			while (this->_p->parent)
			{
				if (this->_p == this->_p->parent->left)
				{
					this->_p = this->_p->parent;
					break ;
				}
				this->_p = this->_p->parent;
			}
		}
		return *this;
	}
	tree_iterator&	operator--()
	{
		if (this->_p->left)
		{
			this->_p = this->_p->left;
			while (this->_p->right)
				this->_p = this->_p->right;
		}
		else
		{
			while (this->_p->parent)
			{
				if (this->_p == this->_p->parent->right)
				{
					this->_p = this->_p->parent;
					break ;
				}
				this->_p = this->_p->parent;
			}
		}
		return *this;
	}
	tree_iterator	operator++(int) { tree_iterator tmp = *this; ++(*this); return tmp; }
	tree_iterator	operator--(int) { tree_iterator tmp = *this; --(*this); return tmp; }

	friend bool operator==(const tree_iterator<node> &lhs, const tree_iterator<node> &rhs) { return lhs._p == rhs._p; }
	friend bool operator!=(const tree_iterator<node> &lhs, const tree_iterator<node> &rhs) { return lhs._p != rhs._p; }
};

}
#endif

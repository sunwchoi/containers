#ifndef MAP_HPP
# define MAP_HPP

#include "pair.hpp"
#include "tree.hpp"
#include "tree_iterator.hpp"
#include <memory>

namespace ft 
{

template <class Key, class T, class Compare = less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
class map
{
public:
    typedef Key											key_type;
    typedef T											mapped_type;
    typedef pair<const key_type, mapped_type>			value_type;
    typedef Compare										key_compare;
    typedef Alloc										allocator_type;
    typedef typename allocator_type::reference			reference;
    typedef typename allocator_type::const_reference	const_reference;
    typedef typename allocator_type::pointer			pointer;
    typedef typename allocator_type::const_pointer		const_pointer;
    typedef typename allocator_type::size_type			size_type;
    typedef typename allocator_type::difference_type	difference_type;

public:
class value_compare
{
	friend class map;
protected:
	Compare comp;
	value_compare (Compare c) : comp(c) {}
public:
	typedef bool result_type;
	typedef value_type first_argument_type;
	typedef value_type second_argument_type;
	bool operator() (const value_type& x, const value_type& y) const
	{
		return comp(x.first, y.first);
	}
};

protected:
	typedef	tree<value_type, value_compare, allocator_type>	tree_type;
	typedef	typename tree_type::iterator		tree_iterator;
	typedef	typename tree_type::const_iterator	const_tree_iterator;

public:
	typedef tree_iterator				iterator;
    typedef const_tree_iterator			const_iterator;
    typedef ft::reverse_iterator<iterator>			reverse_iterator;
    typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

protected:
	tree_type	_tree;

public:
	explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _tree(value_compare(comp), alloc)
	{}

	template <class InputIterator>
	map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _tree(first, last, value_compare(comp), alloc)
	{}
	
	map (const map& x)
	: _tree(x._tree)
	{}

	map& operator= (const map& x) { _tree = x._tree;  return *this; }

	~map() {};

	iterator begin() { return _tree.begin(); };
	const_iterator begin() const { return _tree.begin(); }

	void clear() { _tree.clear(); }

	size_type count (const key_type& k) const { return _tree.count(ft::make_pair(k, mapped_type())); }

	bool empty() const { return _tree.empty(); }

	iterator end() { return _tree.end();}
	const_iterator end() const { return _tree.end(); }

	pair<const_iterator,const_iterator> equal_range (const key_type& k) const { return _tree.equal_range(ft::make_pair(k, mapped_type())); }
	pair<iterator,iterator> equal_range (const key_type& k) { return _tree.equal_range(ft::make_pair(k, mapped_type())); }

	void erase (iterator position) { _tree.erase(position); }
	size_type erase (const key_type& k) { return _tree.erase(ft::make_pair(k, mapped_type())); }
	void erase (iterator first, iterator last)
	{
		map mp(first, last);
		for (iterator it = mp.begin(); it != mp.end(); it++)
			erase(it->first);
	}

	iterator find (const key_type& k) { return _tree.find(ft::make_pair(k, mapped_type())); }
	const_iterator find (const key_type& k) const { return _tree.find(ft::make_pair(k, mapped_type())); }

	allocator_type get_allocator() const { return _tree.get_allocator(); }

	pair<iterator,bool> insert (const value_type& val) { return _tree.insert(val); }
	iterator insert (iterator position, const value_type& val) { return _tree.insert(position, val); }
	template <class InputIterator>  void insert (InputIterator first, InputIterator last) { return _tree.insert(first, last); }

	key_compare key_comp() const { return key_compare(); }

	iterator lower_bound (const key_type& k) { return _tree.lower_bound(ft::make_pair(k, mapped_type())); }
	const_iterator lower_bound (const key_type& k) const { return _tree.lower_bound(ft::make_pair(k, mapped_type())); };

	size_type max_size() const { return _tree.max_size(); }

	mapped_type& operator[] (const key_type& k) { return (_tree[ft::make_pair(k, mapped_type())]).second; }

	reverse_iterator rbegin() { return _tree.rbegin(); }
	const_reverse_iterator rbegin() const { return _tree.rbegin();}

	reverse_iterator rend() { return _tree.rend(); }
	const_reverse_iterator rend() const { return _tree.rend(); }

	size_type size() const { return _tree.size(); }
	
	void swap (map& x) { return _tree.swap(x._tree); }

	iterator upper_bound (const key_type& k) { return _tree.upper_bound(ft::make_pair(k, mapped_type())); }
	const_iterator upper_bound (const key_type& k) const { return _tree.upper_bound(ft::make_pair(k, mapped_type())); }

	friend bool operator== ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs )
	{ return	(lhs._tree == rhs._tree); }

	friend bool operator!= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs )
	{ return lhs._tree != rhs._tree; }

	friend bool operator<  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs )
	{ return lhs._tree < rhs._tree; }

	friend bool operator>  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs )
	{ return lhs._tree > rhs._tree; }

	friend bool operator<= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs )
	{ return lhs._tree <= rhs._tree; }

	friend bool operator>= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs )
	{ return lhs._tree >= rhs._tree; }

	void swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y)
	{ x.swap(y); }

	value_compare value_comp() const { return _tree.key_comp(); }

	//
	//void	print() { _tree.print(); }
	//
};


}

#endif

#ifndef SET_HPP
# define SET_HPP

#include "compare.hpp"
#include "pair.hpp"
#include "tree.hpp"
#include <memory>

namespace ft
{

template <class Key, class Compare = less<Key>, class Alloc = std::allocator<Key> >
class set
{
public:
    // types:
    typedef Key										key_type;
    typedef key_type								value_type;
    typedef Compare									key_compare;
    typedef key_compare								value_compare;
    typedef Alloc									allocator_type;
    typedef typename allocator_type::reference			reference;
    typedef typename allocator_type::const_reference	const_reference;
    typedef typename allocator_type::size_type			size_type;
    typedef typename allocator_type::difference_type	difference_type;
    typedef typename allocator_type::pointer			pointer;
    typedef typename allocator_type::const_pointer		const_pointer;

protected:
	protected:
	typedef	tree<const key_type, Compare, Alloc>		tree_type;
	typedef	typename tree_type::iterator		tree_iterator;
	typedef	typename tree_type::const_iterator	const_tree_iterator;

public:
    typedef const_tree_iterator		iterator;
    typedef const_tree_iterator	const_iterator;
    typedef ft::reverse_iterator<iterator>          reverse_iterator;
    typedef ft::reverse_iterator<const_iterator>    const_reverse_iterator;

protected:
	tree_type	_tree;

public:
	explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _tree(comp, alloc)
	{}

	template <class InputIterator>
	set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _tree(first, last, comp, alloc)
	{
	}
	
	set (const set& x)
	: _tree(x._tree)
	{}

	set& operator= (const set& x) { _tree = x._tree;  return *this; }

	~set() {};

	iterator begin() { return _tree.begin(); };
	const_iterator begin() const { return _tree.begin(); }

	void clear() { _tree.clear(); }

	size_type count (const key_type& k) const { return _tree.count(k); }

	bool empty() const { return _tree.empty(); }

	iterator end() { return _tree.end();}
	const_iterator end() const { return _tree.end(); }

	pair<const_iterator,const_iterator> equal_range (const key_type& k) const { return _tree.equal_range(k); }
	pair<iterator,iterator> equal_range (const key_type& k) { return _tree.equal_range(k); }

	void erase (iterator position) { _tree.erase(*position); }
	size_type erase (const key_type& k) { return _tree.erase(k); }
	void erase (iterator first, iterator last)
	{
		set mp(first, last);
		for (iterator it = mp.begin(); it != mp.end(); it++)
			erase(*it);
	}

	iterator find (const key_type& k) { return _tree.find(k); }
	const_iterator find (const key_type& k) const { return _tree.find(k); }

	allocator_type get_allocator() const { return _tree.get_allocator(); }

	pair<iterator,bool> insert (const value_type& val) { return _tree.insert(val); }
	iterator insert (iterator position, const value_type& val) { position = NULL; return insert(val).first; }
	template <class InputIterator>  void insert (InputIterator first, InputIterator last)
	{ for (; first != last; first++) insert(*first); }

	key_compare key_comp() const { return _tree.key_comp(); }

	iterator lower_bound (const key_type& k) { return _tree.lower_bound(k); }
	const_iterator lower_bound (const key_type& k) const { return _tree.lower_bound(k); };

	size_type max_size() const { return _tree.max_size(); }

	reverse_iterator rbegin() { return _tree.rbegin(); }
	const_reverse_iterator rbegin() const { return _tree.rbegin();}

	reverse_iterator rend() { return _tree.rend(); }
	const_reverse_iterator rend() const { return _tree.rend(); }

	size_type size() const { return _tree.size(); }
	
	void swap (set& x) { return _tree.swap(x._tree); }

	iterator upper_bound (const key_type& k) { return _tree.upper_bound(k); }
	const_iterator upper_bound (const key_type& k) const { return _tree.upper_bound(k); }

	friend bool operator== ( const set<Key,Compare,Alloc>& lhs, const set<Key,Compare,Alloc>& rhs )
	{ return	(lhs._tree == rhs._tree); }

	friend bool operator!= ( const set<Key,Compare,Alloc>& lhs, const set<Key,Compare,Alloc>& rhs )
	{ return lhs._tree != rhs._tree; }

	friend bool operator<  ( const set<Key,Compare,Alloc>& lhs, const set<Key,Compare,Alloc>& rhs )
	{ return lhs._tree < rhs._tree; }

	friend bool operator>  ( const set<Key,Compare,Alloc>& lhs, const set<Key,Compare,Alloc>& rhs )
	{ return lhs._tree > rhs._tree; }

	friend bool operator<= ( const set<Key,Compare,Alloc>& lhs, const set<Key,Compare,Alloc>& rhs )
	{ return lhs._tree <= rhs._tree; }

	friend bool operator>= ( const set<Key,Compare,Alloc>& lhs, const set<Key,Compare,Alloc>& rhs )
	{ return lhs._tree >= rhs._tree; }

	void swap (set<Key,Compare,Alloc>& x, set<Key,Compare,Alloc>& y)
	{ x.swap(y); }

	value_compare value_comp() const { return value_compare(Compare()); }

};

}
#endif

#ifndef TREE_HPP
# define TREE_HPP

#include "compare.hpp"
#include "pair.hpp"
#include "SFINAE.hpp"
#include "tree_iterator.hpp"
#include "reverse_iterator.hpp"
#include <memory>

//#include <iostream>

namespace ft
{


template <class T, class Compare, class Alloc>
class tree
{
public:
    typedef T											value_type;
    typedef Compare										value_compare;
    typedef Alloc										allocator_type;
    typedef typename allocator_type::reference			reference;
    typedef typename allocator_type::const_reference	const_reference;
    typedef typename allocator_type::pointer			pointer;
    typedef typename allocator_type::const_pointer		const_pointer;
    typedef typename allocator_type::size_type			size_type;
    typedef typename allocator_type::difference_type	difference_type;

protected:
struct tree_node
{
	typedef value_type value_type;
	value_type	val;
	char		color;
	tree_node*	parent;
	tree_node*	left;
	tree_node*	right;

	tree_node(const value_type& val = value_type(), const char& color = 'R') : val(val), color(color), parent(NULL), left(NULL), right(NULL) {};
	tree_node(const tree_node&	nde) : val(nde.val), color(nde.color), parent(nde.parent), left(nde.left), right(nde.right) {};
	tree_node& operator= (const tree_node& nde) { val = nde.val; color = nde.color; parent = nde.parent; left = nde.left; right = nde.right; }
	~tree_node() {};
};

	typedef	tree_node		node;
	typedef	node*			node_pointer;
	typedef	const node*		const_node_pointer;

public:
	typedef ft::tree_iterator<node> iterator;
	typedef ft::const_tree_iterator<node> const_iterator;
	typedef ft::reverse_iterator<iterator>          reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>    const_reverse_iterator;

protected:
	typename allocator_type::template rebind<node>::other	_alloc;
	value_compare		_comp;
	node_pointer		_root;
	node_pointer		_end;

protected:	
	node_pointer	makeNode(const value_type& val, const char& color = 'R')
	{
		node_pointer	new_node = _alloc.allocate(1);
		_alloc.construct(new_node, val, color);
		return new_node;
	}

	bool	isRight(node_pointer me)
	{
		if (me->parent->right == me)
			return true;
		return false;
	}

	bool	isLeft(node_pointer me)
	{
		if (me->parent->right == me)
			return false;
		return true;
	}

	bool	isBlack(node_pointer me)
	{
		if (me == NULL || me->color == 'B')
			return true;
		return false;
	}
	
	bool	isRed(node_pointer me)
	{
		if (me == NULL || me->color == 'B')
			return false;
		return true;
	}

	node_pointer	getGrandpa(node_pointer me)
	{
		if (me->parent && (me->parent->parent && me->parent->parent != _end))
			return me->parent->parent;
		else
			return NULL;
	}

	node_pointer	getUncle(node_pointer me)
	{
		node_pointer	grandpa = getGrandpa(me);

		if (grandpa == NULL)
			return NULL;
		if (me->parent == grandpa->left)
			return grandpa->right;
		else
			return grandpa->left;
	}

	node_pointer	getSibling(node_pointer me)
	{
		node_pointer	parent = me->parent;
		if (parent == _end)
			return NULL;
		if (parent->left == me)
			return parent->right;
		else
			return parent->left;
	}

	node_pointer	rotateLeft(node_pointer me)
	{
		node_pointer	grandpa = getGrandpa(me);
		node_pointer	parent = me->parent;
		node_pointer	left = me->left;

		connectParentAndLeft(me, parent);
		connectParentAndRight(parent, left);

		if (grandpa && grandpa->right == parent)
			connectParentAndRight(grandpa, me);
		else if (grandpa && grandpa->left == parent)
			connectParentAndLeft(grandpa, me);
		else if (parent == _root)
		{
			_root = me;
			_root->color = 'B';
			connectEndAndRoot();
		}
		return me;
	}

	node_pointer	rotateRight(node_pointer me)
	{
		node_pointer	grandpa = getGrandpa(me);
		node_pointer	parent = me->parent;
		node_pointer	right = me->right;

		connectParentAndRight(me, parent);
		connectParentAndLeft(parent, right);

		if (grandpa && grandpa->right == parent)
			connectParentAndRight(grandpa, me);
		else if (grandpa && grandpa->left == parent)
			connectParentAndLeft(grandpa, me);
		else if (parent == _root)
		{
			_root = me;
			_root->color = 'B';
			connectEndAndRoot();
		}
		return me;
	}

	node_pointer	searchNode(const value_type& val) const
	{
		if (_root == NULL)
			return NULL;

		node_pointer tmp = _root;
		while (tmp->left || tmp->right)
		{
			if (_comp(val, tmp->val))
			{
				if (!tmp->left)
					break ;
				tmp = tmp->left;
			}
			else if (_comp(tmp->val, val))
			{
				if (!tmp->right)
					break;
				tmp = tmp->right;
			}
			else
				return tmp;
		}
		return tmp;
	}

	void	balanceTreeAfterInsert(node_pointer me)
	{
		node_pointer	parent = me->parent;
		node_pointer	grandpa = getGrandpa(me);
		node_pointer	uncle = getUncle(me);

		if (me == _root)
			me->color = 'B';
		else if (isBlack(parent))
			;
		else if (isRed(parent) && (uncle && isRed(uncle)))
		{
			parent->color = 'B';
			uncle->color = 'B';
			grandpa->color = 'R';
			if (grandpa == _root)
				grandpa->color ='B';
			else
				balanceTreeAfterInsert(grandpa);
		}
		else
		{
			if (isRight(me) && isLeft(parent))
				rotateLeft(me);
			else if (isLeft(me) && isRight(parent))
				rotateRight(me);
			else
				me = me->parent;

			if (isLeft(me))
			{
				rotateRight(me);
				me->right->color = 'R';
				me->color = 'B';
			}
			else
			{
				rotateLeft(me);
				me->left->color = 'R';
				me->color = 'B';
			}
		}
	}

	node_pointer	insertNode(node_pointer new_node)
	{
		if (_root == NULL)
		{
			_root = new_node;
			_root->color = 'B';
			connectEndAndRoot();
			return new_node;
		}

		node_pointer tmp = searchNode(new_node->val);
		if (_comp(new_node->val, tmp->val))
			connectParentAndLeft(tmp, new_node);
		else if (_comp(tmp->val, new_node->val))
			connectParentAndRight(tmp, new_node);
		else
		{
			_alloc.deallocate(new_node, 1);
			return tmp;
		}
		balanceTreeAfterInsert(new_node);
		return new_node;
	}

	node_pointer	replaceNode(node_pointer target, node_pointer replace, bool child_copy = false)
	{
		if (target == _root)
		{
			_root = replace;
			connectEndAndRoot();
		}
		else if (target->parent->right == target)
			connectParentAndRight(target->parent, replace);
		else
			connectParentAndLeft(target->parent, replace);

		if (child_copy == true)
		{
			connectParentAndLeft(replace, target->left);
			connectParentAndRight(replace, target->right);
		}
		return replace;
	}

	void	balanceTreeAfterDelete(node_pointer me, char extra_color)
	{
		if (me == NULL)
			return ;
		else if (extra_color == 'R')
			return ;
		else if (extra_color == 'B' && me->color == 'R')
			me->color = 'B';
		else
			balanceDoubleBlack(me);
	}

	void	makeBlackSibling(node_pointer me)
	{
		node_pointer sibling = getSibling(me);

		if (isBlack(sibling))
			return ;
		if (isLeft(sibling))
		{
			rotateRight(sibling);
			sibling->color = 'R';
			sibling->right->color = 'B';
		}
		else
		{
			rotateLeft(sibling);
			sibling->color ='R';
			sibling->left->color = 'B';
		}
	}

	bool	colorSiblingRed(node_pointer me)
	{
		node_pointer	sibling = getSibling(me);

		if (isRed(me->parent) || isRed(sibling) || isRed(sibling->left) || isRed(sibling->right))
			return false;
		sibling->color = 'R';
		return true;
	}

	bool	onlyParentIsRed(node_pointer me)
	{
		node_pointer sibling = getSibling(me);

		if (!(isRed(me->parent) && isBlack(sibling) && isBlack(sibling->left) && isBlack(sibling->right)))
			return false;
		
		sibling->color = 'R';
		me->parent->color = 'B';
		return true;
	}

	void	makeRedSururbNephew(node_pointer me)
	{
		node_pointer	sibling = getSibling(me);

		if (isRight(sibling) && isBlack(sibling->right))
		{
			rotateRight(sibling->left);
			sibling = getSibling(me);
			sibling->color = 'B';
			sibling->right->color = 'R';
		}
		else if (isLeft(sibling) && isBlack(sibling->left))
		{
			rotateLeft(sibling->right);
			sibling = getSibling(me);
			sibling->color = 'B';
			sibling->right->color = 'R';
		}
	}

	void	NephewIsRed(node_pointer me)
	{
		node_pointer	sibling = getSibling(me);
		node_pointer	parent = me->parent;

		if (isLeft(me))
		{
			rotateLeft(sibling);
			char tmp = parent->color;
			parent->color = sibling->color;
			sibling->color = tmp;
			sibling->right->color = 'B';
		}
		else
		{
			rotateLeft(sibling);
			char tmp = parent->color;
			sibling->color = tmp;
			sibling->left->color = 'B';
		}
	}

	void	balanceDoubleBlack(node_pointer me)
	{
		if (me == _root)
			return ;
		makeBlackSibling(me);
		if (colorSiblingRed(me))
			return balanceDoubleBlack(me->parent);
		if (onlyParentIsRed(me))
			return ;
		makeRedSururbNephew(me);
		NephewIsRed(me);
	}

	void	deleteNode(node_pointer target)
	{
		node_pointer	replace;
		if (!target)
			return ;
		if (!target->left && !target->right)
			replace = replaceNode(target, NULL);
		else if (!target->left)
			replace = replaceNode(target, target->right);
		else if (!target->right)
			replace = replaceNode(target, target->left);
		else
		{
			node_pointer successor = (++iterator(target)).base();
			replace = makeNode(successor->val, target->color);

			replaceNode(target, replace, true);
			_alloc.destroy(target);
			_alloc.deallocate(target, 1);
			return deleteNode(successor);
			
		}
		balanceTreeAfterDelete(replace,target->color);
		_alloc.destroy(target);
		_alloc.deallocate(target, 1);
	}


	void	deleteAllNode(node_pointer *target)
	{
		if (!*target)
			return ;

		if ((*target)->left)
			deleteAllNode(&(*target)->left);
		if ((*target)->right)
			deleteAllNode(&(*target)->right);
		_alloc.deallocate((*target), 1);
		*target = NULL;
	}

	node_pointer	copyAllNode(node *target)
	{
		if (!target)
			return NULL;

		node_pointer	ret = makeNode(target->val);

		if (target->left)
			connectParentAndLeft(ret, copyAllNode(target->left));
		if (target->right)
			connectParentAndRight(ret, copyAllNode(target->right));
		return ret;
	}

	size_type	checkSize(node_pointer target) const
	{
		size_type size = 1;

		if (!target->left && !target->right)
			return size;
		if (target->left)
			size += checkSize(target->left);
		if (target->right)
			size += checkSize(target->right);

		return size;
	}
;
	void	connectParentAndRight(node_pointer Parent, node_pointer Right)
	{
		Parent->right = Right;
		if (Right)
			Right->parent = Parent;
	}
	void	connectParentAndLeft(node_pointer Parent, node_pointer Left)
	{
		Parent->left = Left;
		if (Left)
			Left->parent = Parent;
	}
	void	connectEndAndRoot()
	{
		connectParentAndLeft(_end, _root);
		connectParentAndRight(_end, _root);
	}



public:
	//OCCF
	tree(const value_compare& comp = value_compare(), const allocator_type& alloc = allocator_type())
	: _alloc(alloc), _comp(comp), _root(NULL), _end(makeNode(value_type()))
	{
		connectEndAndRoot();
	}
	tree(const tree& x)
	: _alloc(x._alloc), _comp(x._comp), _root(NULL), _end(makeNode(value_type()))
	{
		_root = copyAllNode(x._root);
		connectEndAndRoot();
	}
	template<class InputIterator>
	tree(InputIterator first, InputIterator last, const value_compare& comp = value_compare(), const allocator_type& alloc = allocator_type())
	: _alloc(alloc), _comp(comp), _root(NULL), _end(makeNode(value_type()))
	{ 
		connectEndAndRoot();
		for (; first != last; first++) insertNode(makeNode(*first));
	}

	tree&	operator=(const tree& x)
	{
		deleteAllNode(&_root);
		_root = copyAllNode(x._root);
		connectEndAndRoot();
		return *this;
	}
	~tree()
	{
		deleteAllNode(&_root);
		_alloc.deallocate(_end, 1);
	}
	iterator begin() { iterator it(_end); ++it; return it; }
	const_iterator begin() const { const_iterator it(_end); ++it; return it; }

	void clear()
	{ 
		deleteAllNode(&_root);
		connectEndAndRoot();
	}

	size_type count(const value_type& val) const
	{
		node_pointer node = searchNode(val);
		if (_comp(node->val, val) || _comp(val, node->val))
			return 0;
		return 1;
	}

	bool empty() const { return (_root == NULL); }

	iterator end() { return (_end);}
	const_iterator end() const { return const_iterator(_end); }
	
	iterator lower_bound (const value_type& val)
	{
		node_pointer	position = searchNode(val);

		if (!_comp(position->val, val))
			return iterator(position);
		return ++iterator(position);
	}
	const_iterator lower_bound (const value_type& val) const
	{
		node_pointer	position = searchNode(val);

		if (!_comp(position->val, val))
			return const_iterator(position);
		return ++const_iterator(position);
	}

	iterator upper_bound (const value_type& val)
	{
		node_pointer	position = searchNode(val);

		if (_comp(val, position->val))
			return iterator(position);
		return ++iterator(position);
	}
	const_iterator upper_bound (const value_type& val) const
	{
		node_pointer position = searchNode(val);

		if (_comp(val, position->val))
			return const_iterator(position);
		return ++const_iterator(position);
	}

	pair<const_iterator,const_iterator> equal_range (const value_type& val) const
	{ return make_pair(lower_bound(val), upper_bound(val)); }
	pair<iterator,iterator>equal_range (const value_type& val)
	{ return make_pair(lower_bound(val), upper_bound(val)); }

	size_type erase (const value_type& val)
	{
		node_pointer	target = searchNode(val);
		if (_comp(val, target->val) || _comp(target->val, val))
			return 0;
		deleteNode(target);
		return 1;
	}
	void erase (iterator position) 
	{ erase(*position); }

	iterator find (const value_type& val)
	{
		node_pointer position = searchNode(val);

		if (_comp(position->val, val) || _comp(val, position->val))
			return (iterator(_end));
		return iterator(position);
	}
	const_iterator find (const value_type& val) const
	{
		node_pointer position = searchNode(val);

		if (_comp(position->val, val) || _comp(val, position->val))
			return (const_iterator(_end));
		return const_iterator(position);
	}

	allocator_type get_allocator() const { return _alloc; }

	pair<iterator,bool> insert (const value_type& val)
	{
		node_pointer	inserted_node = makeNode(val);
		node_pointer	inserted_position = insertNode(inserted_node);

		if (inserted_position != inserted_node)
			return pair<iterator, bool>(inserted_position, false);
		return pair<iterator, bool>(inserted_position, true);
	}

	iterator insert (iterator position, const value_type& val)
	{ position = NULL; return this->insert(val).first; }

	template <class InputIterator>  void insert (InputIterator first, InputIterator last)
	{ for (; first != last; first++) insert(*first); }

	value_compare key_comp() const { return _comp;}
	
	size_type max_size() const { return (size_type(-1) / sizeof(node));}

	value_type& operator[] (const value_type& val)
	{
		iterator it = insert(val).first;
		return *it;
	}
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(end()); }

	size_type size() const
	{
		if (!_root)
			return 0;
		return checkSize(_root);
	}

	void swap (tree& x)
	{
		node_pointer	tmp = x._root;
		
		x._root = _root;
		x._end = _end;
		
		_root = tmp;
		_end = tmp->parent;
	}

	friend bool operator== ( const tree<T,Compare,Alloc>& lhs, const tree<T,Compare,Alloc>& rhs )
	{ return	(lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin())); }

	friend bool operator!= ( const tree<T,Compare,Alloc>& lhs, const tree<T,Compare,Alloc>& rhs )
	{ return !(lhs == rhs); }

	friend bool operator<  ( const tree<T,Compare,Alloc>& lhs, const tree<T,Compare,Alloc>& rhs )
	{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

	friend bool operator>  ( const tree<T,Compare,Alloc>& lhs, const tree<T,Compare,Alloc>& rhs )
	{ return (rhs < lhs); }

	friend bool operator<= ( const tree<T,Compare,Alloc>& lhs, const tree<T,Compare,Alloc>& rhs )
	{ return !(lhs > rhs); }

	friend bool operator>= ( const tree<T,Compare,Alloc>& lhs, const tree<T,Compare,Alloc>& rhs )
	{ return !(lhs < rhs); }

	void swap (tree<T,Compare,Alloc>& x, tree<T,Compare,Alloc>& y)
	{ x.swap(y); }


/*
	void	printNode(node_pointer me, std::string str)
	{
		std::cout << str;
		if (me == _root)
			std::cout << "	";
		else if (isLeft(me))
			std::cout << "'----";
		else 
			std::cout << ",----";
		std::cout << me->color << "(" << me->val.first << ")" << std::endl;
	}

	void	printAllNode(node_pointer me, std::string str)
	{
		if (!me)
			return;

		if (me->right)
		{
			if (me != _root && isLeft(me))
				printAllNode(me->right, str + "|	" );
			else
				printAllNode(me->right, str + "	" );
		}
		printNode(me, str);
		if (me->left)
		{
			if (me != _root && isRight(me))
				printAllNode(me->left, str + "|	" );
			else
				printAllNode(me->left, str + "	" );
		}
	}

	void	print() { printAllNode(_root, "");}
*/



};

}
#endif


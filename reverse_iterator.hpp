#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

#include "iterator_trait.hpp"

namespace ft 
{
template <class Iterator>
class reverse_iterator
{
public:
	typedef Iterator iterator;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;

protected:
	iterator	_it;

public:
	//OCCF
	reverse_iterator() : _it() {}
	reverse_iterator(const iterator& it) : _it(it) {}
	template <class Iter>
	reverse_iterator (const reverse_iterator<Iter>& rev_it) : _it(rev_it.base()) {}
	template <class Iter>
	reverse_iterator& operator= (const reverse_iterator<Iter>& rev_it) { _it  = rev_it.base(); return *this; }
	~reverse_iterator() {};
	
	//dereference
	iterator	base() const { return _it; };
	reference	operator*() const { iterator it = _it; return *(--it); };
	pointer		operator->() const { return &(operator*()); }

	//incre, decrement
	reverse_iterator&	operator++() { --_it; return *this; };
	reverse_iterator&	operator--() { ++_it; return *this; };
	reverse_iterator	operator++(int) { reverse_iterator tmp = *this; ++(*this); return tmp; };
	reverse_iterator	operator--(int) { reverse_iterator tmp = *this; --(*this); return tmp; };

	//arithmatic
	reverse_iterator operator+(difference_type n) const { reverse_iterator tmp = *this; while (n--) ++tmp; return tmp; }
	reverse_iterator operator-(difference_type n) const { reverse_iterator tmp = *this; while (n--) --tmp; return tmp; }
	reverse_iterator& operator+= (difference_type n) { *this = *this + n; return *this; };
	reverse_iterator& operator-= (difference_type n) { *this = *this - n; return *this; };
	
	reference	operator[](difference_type n) const { return *(*this + n); };

	friend reverse_iterator<Iterator> operator+ (typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& rev_it) { return (rev_it + n);}
	template<class Iterator1, class Iterator2>friend typename reverse_iterator<Iterator>::difference_type operator- (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return (rhs._it - lhs._it); };
	template<class Iterator1, class Iterator2>friend bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{ return (lhs._it == rhs._it);};
	template<class Iterator1, class Iterator2>friend bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{ return (lhs._it != rhs._it);};
	template<class Iterator1, class Iterator2>friend bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{ return (lhs._it > rhs._it);};
	template<class Iterator1, class Iterator2>friend bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{ return (lhs._it < rhs._it);};
	template<class Iterator1, class Iterator2>friend bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{ return !(lhs < rhs);};
	template<class Iterator1, class Iterator2>friend bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{ return !(lhs > rhs);};


};

}
#endif

#ifndef ITERATOR_TRAIT_HPP
# define ITERATOR_TRAIT_HPP

#include <cstddef>

namespace ft
{

struct	input_iterator_tag {};
struct	forward_iterator_tag  : public input_iterator_tag{};
struct	bidirectional_iterator_tag : public forward_iterator_tag{};
struct	random_access_iterator_tag : public bidirectional_iterator_tag{};

template <class Iterator>
class iterator_traits
{
public:
	typedef typename Iterator::value_type			value_type;
    typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::pointer				pointer;
    typedef typename Iterator::reference			reference;
    typedef typename Iterator::iterator_category	iterator_category;
};

template <class T> class iterator_traits<T*>
{
public:
	typedef T			value_type;
	typedef ptrdiff_t	difference_type;
	typedef T*			pointer;
	typedef T&			reference;
	typedef random_access_iterator_tag	iterator_category;
};

template <class T> class iterator_traits<const T*>
{
public:
	typedef T		value_type;
	typedef ptrdiff_t	difference_type;
	typedef const T*	pointer;
	typedef const T&	reference;
	typedef random_access_iterator_tag	iterator_category;
};



template<class iter>
class wrap_iter
{
public:
	typedef typename iterator_traits<iter>::value_type			value_type;
	typedef typename iterator_traits<iter>::difference_type		difference_type;
	typedef typename iterator_traits<iter>::pointer				pointer;
	typedef typename iterator_traits<iter>::reference			reference;
	typedef typename iterator_traits<iter>::iterator_category	iterator_category;
protected:
	iter	_i;

public:
	wrap_iter() : _i(NULL) {};
	wrap_iter(const wrap_iter& w_it) : _i(w_it._i) {};
	wrap_iter& operator=(const wrap_iter& w_it) { _i = w_it._i; };
	~wrap_iter() {};
	wrap_iter(const iter& i) : _i(i) {};

	iter	base() const { return _i; }
	reference	operator*() const { return *_i; }
	pointer		operator->() const { return &(operator*()); }

	wrap_iter&	operator++() { ++_i; return *this; }
	wrap_iter&	operator--() { --_i; return *this; }
	wrap_iter	operator++(int) { wrap_iter tmp = *this; ++(*this); return tmp; }
	wrap_iter	operator--(int) { wrap_iter tmp = *this; --(*this); return tmp; }

	wrap_iter operator+(difference_type n) const { wrap_iter tmp = *this; while (n--) ++tmp; return tmp; }
	wrap_iter operator-(difference_type n) const { wrap_iter tmp = *this; while (n--) --tmp; return tmp; }
	wrap_iter& operator+= (difference_type n) { *this = *this + n; return *this; };
	wrap_iter& operator-= (difference_type n) { *this = *this - n; return *this; };

	friend wrap_iter<iter> operator+ (typename wrap_iter<iter>::difference_type n, const wrap_iter<iter>& w_it) { return (w_it + n); }
	template<class Iterator1, class Iterator2>friend typename wrap_iter<iter>::difference_type operator- (const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs) { return (lhs._i - rhs._i); };
	template<class Iterator1, class Iterator2>friend bool operator==(const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs)
	{ return (lhs._i == rhs._i);};
	template<class Iterator1, class Iterator2>friend bool operator!=(const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs)
	{ return (lhs._i != rhs._i);};
	template<class Iterator1, class Iterator2>friend bool operator<(const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs)
	{ return (lhs._i > rhs._i);};
	template<class Iterator1, class Iterator2>friend bool operator>(const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs)
	{ return (lhs._i < rhs._i);};
	template<class Iterator1, class Iterator2>friend bool operator>=(const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs)
	{ return !(lhs < rhs);};
	template<class Iterator1, class Iterator2>friend bool operator<=(const wrap_iter<Iterator1>& lhs, const wrap_iter<Iterator2>& rhs)
	{ return !(lhs > rhs);};
	
};

}
#endif

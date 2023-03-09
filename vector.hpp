#ifndef VECTOR_HPP
# define VECTOR_HPP

#include "SFINAE.hpp"
#include "reverse_iterator.hpp"
#include "compare.hpp"
#include <cstddef>
#include <memory>

namespace ft
{

	template<class T, class Alloc = std::allocator<T> >
class vector
{
public:
	typedef T value_type;
	typedef Alloc allocator_type;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference  const_reference;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer  const_pointer;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef typename ft::reverse_iterator<iterator> reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef ptrdiff_t different_type;
	typedef size_t size_type;

protected:
	typename allocator_type::template rebind<T>::other	_alloc;
	iterator	_data;
	iterator	_avail;
	iterator	_limit;

protected:
	void	initiateNewData(size_type n)
	{
		_data = _alloc.allocate(n);
		_avail = _data;
		_limit = _data + n;
	}
	
	void	deleteOldData(iterator& old_data, iterator& old_avail, iterator& old_limit)
	{
		for (; old_avail > old_data; old_avail--) 
			_alloc.destroy(old_avail);
		_alloc.destroy(old_data);
		_alloc.deallocate(old_data, old_limit - old_data);

		old_data = NULL;
		old_avail = NULL;
		old_limit = NULL;

	} 

	void	copyWithValue(size_type n, const value_type& val)
	{
		for(size_type	idx = 0; idx < n; idx++)
			*(_avail++) = val;
	}

	template<class InputIterator>
	void	copyWithIterator(InputIterator first, InputIterator last)
	{
		for (; first != last; first++)
			*(_avail++) = *first;
	}

	template<class InputIterator1, class InputIterator2> 
	InputIterator2	copyWithIteratorToDestination(InputIterator1 first, InputIterator1 last, InputIterator2 dst)
	{
		for (; first != last; first++)
			*(dst++) = *first;
		return dst;
	}

	template<class InputIterator>
	InputIterator	copyWithValueToDestination(size_type n, const value_type& val, InputIterator dst)
	{
		for(size_type	idx = 0; idx < n; idx++)
			*(dst++) = val;
		return dst;
	}

	size_type	getExtendedCapacity(const size_type& n = 1)
	{
		size_type old_capacity = capacity();
		size_type new_capacity = old_capacity * 2;

		if (!old_capacity)
			return (n);
		for (; new_capacity <= old_capacity + n; new_capacity *= 2)
			;
		return (new_capacity);
	}

	template<class InputIterator>
	size_type	checkSize(InputIterator first, InputIterator last)
	{
		size_type	ret = 0;

		for (InputIterator it = first; it != last; it++)
			ret++;
		return ret;
	}

public:
	// OCCF 
	explicit vector (const allocator_type& alloc = allocator_type())
	: _alloc(alloc), _data(NULL), _avail(NULL), _limit(NULL)
	{}

	explicit vector (size_t n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
	: _alloc(alloc)
	{
		initiateNewData(n);
		copyWithValue(n, val);
	}

	template <class InputIterator>
	vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value, int>::type = 0)
	: _alloc(alloc)
	{
		initiateNewData(checkSize(first, last));
		for (; _avail < _limit; _avail++)
			*_avail = *(first++);
	}

	vector(const vector& x)
	: _data(NULL), _avail(NULL), _limit(NULL)
	{
		_alloc = x.get_allocator();
		assign(x.begin(), x.end()); 
	}
	vector& operator= (const vector& x) { assign(x.begin(), x.end()); return *this; }

	~vector()
	{
		deleteOldData(_data, _avail, _limit);
	}

	//complicated function
	void pop_back()
	{ 
		if (_avail > _data) 
			_alloc.destroy(_avail--); 
		else if (_avail == _data)
			deleteOldData(_data, _avail, _limit);
		
	}

	void push_back (const value_type& val)
	{
		iterator	old_data = _data;
		iterator	old_avail = _avail;
		iterator	old_limit = _limit;

		if (!_data)
		{
			initiateNewData(1);
			copyWithValue(1, val);
		}
		else if (_avail == _limit)
		{
			initiateNewData(getExtendedCapacity());
			copyWithIterator(old_data, old_avail);
			copyWithValue(1, val);
			deleteOldData(old_data, old_avail, old_limit);
		}
		else
			copyWithValue(1, val);
	}
	
	template <class InputIterator>
	void assign (InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, int>::type = 0)
	{
		if (_data)
			deleteOldData(_data, _avail, _limit); 
		initiateNewData(checkSize(first, last));
		copyWithIterator(first, last);
	}

	void assign (size_type n, const value_type& val)
	{
		if (_data)
			deleteOldData(_data, _avail, _limit);
		initiateNewData(n);
		copyWithValue(n, val);
	}

	iterator erase(iterator position)
	{
		iterator	copy_begin = position + 1;
		iterator	copy_end = _avail;

		_avail = position;
		copyWithIterator(copy_begin, copy_end);

		return (position);
	}

	iterator erase (iterator first, iterator last)
	{
		iterator	copy_begin = last;
		iterator	copy_end = _avail;

		_avail = first;
		copyWithIterator(copy_begin, copy_end);

		return (first);
	}

	iterator insert(iterator position, const value_type& val)
	{
		iterator	old_data = _data;
		iterator	old_avail = _avail;
		iterator	old_limit = _limit;
		iterator	inserted_position = NULL;

		if (size() + 1 > capacity())
		{
			initiateNewData(getExtendedCapacity());
			copyWithIterator(old_data, position);
			inserted_position = _avail;
			copyWithValue(1, val);
			copyWithIterator(position, old_avail);
			deleteOldData(old_data, old_avail, old_limit);
		}
		else if (size() + 1 <= capacity())
		{
			inserted_position = position;
			copyWithIteratorToDestination(reverse_iterator(_avail), reverse_iterator(position), reverse_iterator(_avail + 1));
			copyWithValueToDestination(1, val, position);
			_avail++;
		}
		return inserted_position;
	}
    void insert (iterator position, size_type n, const value_type& val)
	{
		iterator	old_data = _data;
		iterator	old_avail = _avail;
		iterator	old_limit = _limit;

		if (size() + n > capacity())
		{
			initiateNewData(getExtendedCapacity(n));
			copyWithIterator(old_data, position);
			copyWithValue(n, val);
			copyWithIterator(position, old_avail);
			deleteOldData(old_data, old_avail, old_limit);
		}
		else if (size() + n <= capacity())
		{
			copyWithIteratorToDestination(reverse_iterator(_avail), reverse_iterator(position), reverse_iterator(_avail + n));
			copyWithValueToDestination(n, val, position);
			_avail += n;
		}
	}
	template <class InputIterator>
	void insert (iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, int>::type = 0)
	{
		size_type	n = checkSize(first, last);
		iterator	old_data = _data;
		iterator	old_avail = _avail;
		iterator	old_limit = _limit;

		if (size() + n > capacity())
		{
			initiateNewData(getExtendedCapacity(n));
			copyWithIterator(old_data, position);
			copyWithIterator(first, last);
			copyWithIterator(position, old_avail);
			deleteOldData(old_data, old_avail, old_limit);
		}
		else if (size() + n <= capacity())
		{
			copyWithIteratorToDestination(reverse_iterator(_avail), reverse_iterator(position), reverse_iterator(_avail + n));
			copyWithIteratorToDestination(first, last, position);
			_avail += n;
		}
	}

	void reserve (size_type n)
	{
		if (n < capacity())
			return;
	
		iterator	old_data = _data;
		iterator	old_avail = _avail;
		iterator	old_limit = _limit;

		initiateNewData(n);
		copyWithIterator(old_data, old_avail);
		deleteOldData(old_data, old_avail, old_limit); 
	}

	void resize (size_type n, value_type val = value_type())
	{
		if (n < size())
		{
			for (; size() != n; ) _alloc.destroy(_avail--);
		}
		else if (size() < n && n <= capacity())
			copyWithValue(n - size(), val);
		else if (capacity() < n)
		{
			iterator	old_data = _data;
			iterator	old_avail = _avail;
			iterator	old_limit = _limit;

			initiateNewData(n);
			copyWithIterator(old_data, old_avail);
			copyWithValue(n - size(), val);
			deleteOldData(old_data, old_avail, old_limit); 
		}
	}

	void swap (vector& x)
	{
		iterator	tmp_data = x._data;
		iterator	tmp_avail = x._avail;
		iterator	tmp_limit = x._limit;

		x._data = _data;
		x._avail = _avail;
		x._limit = _limit;

		_data = tmp_data;
		_avail = tmp_avail;
		_limit = tmp_limit;
	}

	//simple function
	
	reference back() { return *(_avail - 1); }
	const_reference back() const { return *(_avail - 1); }

	iterator begin() { return _data; }
	const_iterator begin() const { return _data; }

	size_type capacity() const { return (_limit - _data); }
	size_type size() const { return _avail - _data;}

	void clear() { _avail = _data; }
	
	bool empty() const { return (_data == _avail); }

	iterator end() { return _avail; }
	const_iterator end() const { return _avail; }

	reference front() { return *_data; }
	const_reference front() const { return *_data; }

	allocator_type get_allocator() const { return _alloc; }

	size_type max_size() const { return ((size_type)-1 / sizeof(value_type));}

	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }

	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

	reference operator[] (size_type n)
	{
		if (n >= size())
			throw std::out_of_range("vector");
		return _data[n];
	}
	const_reference operator[] (size_type n) const
	{
		if (n >= size())
			throw std::out_of_range("vector");
		return _data[n];
	}

	reference at(size_type n) 
	{
		if (n >= size())
			throw std::out_of_range("vector");
		return _data[n];
	}
	const_reference at(size_type n) const 
	{
		if (n >= size())
			throw std::out_of_range("vector");
		return _data[n];
	}


	//friend function
	friend bool operator==(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return	(lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin())); }

	friend bool operator!=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return	!(lhs == rhs); }
	
	friend bool operator<(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
	
	friend bool operator>(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return (rhs < lhs); }
	
	friend bool operator<=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return !(lhs > rhs); }
	
	friend bool operator>=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return !(lhs < rhs); }



	friend void swap (vector<T,Alloc>& x, vector<T,Alloc>& y)
	{ x.swap(y); }
};


};
#endif

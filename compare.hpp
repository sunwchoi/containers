#ifndef COMPARE_HPP
# define COMPARE_HPP
namespace ft
{

template <class InputIterator, class InputIterator2>
bool equal (InputIterator first1, InputIterator last1, InputIterator2 first2)
{
	 while (first1 != last1)
	{
		if (*first1 != *first2)
			return false;
		++first1; ++first2;
	}
	return true;
}
template <class InputIterator1, class InputIterator2, class BinaryPredicate>
bool equal (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred)
{
	 while (first1 != last1)
	{
		if (*first1 != *first2 || !pred(*first1, *first2))
			return false;
		++first1; ++first2;
	}
	return true;
}

template <class InputIterator, class InputIterator2>
bool lexicographical_compare (InputIterator first1, InputIterator last1, InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first2 < *first1)
			return false;
		else if (*first1 < *first2)
			return true;
		++first1; ++first2;
	}

	if (first1 == last1 && first2 == last2)
		return false;
	else if (first2 == last2)
		return false;
	return (true);
}
template <class InputIterator, class InputIterator2, class Compare>
bool lexicographical_compare (InputIterator first1, InputIterator last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first2 < *first1)
			return false;
		else if (*first1 < *first2)
			return true;
		if (comp(*first2, *first1))
			return false;
		else if (comp(*first1, *first2))
			return true;
		++first1; ++first2;
	}

	if (first1 == last1 && first2 == last2)
		return false;
	else if (first2 == last2)
		return false;
	return (true);
}

template <class T> struct less
{
	bool operator() (const T& x, const T& y) const { return x < y; }
};

}
#endif

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <deque>

#if 0
#if TEST//CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
void	leak_check() { };
#else
	#include "map.hpp"
	#include "stack.hpp"
	#include "vector.hpp"
void	leak_check()
{
	system("leaks my_containers");
}
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

int main(int argc, char** argv) {



	std::atexit(leak_check);
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	clock_t t = clock(); 
	const int seed = atoi(argv[1]);
	srand(seed);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	ft::map<int, int> map_int;

	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}
	
	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(rand(), rand()));
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		ft::map<int, int> copy = map_int;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}
	std::cout << std::endl;
	std::cout << t - clock() << std::endl;
	return (0);
}
//#else
#include "vector.hpp"
#include "map.hpp"

void	leak_check()
{
	system("leaks my_containers");
}

template<class InputIterator>
void	printAll(InputIterator first, InputIterator last)
{
	std::cout << "| ";
	for (; first != last; ++first)
		std::cout << *first << "| ";
	std::cout << std::endl;
}

int main()
{
	clock_t t = clock();

	ft::vector<int> vec;

	std::atexit(leak_check);
	
	std::cout << "************************VECTOR*****************************" << std::endl;
	getchar();

	std::cout << "************************push-pop*****************************" << std::endl;
	getchar();
	
	for (int i = 0; i < 8; i++)
	{
		int val = rand();
		std::cout << "push: " << val << std::endl;
		vec.push_back(val);
		printAll(vec.begin(), vec.end());
		if (i % 7 == 4)
		{
			std::cout << std::endl;
			std::cout << "pop " << std::endl;
			vec.pop_back();
			printAll(vec.begin(), vec.end());
			std::cout << std::endl;
		}
	}


	getchar();
	std::cout << "************************size-capacity*****************************" << std::endl;
	getchar();
	
	for (int i = 0; i < 60; i++)
	{
		std::cout	<< "size: " << vec.size()
					<< " capacity: " << vec.capacity()
					<< std::endl;
		if (i ==  0 )
		{
			vec.insert(vec.end(), 10, 1);
			std::cout << "insert 10 elements" << std::endl;
			std::cout	<< "size: " << vec.size()
						<< " capacity: " << vec.capacity()
						<< std::endl;


		}
		vec.push_back(i);
	}

	getchar();
	std::cout << "***********************erase-insert******************************" << std::endl;
	getchar();
	
	vec.erase(vec.begin() + 7, vec.end());
	printAll(vec.begin(), vec.end());
	vec.insert(vec.begin() + 2, 0);
	printAll(vec.begin(), vec.end());


	getchar();
	std::cout << "************************MAP*****************************" << std::endl;
	getchar();

	getchar();
	std::cout << "************************insert-erase*****************************" << std::endl;
	getchar();

	ft::map<int, int> mp;
	ft::map<int, int>::iterator it;
	for (int i = 0; i < 100; i++)
	{
		int key = rand() % 100;
		std::cout << "insert: " << key << std::endl;
		mp.insert(ft::make_pair(key, 0));
		//mp.print();
		getchar();
		if (i % 7 == 5)
		{
			std::cout << std::endl;
			it = ++(++(++mp.begin()));
			std::cout << "erase: " <<  it->first << std::endl;
			mp.erase(it->first);
			//mp.print();
			getchar();
			std::cout << std::endl;
		}
	}
	std::cout << "hi"<< std::endl;	
	std::cout << t - clock() << std::endl;
}
#else

#include <cstdio>
#include <vector>
#include <map>
#include "map"
int _ratio = 1;
template <class T, class V>
std::vector<int> erase_test_1(std::map<T, V> mp) {
    std::vector<int> v;
    v.push_back(mp.erase(3));
    for (int i = 0, j = 0; i < 30 * _ratio; ++i, ++j)
        mp.insert(std::make_pair(i, j));
    typename std::map<T, V>::iterator it = mp.begin();
    v.push_back(it->first);
    v.push_back(mp.erase(-5));
    v.push_back(mp.size());
    v.push_back(mp.erase(0));
    v.push_back(mp.size());
    it = mp.begin();
    v.push_back(it->first);
    typename std::map<T, V>::iterator it4 = mp.begin();
    for (; it4 != mp.end(); it4 = mp.begin())
        mp.erase(it4->first);
    v.push_back(mp.erase(30 * _ratio - 1));
    v.push_back(mp.size());
    std::map<int, int> mp2;
    for (int i = 0, j = 0; i < 10 ; ++i, ++j)
        mp2.insert(std::make_pair(i, j));
    mp2.erase(2);
    mp2.erase(7);
    typename std::map<T, V>::iterator it3 = mp2.begin();
    for (; it3 != mp2.end(); ++it3) {
        v.push_back(it3->first);
        v.push_back(it3->second);
    }
    return v;
}
template <class T, class V>
std::vector<int> erase_test_1(ft::map<T, V> mp) {
    std::vector<int> v;
    v.push_back(mp.erase(3));
    for (int i = 0, j = 0; i < 30 * _ratio; ++i, ++j)
        mp.insert(ft::make_pair(i, j));
    typename ft::map<T, V>::iterator it = mp.begin();
    v.push_back(it->first);
    v.push_back(mp.erase(-5));
    v.push_back(mp.size());
    v.push_back(mp.erase(0));
    v.push_back(mp.size());
	it = mp.begin();
    v.push_back(it->first);
    typename ft::map<T, V>::iterator it4 = mp.begin();
    g_start2 = timer();
    for (; it4 != mp.end(); it4 = mp.begin())
        mp.erase(it4->first);
    g_end2 = timer();
    v.push_back(mp.erase(30 * _ratio - 1));
    v.push_back(mp.size());
    ft::map<int, int> mp2;
    for (int i = 0, j = 0; i < 10 ; ++i, ++j)
        mp2.insert(ft::make_pair(i, j));
    mp2.erase(2);
    mp2.erase(7);
    typename ft::map<T, V>::iterator it3 = mp2.begin();
    for (; it3 != mp2.end(); ++it3) {
        v.push_back(it3->first);
        v.push_back(it3->second);
    }
    return v;
}

int main() {

    exit(run_map_unit_test<int, int>("erase(key)", erase_test_1, erase_test_1));
}

#endif

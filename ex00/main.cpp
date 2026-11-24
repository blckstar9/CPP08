#include "easyfind.hpp"

#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

static void	testVector(void)
{
	std::cout << "=== Test std::vector<int> ===" << std::endl;
	std::vector<int>	vec;

	vec.push_back(10);
	vec.push_back(42);
	vec.push_back(7);
	vec.push_back(42);
	vec.push_back(-5);

	try
	{
		std::vector<int>::iterator	it = easyfind(vec, 42);
		std::cout << "Found 42 at index: " << std::distance(vec.begin(), it) << std::endl;
		*it = 99;
		std::cout << "Modified first occurrence to: " << vec[1]
				  << " (second stays " << vec[3] << ")" << std::endl;

		it = easyfind(vec, 10);
		std::cout << "Found first element (10) at index: "
				  << std::distance(vec.begin(), it) << std::endl;

		it = easyfind(vec, -5);
		std::cout << "Found last element (-5) at index: "
				  << std::distance(vec.begin(), it) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Unexpected exception: " << e.what() << std::endl;
	}

	try
	{
		std::cout << "Searching absent value 1234..." << std::endl;
		easyfind(vec, 1234);
		std::cout << "ERROR: exception was not thrown!" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}

	std::vector<int>	emptyVec;
	try
	{
		std::cout << "Searching in empty vector..." << std::endl;
		easyfind(emptyVec, 1);
		std::cout << "ERROR: exception was not thrown!" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}
}

static void	testConstVector(void)
{
	std::cout << "\n=== Test const std::vector<int> ===" << std::endl;
	std::vector<int>	tmp;
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(3);
	const std::vector<int>	cvec(tmp);

	try
	{
		std::vector<int>::const_iterator	it = easyfind(cvec, 2);
		std::cout << "Found 2 in const vector at index: "
				  << std::distance(cvec.begin(), it) << " (*it = " << *it << ")" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Unexpected exception: " << e.what() << std::endl;
	}

	try
	{
		easyfind(cvec, 999);
		std::cout << "ERROR: exception was not thrown!" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught expected exception in const vector: " << e.what() << std::endl;
	}
}

static void	testList(void)
{
	std::cout << "\n=== Test std::list<int> ===" << std::endl;
	std::list<int>	lst;

	lst.push_back(5);
	lst.push_back(15);
	lst.push_back(25);
	lst.push_back(15);

	try
	{
		std::list<int>::iterator	it = easyfind(lst, 15);
		std::cout << "Found 15 at position: " << std::distance(lst.begin(), it) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Unexpected exception: " << e.what() << std::endl;
	}

	try
	{
		std::cout << "Searching absent value -100..." << std::endl;
		easyfind(lst, -100);
		std::cout << "ERROR: exception was not thrown!" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}

	std::list<int>	emptyLst;
	try
	{
		std::cout << "Searching in empty list..." << std::endl;
		easyfind(emptyLst, 0);
		std::cout << "ERROR: exception was not thrown!" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}
}

static void	testDeque(void)
{
	std::cout << "\n=== Test std::deque<int> ===" << std::endl;
	std::deque<int>	deq;

	deq.push_back(100);
	deq.push_front(50);
	deq.push_back(200);

	try
	{
		std::deque<int>::iterator	it = easyfind(deq, 50);
		std::cout << "Found 50 at index: " << std::distance(deq.begin(), it) << std::endl;
		easyfind(deq, 300);
		std::cout << "ERROR: exception was not thrown!" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught expected exception: " << e.what() << std::endl;
	}
}

int	main(void)
{
	testVector();
	testConstVector();
	testList();
	testDeque();
	return (0);
}

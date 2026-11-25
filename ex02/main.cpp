#include "MutantStack.hpp"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <stack>
#include <string>
#include <vector>

static int	g_failures = 0;

static void	check(bool condition, const std::string &label)
{
	if (condition)
		std::cout << "[OK] " << label << "\n";
	else
	{
		std::cout << "[KO] " << label << "\n";
		++g_failures;
	}
}

static std::vector<int>	makeValues(void)
{
	std::vector<int>	values;

	values.push_back(5);
	values.push_back(3);
	values.push_back(5);
	values.push_back(737);
	values.push_back(0);
	return (values);
}

static void	testSubjectExample(void)
{
	MutantStack<int>	mstack;
	MutantStack<int>::iterator	it;
	MutantStack<int>::iterator	ite;
	std::vector<int>			values;
	std::size_t					index;

	check(mstack.empty(), "subject: new stack is empty");
	check(mstack.begin() == mstack.end(), "subject: empty begin equals end");

	mstack.push(5);
	mstack.push(17);
	check(mstack.top() == 17, "subject: top is 17");
	check(mstack.size() == 2, "subject: size is 2");

	mstack.pop();
	check(mstack.size() == 1, "subject: size is 1 after pop");
	check(mstack.top() == 5, "subject: top is 5 after pop");

	mstack.push(3);
	mstack.push(5);
	mstack.push(737);
	mstack.push(0);

	values = makeValues();
	index = 0;
	it = mstack.begin();
	ite = mstack.end();
	++it;
	--it;
	while (it != ite)
	{
		check(*it == values[index], "subject: iterator order");
		++it;
		++index;
	}
	check(index == values.size(), "subject: iterator visited all elements");

	std::stack<int>	plain(mstack);
	check(plain.size() == mstack.size(), "subject: conversion to std::stack");
	check(plain.top() == mstack.top(), "subject: converted stack top");
}

static void	testListComparison(void)
{
	MutantStack<int>	stack;
	std::list<int>		list;
	int					values[] = {5, 17, 3, 5, 737, 0, -8, 42};
	std::size_t			i;

	i = 0;
	while (i < sizeof(values) / sizeof(values[0]))
	{
		stack.push(values[i]);
		list.push_back(values[i]);
		++i;
	}
	check(std::equal(stack.begin(), stack.end(), list.begin()),
		"list comparison: forward iteration");
	check(std::equal(stack.rbegin(), stack.rend(), list.rbegin()),
		"list comparison: reverse iteration");

	while (!stack.empty())
	{
		check(stack.top() == list.back(), "list comparison: top equals back");
		stack.pop();
		list.pop_back();
	}
	check(list.empty(), "list comparison: both containers are empty");
}

static void	testCopyAndAssignment(void)
{
	MutantStack<int>	source;
	MutantStack<int>	assigned;

	source.push(1);
	source.push(2);
	source.push(3);
	MutantStack<int>	copy(source);
	check(std::equal(copy.begin(), copy.end(), source.begin()),
		"copy: same contents");
	check(copy.size() == source.size(), "copy: same size");

	copy.push(4);
	check(copy.size() == 4, "copy: push on copy");
	check(source.size() == 3, "copy: source is independent");

	assigned = source;
	check(std::equal(assigned.begin(), assigned.end(), source.begin()),
		"assignment: same contents");
	assigned.pop();
	check(assigned.size() == 2, "assignment: pop on assigned stack");
	check(source.size() == 3, "assignment: source is independent");

	source = source;
	check(source.size() == 3, "self-assignment: size is unchanged");
	check(source.top() == 3, "self-assignment: top is unchanged");

	MutantStack<int>	empty;
	MutantStack<int>	emptyCopy(empty);
	check(emptyCopy.empty(), "copy: empty stack");
	assigned = empty;
	check(assigned.empty(), "assignment: empty stack");
}

static void	testVectorContainer(void)
{
	MutantStack<int, std::vector<int> >	stack;
	std::vector<int>					expected;

	stack.push(10);
	stack.push(20);
	stack.push(30);
	expected.push_back(10);
	expected.push_back(20);
	expected.push_back(30);
	check(std::equal(stack.begin(), stack.end(), expected.begin()),
		"vector container: forward iteration");
	check(*stack.rbegin() == 30, "vector container: reverse begin");
	check(stack.top() == 30, "vector container: top");
	stack.pop();
	check(stack.size() == 2, "vector container: pop");
	check(stack.top() == 20, "vector container: top after pop");
}

static void	testListContainer(void)
{
	MutantStack<int, std::list<int> >	stack;

	stack.push(-1);
	stack.push(-2);
	stack.push(-3);
	check(stack.size() == 3, "list container: size");
	check(*stack.begin() == -1, "list container: first element");
	check(*stack.rbegin() == -3, "list container: last element");
	check(stack.top() == -3, "list container: top");
}

static void	testConstIterators(void)
{
	MutantStack<int>		mutableStack;
	const MutantStack<int>	stack(mutableStack);
	std::vector<int>		expected;
	MutantStack<int>::const_iterator			it;
	MutantStack<int>::const_reverse_iterator	rit;
	std::size_t									index;

	mutableStack.push(7);
	mutableStack.push(8);
	mutableStack.push(9);
	const MutantStack<int>	filled(mutableStack);
	expected.push_back(7);
	expected.push_back(8);
	expected.push_back(9);
	check(stack.empty(), "const iterators: empty const stack");
	check(std::equal(filled.begin(), filled.end(), expected.begin()),
		"const iterators: begin/end");
	index = 0;
	it = filled.begin();
	while (it != filled.end())
	{
		check(*it == expected[index], "const iterators: element order");
		++it;
		++index;
	}
	rit = filled.rbegin();
	check(*rit == 9, "const iterators: reverse begin");
	++rit;
	check(*rit == 8, "const iterators: reverse increment");
}

static void	testLargeStack(void)
{
	const int			count = 10000;
	MutantStack<int>	stack;
	int					i;

	i = 0;
	while (i < count)
	{
		stack.push(i);
		++i;
	}
	check(stack.size() == static_cast<std::size_t>(count),
		"large stack: size");
	check(std::distance(stack.begin(), stack.end()) == count,
		"large stack: iterator distance");
	check(*stack.begin() == 0, "large stack: first element");
	check(*stack.rbegin() == count - 1, "large stack: last element");
	check(stack.top() == count - 1, "large stack: top");
}

int	main(void)
{
	testSubjectExample();
	testListComparison();
	testCopyAndAssignment();
	testVectorContainer();
	testListContainer();
	testConstIterators();
	testLargeStack();

	if (g_failures == 0)
		std::cout << "All MutantStack tests passed.\n";
	else
		std::cout << g_failures << " MutantStack test(s) failed.\n";
	return (g_failures == 0 ? 0 : 1);
}

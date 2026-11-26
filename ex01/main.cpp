#include "Span.hpp"

#include <climits>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
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

static void	checkSpans(const Span &span, unsigned int shortest,
	unsigned int longest, const std::string &label)
{
	try
	{
		check(span.shortestSpan() == shortest, label + " (shortestSpan)");
		check(span.longestSpan() == longest, label + " (longestSpan)");
	}
	catch (const std::exception &e)
	{
		std::cout << "[KO] " << label << ": unexpected exception: "
			<< e.what() << "\n";
		++g_failures;
	}
}

static void	testSubjectExample(void)
{
	Span	span(5);

	span.addNumber(6);
	span.addNumber(3);
	span.addNumber(17);
	span.addNumber(9);
	span.addNumber(11);
	check(span.size() == 5, "subject example: size is 5");
	check(span.capacity() == 5, "subject example: capacity is 5");
	checkSpans(span, 2, 14, "subject example");
}

static void	testEmptyAndSingle(void)
{
	Span	empty;
	Span	one(3);
	bool	thrown;

	check(empty.size() == 0, "default Span: size is 0");
	check(empty.capacity() == 0, "default Span: capacity is 0");

	thrown = false;
	try
	{
		empty.shortestSpan();
	}
	catch (const std::logic_error &)
	{
		thrown = true;
	}
	check(thrown, "empty Span: shortestSpan throws");

	thrown = false;
	try
	{
		empty.longestSpan();
	}
	catch (const std::logic_error &)
	{
		thrown = true;
	}
	check(thrown, "empty Span: longestSpan throws");

	thrown = false;
	try
	{
		empty.addNumber(1);
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "default Span: addNumber throws");

	one.addNumber(42);
	thrown = false;
	try
	{
		one.shortestSpan();
	}
	catch (const std::logic_error &)
	{
		thrown = true;
	}
	check(thrown, "single element: shortestSpan throws");

	thrown = false;
	try
	{
		one.longestSpan();
	}
	catch (const std::logic_error &)
	{
		thrown = true;
	}
	check(thrown, "single element: longestSpan throws");
}

static void	testDuplicatesAndSigns(void)
{
	Span	duplicates(4);
	Span	mixed(5);

	duplicates.addNumber(5);
	duplicates.addNumber(5);
	duplicates.addNumber(7);
	duplicates.addNumber(7);
	checkSpans(duplicates, 0, 2, "duplicates: zero shortest span");

	mixed.addNumber(-10);
	mixed.addNumber(-2);
	mixed.addNumber(5);
	mixed.addNumber(100);
	mixed.addNumber(-42);
	checkSpans(mixed, 7, 142, "positive and negative values");
}

static void	testIntegerLimits(void)
{
	Span	extremes(3);

	extremes.addNumber(INT_MIN);
	extremes.addNumber(INT_MAX);
	checkSpans(extremes, 4294967295U, 4294967295U,
		"INT_MIN/INT_MAX without overflow");
	extremes.addNumber(0);
	checkSpans(extremes, 2147483647U, 4294967295U,
		"INT_MIN/0/INT_MAX without overflow");
}

static void	testCapacity(void)
{
	Span				full(2);
	Span				partial(5);
	Span				rangeSpan(5);
	std::vector<int>	tooMany;
	std::vector<int>	values;
	std::vector<int>	emptyRange;
	bool				thrown;

	full.addNumber(1);
	full.addNumber(2);
	thrown = false;
	try
	{
		full.addNumber(3);
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "capacity reached: addNumber throws");
	check(full.size() == 2, "capacity reached: size is unchanged");

	partial.addNumber(10);
	partial.addNumber(20);
	partial.addNumber(30);
	tooMany.push_back(1);
	tooMany.push_back(2);
	tooMany.push_back(3);
	thrown = false;
	try
	{
		partial.addNumbers(tooMany.begin(), tooMany.end());
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "range overflow: addNumbers throws");
	check(partial.size() == 3, "range overflow: no partial insertion");
	checkSpans(partial, 10, 20, "range overflow: existing data intact");

	emptyRange.clear();
	rangeSpan.addNumbers(emptyRange.begin(), emptyRange.end());
	check(rangeSpan.size() == 0, "empty range: nothing is added");

	values.push_back(1);
	values.push_back(4);
	values.push_back(9);
	values.push_back(16);
	values.push_back(25);
	rangeSpan.addNumbers(values.begin(), values.end());
	check(rangeSpan.size() == 5, "range add: all values added");
	checkSpans(rangeSpan, 3, 24, "range add");
}

static void	testRangeFromList(void)
{
	Span		span(4);
	std::list<int>	values;
	bool		thrown;

	values.push_back(-7);
	values.push_back(3);
	values.push_back(8);
	values.push_back(-20);
	span.addNumbers(values.begin(), values.end());
	checkSpans(span, 5, 28, "range add from std::list");

	thrown = false;
	try
	{
		std::list<int>	extra;

		extra.push_back(1);
		span.addNumbers(extra.begin(), extra.end());
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "range add: full Span throws");
}

static void	testInputIterator(void)
{
	Span				span(4);
	std::istringstream	input("4 8 15");

	span.addNumbers(std::istream_iterator<int>(input),
		std::istream_iterator<int>());
	check(span.size() == 3, "input iterator: all values added");
	checkSpans(span, 4, 11, "input iterator");
}

static void	testInputIteratorOverflow(void)
{
	Span				span(3);
	std::istringstream	input("30 40");
	bool				thrown;

	span.addNumber(10);
	span.addNumber(20);
	thrown = false;
	try
	{
		span.addNumbers(std::istream_iterator<int>(input),
			std::istream_iterator<int>());
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "input iterator overflow: addNumbers throws");
	check(span.size() == 2, "input iterator overflow: no partial insertion");
	checkSpans(span, 10, 10, "input iterator overflow: existing data intact");
}

static void	testTenThousandNumbers(void)
{
	const unsigned int	count = 10000;
	Span				span(count);
	std::vector<int>	values;
	unsigned int		i;
	bool				thrown;

	i = 0;
	while (i < count)
	{
		values.push_back(static_cast<int>(i) * 2);
		++i;
	}
	span.addNumbers(values.begin(), values.end());
	check(span.size() == count, "10000 numbers: size");
	checkSpans(span, 2, 19998, "10000 numbers");

	thrown = false;
	try
	{
		span.addNumber(42);
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "10000 numbers: capacity is enforced");
}

static void	testCopyAndAssignment(void)
{
	Span	source(5);
	Span	assigned(2);
	bool	thrown;

	source.addNumber(1);
	source.addNumber(5);
	source.addNumber(10);
	Span	copy(source);
	check(copy.capacity() == 5, "copy: capacity is copied");
	check(copy.size() == 3, "copy: size is copied");
	checkSpans(copy, 4, 9, "copy");
	copy.addNumber(20);
	checkSpans(source, 4, 9, "copy: source is independent");
	checkSpans(copy, 4, 19, "copy: copy is independent");

	assigned = source;
	check(assigned.capacity() == 5, "assignment: capacity is copied");
	check(assigned.size() == 3, "assignment: size is copied");
	checkSpans(assigned, 4, 9, "assignment");

	source = source;
	checkSpans(source, 4, 9, "self-assignment");

	thrown = false;
	try
	{
		Span	tooSmall(1);

		tooSmall.addNumber(1);
		tooSmall.addNumber(2);
	}
	catch (const std::out_of_range &)
	{
		thrown = true;
	}
	check(thrown, "small capacity: addNumber throws");

	const Span	constSpan(source);
	checkSpans(constSpan, 4, 9, "const Span");
}

int	main(void)
{
	testSubjectExample();
	testEmptyAndSingle();
	testDuplicatesAndSigns();
	testIntegerLimits();
	testCapacity();
	testRangeFromList();
	testInputIterator();
	testInputIteratorOverflow();
	testTenThousandNumbers();
	testCopyAndAssignment();

	if (g_failures == 0)
		std::cout << "All Span tests passed.\n";
	else
		std::cout << g_failures << " Span test(s) failed.\n";
	return (g_failures == 0 ? 0 : 1);
}

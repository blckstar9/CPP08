#include "Span.hpp"

#include <cstddef>
#include <stdexcept>

Span::Span() : _capacity(0), _numbers()
{
}

Span::Span(unsigned int n) : _capacity(n), _numbers()
{
	_numbers.reserve(n);
}

Span::Span(const Span &other)
	: _capacity(other._capacity), _numbers(other._numbers)
{
}

Span	&Span::operator=(const Span &other)
{
	if (this != &other)
	{
		_capacity = other._capacity;
		_numbers = other._numbers;
	}
	return (*this);
}

Span::~Span()
{
}

void	Span::addNumber(int number)
{
	if (_numbers.size() >= _capacity)
		throw std::out_of_range("Span: maximum capacity reached");
	_numbers.push_back(number);
}

unsigned int	Span::shortestSpan() const
{
	std::vector<int>		sorted;
	std::vector<unsigned int>	spans;
	std::size_t				i;

	if (_numbers.size() < 2)
		throw std::logic_error("Span: at least two numbers are required");
	sorted = _numbers;
	std::sort(sorted.begin(), sorted.end());
	spans.reserve(sorted.size());
	i = 1;
	while (i < sorted.size())
	{
		spans.push_back(static_cast<unsigned int>(sorted[i])
			- static_cast<unsigned int>(sorted[i - 1]));
		++i;
	}
	return (*std::min_element(spans.begin(), spans.end()));
}

unsigned int	Span::longestSpan() const
{
	std::vector<int>::const_iterator	minIt;
	std::vector<int>::const_iterator	maxIt;

	if (_numbers.size() < 2)
		throw std::logic_error("Span: at least two numbers are required");
	minIt = std::min_element(_numbers.begin(), _numbers.end());
	maxIt = std::max_element(_numbers.begin(), _numbers.end());
	return (static_cast<unsigned int>(*maxIt) - static_cast<unsigned int>(*minIt));
}

unsigned int	Span::size() const
{
	return (static_cast<unsigned int>(_numbers.size()));
}

unsigned int	Span::capacity() const
{
	return (_capacity);
}

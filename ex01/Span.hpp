#ifndef SPAN_HPP
# define SPAN_HPP

# include <algorithm>
# include <iterator>
# include <numeric>
# include <stdexcept>
# include <vector>

class Span
{
public:
	Span();
	explicit Span(unsigned int n);
	Span(const Span &other);
	Span &operator=(const Span &other);
	~Span();

	void	addNumber(int number);

	template <typename InputIterator>
	void	addNumbers(InputIterator begin, InputIterator end);

	unsigned int	shortestSpan() const;
	unsigned int	longestSpan() const;

	unsigned int		size() const;
	unsigned int		capacity() const;

private:
	unsigned int		_capacity;
	std::vector<int>	_numbers;
};

template <typename InputIterator>
void	Span::addNumbers(InputIterator begin, InputIterator end)
{
	typename std::iterator_traits<InputIterator>::difference_type	count;

	count = std::distance(begin, end);
	if (count < 0)
		throw std::invalid_argument("Span: invalid iterator range");
	if (static_cast<unsigned long>(count)
		> static_cast<unsigned long>(_capacity - _numbers.size()))
		throw std::out_of_range("Span: not enough capacity for this range");
	_numbers.reserve(_numbers.size() + static_cast<unsigned long>(count));
	std::copy(begin, end, std::back_inserter(_numbers));
}

#endif

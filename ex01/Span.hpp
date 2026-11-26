#ifndef SPAN_HPP
# define SPAN_HPP

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
	std::vector<int>	pending;

	while (begin != end)
	{
		if (_numbers.size() + pending.size() >= _capacity)
			throw std::out_of_range("Span: not enough capacity for this range");
		pending.push_back(*begin);
		++begin;
	}
	_numbers.reserve(_numbers.size() + pending.size());
	_numbers.insert(_numbers.end(), pending.begin(), pending.end());
}

#endif

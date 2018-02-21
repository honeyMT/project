#pragma once

//
// 迭代器的型别
//
struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : public InputIteratorTag {};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};

template <class Iterator>
struct IteratorTraits
{
	typedef typename Iterator::IteratorCategory IteratorCategory;
	typedef typename Iterator::ValueType        ValueType;
	typedef typename Iterator::DifferenceType   DifferenceType;
	typedef typename Iterator::Pointer           Pointer;
	typedef typename Iterator::Reference         Reference;
};

template <class T>
struct IteratorTraits<T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T        ValueType;
	typedef size_t   DifferenceType;
	typedef T*       Pointer;
	typedef T&       Reference;
};

template <class T>
struct IteratorTraits<const T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T        ValueType;
	typedef size_t   DifferenceType;
	typedef const T*       Pointer;
	typedef const T&       Reference;
};

// O(1)
template<class InputIterator>
size_t __Distance(InputIterator first, InputIterator last, RandomAccessIteratorTag)
{
	return last - first;
}

// O(N)
template<class InputIterator>
size_t __Distance(InputIterator first, InputIterator last, InputIteratorTag)
{
	size_t n = 0;
	while (first != last)
	{
		++n;
		++first;
	}

	return n;
}

// Vector  -> T*
// List  -> __ListIterator

//template<class InputIterator>
//size_t Distance(InputIterator first, InputIterator last)
//{
//	return __Distance(first, last, InputIterator::IteratorCategory());
//}

template<class InputIterator>
size_t Distance(InputIterator first, InputIterator last)
{
	return __Distance(first, last, IteratorTraits<InputIterator>::IteratorCategory());
}

//////////////////////////////////////////////////////////
template<class InputIterator>
InputIterator Advance(InputIterator it, int n)
{
	return __Advance(it, n, IteratorTraits<InputIterator>::IteratorCategory())
}

// O(N)
template<class InputIterator>
InputIterator __Advance(InputIterator it, int n, InputIteratorTag)
{
	assert(n >= 0);

	while (n--)
		++it;

	return it;
}

template<class InputIterator>
InputIterator __Advance(InputIterator it, int n, BidirectionalIteratorTag)
{
	if (n > 0)
	{
		while (n--)
			++it;
	}
	else
	{
		while (n++)
			--it;
	}

	return it;
}

// O(1)
template<class InputIterator>
InputIterator __Advance(InputIterator it, int n, RandomAccessIteratorTag)
{
	return it + n;
}

///////////////////////////////////////////////////////////////////////
// 适配器
template<class Iterator>
struct ReverseIterator
{
	Iterator _it; // 正向迭代器

	typedef ReverseIterator<Iterator> Self;

	typedef typename IteratorTraits<Iterator>::IteratorCategory IteratorCategory;
	typedef typename IteratorTraits<Iterator>::ValueType ValueType;
	typedef typename IteratorTraits<Iterator>::DifferenceType DifferenceType;
	typedef typename IteratorTraits<Iterator>::Reference Reference;
	typedef typename IteratorTraits<Iterator>::Pointer Pointer;


	explicit ReverseIterator(Iterator it)
		:_it(it)
	{}

	Reference operator*()
	{
		Iterator tmp = _it;
		--tmp;
		return *tmp;
	}

	Pointer operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		--_it;
		return *this;
	}

	Self& operator--()
	{
		++_it;
		return *this;
	}

	bool operator==(const Self& s)
	{
		return _it == s._it;
	}

	bool operator!=(const Self& s)
	{
		return _it != s._it;
	}
};

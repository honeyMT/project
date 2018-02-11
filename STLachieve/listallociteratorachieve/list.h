#pragma once
//#include <list>

#include "Alloc.h"
#include "Iterator.h"

template<class T>
struct __ListNode
{
	__ListNode<T>* _next;
	__ListNode<T>* _prev;
	T _data;

	__ListNode(const T& x)
		:_data(x)
		, _next(NULL)
		, _prev(NULL)
	{}
};

// STL设计哲学  效率+复用

//typedef __ListIterator<T, T&, T*> Iterator;
//typedef __ListIterator<T, const T&, const T*> ConstIterator;
template<class T, class Ref, class Ptr>
struct __ListIterator
{
	typedef __ListNode<T> Node;
	typedef __ListIterator<T, Ref, Ptr> Self;

	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptr Pointer;
	typedef Ref Reference;
	typedef size_t DifferenceType;

	Node* _node;


	__ListIterator(Node* x)
		:_node(x)
	{}

	//T& operator*()
	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		//return &_node->_data;
		return &(operator*());
	}

	bool operator != (const Self& s)
	{
		return _node != s._node;
	}

	bool operator==(const Self& s)
	{
		return _node == s->_node;
	}

	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}

	/*Self operator++(int)
	{
	_node = _node->_next;
	return _node->_prev;
	}*/

	Self operator++(int)
	{
		Self tmp(*this);
		_node = _node->_next;
		return tmp;
	}

	Self& operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp(*this);
		_node = _node->_prev;
		return tmp;
	}
};

template<class T, class alloc = __DefaultAllocTemplate<0, 0>>
class List
{
	typedef __ListNode<T> Node;
	typedef SimpleAlloc<Node, alloc> ListNodeAllocator;
public:
	typedef __ListIterator<T, T&, T*> Iterator;
	typedef __ListIterator<T, const T&, const T*> ConstIterator;

	typedef ReverseIterator<ConstIterator> ConstReverseIterator;
	typedef ReverseIterator<Iterator> ReverseIterator;


	Node* BuyNode(const T& x)
	{
		Node* node = ListNodeAllocator::Allocate();
		new(node)Node(x);
		return node;
	}

	void DestoryNode(Node* node)
	{
		node->~Node();
		ListNodeAllocator::Deallocate(node);
	}

	List()
	{
		_head = BuyNode(T());
		_head->_next = _head;
		_head->_prev = _head;
	}

	~List()
	{
		Clear();
		//delete _head;
		DestoryNode(_head);
		_head = NULL;
	}

	void Clear()
	{
		Iterator it = Begin();
		while (it != End())
		{
			Node* del = it._node;
			++it;
			//delete del;
			DestoryNode(del);
		}

		_head->_next = _head;
		_head->_prev = _head;
	}

	Iterator Begin()
	{
		return _head->_next;
	}

	Iterator End()
	{
		return _head;
	}

	ReverseIterator RBegin()
	{
		return ReverseIterator(End());
	}

	ReverseIterator REnd()
	{
		return ReverseIterator(Begin());
	}

	ConstIterator Begin() const
	{
		return _head->_next;
	}

	ConstIterator End() const
	{
		return _head;
	}

	void PushBack(const T& x)
	{
		Insert(End(), x);
	}

	void PushFront(const T& x)
	{
		Insert(Begin(), x)
	}

	void PopBack()
	{
		Erase(--End());
	}

	void PopFront()
	{
		Erase(Begin());
	}

	void Insert(Iterator pos, const T& x)
	{
		Node* cur = pos._node;
		Node* prev = cur->_prev;

		//Node* tmp = new Node(x);
		Node* tmp = BuyNode(x);

		prev->_next = tmp;
		tmp->_prev = prev;

		tmp->_next = cur;
		cur->_prev = tmp;
	}

	Iterator Erase(Iterator& pos)
	{
		//assert(pos != End());
		assert(pos._node != _head);

		Node* cur = pos._node;
		Node* prev = cur->_prev;
		Node* next = cur->_next;

		prev->_next = next;
		next->_prev = prev;
		//delete cur;
		DestoryNode(cur);

		pos = prev;

		return next;
	}

	void Assign(size_t n, const T& val)
	{
		Clear();
		for (size_t i = 0; i < n; ++i)
		{
			PushBack(val);
		}
	}

	template<class InputIterator>
	void Assign(InputIterator first, InputIterator last)
	{
		Clear();
		while (first != last)
		{
			PushBack(*first);
			++first;
		}
	}

protected:
	Node* _head;
};

struct AA
{
	int _a1;
	int _a2;
};

void PrintList(const List<int>& l)
{
	List<int>::ConstIterator it = l.Begin();
	while (it != l.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}

void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);

	PrintList(l);

	List<int>::ReverseIterator rit = l.RBegin();
	while (rit != l.REnd())
	{
		cout << *rit << " ";
		++rit;
	}
	cout << endl;
}
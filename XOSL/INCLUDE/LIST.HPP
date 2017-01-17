#ifndef ListHPP
#define ListHPP

#include <newdefs.h>

// Borland C++ 3.1 limitations:

// Borland C++ 3.1 issues some weird warning telling
// some non-inline function cannot be expanded as
// inline function ?!
//
// LIST.HPP 153: Functions containing for are not expanded inline
// LIST.HPP 174: Functions containing for are not expanded inline
#pragma warn -inl

// When TType is a pointer (e.g. CTest *), the compiler generates
// the following error:
//
// List.hpp 84: operator-> must return a pointer or a class
//
// Borland also compiles template functions that are not used,
// hence operator-> is not implemented. 



template <class TType>
class list {
public:
	class iterator;
	typedef TType &reference;
	typedef TType *pointer;
	typedef size_t size_type;

	list();
	~list();

private:
	// For now, specifically disable the 
	// copy constructor and assignment operator
	list(const list<TType> &);
	void operator =(const list<TType> &);

private:
	class CNode;
	typedef CNode *CNodePtr;
	friend class CNode;
	class CNode {
	public:
		inline CNode(){}
		inline CNode(const TType &Value){ this->Value = Value; }
		TType Value;
		CNodePtr Next;
		CNodePtr Prev;
	};

private:
	CNode ListFront;
	CNode ListBack;
	int ItemCount;

public:
	friend class iterator;
	class iterator {
		friend class list<TType>;
	public:
		inline iterator(){}

		void operator ++()
		{
			if (Current->Next)
				Current = Current->Next;
		}

		void operator --()
		{
			if (Current->Prev->Prev)
				Current = Current->Prev;
		}

		inline reference operator *()
		{
			return Current->Value;
		}

		inline bool operator ==(const iterator &It)
		{
			return Current == It.Current;
		}

		inline bool operator !=(const iterator &It)
		{
			return Current != It.Current;
		}

/*		inline pointer operator ->()
		{
			&Current->Value;
		}
*/

/*		inline pointer operator =()
		{
			return &Current->Value;
		}
*/

	private:
		CNodePtr Current;

		inline iterator(CNodePtr Node)
		{
			Current = Node;
		}
	};

public:
	// element access
	inline reference front()
	{
		return ListFront.Next->Value;
	}

	inline reference back()
	{
		return ListBack.Prev->Value;
	}

	// iterators
	inline iterator begin()
	{
		return ListFront.Next;
	}

    inline iterator end()
	{
		return &ListBack;
	}

	// capacity;
	inline bool empty() const
	{
		return ItemCount == 0;
	}

	inline size_type size () const
	{
		return ItemCount;
	}

	// modifiers
	inline void push_front(const TType &Value)
	{
		insert(begin(),Value);
	}
	
	inline void pop_front()
	{
		erase(begin());
	}

	inline void push_back(const TType &Value)
	{
		insert(end(),Value);
	}

	inline void pop_back()
	{
		erase(iterator(ListBack.Prev));
	}

	iterator insert(iterator Position, const TType &Value)
	{
		CNodePtr NewItem = new CNode(Value);

		NewItem->Next = Position.Current;
		NewItem->Prev = Position.Current->Prev;
		Position.Current->Prev->Next = NewItem;
		Position.Current->Prev = NewItem;
		++ItemCount;
		  return NewItem;
	}
	void insert(iterator Position, size_type Count, const TType &Value)
	{
		for (; Count--; insert(Position,Value));
	}

	iterator erase(iterator Position)
	{
		CNodePtr Next;

		if (Position == end()) {
			return Position;
		}

		Next = Position.Current->Next;
		Position.Current->Prev->Next = Position.Current->Next;
		Position.Current->Next->Prev = Position.Current->Prev;
		delete Position.Current;
		--ItemCount;
		return Next;
	}

	iterator erase(iterator Begin, iterator End)
	{
		for (; Begin != End; Begin = erase(Begin));
		return End;
	}

	inline void clear()
	{
		erase(begin(),end());
	}



};

template <class TType>
list<TType>::list()
{
	ListFront.Prev = NULL;
	ListFront.Next = &ListBack;
	ListBack.Prev = &ListFront;
	ListBack.Next = NULL;
	ItemCount = 0;
}

template <class TType>
list<TType>::~list()
{
	clear();
}

#endif

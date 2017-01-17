#ifndef AllocatedHPP
#define AllocatedHPP

#include <newdefs.h>

template<class TType>
inline TType *Allocate(int Count, TType *)
{
	if (Count < 0)
		Count = 0;
	return new TType[Count];
}

template<class _T1, class _T2>
inline void _Construct(_T1 *_P, const _T2& _V)
{
	new ((void *)_P) _T1(_V);
}



template<class TType> class allocator {
public:
	typedef size_t size_type;
	typedef long difference_type;
	typedef TType *pointer;
	typedef const TType *const_pointer;
	typedef TType &reference;
	typedef const TType &const_reference;
	typedef TType value_type;

	inline pointer address(reference Value) const
	{
		return (&Value);
	}
	
	inline const_pointer address(const_reference Value) const
	{
		return (&Value);
	}
	
	pointer allocate(size_type Count, const void *)
	{
		return Allocate((difference_type)Count, (pointer)NULL); 
	}
	
	char  *_Charalloc(size_type Count)
	{
		return Allocate((difference_type)Count,(char  *)NULL);
	}
	
	void deallocate(void *Pointer, size_type)
	{
		delete Pointer;
	}
	
	void construct(pointer Pointer, const TType& _V)
		{_Construct(_P, _V); }
	
	void destroy(pointer _P)
		{_Destroy(_P); }
	
	size_t max_size() const
		{size_t _N = (size_t)(-1) / sizeof (TType);
		return (0 < _N ? _N : 1); }
	};

#endif

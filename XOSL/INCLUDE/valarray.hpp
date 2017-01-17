#ifndef valarrayHPP
#define valarrayHPP

#include <newdefs.h>

template<class TValue>
class valarray {
public:
	valarray(size_t Size = 0);
	valarray(const TValue &Value, size_t Size);
	valarray(const TValue *ValueArray, size_t Size);
	valarray(const valarray<TValue> &ValArray);
	~valarray();

	valarray<TValue> &operator =(const valarray<TValue> &ValArray);
	valarray<TValue> &operator =(const TValue &Value);
	
	void resize(size_t NewSize, const TValue &FillValue = TValue());
	inline size_t size() const;
	
	inline TValue operator [](size_t Index) const;
	inline TValue &operator [](size_t Index);
	
	TValue sum() const;
	TValue min() const;
	TValue max() const;
	
	void free();

private:
	void Grow(size_t Size, const TValue *Source = NULL, size_t Distance = 0, bool Trim = false);
	void Tidy(bool Constructed = false);

	TValue *Array;
	size_t Length, Reserved;
};




template<class TValue>
valarray<TValue>::valarray(size_t Size = 0)
{
	Tidy();
	Res = Size;
	Grow(Size); 
}

template<class TValue>
valarray<TValue>::valarray(const TValue &Value, size_t Size)
{
	Tidy();
	Grow(Size, &Value);
}

template<class TValue>
valarray<TValue>::valarray(const TValue *ValueArray, size_t Size)
{
	Tidy();
	Grow(Size, ValueArray, 1);
}

template<class TValue>
valarray<TValue>::valarray(const valarray<TValue> &ValArray)
{
	Tidy();
	Grow(ValArray.size(), ValArray.Array, 1);
}

template<class TValue>
valarray<TValue>::~valarray()
{
	Tidy(true);
}
	
template<class TValue>
valarray<TValue> &valarray<TValue>::operator =(const valarray<TValue> &ValArray)
{
	if (this == &ValArray)
		return (*this);
	Grow(ValArray.size(), 0, 0, true);
	for (size_t Index = 0; Index < size(); ++Index) 
		Array[Index] = ValArray[Index];
	return *this;
}


template<class TValue>
valarray<TValue> &valarray<TValue>::operator =(const TValue &Value)
{
	for (size_t Index = 0; Index < size(); ++Index) 
		Array[Index] = Value;
	return *this;

}
	
template<class TValue>
void valarray<TValue>::resize(size_t NewSize, const TValue &FillValue = TValue())
{
	Grow(NewSize, &FillValue, 0, true);
}
	
template<class TValue>
inline size_t valarray<TValue>::size() const
{
	return Length; 
}
	
template<class TValue>
inline TValue valarray<TValue>::operator [](size_t Index) const
{
	return (Array[Index]);
} 

template<class TValue>
inline TValue &valarray<TValue>::operator [](size_t Index)
{
	return (Array[Index]);
}
	

template<class TValue>
TValue valarray<TValue>::sum() const
{
	TValue Sum = Array[0];

	for (size_t Index = 1; Index < size(); ++Index)
		Sum += Array[Index];
	return Sum;
}

	
template<class TValue>
TValue valarray<TValue>::min() const
{
	TValue Min = Array[0];

	for (size_t Index = 1; Index < size(); ++Index)
		if (Min > Array[Index])
			Min = Array[Index];
	return Min;
}

template<class TValue>
TValue valarray<TValue>::max() const
{
	TValue Max = Array[0];

	for (size_t Index = 1; Index < size(); ++Index)
		if (Max < Array[Index])
			Max = Array[Index];
	return Max; 
}
	
	
template<class TValue>
void valarray<TValue>::free()
{
	Tidy(true); 
}

template<class TValue>
void valarray<TValue>::Grow(size_t Size, const TValue *Source = NULL, size_t Distance = 0, bool Trim = false)
{
	size_t OldSize = !Array ? 0 : Reserved;

	if (!Size) {
		if (Trim)
			Tidy(true); 
	}
	else if (Size == OldSize || Size < OldSize && !Trim)
		;
	else {
		size_t Index;
		size_t AllocSize = !Array && Size < Reserved ? Reserved : Size;

		TValue *NewArray = new TValue[AllocSize];
		size_t NewSize = Size < Length ? Size : Length;
			
		for (Index = 0; Index < NewSize; ++Index)
			NewArray[Index] = Array[Index];
			
		if (Source)
			for (; Index < AllocSize; ++Index, Source += Distance)
				NewArray[Index] = *Source;

		Tidy(true);
		Array = NewArray;
		Reserved = AllocSize; 
	}
		
	Length = Size; 
}
	

template<class TValue>
void valarray<TValue>::Tidy(bool Constructed = 0)
{
	if (Constructed && Array)
		delete[] Array;
	Length = 0;
	Array = 0;
	Reserved = 0; 
}


#endif
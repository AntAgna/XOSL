#ifndef AlgorithHPP
#define AlgorithHPP

template <class InputIterator, class TKey>
inline InputIterator find(InputIterator first, InputIterator last, /*const*/ TKey& value)
{
	for (; first != last && *first != value; ++first);
	return first;
}


#endif
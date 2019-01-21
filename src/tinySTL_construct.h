#ifndef __TINYSTL_CONSTRUCT_H
#define __TINYSTL_CONSTRUCT_H

#include <new>
#include "tinySTL_iterator.h"
#include "tinySTL_type_traits.h"

namespace TinySTL {

	/*
		构造和析构的基本工具
	*/

	template <typename T1, typename T2>
	inline void construct(T1 *p, const T2& value)
	{
		// 使用placement new，调用T1::T1(value)
		new (p) T1(value);
	}

	template <typename T>
	inline void destroy(T *pointer)
	{
		// 调用析构函数
		pointer->~T();
	}

	// 有non-trivial destructor时的处理
	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
	{
		for (; first < last; ++first)
			destroy(&*first);	// 调用析构函数
	}

	// 有trivial destructor时的处理
	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
	{
		// 不用调用析构函数
	}

	// 根据迭代器的value type是否有trivial destructor分别处理
	template <typename ForwardIterator, typename T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	template <typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		__destroy(first, last, value_type(first));
	}



}	// end of namespace TinySTL

#endif // !__TINYSTL_CONSTRUCT_H


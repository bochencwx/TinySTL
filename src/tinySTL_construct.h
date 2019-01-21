#ifndef __TINYSTL_CONSTRUCT_H
#define __TINYSTL_CONSTRUCT_H

#include <new>
#include "tinySTL_iterator.h"
#include "tinySTL_type_traits.h"

namespace TinySTL {

	/*
		����������Ļ�������
	*/

	template <typename T1, typename T2>
	inline void construct(T1 *p, const T2& value)
	{
		// ʹ��placement new������T1::T1(value)
		new (p) T1(value);
	}

	template <typename T>
	inline void destroy(T *pointer)
	{
		// ������������
		pointer->~T();
	}

	// ��non-trivial destructorʱ�Ĵ���
	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
	{
		for (; first < last; ++first)
			destroy(&*first);	// ������������
	}

	// ��trivial destructorʱ�Ĵ���
	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
	{
		// ���õ�����������
	}

	// ���ݵ�������value type�Ƿ���trivial destructor�ֱ���
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


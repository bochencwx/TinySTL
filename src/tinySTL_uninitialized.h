#ifndef __TINYSTL_UNINITIALIZED_H
#define __TINYSTL_UNINITIALIZED_H

#include "tinySTL_iterator.h"
#include "tinySTL_type_traits.h"
#include "tinySTL_construct.h"

namespace TinySTL {

	/*
		�ڴ洦���������
	*/

	// PODʱ��fill_n����
	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, true_type)
	{
		// fill_n�㷨
		for (; n > 0; --n, ++first)
			*first = x;
		return first;
	}

	// ��PODʱ��fill_n����
	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, false_type)
	{
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
			construct(&*cur, x);
		return cur;
	}

	// �����Ƿ�ΪPOD�ֱ���
	template <typename ForwardIterator, typename Size, typename T, typename U>
	ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, U*)
	{
		typedef typename type_traits<U>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

	// PODʱ��copy����
	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, true_type)
	{
		// ֮��ʵ����copy�㷨����ʹ��copy�㷨
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
	}

	// ��PODʱ��copy����
	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, false_type)
	{
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
	}

	// �����Ƿ�ΪPOD�ֱ���
	template <typename InputIterator, typename ForwardIterator,typename T>
	ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
	{
		typedef typename type_traits<T>::is_POD_type is_POD;
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return __uninitialized_copy(first, last, result, value_type(result));
	}

	// PODʱ��fill����
	template <typename ForwardIterator, typename T>
	ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, true_type)
	{
		// ֮��ʵ����copy�㷨����ʹ��fill�㷨
		ForwardIterator cur = first;
		for (; cur != last; ++cur)
			construct(&*cur, x);
	}

	// ��PODʱ��fill����
	template <typename ForwardIterator, typename T>
	void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, false_type)
	{
		ForwardIterator cur = first;
		for (; cur != last; ++cur)
			construct(&*cur, x);
	}

	// �����Ƿ�ΪPOD�ֱ���
	template <typename ForwardIterator, typename T, typename T1>
	void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
	{
		typedef typename type_traits<T1>::is_POD_type is_POD;
		__uninitialized_copy_aux(first, last, x, is_POD());
	}

	template <typename ForwardIterator, typename T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
	{
		__uninitialized_copy(first, last, x, value_type(first));
	}

}	// end of namespace TinySTL

#endif // !__TINYSTL_UNINITIALIZED_H


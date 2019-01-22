#ifndef __TINYSTL_ALGORITHM_H
#define __TINYSTL_ALGORITHM_H

#include <string>
#include "tinySTL_iterator.h"
#include "tinySTL_type_traits.h"

namespace TinySTL
{

	/*
		一些基础的算法
	*/

	// 将[first, last)内的所有元素改填新值
	template <typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; ++first)
			*first = value;
	}

	// 将[first, first+n)内的所有元素改填新值
	template <typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}

/***************************************************    copy    ********************************************************/

	// 迭代器是非自由处理迭代器情况下的操作
	template <typename InputIterator, typename OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		// 以迭代器等同与否，决定循环是否继续。速度慢
		for (; first != last; ++result, ++first)
			*result = *first;
		return result;
	}

	// 迭代器是自由处理迭代器情况下的复制操作
	template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
	OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
	{
		// 自由处理迭代器可以计算两个迭代器之间的距离，以n决定循环的执行次数。速度较快
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
			*result = *first;	// 赋值运算符
		return result;
	}

	template <typename RandomAccessIterator, typename OutputIterator>
	OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d(first, last, result, distance_type(first));
	}

	// 第二层泛化，迭代器为class类型时的copy操作
	template <typename InputIterator, typename OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			// 根据迭代器类型选择copy操作
			return __copy(first, last, result, iterator_category(first));
		}
	};

	// 指针迭代器所指对象具有 trivial assignment operator时的copy操作
	template <typename T>
	T* __copy_t(const T* first, const T* last, T* result, true_type)
	{
		memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}

	// 指针迭代器所指对象具有 non-trivial assignment operator时的copy操作
	template <typename T>
	T* __copy_t(const T* first, const T* last, T* result, false_type)
	{
		// 原生指针是一种自由处理迭代器
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	// 第二层偏特化，迭代器为原生指针情况下的copy操作
	template <typename T>
	struct __copy_dispatch<T*, T*>
	{
		T* operator()(T* first, T* last, T* result)
		{
			// 根据是否拥有trivial assignment operator进行不同处理
			typedef typename type_traits<T>::has_trivial_assignment_constructor has_trivial_assignment;
			return __copy_t(first, last, result, has_trivial_assignment());
		}
	};

	// 第二层偏特化，迭代器为原生const指针情况下的copy操作
	template<typename T>
	struct __copy_dispatch<const T*, T*>
	{
		T* operator()(const T* first, const T* last, T* result)
		{
			// 根据是否拥有trivial assignment operator进行不同处理
			typedef typename type_traits<T>::has_trivial_assignment_constructor has_trivial_assignment;
			return __copy_t(first, last, result, has_trivial_assignment());
		}
	};

	// 第一层泛化，非char*、wchar*迭代器的copy操作
	template <typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		// 调用__copy_dispatch的operator()函数，根据迭代器是类还是原生指针进行不同处理
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}
/*
	// 第一层特化：迭代器为char*类型时的copy操作
	template<>
	char* copy(const char* first, const char* last, char* result)
	{
		// 直接进行内存拷贝操作。速度最快
		memmove(result, first, last - first);
		return result + (last - first);
	}

	template<>
	// 第一层特化：迭代器为wchar_t*类型时的copy操作
	wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		// 直接进行内存拷贝操作。速度最快
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}
*/

/***************************************************    copy    ********************************************************/

/***********************************************    copy_backward    ********************************************************/

	template<typename InputIterator, typename OutputIterator, typename Distance>
	OutputIterator __copy_d_backward(InputIterator first, InputIterator last, OutputIterator result, Distance*)
	{
		Distance n = last - first;
		for (; n>0; --n)
		{
			*(result - 1) = *(last - 1);
			--result;
			--last;
		}
		return result;
	};


	template<typename InputIterator, typename OutputIterator>
	OutputIterator __copy_t_backward(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		while (last != first)
		{
			*(result - 1) = *(last - 1);
			--last;
			--result;
		}
		return result;
	};

	template<typename InputIterator, typename OutputIterator>
	OutputIterator __copy_t_backward(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d_backward(first, last, result, difference_type(first));
	};

	template<typename InputIterator, typename OutputIterator>
	struct __copy_dispatch_backward
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			return __copy_t_backward(first, last, result, iterator_category(first));
		}
	};

	template<typename T>
	struct __copy_dispatch_backward<T*, T*>
	{
		T* operator()(T* first, T* last, T* result)
		{
			return __copy_d_backward(first, last, result, (ptrdiff_t*)(0));
		}
	};

	template<typename T>
	struct __copy_dispatch_backward<const T*, T*>
	{
		T* operator()(const T* first, const T* last, T* result)
		{
			return __copy_d_backward(first, last, result, (ptrdiff_t*)(0));
		}
	};

	template<typename InputIterator, typename OutputIterator>
	OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result)
	{
		return	__copy_dispatch_backward<InputIterator, OutputIterator>()(first, last, result);
	}

/***********************************************    copy_backward    ********************************************************/

}	// end of namespace TinySTL

#endif // !__TINYSTL_ALGORITHM_H


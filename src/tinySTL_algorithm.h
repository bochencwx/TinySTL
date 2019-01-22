#ifndef __TINYSTL_ALGORITHM_H
#define __TINYSTL_ALGORITHM_H

#include <string>
#include "tinySTL_iterator.h"
#include "tinySTL_type_traits.h"

namespace TinySTL
{

	/*
		һЩ�������㷨
	*/

	// ��[first, last)�ڵ�����Ԫ�ظ�����ֵ
	template <typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		for (; first != last; ++first)
			*first = value;
	}

	// ��[first, first+n)�ڵ�����Ԫ�ظ�����ֵ
	template <typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}

/***************************************************    copy    ********************************************************/

	// �������Ƿ����ɴ������������µĲ���
	template <typename InputIterator, typename OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		// �Ե�������ͬ��񣬾���ѭ���Ƿ�������ٶ���
		for (; first != last; ++result, ++first)
			*result = *first;
		return result;
	}

	// �����������ɴ������������µĸ��Ʋ���
	template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
	OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
	{
		// ���ɴ�����������Լ�������������֮��ľ��룬��n����ѭ����ִ�д������ٶȽϿ�
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
			*result = *first;	// ��ֵ�����
		return result;
	}

	template <typename RandomAccessIterator, typename OutputIterator>
	OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d(first, last, result, distance_type(first));
	}

	// �ڶ��㷺����������Ϊclass����ʱ��copy����
	template <typename InputIterator, typename OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			// ���ݵ���������ѡ��copy����
			return __copy(first, last, result, iterator_category(first));
		}
	};

	// ָ���������ָ������� trivial assignment operatorʱ��copy����
	template <typename T>
	T* __copy_t(const T* first, const T* last, T* result, true_type)
	{
		memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}

	// ָ���������ָ������� non-trivial assignment operatorʱ��copy����
	template <typename T>
	T* __copy_t(const T* first, const T* last, T* result, false_type)
	{
		// ԭ��ָ����һ�����ɴ��������
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	// �ڶ���ƫ�ػ���������Ϊԭ��ָ������µ�copy����
	template <typename T>
	struct __copy_dispatch<T*, T*>
	{
		T* operator()(T* first, T* last, T* result)
		{
			// �����Ƿ�ӵ��trivial assignment operator���в�ͬ����
			typedef typename type_traits<T>::has_trivial_assignment_constructor has_trivial_assignment;
			return __copy_t(first, last, result, has_trivial_assignment());
		}
	};

	// �ڶ���ƫ�ػ���������Ϊԭ��constָ������µ�copy����
	template<typename T>
	struct __copy_dispatch<const T*, T*>
	{
		T* operator()(const T* first, const T* last, T* result)
		{
			// �����Ƿ�ӵ��trivial assignment operator���в�ͬ����
			typedef typename type_traits<T>::has_trivial_assignment_constructor has_trivial_assignment;
			return __copy_t(first, last, result, has_trivial_assignment());
		}
	};

	// ��һ�㷺������char*��wchar*��������copy����
	template <typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		// ����__copy_dispatch��operator()���������ݵ��������໹��ԭ��ָ����в�ͬ����
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}
/*
	// ��һ���ػ���������Ϊchar*����ʱ��copy����
	template<>
	char* copy(const char* first, const char* last, char* result)
	{
		// ֱ�ӽ����ڴ濽���������ٶ����
		memmove(result, first, last - first);
		return result + (last - first);
	}

	template<>
	// ��һ���ػ���������Ϊwchar_t*����ʱ��copy����
	wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		// ֱ�ӽ����ڴ濽���������ٶ����
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


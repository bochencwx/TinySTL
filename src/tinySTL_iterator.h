#ifndef __TINYSTL_ITERATOR_H
#define __TINYSTL_ITERATOR_H

#include <cstddef>

namespace TinySTL {

	/*
		���ֵ���������
	*/
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };

	/*
		�������淶
	*/
	template <typename Category, 
				typename T,
				typename Distance = ptrdiff_t,
				typename Pointer = T*,
				typename Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	/*
		������������ȡ��
	*/
	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	// ���ԭ��ָ���ƫ�ػ���
	template <typename T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	// ���ԭ����constָ���ƫ�ػ���
	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	// ����ĳ�������������ͣ�����һ���õ���������ʵ����������һ������
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// ����ĳ����������distance type�����ظ����͵�ָ�룬������һ������
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	// ����ĳ����������value type�����ظ����͵�ָ�룬������һ������
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

}	// end of namespace TinySTL

#endif // !__TINYSTL_ITERATOR_H


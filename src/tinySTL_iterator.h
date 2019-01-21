#ifndef __TINYSTL_ITERATOR_H
#define __TINYSTL_ITERATOR_H

#include <cstddef>

namespace TinySTL {

	/*
		五种迭代器类型
	*/
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };

	/*
		迭代器规范
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
		迭代器属性萃取类
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

	// 针对原生指针的偏特化版
	template <typename T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	// 针对原生的const指针的偏特化版
	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	// 决定某个迭代器的类型，返回一个该迭代器类型实例，而不是一个类型
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// 决定某个迭代器的distance type，返回该类型的指针，而不是一个类型
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	// 决定某个迭代器的value type，返回该类型的指针，而不是一个类型
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

}	// end of namespace TinySTL

#endif // !__TINYSTL_ITERATOR_H


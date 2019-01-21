#ifndef __TINYSTL_TYPE_TRAITS_H
#define __TINYSTL_TYPE_TRAITS_H

namespace TinySTL {

	struct true_type { };
	struct false_type { };

	/*
		萃取型别特性，需要对确切的类型进行特化声明，不然都按false_type处理
	*/
	template <typename type>
	struct type_traits
	{
		typedef false_type has_trivial_default_constructor;		// 平凡的默认构造函数（什么都不干），默认不是
		typedef false_type has_trivial_copy_constructor;			// 平凡的复制构造函数，默认不是
		typedef false_type has_trivial_assignment_constructor;	// 平凡的移动构造函数，默认不是
		typedef false_type has_trivial_destructor;				// 平凡的析构函数，默认不是
		typedef false_type is_POD_type;							// POD类型，默认不是
	};

	template<>
	struct type_traits<char>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<signed char>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<unsigned char>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<short>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<unsigned short>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<int>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<unsigned int>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<long>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<unsigned long>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<float>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<double>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template<>
	struct type_traits<long double>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

	template <typename T>
	struct type_traits<T*>
	{
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_constructor;
		typedef true_type has_trivial_assignment_constructor;
		typedef true_type has_trivial_destructor;
		typedef true_type is_POD_type;
	};

}	// end of namespace TinySTL

#endif // !__TINYSTL_TYPE_TRAITS_H


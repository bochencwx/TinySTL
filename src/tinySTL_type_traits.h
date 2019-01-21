#ifndef __TINYSTL_TYPE_TRAITS_H
#define __TINYSTL_TYPE_TRAITS_H

namespace TinySTL {

	struct true_type { };
	struct false_type { };

	/*
		��ȡ�ͱ����ԣ���Ҫ��ȷ�е����ͽ����ػ���������Ȼ����false_type����
	*/
	template <typename type>
	struct type_traits
	{
		typedef false_type has_trivial_default_constructor;		// ƽ����Ĭ�Ϲ��캯����ʲô�����ɣ���Ĭ�ϲ���
		typedef false_type has_trivial_copy_constructor;			// ƽ���ĸ��ƹ��캯����Ĭ�ϲ���
		typedef false_type has_trivial_assignment_constructor;	// ƽ�����ƶ����캯����Ĭ�ϲ���
		typedef false_type has_trivial_destructor;				// ƽ��������������Ĭ�ϲ���
		typedef false_type is_POD_type;							// POD���ͣ�Ĭ�ϲ���
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


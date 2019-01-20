#ifndef __TINYSTL_ALLOC_H
#define __TINYSTL_ALLOC_H

#include <stdlib.h>

namespace TinySTL
{

	/*
		SGI�ĵ�һ���ռ�������
		1�����ֽ�Ϊ��λ����
		2��ֻ�Ƕ�C���ڴ��������ͷź������м򵥷�װ
		3��ʵ��������C++��set_new_handler()�ڴ治�㴦�����
	*/
	class __malloc_alloc
	{
	private:
		/*		�ڴ����ʧ�ܴ�����		*/	
		static void(*__malloc_alloc_oom_handler)();
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *p, size_t);

	public:
		static void *allocate(size_t n)
		{
			void *result = malloc(n);
			if (result == 0)
				result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);
		}

		static void *reallocate(void *p, size_t, size_t new_sz)
		{
			void *result = realloc(p, new_sz);
			if (result == 0)
				result = oom_realloc(p, new_sz);
			return result;
		}

		// �����ڴ治�㴦����
		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old_handler)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old_handler;
		}
	};
	
	typedef __malloc_alloc malloc_alloc;

	enum {__ALIGN = 8};		// С��ȡ����ϵ��߽�
	enum {__MAX_BYTES = 128};	// С�����������
	enum {__NFREELISTS = __MAX_BYTES / __ALIGN};	// free-lists�ĸ���

	/*
		SGI�ĵڶ����ռ�������
		1�����ֽ�Ϊ��λ����
		2�������ڴ�ط�ʽ����С�����������Լ����ڴ���Ƭ������������ʹ�õ�һ��������
	*/
	class alloc
	{
	private:
		/*
			������
			�ڻ�δ�Ѹò����ڴ������û�ʱ������ָ����һ���ڵ��ָ�루free_list_link����������������
			�ڰѸò����ڴ������û�ʱ������ָ��ò����ڴ��׵�ַ��ָ�루client_data��
		*/
		union obj
		{
			union obj *free_list_link;
			char client_data[1];
		};

	private:
		// ��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t  bytes)
		{
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}
		// ����bytes��С������Ӧʹ�õ�free-list�±�
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}
		// ����һ����СΪn��obj�����ڴ棬�����ܼ����СΪn���������鵽free list
		static void *refill(size_t n);
		// ����һ���ռ䣬������nobjs����СΪ��size�������飻���û������nobjs��nobjs���ܻή��
		static char *chunk_alloc(size_t size, int &nobjs);

	private:
		static obj * free_list[__NFREELISTS];	// __NFREELISTS��free-lists��ÿһ��ָ��һ��objָ�룬Ϊ��free-list��ͷ��
		static char *start_free;	// �ڴ����ʼλ�ã�ֻ��chunk_alloc()�б仯
		static char *end_free;		// �ڴ�ؽ���λ�ã�ֻ��chunk_alloc()�б仯
		static size_t heap_size;

	public:
		static void *allocate(size_t);
		static void deallocate(void *, size_t);
		static void *reallocate(void *, size_t, size_t);
	};

	/*
		����STL���Ľӿ�
	*/
	template<typename T, typename Alloc>
	class simple_alloc
	{
	public:
		static T* allocate(size_t n)
		{
			return 0 == n ? 0 : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
		}
		static T* allocate()
		{
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		static void deallocate(T* p, size_t n)
		{
			if (0 != n)
			{
				Alloc::deallocate(p, sizeof(T)*n);
			}
		}
		static void deallocate(T* p)
		{
			Alloc::deallocate(p, sizeof(T));
		}
	};

}	// end of namespace TinySTL

#endif // !__TINYSTL_ALLOC_H


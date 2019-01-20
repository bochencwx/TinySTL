#ifndef __TINYSTL_ALLOC_H
#define __TINYSTL_ALLOC_H

#include <stdlib.h>

namespace TinySTL
{

	/*
		SGI的第一级空间配置器
		1、以字节为单位配置
		2、只是对C的内存配置与释放函数进行简单封装
		3、实现了类似C++的set_new_handler()内存不足处理机制
	*/
	class __malloc_alloc
	{
	private:
		/*		内存分配失败处理函数		*/	
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

		// 设置内存不足处理函数
		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old_handler)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old_handler;
		}
	};
	
	typedef __malloc_alloc malloc_alloc;

	enum {__ALIGN = 8};		// 小型取块的上调边界
	enum {__MAX_BYTES = 128};	// 小型区块的上限
	enum {__NFREELISTS = __MAX_BYTES / __ALIGN};	// free-lists的个数

	/*
		SGI的第二级空间配置器
		1、以字节为单位配置
		2、利用内存池方式管理小型区块需求，以减少内存碎片；大区块需求使用第一级配置器
	*/
	class alloc
	{
	private:
		/*
			联合体
			在还未把该部分内存分配给用户时，保存指向下一个节点的指针（free_list_link），构成自由链表
			在把该部分内存分配给用户时，保存指向该部分内存首地址的指针（client_data）
		*/
		union obj
		{
			union obj *free_list_link;
			char client_data[1];
		};

	private:
		// 将bytes上调至8的倍数
		static size_t ROUND_UP(size_t  bytes)
		{
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}
		// 返回bytes大小的区块应使用的free-list下标
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes + __ALIGN - 1) / __ALIGN - 1);
		}
		// 返回一个大小为n的obj对象内存，并可能加入大小为n的其他区块到free list
		static void *refill(size_t n);
		// 配置一块大空间，可容纳nobjs个大小为“size”的区块；如果没法配置nobjs，nobjs可能会降低
		static char *chunk_alloc(size_t size, int &nobjs);

	private:
		static obj * free_list[__NFREELISTS];	// __NFREELISTS个free-lists，每一个指向一个obj指针，为该free-list的头部
		static char *start_free;	// 内存池起始位置，只在chunk_alloc()中变化
		static char *end_free;		// 内存池结束位置，只在chunk_alloc()中变化
		static size_t heap_size;

	public:
		static void *allocate(size_t);
		static void deallocate(void *, size_t);
		static void *reallocate(void *, size_t, size_t);
	};

	/*
		符合STL规格的接口
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


#include "tinySTL_alloc.h"

#include <iostream>

#define __THROW_BAD_ALLOC	\
	do {	\
		std::cerr << "out of memory" << std::endl;	\
		exit(1);	\
	}while(0)

void(* TinySTL::__malloc_alloc::__malloc_alloc_oom_handler)() = 0;

void * TinySTL::__malloc_alloc::oom_malloc(size_t n)
{
	void(*handler)();
	void *result;

	// 不断尝试内存不足处理、配置...
	for (;;)
	{
		handler = __malloc_alloc_oom_handler;	// 在循环中判断内存不足处理函数，可能会有某种途径更改该函数
		if (handler == 0)
			__THROW_BAD_ALLOC;
		(*handler)();
		result = malloc(n);
		if (result)
			return result;
	}
}

void * TinySTL::__malloc_alloc::oom_realloc(void *p, size_t n)
{
	void(*handler)();
	void *result;

	// 不断尝试内存不足处理、配置...
	for (;;)
	{
		handler = __malloc_alloc_oom_handler;	// 在循环中判断内存不足处理函数，可能会有某种途径更改该函数
		if (handler == 0)
			__THROW_BAD_ALLOC;
		result = realloc(p, n);
		if (result)
			return result;
	}
}

char *TinySTL::alloc::start_free = 0;

char *TinySTL::alloc::end_free = 0;

size_t TinySTL::alloc::heap_size = 0;

TinySTL::alloc::obj * TinySTL::alloc::free_list[TinySTL::__NFREELISTS] =
{ 0, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

// 从自由链表中配置内存
void * TinySTL::alloc::allocate(size_t n)
{
	obj **my_free_list;		// 要使用的free-list地址
	obj *result;			// 要使用的free-list

	// 大于128bytes，调用第一级配置器
	if (n > (size_t)TinySTL::__MAX_BYTES)
	{
		return malloc_alloc::allocate(n);
	}

	// 找到适当的free list
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0)	// 没找到可以的free list（该free list为空），重新填充free list
	{
		// 重新填充free list，*my_free_list在函数中调整，返回配置内存的首地址
		void *r = refill(ROUND_UP(n));
		return r;
	}

	// result作为配置给用户的内存返回，*my_free_list指向下一个可用位置
	*my_free_list = result->free_list_link;
	return result;
}

// 将内存回收到自由链表中
void TinySTL::alloc::deallocate(void *p, size_t n)
{
	obj *q = (obj *)p;	// 将回收位置的指针强制转换为obj指针，回收到适当的free list
	obj **my_free_list;	// 要使用的free-list地址

	// 大于128bytes，调用第一级配置器回收内存
	if (n > (size_t)TinySTL::__MAX_BYTES)
	{
		malloc_alloc::deallocate(p, n);
		return;
	}

	// 找到适当的free list
	my_free_list = free_list + FREELIST_INDEX(n);
	// 调整free list，回收区块，回收区块成为链表头
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

void * TinySTL::alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
{
	if (old_sz > (size_t)TinySTL::__MAX_BYTES && new_sz > (size_t)TinySTL::__MAX_BYTES)	// 原内存和新内存都大于128
	{
		// 直接使用realloc调整
		return realloc(p, new_sz);
	}
	if (ROUND_UP(old_sz) == ROUND_UP(new_sz))	// 原内存和新内存属于同大小区块，不需调整
	{
		return p;
	}

	/*		
		剩余情况：原内存和新内存都小于等于128，一个大于128一个小于等于128
		这两种情况原内存和新内存都指向不同的内存空间
		需要分配新内存，把旧内存内容复制到新内存，释放旧内存内容
	*/
	// 分配新内存
	void *result = allocate(new_sz);
	size_t copy_sz = old_sz < new_sz ? old_sz : new_sz;
	memcpy(result, p, copy_sz);
	deallocate(p, old_sz);
	return result;
}

void * TinySTL::alloc::refill(size_t n)
{
	// 默认情况下尝试取得20个区块作为free list的新节点，但实际得到的区块可能少于20个，nobjs保存最终得到的区块个数
	int nobjs = 20;
	char *chunk = chunk_alloc(n, nobjs);

	obj **my_free_list;	
	obj *result;
	obj *current_obj, *next_obj;
	int i;

	if (nobjs == 1)		// 只获得一个区块，直接把该区块返回，*my_free_list仍为空
	{
		return chunk;
	}

	// 调整free list，容纳新的节点
	my_free_list = free_list + FREELIST_INDEX(n);
	// 记录要返回的结果
	result = (obj*)chunk;
	// 第一个obj是要给用户的，free list应从第二个节点开始
	*my_free_list = next_obj = (obj *)(chunk + n);
	// 把新节点串接起来
	for(i = 1; ; i++)
	{
		current_obj = next_obj;
		next_obj = (obj *)((char *)next_obj + n);
		if (i == nobjs - 1)
		{
			current_obj->free_list_link = 0;
			break;
		}
		else
		{
			current_obj->free_list_link = next_obj;
		}
	}
	return result;
}

char* TinySTL::alloc::chunk_alloc(size_t size, int& nobjs)
{
	char *result;
	size_t total_bytes = size * nobjs;	// 期望分配的总字节数
	size_t bytes_left = end_free - start_free;	// 内存池剩余空间

	if (bytes_left >= total_bytes)	// 内存池剩余空间完全满足需求
	{
		// 直接从内存池中拨出
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size)	// 内存池剩余空间不能完全需求满足，但可以分配一个或一个以上的区块
	{
		// 从内存池中拨出剩余空间中可以分配的区块
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else	// 内存池剩余空间中连一个区块大小都无法提供
	{
		// 申请新内存
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0)		// 充分利用内存池剩余空间
		{
			// 找到内存剩余空间可以分配最大区块对应的free-list，把剩余空间编入该链表
			obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj *)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj *)start_free;
		}

		// 向堆空间申请内存
		start_free = (char *)malloc(bytes_to_get);
		if (start_free == 0)	// heap空间不足
		{
			int i;
			obj **my_free_list;
			obj *p;
			// 更大区块的free list中是否有剩余区块，可以取一个来用
			for (i = size; i <= TinySTL::__MAX_BYTES; i += TinySTL::__ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0)
				{
					// 该free list第一个节点内存回收到内存池
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					// 递归调用自己，为了修正nobjs
					return chunk_alloc(size, nobjs);
				}
			}

			// 至此说明到处都没有内存可用
			end_free = 0;
			// 调用第一级配置器，其有内存不足处理程序，看是否能够得到内存
			start_free = (char *)malloc_alloc::allocate(bytes_to_get);
		}
		// 申请内存成功
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		// 递归调用自己，为了修正nobjs
		return chunk_alloc(size, nobjs);
	}
}

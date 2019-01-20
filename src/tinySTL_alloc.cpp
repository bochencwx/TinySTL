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

	// ���ϳ����ڴ治�㴦������...
	for (;;)
	{
		handler = __malloc_alloc_oom_handler;	// ��ѭ�����ж��ڴ治�㴦���������ܻ���ĳ��;�����ĸú���
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

	// ���ϳ����ڴ治�㴦������...
	for (;;)
	{
		handler = __malloc_alloc_oom_handler;	// ��ѭ�����ж��ڴ治�㴦���������ܻ���ĳ��;�����ĸú���
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

// �����������������ڴ�
void * TinySTL::alloc::allocate(size_t n)
{
	obj **my_free_list;		// Ҫʹ�õ�free-list��ַ
	obj *result;			// Ҫʹ�õ�free-list

	// ����128bytes�����õ�һ��������
	if (n > (size_t)TinySTL::__MAX_BYTES)
	{
		return malloc_alloc::allocate(n);
	}

	// �ҵ��ʵ���free list
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0)	// û�ҵ����Ե�free list����free listΪ�գ����������free list
	{
		// �������free list��*my_free_list�ں����е��������������ڴ���׵�ַ
		void *r = refill(ROUND_UP(n));
		return r;
	}

	// result��Ϊ���ø��û����ڴ淵�أ�*my_free_listָ����һ������λ��
	*my_free_list = result->free_list_link;
	return result;
}

// ���ڴ���յ�����������
void TinySTL::alloc::deallocate(void *p, size_t n)
{
	obj *q = (obj *)p;	// ������λ�õ�ָ��ǿ��ת��Ϊobjָ�룬���յ��ʵ���free list
	obj **my_free_list;	// Ҫʹ�õ�free-list��ַ

	// ����128bytes�����õ�һ�������������ڴ�
	if (n > (size_t)TinySTL::__MAX_BYTES)
	{
		malloc_alloc::deallocate(p, n);
		return;
	}

	// �ҵ��ʵ���free list
	my_free_list = free_list + FREELIST_INDEX(n);
	// ����free list���������飬���������Ϊ����ͷ
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

void * TinySTL::alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
{
	if (old_sz > (size_t)TinySTL::__MAX_BYTES && new_sz > (size_t)TinySTL::__MAX_BYTES)	// ԭ�ڴ�����ڴ涼����128
	{
		// ֱ��ʹ��realloc����
		return realloc(p, new_sz);
	}
	if (ROUND_UP(old_sz) == ROUND_UP(new_sz))	// ԭ�ڴ�����ڴ�����ͬ��С���飬�������
	{
		return p;
	}

	/*		
		ʣ�������ԭ�ڴ�����ڴ涼С�ڵ���128��һ������128һ��С�ڵ���128
		���������ԭ�ڴ�����ڴ涼ָ��ͬ���ڴ�ռ�
		��Ҫ�������ڴ棬�Ѿ��ڴ����ݸ��Ƶ����ڴ棬�ͷž��ڴ�����
	*/
	// �������ڴ�
	void *result = allocate(new_sz);
	size_t copy_sz = old_sz < new_sz ? old_sz : new_sz;
	memcpy(result, p, copy_sz);
	deallocate(p, old_sz);
	return result;
}

void * TinySTL::alloc::refill(size_t n)
{
	// Ĭ������³���ȡ��20��������Ϊfree list���½ڵ㣬��ʵ�ʵõ��������������20����nobjs�������յõ����������
	int nobjs = 20;
	char *chunk = chunk_alloc(n, nobjs);

	obj **my_free_list;	
	obj *result;
	obj *current_obj, *next_obj;
	int i;

	if (nobjs == 1)		// ֻ���һ�����飬ֱ�ӰѸ����鷵�أ�*my_free_list��Ϊ��
	{
		return chunk;
	}

	// ����free list�������µĽڵ�
	my_free_list = free_list + FREELIST_INDEX(n);
	// ��¼Ҫ���صĽ��
	result = (obj*)chunk;
	// ��һ��obj��Ҫ���û��ģ�free listӦ�ӵڶ����ڵ㿪ʼ
	*my_free_list = next_obj = (obj *)(chunk + n);
	// ���½ڵ㴮������
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
	size_t total_bytes = size * nobjs;	// ������������ֽ���
	size_t bytes_left = end_free - start_free;	// �ڴ��ʣ��ռ�

	if (bytes_left >= total_bytes)	// �ڴ��ʣ��ռ���ȫ��������
	{
		// ֱ�Ӵ��ڴ���в���
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else if (bytes_left >= size)	// �ڴ��ʣ��ռ䲻����ȫ�������㣬�����Է���һ����һ�����ϵ�����
	{
		// ���ڴ���в���ʣ��ռ��п��Է��������
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else	// �ڴ��ʣ��ռ�����һ�������С���޷��ṩ
	{
		// �������ڴ�
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0)		// ��������ڴ��ʣ��ռ�
		{
			// �ҵ��ڴ�ʣ��ռ���Է�����������Ӧ��free-list����ʣ��ռ���������
			obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj *)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj *)start_free;
		}

		// ��ѿռ������ڴ�
		start_free = (char *)malloc(bytes_to_get);
		if (start_free == 0)	// heap�ռ䲻��
		{
			int i;
			obj **my_free_list;
			obj *p;
			// ���������free list���Ƿ���ʣ�����飬����ȡһ������
			for (i = size; i <= TinySTL::__MAX_BYTES; i += TinySTL::__ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0)
				{
					// ��free list��һ���ڵ��ڴ���յ��ڴ��
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					// �ݹ�����Լ���Ϊ������nobjs
					return chunk_alloc(size, nobjs);
				}
			}

			// ����˵��������û���ڴ����
			end_free = 0;
			// ���õ�һ���������������ڴ治�㴦����򣬿��Ƿ��ܹ��õ��ڴ�
			start_free = (char *)malloc_alloc::allocate(bytes_to_get);
		}
		// �����ڴ�ɹ�
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		// �ݹ�����Լ���Ϊ������nobjs
		return chunk_alloc(size, nobjs);
	}
}

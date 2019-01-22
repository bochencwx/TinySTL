#ifndef __TINYSTL_VECTOR_H
#define __TINYSTL_VECTOR_H

#include "tinySTL_alloc.h"
#include "tinySTL_uninitialized.h"
#include "tinySTL_algorithm.h"
#include "tinySTL_construct.h"

namespace TinySTL {

	template <typename T, typename Alloc = alloc>
	class vector
	{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef T&			reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T*			iterator;	// 定义vector的迭代器类型，为原生指针

	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;		// 目前使用空间的头
		iterator finish;	// 目前使用空间的尾，指向一个空位
		iterator end_of_storage;	// 目前可用空间的尾

		// 在position位置前插入一个x值
		void insert_aux(iterator position, const T& x)
		{
			if (finish != end_of_storage)	// 还有备用空间
			{
				// 在最后构造一个元素，并以vector最后一个元素值为初值
				construct(finish, *(finish - 1));
				++finish;
				// 元素后移
				copy_backward(position, finish - 2, finish - 1);
				*position = x;
			}
			else	// 已无备用空间
			{
				const size_type old_size = size();
				const size_type new_size = old_size != 0 ? 2 * old_size : 1;
				// 申请新空间，大小为new_size
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;

				// 将原vector的内容拷贝到新vector
				// 先复制[start, position)
				new_finish = uninitialized_copy(start, position, new_start);
				// 插入新值
				construct(new_finish, x);
				++new_finish;
				// 复制[position, finish)
				new_finish = uninitialized_copy(position, finish, new_finish);

				// 析构并释放原vector
				destroy(start, finish);
				deallocate();

				// 调整迭代器，指向新的vector
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_size;
			}
		}
		// 释放vector占用的空间
		void deallocate()
		{
			// 有分配空间情况下才需要释放
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}
		// 配置而后填充
		iterator allocate_and_fill(size_type n, const T& x)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}
		// 填充并予以初始化
		void fill_initialize(size_type n, const T& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

	public:
		/*		构造函数、析构函数		*/	
		vector() : start(0), finish(0), end_of_storage(0) {}
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(long n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_t n) { fill_initialize(n, T()); }
		~vector()
		{
			destroy(start, finish);
			deallocate();
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return size_type(finish - start); }
		size_type capacity() const { return size_type(end_of_storage - start); }	// 容量
		bool empty() const { return start == finish; }
		reference operator[](size_type n) { return *(start + n); }
		reference front() { return *start; }
		reference back() { return *(finish - 1); }

		void push_back(const T& x)
		{
			if (finish != end_of_storage)
			{
				construct(finish, x);
				++finish;
			}
			else
				insert_aux(end(), x);
		}

		void pop_back()
		{
			--finish;
			destroy(finish);
		}

		iterator erase(iterator position)
		{
			if (position + 1 != finish)
				copy(position + 1, finish, position);	// 后续元素往前移
			--finish;
			destroy(finish);
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		// 从position开始，插入n个值为x的元素
		void insert(iterator position, size_type n, const T& x)
		{
			if (n <= 0)
				return;
			if (size_type(end_of_storage - finish) >= n)	// 备用空间大于等于新增元素个数
			{
				T x_copy = x;
				// 计算插入点之后的现有元素个数
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n)	// “插入点之后的现有元素个数”大于“新增元素个数”
				{
					// 输入输出区间有重叠，需要分两步后移
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					// 从插入点开始填值
					fill(position, position + n, x_copy);
				}
				else
				{
					// 不会有重叠
					uninitialized_copy(position, old_finish, old_finish + n - elems_after);
					finish += n;
					fill(position, position + n, x_copy);
				}
			}
			else	// 备用空间小于新增元素个数
			{
				// 需要分配新内存
				const size_type old_size = size();
				const size_type new_size = old_size + (old_size > n ? old_size : n);
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;

				// 把旧vector迁移到新vector并完成插入操作
				new_finish = uninitialized_copy(start, position, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(position, finish, new_finish);

				// 清除并释放旧的vector
				destroy(start, finish);
				deallocate();

				// 调整容器
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_size;
			}
		}

		void resize(size_type new_size, const T& x)
		{
			if (new_size < size())
				erase(start + new_size, finish);
			else
				insert(finish, new_size - size(), x);
		}

		void resize(size_type new_size)
		{
			resize(new_size, T());
		}

		void clear()
		{
			erase(start, finish);
		}

	};

}	// end of namespace TinySTL

#endif // !__TINYSTL_VECTOR_H


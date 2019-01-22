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
		typedef T*			iterator;	// ����vector�ĵ��������ͣ�Ϊԭ��ָ��

	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;		// Ŀǰʹ�ÿռ��ͷ
		iterator finish;	// Ŀǰʹ�ÿռ��β��ָ��һ����λ
		iterator end_of_storage;	// Ŀǰ���ÿռ��β

		// ��positionλ��ǰ����һ��xֵ
		void insert_aux(iterator position, const T& x)
		{
			if (finish != end_of_storage)	// ���б��ÿռ�
			{
				// �������һ��Ԫ�أ�����vector���һ��Ԫ��ֵΪ��ֵ
				construct(finish, *(finish - 1));
				++finish;
				// Ԫ�غ���
				copy_backward(position, finish - 2, finish - 1);
				*position = x;
			}
			else	// ���ޱ��ÿռ�
			{
				const size_type old_size = size();
				const size_type new_size = old_size != 0 ? 2 * old_size : 1;
				// �����¿ռ䣬��СΪnew_size
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;

				// ��ԭvector�����ݿ�������vector
				// �ȸ���[start, position)
				new_finish = uninitialized_copy(start, position, new_start);
				// ������ֵ
				construct(new_finish, x);
				++new_finish;
				// ����[position, finish)
				new_finish = uninitialized_copy(position, finish, new_finish);

				// �������ͷ�ԭvector
				destroy(start, finish);
				deallocate();

				// ������������ָ���µ�vector
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_size;
			}
		}
		// �ͷ�vectorռ�õĿռ�
		void deallocate()
		{
			// �з���ռ�����²���Ҫ�ͷ�
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}
		// ���ö������
		iterator allocate_and_fill(size_type n, const T& x)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}
		// ��䲢���Գ�ʼ��
		void fill_initialize(size_type n, const T& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

	public:
		/*		���캯������������		*/	
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
		size_type capacity() const { return size_type(end_of_storage - start); }	// ����
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
				copy(position + 1, finish, position);	// ����Ԫ����ǰ��
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

		// ��position��ʼ������n��ֵΪx��Ԫ��
		void insert(iterator position, size_type n, const T& x)
		{
			if (n <= 0)
				return;
			if (size_type(end_of_storage - finish) >= n)	// ���ÿռ���ڵ�������Ԫ�ظ���
			{
				T x_copy = x;
				// ��������֮�������Ԫ�ظ���
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n)	// �������֮�������Ԫ�ظ��������ڡ�����Ԫ�ظ�����
				{
					// ��������������ص�����Ҫ����������
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					// �Ӳ���㿪ʼ��ֵ
					fill(position, position + n, x_copy);
				}
				else
				{
					// �������ص�
					uninitialized_copy(position, old_finish, old_finish + n - elems_after);
					finish += n;
					fill(position, position + n, x_copy);
				}
			}
			else	// ���ÿռ�С������Ԫ�ظ���
			{
				// ��Ҫ�������ڴ�
				const size_type old_size = size();
				const size_type new_size = old_size + (old_size > n ? old_size : n);
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;

				// �Ѿ�vectorǨ�Ƶ���vector����ɲ������
				new_finish = uninitialized_copy(start, position, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(position, finish, new_finish);

				// ������ͷžɵ�vector
				destroy(start, finish);
				deallocate();

				// ��������
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


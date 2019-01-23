#ifndef __TINYSTL_LIST_H
#define __TINYSTL_LIST_H

#include "tinySTL_iterator.h"
#include "tinySTL_alloc.h"
#include "tinySTL_construct.h"

namespace TinySTL {

	/*
		�ڵ����ݽṹ
	*/
	template<typename T>
	struct node
	{
		node<T>* prev;
		node<T>* next;
		T data;

		node(const T& val = T()) :prev(0), next(0), data(val) {}
		node(const T& val, node<T>* prev_, node<T>* next_) :prev(prev_), next(next_), data(val) {}

		bool operator==(const node<T>& nod)
		{
			return prev == nod.prev && next == nod.next && data == nod.data;
		}

		bool operator!=(const node<T>& nod)
		{
			return prev != nod.prev || next != nod.next || data != nod.data;
		}

	};

	/*
		list�ĵ�����
	*/
	template<typename T>
	struct listIterator
	{
		typedef bidirectional_iterator_tag	iterator_category;	// ˫�������
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;

		typedef node<T>*					nodePtr;
		typedef listIterator<T>				self;

		nodePtr p;	// �������ڲ�������ʵ����ָͨ�룬ָ��list�Ľڵ�

		listIterator(const nodePtr& _p = 0) :p(_p) {}

		bool operator==(const listIterator<T>& iter)
		{
			return p == iter.p;
		}

		bool operator!=(const listIterator<T>& iter)
		{
			return p != iter.p;
		}

		reference operator*()const
		{
			return (*p).data;
		}

		pointer operator->()const
		{
			return &((*p).data);
		}

		// ǰ��++
		self& operator++()
		{
			p = (nodePtr)(p->next);
			return *this;
		}

		// ����++
		self operator++(int)
		{
			self tmp = *this;
			p = (nodePtr)(p->next);
			return tmp;
		}

		// ǰ��--
		self& operator--()
		{
			p = (nodePtr)(p->prev);
			return *this;
		}

		// ����--
		self operator--(int)
		{
			self tmp = *this;
			p = (nodePtr)(p->prev);
			return tmp;
		}

	};

	/*
		�����ݽṹ
	*/
	template<typename T, typename Alloc = alloc>
	class list
	{
	public:
		typedef T		value_type;
		typedef size_t	size_type;
		typedef T&		reference;

		typedef node<T>*				nodePtr;
		typedef listIterator<T>			iterator;
		typedef listIterator<const T>	const_iterator;

		typedef simple_alloc<node<T>, Alloc> list_node_allocator;

	protected:
		iterator head;	// ͷ
		iterator tail;	// β

	protected:
		// ����һ���ڵ㣬��������û��������
		nodePtr newNode(const T& value = T())
		{
			nodePtr ptr = list_node_allocator::allocate();
			construct(ptr, node<T>(value));
			return ptr;
		}
		// ɾ��һ���ڵ㣬����ɾ��û�е�������
		void deleteNode(nodePtr p)
		{
			p->prev = p->next = 0;
			destroy(p);
			list_node_allocator::deallocate(p);
		}
		// ��������[first,last)�Ⱥ�ڵ�Ĺ�ϵ��ֻ�ʺ�list.begin(),list.end()���ã���С��ʹ��
		void transfer(iterator position, iterator first, iterator last)
		{
			if (first == last)
				return;

			if (position == begin())
			{
				nodePtr old_head = position.p;
				nodePtr lastPrev = last.p->prev;
				first.p->prev = 0;
				head.p = first.p;
				old_head->prev = lastPrev;
				lastPrev->next = old_head;
			}
			else
			{
				nodePtr positionPrev = position.p->prev;
				nodePtr lastPrev = last.p->prev;
				positionPrev->next = first.p;
				first.p->prev = positionPrev;
				position.p->prev = lastPrev;
				lastPrev->next = position.p;
			}
		}

	public:
		list()
		{
			head = iterator();
			tail = iterator();
			head.p = newNode();
			tail.p = head.p;
		}
		~list()
		{
			for (; head != tail;)
			{
				nodePtr tmpnode = head.p;
				++head;
				deleteNode(tmpnode);

			}
			deleteNode(tail.p);
		}

		iterator begin() { return head; }
		iterator end() { return tail; }
		bool empty() { return head == tail; }
		reference front() { return *head; }
		reference back() { return (tail.p->prev->data);}

		void push_front(const T& value)
		{
			nodePtr newnode = newNode(value);
			newnode->next = head.p;
			head.p->prev = newnode;
			head.p = newnode;
		}

		void pop_front()
		{
			nodePtr nextnode = head.p->next;
			nextnode->prev = 0;
			nodePtr tmp = head.p;
			head.p = nextnode;
			deleteNode(tmp);
		}
		void push_back(const T& value)
		{
			nodePtr newnode = newNode();
			newnode->prev = tail.p;
			tail.p->next = newnode;
			tail.p->data = value;
			tail.p = newnode;
		}
		void pop_back()
		{
			nodePtr prevnode = tail.p->prev;
			prevnode->next = 0;
			nodePtr tmp = tail.p;
			tail.p = prevnode;
			deleteNode(tmp);
		}

		iterator insert(iterator position, const T& value)
		{
			nodePtr newnode = newNode(value);
			nodePtr prevnode = position.p->prev;
			prevnode->next = newnode;
			newnode->prev = prevnode;
			newnode->next = position.p;
			position.p->prev = newnode;
			position.p = newnode;
			return position;
		}

		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last)
		{
			for (--last; first != last; --last)
			{
				position = insert(position, *first);
			}
			insert(position, *last);
		}

		iterator erase(iterator position)
		{
			nodePtr prevNode = position.p->prev;
			nodePtr nextNode = position.p->next;
			prevNode->next = nextNode;
			nextNode->prev = prevNode;
			deleteNode(position.p);
			return iterator(nextNode);
		}

		void splice(iterator position, list<T, Alloc>& other)
		{
			transfer(position, other.begin(), other.end());
			other.head = other.tail;
			nodePtr dummyNode = tail.p;
			dummyNode->prev = dummyNode->next = 0;
		}

	};

}	// end of namespace TinySTL

#endif // !__TINYSTL_LIST_H

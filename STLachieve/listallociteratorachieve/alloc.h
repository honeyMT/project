#pragma once

#include "Commod.h"

typedef bool(*HANDLE_FUNC)();

template <int inst>
class __MallocAllocTemplate {

private:
	static HANDLE_FUNC __malloc_alloc_oom_handler;
public:

	static void* Allocate(size_t n)
	{
		void *result = malloc(n);
		if (0 == result)
			result = OOMMalloc(n);
		return result;
	}

	static void Deallocate(void *p, size_t /* n */)
	{
		free(p);
	}

	static void* OOMMalloc(size_t n)
	{
		while (1)
		{
			if (__malloc_alloc_oom_handler == 0
				|| __malloc_alloc_oom_handler() == false)
			{
				throw bad_alloc();
			}

			void* ret = malloc(n);
			if (ret)
			{
				return ret;
			}
		}

	}

	static HANDLE_FUNC SetMallocHandler(HANDLE_FUNC f)
	{
		HANDLE_FUNC old = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

template<int inst>
HANDLE_FUNC __MallocAllocTemplate<inst>::__malloc_alloc_oom_handler = 0;


bool AllocErrorHandle()
{
	cout << "回调处理内存不足" << endl;

	return false;
}

/////////////////////////////////////////////////////////////////////
// 二级空间配置器


template <bool threads, int inst>
class __DefaultAllocTemplate
{
	union Obj
	{
		Obj* _freeListLink;
		char _client_data[1]; // 做硬件层优化
	};

	enum { __ALIGN = 8 };
	enum { __MAX_BYTES = 128 };
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
public:
	static size_t FREE_LIST_INDEX(size_t n)
	{
		return (n + __ALIGN - 1) / __ALIGN - 1;
	}

	static size_t ROUND_UP(size_t n)
	{
		return (n + __ALIGN - 1) & (~(__ALIGN - 1));
	}

	static char* ChunkAlloc(size_t n, size_t& nobjs)
	{
		size_t totalBytes = nobjs*n;
		size_t leftBytes = _endFree - _startFree;
		__TRACE_DEBUG("内存池剩余%dbytes，需要申请20个对象，一共%dbytes\n", leftBytes, totalBytes);

		//1.
		//2.
		//3.
		if (leftBytes >= totalBytes)
		{
			__TRACE_DEBUG("向内存池申请了20个对象，内存池剩余%d bytes\n", leftBytes - totalBytes);

			char* ret = _startFree;
			_startFree += totalBytes;
			return ret;
		}
		else if (leftBytes > n)
		{
			nobjs = leftBytes / n;
			totalBytes = nobjs*n;
			char* ret = _startFree;
			_startFree += totalBytes;

			__TRACE_DEBUG("向内存池申请了%d个对象，内存池剩余%d bytes\n", nobjs, leftBytes - totalBytes);

			return ret;
		}
		else
		{
			// 处理剩余的小块内存
			if (leftBytes > 0)
			{
				size_t index = FREE_LIST_INDEX(leftBytes);

				((Obj*)_startFree)->_freeListLink = _freeList[index];
				_freeList[index] = (Obj*)_startFree;
			}

			// 向系统申请大块内存
			size_t bytesToGet = totalBytes * 2 + ROUND_UP(_heapSize >> 4);
			_startFree = (char*)malloc(bytesToGet);
			__TRACE_DEBUG("向系统malloc %dbytes\n", bytesToGet);
			if (_startFree == NULL) // 系统内存吃紧
			{
				__TRACE_DEBUG("到更大自由链表中去申请\n");
				// 到更大的自由链表中去找
				size_t index = FREE_LIST_INDEX(n);
				for (; index < __NFREELISTS; ++index)
				{
					if (_freeList[index])
					{
						Obj* obj = _freeList[index];
						_freeList[index] = obj->_freeListLink;
						_startFree = (char*)obj;
						_endFree = _startFree + (index + 1)*__ALIGN;
						return ChunkAlloc(n, nobjs);
					}
				}

				// 山穷水尽
				_endFree = NULL;
				_startFree = (char*)__MallocAllocTemplate<0>::Allocate(bytesToGet);
			}

			_heapSize += bytesToGet;
			_endFree = _startFree + bytesToGet;
			return ChunkAlloc(n, nobjs);
		}
	}

	static void* Refill(size_t n)
	{
		size_t nobjs = 20;
		char* chunk = ChunkAlloc(n, nobjs);
		if (nobjs == 1)
			return chunk;

		Obj* cur = (Obj*)(chunk + n);
		size_t index = FREE_LIST_INDEX(n);
		_freeList[index] = cur;

		__TRACE_DEBUG("挂%d个对象到freelist[%d],返回一个\n", nobjs - 1, index);

		for (size_t i = 0; i < nobjs - 2; ++i)
		{
			Obj* next = (Obj*)((char*)cur + n);
			cur->_freeListLink = next;
			cur = next;
		}
		cur->_freeListLink = NULL;

		return chunk;
	}

	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("向二级空间配置申请 %dbytes\n", n);

		if (n > __MAX_BYTES)
		{
			return __MallocAllocTemplate<0>::Allocate(n);
		}

		size_t index = FREE_LIST_INDEX(n);

		// Lock()

		if (_freeList[index] == 0)
		{
			__TRACE_DEBUG("freeList[%d]下没有内存块对象\n", index);
			return Refill(ROUND_UP(n));
		}
		else
		{
			__TRACE_DEBUG("在freeList[%d]下取一个%dbytes对象\n", index, ROUND_UP(n));

			Obj* ret = _freeList[index];
			_freeList[index] = ret->_freeListLink;
			return ret;
		}

		// Unlock()
	}

	static void Deallocate(void *p, size_t n)
	{
		if (n > __MAX_BYTES)
		{
			return __MallocAllocTemplate<0>::Deallocate(p, n);
		}

		size_t index = FREE_LIST_INDEX(n);
		__TRACE_DEBUG("释放的内存对象挂到，freelist[%d]\n", index);

		((Obj*)p)->_freeListLink = _freeList[index];
		_freeList[index] = (Obj*)p;
	}

private:
	static Obj* _freeList[__NFREELISTS];

	static char* _startFree;
	static char* _endFree;
	static size_t _heapSize;
};


template<bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj*
__DefaultAllocTemplate<threads, inst>::_freeList[__NFREELISTS] = { 0 };


template<bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_startFree = NULL;


template<bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_endFree = NULL;

template<bool threads, int inst>
size_t __DefaultAllocTemplate<threads, inst>::_heapSize = 0;


template<class T, class Alloc>
class SimpleAlloc {

public:
	static T* Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));
	}

	static T* Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof (T));
	}

	static void Deallocate(T *p, size_t n)
	{
		if (0 != n) Alloc::Deallocate(p, n * sizeof (T));
	}

	static void Deallocate(T *p)
	{
		Alloc::Deallocate(p, sizeof (T));
	}
};


void TestAlloc()
{
	for (size_t i = 0; i < 21; ++i)
	{
		__DefaultAllocTemplate<0, 0>::Allocate(10);
	}
}
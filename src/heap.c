/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Keren Robbins
 * Date: September 19th, 2021
 *
 * Description: Heap DS implementation
 *
 * Infinity Labs OL108
*******************************************************************************/
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memcpy() */

#include "heap.h"
#include "dynamic_vector.h" /* dynamic vector API */

#define INIT_CAPACITY 2
#define PTR_SIZE sizeof(void*)

struct heap
{
    d_vector_t *d_vec;
    heap_cmp_t cmp_func;
};

static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index, size_t size);
static int IsNotHeapified(heap_t *heap, size_t parent_idx, size_t child_idx);
static void Swap(d_vector_t *vec, size_t index1, size_t index2);
static void *RemoveFromIndex(heap_t *heap, size_t index, size_t last_index);
static size_t GetParentIndex(size_t index);
static size_t GetLeftChildIndex(size_t index);
static size_t GetRightChildIndex(size_t index);
/******************************************************************************/
heap_t *HeapCreate(heap_cmp_t cmp_func)
{
    heap_t *heap = NULL;

    assert(NULL != cmp_func);

    heap = (heap_t*)malloc(sizeof(heap_t));
	if (NULL == heap)
	{
		return NULL;
	}
	
	heap->d_vec = VectorCreate(PTR_SIZE, INIT_CAPACITY);
    if (NULL == heap->d_vec)
    {
        free(heap);
        return NULL;
    }

    heap->cmp_func = cmp_func;

    return heap;
}


void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);

    VectorDestroy(heap->d_vec);
    heap->d_vec = NULL;

    free(heap);
    heap = NULL;
}


int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);

    return (0 == VectorGetSize(heap->d_vec));
}


size_t HeapSize(const heap_t *heap)
{
     assert(NULL != heap);

    return VectorGetSize(heap->d_vec);
}


int HeapPush(heap_t *heap, const void *data)
{
    int is_pushed = 0;

    assert(NULL != heap);

    is_pushed = VectorPushBack(heap->d_vec, &data);

    if (0 == is_pushed)
    {
        HeapifyUp(heap, HeapSize(heap) - 1);
    }

    return is_pushed;
}


void *HeapPop(heap_t *heap)
{
    void *data = NULL;
    size_t size = 0;

    assert(NULL != heap);

    size = VectorGetSize(heap->d_vec);

    if (0 != size)
    {
        data = RemoveFromIndex(heap, 0, --size);
    }

    return data;
}


void *HeapPeek(const heap_t *heap)
{
    void *data = NULL;

    assert(NULL != heap);

    if(!HeapIsEmpty(heap))
    {
        data = *(void **)VectorGetData(heap->d_vec, 0);
    }

    return data;
}


void *HeapRemove(heap_t *heap, const void *to_remove, is_match_heap_t is_match)
{
    size_t i = 0, size = 0;
    int is_found = 0;
    void *removed = NULL;

    assert(NULL != heap);
    assert(NULL != is_match);

    size = VectorGetSize(heap->d_vec);
    
    for (; !is_found && i < size; ++i)
    {
        is_found = is_match(to_remove, *(void**)VectorGetData(heap->d_vec, i));
    }

    if (is_found)
    {
        removed = RemoveFromIndex(heap , --i, --size);
        i -=  (i == size);
        HeapifyUp(heap, i);
    }
    
    return removed;
}

/******************************************************************************/
static void HeapifyUp(heap_t *heap, size_t index)
{
    if (0 != index)
    {
        size_t parent_index = GetParentIndex(index);

        if (IsNotHeapified(heap, parent_index, index))
        {
            Swap(heap->d_vec, index, parent_index);

            HeapifyUp(heap, parent_index);
        }
    }
}

 
static void HeapifyDown(heap_t *heap, size_t index, size_t size)
{
    size_t left = GetLeftChildIndex(index);
    size_t right = GetRightChildIndex(index);
    size_t above = index;
 
    if (left < size && IsNotHeapified(heap, index, left))
    {
        above = left;
    }

    if (right < size && IsNotHeapified(heap, above, right)) 
    {
        above = right;
    }
    
    if (above != index)
    {
        Swap(heap->d_vec, index, above);

        HeapifyDown(heap, above, size);
    }
}


static int IsNotHeapified(heap_t *heap, size_t parent_idx, size_t child_idx)
{
    return (0 < heap->cmp_func( *(void**)VectorGetData(heap->d_vec, parent_idx),
                          *(void**)VectorGetData(heap->d_vec, child_idx)));
}


static void Swap(d_vector_t *vec, size_t index1, size_t index2)
 {
    void *temp[PTR_SIZE];

    memcpy(temp, VectorGetData(vec, index1), sizeof(void*));
    VectorSetData(vec, index1,  VectorGetData(vec, index2));
    VectorSetData(vec, index2, temp);
 }


 static void *RemoveFromIndex(heap_t *heap, size_t index, size_t last_index)
{
    void *data = NULL;

    Swap(heap->d_vec, index, last_index);
    data = *(void **)VectorGetData(heap->d_vec, last_index);
    VectorPopBack(heap->d_vec);
    HeapifyDown(heap, index, last_index);

    return data;
}


static size_t GetParentIndex(size_t index) 
{
    return (index - 1) / 2;
}


static size_t GetLeftChildIndex(size_t index) 
{
    return (2 * index + 1);
}
 

static size_t GetRightChildIndex(size_t index)
{
    return (2 * index + 2);
}
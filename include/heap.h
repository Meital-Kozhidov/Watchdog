/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Keren Robbins
 * Date: September 19th, 2021
 *
 * Description: Heap DS implementation
 *
 * Infinity Labs OL108
*******************************************************************************/

#ifndef __HEAP_H_OL108_ILRD__
#define __HEAP_H_OL108_ILRD__

#include <stddef.h> /* size_t */

typedef struct heap heap_t;

/**
 * @Description: Function pointer type that compares the data of 
 * 				       two elements.
 * @Parameters: Two void pointers to data from two elements.
 * @Return: Zero if they are equal, a negative number if node_data should be 
 *          above compare_data, or a positive number if node_data should be 
 *          below cmp_data.
**/
typedef int (*heap_cmp_t)(const void *node_data, const void *compare_data);

/**
 * @Description: Pointer to a function that compares data of one data element
 *               with data of another void pointer.
 * @Parameters: Two void pointers to the data to compare.
 * @Return: One if they are equal, zero if not.
**/
typedef int (*is_match_heap_t)(const void *node_data, const void *cmp_data);


/**
 * @Description: Creates an empty heap.
 * @Parameters: cmp_func - function to compare different elements.
 * @Return: a pointer to the new heap if created successfuly, otherwise NULL.
 * @Complexity: O(1)
**/
heap_t *HeapCreate(heap_cmp_t cmp_func);


/**
 * @Description: Destroys the given heap (but not the data).
 * @Parameters: heap - a pointer to the heap.
 * @Return: void.
 * @Complexity: O(1)
**/
void HeapDestroy(heap_t *heap);


/**
 * @Description: Inserts the given data to the heap.
 * @Parameters: heap - pointer to the heap
 *				    data - pointer to the data
 * @Return: 0 if inserted successfuly, 1 if failed (allocation fail)
 * @Complexity: O(log n)
**/
int HeapPush(heap_t *heap, const void *data);


/**
 * @Description: Removes the root element of the heap.
 * @Parameters: heap - pointer to the heap
 * @Return: The data of the removed element.
 * @Notes: If the data is not in the hash table, NULL will be returned.
 * @Complexity: O(log n)
**/
void *HeapPop(heap_t *heap);


/**
 * @Description: Gets the root element of the heap.
 * @Parameters: heap - pointer to the heap
 * @Return: The data of the min/max element.
 * @Notes: If the data is not in the hash table, NULL will be returned.
 * @Complexity: O(1)
**/
void *HeapPeek(const heap_t *heap);


/**
 * @Description: Gets the size of the heap.
 * @Parameters: heap - a pointer to the heap.
 * @Return: The size of the heap (number of elements in the heap).
 * @Complexity: O(1).
**/
size_t HeapSize(const heap_t *heap);


/**
 * @Description: Checks if the heap is empty.
 * @Parameters: heap - a pointer to the heap.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1)
**/
int HeapIsEmpty(const heap_t *heap);


/**
 * @Description: Removes element with given data from the heap.
 * @Parameters: heap - pointer to the heap
 *              to_remove - pointer to data of the element to remove.
 *              is_match - function to compare different elements 
 *                         based on to_remove.
 * @Return: The data of the removed element.
 * @Notes: If the data is not in the heap, NULL is returned.
 * @Complexity: O(n)
**/
void *HeapRemove(heap_t *heap, const void *to_remove, is_match_heap_t is_match);

#endif /* __HEAP_H_OL108_ILRD__ */
/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Ido Sapan
 * Date: August 12th, 2021
 
 * Description: priority queue implementation
 *
 * Infinity Labs OL108
*******************************************************************************/

#ifndef __PRIORITY_QUEUE_H_OL108__
#define __PRIORITY_QUEUE_H_OL108__

#include <stddef.h> /* size_t */

typedef struct pq pq_t;

/**
 * @Description: Function pointer type that compares the priority of 
 * 				 two elements.
 * @Parameters: Two void pointers to the data from two elements.
 * @Return: Zero if they are equal, a negative number if lhs should be before
 *          rhs, and a positive number if rhs should be before lhs.
**/
typedef int (*cmp_priority_t)(const void *lhs, const void *rhs);


/**
 * @Description: Pointer to a function that compares the data of an element
 *               with data of another void pointer.
 * @Parameters: Two void pointers to the data to compare.
 * @Return: One if they are equal, zero if not.
**/
typedef int (*is_match_pq_t)(const void *lhs, const void *rhs);


/**
 * @Description: Creates a Priority Queue with the priority determined by the 
 *				cmp_priority. 
 * @Parameters: A cmp_priority_t function.
 * @Return: Pointer to the PQ, if the creation failed return NULL.
 * @Complexity: O(1).
**/
pq_t *PQCreate(cmp_priority_t cmp_priority);


/**
 * @Description: Destroys the given Priority Queue.
 * @Parameters: A pointer to the PQ.
 * @Return: None.
 * @Complexity: O(n) - where n is the number of elements in the PQ.
**/
void PQDestroy(pq_t *pqueue);


/**
 * @Description: Inserts an element with the given data to the given PQ.
 * @Parameters: A pointer to the PQ, a pointer to data.
 * @Return: 0 if insert successfully, 1 otherwise.
 * @Complexity: O(n) - where n is the number of elements in the PQ.
**/
int PQEnqueue(pq_t *pqueue, void *data);


/**
 * @Description: Removes the first element in the given Priority Queue, and 
 *				returns the data of the removed element.
 * @Parameters: A pointer to the PQ.
 * @Return: void pointer to the data of the removed element.
 * @Complexity: O(1).
**/
void *PQDequeue(pq_t *pqueue);


/**
 * @Description: Peeks at the first element in the given Priority Queue.
 * @Parameters: A pointer to the PQ.
 * @Return: void pointer to the data of the peeked element.
 * @Complexity: O(1).
**/
void *PQPeek(const pq_t *pqueue);


/**
 * @Description: Checks whether the given Priority Queue is empty or not.
 * @Parameters: A pointer to the PQ.
 * @Return: 1 if the PQ is empty, 0 otherwise.
 * @Complexity: O(1).
**/
int PQIsEmpty(const pq_t *pqueue);


/**
 * @Description: Returns the number of elements in the given Priority Queue.
 * @Parameters: A pointer to the PQ.
 * @Return: the number of elements.
 * @Complexity: O(n) - where n is the number of elements in the PQ.
**/
size_t PQSize(const pq_t *pqueue);


/**
 * @Description: Remove all the elements from a given Priority Queue.
 * @Parameters: A pointer to the PQ.
 * @Return: None.
 * @Complexity: O(n) - where n is the number of elements in the PQ.
**/
void PQClear(pq_t *pqueue);


/**
 * @Description: Removes the first appearance of an element with data matched
 *				to "to_del" by the is_match function.
 * @Parameters: A pointer to the PQ, a pointer to to_del, a is_match_t function.
 * @Return: The data of the removed element if such an element exists, NULL if 
 *			There is no such element in the PQ.
 * @Complexity: O(n) - where n is the number of elements in the PQ.
**/
void* PQErase(pq_t *pqueue, void *to_del, is_match_pq_t is_match);

#endif /* __PRIORITY_QUEUE_H_OL108__ */

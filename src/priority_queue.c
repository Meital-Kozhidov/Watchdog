/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Ido Sapan
 * Date: August 12th, 2021
 
 * Description: priority queue implementation
 *
 * Infinity Labs OL108
*******************************************************************************/

#include <stdlib.h> /* malloc() , free() */
#include <assert.h> /* assert() */

#include "priority_queue.h"
#include "heap.h" /* heap API */

struct pq
{
	heap_t *pqueue; 
};

/******************************************************************************/

pq_t *PQCreate(cmp_priority_t cmp_priority)
{
	pq_t *pq = (pq_t*)malloc(sizeof(pq_t));
	
	if (NULL == pq)
	{
		return NULL;
	}
	
	pq->pqueue = HeapCreate(cmp_priority);
	
	if (NULL == pq->pqueue)
	{
		free(pq);
		pq = NULL;
		
		return NULL;
	}
	
	return pq;
}


void PQDestroy(pq_t *pqueue)
{
	assert (NULL != pqueue);
	
	HeapDestroy(pqueue->pqueue);
	pqueue->pqueue = NULL;
	
	free(pqueue);
	pqueue = NULL;
}


int PQEnqueue(pq_t *pqueue, void *data)
{	 
	assert (NULL != pqueue);

	return (HeapPush(pqueue->pqueue, data));
}


int PQIsEmpty(const pq_t *pqueue)
{
	assert (NULL != pqueue);
	
	return HeapIsEmpty(pqueue->pqueue);
}


size_t PQSize(const pq_t *pqueue)
{
	assert (NULL != pqueue);
	
	return HeapSize(pqueue->pqueue);
}


void *PQDequeue(pq_t *pqueue)
{
	assert (NULL != pqueue);
	
	return HeapPop(pqueue->pqueue);
}


void *PQPeek(const pq_t *pqueue)
{
	assert (NULL != pqueue);
	
	return HeapPeek(pqueue->pqueue);
}


void PQClear(pq_t *pqueue)
{
	assert (NULL != pqueue);
	
	while (!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}


void* PQErase(pq_t *pqueue, void *to_del, is_match_pq_t is_match)
{
	assert (NULL != pqueue);

	return HeapRemove(pqueue->pqueue, to_del, is_match);
}


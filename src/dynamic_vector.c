/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Aharon Luzon
 * Date: July 28th, 2021
 
 * Description: Dynamic vector implementation
 *
 * Infinity Labs OL108
*******************************************************************************/

#include <stdlib.h>   /* malloc(), realloc(), free() */
#include <assert.h>  /* assert() */
#include <string.h> /* memcpy() */

#include "dynamic_vector.h"

#define GROWTH_FACTOR 1.5

struct vector {
    void *data;
    size_t capacity;
    size_t vector_size;
    size_t data_size;
};
/******************************************************************************/

d_vector_t *VectorCreate(size_t data_size, size_t capacity)
{
	d_vector_t *vector = (d_vector_t*)malloc(sizeof(d_vector_t));
	
	if (NULL == vector)
	{
		return NULL;
	}
	
	vector->data = malloc(data_size * capacity);
	
	if (NULL == vector->data)
	{
		free(vector);
		
		return NULL;
	}
    vector->capacity = capacity;
    vector->data_size = data_size;
    vector->vector_size = 0;
    
    return vector;
}


void VectorDestroy(d_vector_t *vector)
{
	free(vector->data);
	free(vector);
}


int VectorPushBack(d_vector_t *vector, const void *data)
{
	int is_pushed = 0;
	
	assert(NULL != vector);
	assert(NULL != data);
	
	if (VectorGetSize(vector) == VectorGetCapacity(vector))
	{
		is_pushed = VectorResize(vector, vector->capacity * GROWTH_FACTOR);
	}
	
	VectorSetData(vector, vector->vector_size, data);
	++(vector->vector_size);
	
	return is_pushed;
}


void VectorPopBack(d_vector_t *vector)
{
	assert(NULL != vector);
	
	if (0 < VectorGetSize(vector))
	{
    	--(vector->vector_size);
    }
}


void VectorSetData(d_vector_t *vector, size_t index, const void *data)
{	
	void *data_to_set = NULL;

	assert(NULL != vector);
	assert(NULL != data);
	
	data_to_set = (char *)vector->data + index * (vector->data_size);
	
	if (vector->capacity > index)
	{
		memcpy(data_to_set, data, vector->data_size);
	}
}


void *VectorGetData(const d_vector_t *vector, size_t index)
{
	assert(NULL != vector);
	
	if (vector->vector_size > index)
	{
		return (char *)vector->data + index * (vector->data_size);
	}
	
	return NULL;
}


int VectorShrink(d_vector_t *vector)
{
	assert(NULL != vector);
	
	if (vector->vector_size > 0)
	{
		return VectorResize(vector, vector->vector_size);
	}
	
	return 1;
}


int VectorResize(d_vector_t *vector , size_t new_capacity)
{
	d_vector_t *temp_data = NULL;
	
	assert(NULL != vector);
	
	temp_data = realloc(vector->data, new_capacity * vector->data_size);
	
	if (NULL == temp_data)
	{
		return 1;	
	}
	
	vector->data = temp_data;
	vector->capacity = new_capacity;
	
	return 0;
}


size_t VectorGetSize(const d_vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->vector_size;
}


size_t VectorGetCapacity(const d_vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->capacity;
}



/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Keren Robbins
 * Date: August 15th, 2021
 
 * Description: Task implemantation
 *
 * Infinity Labs OL108
*******************************************************************************/

#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc() */

#include "task.h"

struct task
{
	uuid_t uid;
	time_t start_time;
	time_t time_interval;
	task_operation_func_t operation;
	task_cleanup_func_t cleanup;
	void *args;
};

/******************************************************************************/

task_t *TaskCreate(task_operation_func_t operation, time_t start_time
		, time_t time_interval, void *args, task_cleanup_func_t cleanup)
{
	task_t *task = NULL;
	
	assert(NULL != operation);
	assert(((time_t)-1) != start_time);
	assert(((time_t)-1) != time_interval);
	assert(NULL != cleanup);
	
	task = (task_t*)malloc(sizeof(task_t));
	
	if (NULL == task)
	{
		return NULL;
	}
	
	task->uid = UuidCreate();
	if (IsSameUuid(task->uid, bad_uuid))
	{
		free(task);
		task = NULL;

		return NULL;
	}
	task->start_time = start_time;
	task->time_interval = time_interval;
	task->operation = operation;
	task->cleanup = cleanup;
	task->args = args;
	
	return task;
}


void TaskDestroy(task_t *task)
{
	assert (NULL != task);
	
	task->cleanup(task->args);
	free(task);
	task = NULL;
}


uuid_t TaskGetUID(const task_t *task)
{
	assert (NULL != task);
	
	return task->uid;
}


time_t TaskGetStartTime(const task_t *task)
{
	assert (NULL != task);
	
	return task->start_time;
}


int TaskRunOperation(const task_t *task)
{
	assert (NULL != task);
	
	return task->operation(task->args);
}


task_t *TaskUpdateStartTime(task_t *task)
{
	assert (NULL != task);
	
	task->start_time += task->time_interval;
	
	return task;
}

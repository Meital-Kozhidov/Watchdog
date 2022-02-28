/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Keren Robbins
 * Date: August 15th, 2021
 
 * Description: Scheduler implemantation
 *
 * Infinity Labs OL108
*******************************************************************************/

#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() */

#include "priority_queue.h"
#include "task.h"
#include "scheduler.h"

struct scheduler
{
	pq_t *pq;
	int stop_flag;
};

static int StartTimeCmp(const void *task1, const void *task2);
static int IsSameTask(const void *task, const void *uuid);

/******************************************************************************/

sched_t *SchedCreate(void)
{
	sched_t *sched = (sched_t*)malloc(sizeof(sched_t));
	
	if (NULL == sched)
	{
		return NULL;
	}
	
	sched->pq = PQCreate(StartTimeCmp);
	
	if (NULL == sched->pq)
	{
		free(sched);
		sched = NULL;
		
		return NULL;
	}
	
	sched->stop_flag = 0;
	
	return sched;
}


void SchedDestroy(sched_t *sched)
{
	assert (NULL != sched);
	
	SchedClear(sched);
	PQDestroy(sched->pq);
	sched->pq = NULL;
	
	free(sched);
	sched = NULL;	
}


int SchedIsEmpty(const sched_t *sched)
{
	assert (NULL != sched);
	
	return (PQIsEmpty(sched->pq));
}


size_t SchedSize(const sched_t *sched)
{
	assert (NULL != sched);
	
	return (PQSize(sched->pq));
}


void SchedClear(sched_t *sched)
{
	assert(NULL != sched);
	
	while (!PQIsEmpty(sched->pq))
	{
		TaskDestroy(PQDequeue(sched->pq));
	}
}


uuid_t SchedAddTask(sched_t *sched, sched_operation_func_t task_func
		, time_t start_time, time_t time_interval, void *args
		, sched_cleanup_func_t cleanup)
{
	uuid_t ret_uid = bad_uuid;
	task_t *task = NULL;
	
	assert (NULL != sched);
	assert(NULL != task_func);
	assert(((time_t)-1) != start_time);
	assert(((time_t)-1) != time_interval);
	assert(NULL != cleanup);
	
	task = TaskCreate(task_func, start_time, time_interval, args, cleanup);
	
	if (NULL == task)
	{
		return bad_uuid;
	}
	
	if (0 == PQEnqueue(sched->pq, task))
	{
		ret_uid = TaskGetUID(task);
	}
	
	return ret_uid; 
}


int SchedRemoveTask(sched_t *sched, uuid_t uuid)
{
	task_t *task_to_rem = NULL;
	int is_found  = 0;
	
	assert (NULL != sched);
	
	task_to_rem = PQErase(sched->pq, &uuid, IsSameTask);
	
	is_found = (NULL == task_to_rem);
	
	if (0 == is_found)
	{
		TaskDestroy(task_to_rem);
	}
	
	return is_found;
}


void SchedRun(sched_t *sched)
{
	assert (NULL != sched);
	
	sched->stop_flag = 0;
	
	while ( !sched->stop_flag && !SchedIsEmpty(sched) )
	{
		task_t *curr_task = (task_t*)PQDequeue(sched->pq);
		
		while (TaskGetStartTime(curr_task) > time(NULL))
		{
			/* empty loop */
		}
		
		if (0 != TaskRunOperation(curr_task))
		{
			curr_task = TaskUpdateStartTime(curr_task);
			PQEnqueue(sched->pq, curr_task); 
		}
		
		else
		{
			TaskDestroy(curr_task);
		}
	}
}


int SchedStop(sched_t *sched)
{
	assert (NULL != sched);
	
	sched->stop_flag = 1;
	
	return 0;
}


/******************************************************************************/

static int StartTimeCmp(const void *task1, const void *task2)
{
	assert (NULL != task1);
	assert (NULL != task2);
	
	return TaskGetStartTime((task_t*)task1) - TaskGetStartTime((task_t*)task2);
}


static int IsSameTask(const void *task, const void *uuid)
{
	assert (NULL != task);
	assert (NULL != uuid);
	
	return IsSameUuid(*(uuid_t*)task,*(uuid_t*)uuid);
}


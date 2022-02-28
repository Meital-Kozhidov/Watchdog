/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Keren Robbins
 * Date: August 15th, 2021
 
 * Description: Scheduler implemantation
 *
 * Infinity Labs OL108
*******************************************************************************/

#ifndef __SCHEDULER_H_OL108_ILRD__
#define __SCHEDULER_H_OL108_ILRD__

#include <time.h>

#include "uid.h"

typedef struct scheduler sched_t;

typedef int (*sched_operation_func_t) (void *);
typedef void (*sched_cleanup_func_t) (void *);

/******************************************************************************/

/**
 * @Description: Creates a new empty scheduler.
 * @Parameters: void.
 * @Return: A pointer to the new scheduler that was created. Returns NULL if the
 *			creation fails.
 * @Complexity: O(1).
**/
sched_t *SchedCreate(void);


/**
 * @Description: Destroy the given scheduler and all the tasks in it.
 * @Parameters: A pointer to a scheduler.
 * @Return: void.
 * @Complexity: O(n) - where n is the number of tasks in the scheduler.
**/
void SchedDestroy(sched_t *sched);


/**
 * @Description: Creates a new Task and add it to the schduler.
 * @Parameters: The scheduler that the new task will be added to.
 *				The operation function of the new task, a time to start, the
 *				length of time between two operations of the task, void pointer
 *				to the argumnets that is given to the operation function, and a 
 *				clean function that will run and clean all what the function did.
 * @Return: The uuid of the new task. Returns bad_uuid if the creation of the
 *			new task fails.
 * @Complexity: O(n) - where n is the number of tasks in the scheduler.
**/
uuid_t SchedAddTask(sched_t *sched, sched_operation_func_t operation_func
		, time_t start_time, time_t time_interval, void *args
		, sched_cleanup_func_t cleanup_func);


/**
 * @Description: remove task with given uuid in given scheduler.
 * @Parameters: A pointer to a scheduler and a uuid (of a task).
 * @Return: 0 if the task was found (and destroyed), 1 otherwise.
 * @Complexity: O(n) - where n is the number of tasks in the scheduler.
**/
int SchedRemoveTask(sched_t *sched, uuid_t uuid);


/**
 * @Description: Runs all the tasks in the scheduler till the scheduler is empty
 *				 or there is an appearance of a Stop Task.
 * @Parameters: A pointer to a scheduler.
 * @Return: Nothing.
 * @Complexity: O(n*m) - where n is the number of tasks in the scheduler, and m
 *				is the maximum times that a task will run.
**/
void SchedRun(sched_t *sched);


/**
 * @Description: A function that can be added to the scheduler as an operation_func
 *				 of a task to stop the scheduler from continuing running.
 * @Parameters: A pointer to a scheduler.
 * @Return: int of zero (no repetitions for this task).
 * @Complexity: O(1).
**/
int SchedStop(sched_t *sched);


/**
 * @Description: Checks whether a given scheduler is empty or not.
 * @Parameters: A pointer to a scheduler.
 * @Return: 1 if the scheduler is empty, 0 otherwise.
 * @Complexity: O(1).
**/
int SchedIsEmpty(const sched_t *sched);


/**
 * @Description: Returns the size of a given scheduler.
 * @Parameters: A pointer to a scheduler.
 * @Return: The size of the given scheduler.
 * @Complexity: O(n) - where n is the number of tasks in the scheduler.
**/
size_t SchedSize(const sched_t *sched);


/**
 * @Description: Remove and destroy all the tasks from the given scheduler.
 * @Parameters: A pointer to a scheduler.
 * @Return: void.
 * @Complexity: O(n) - where n is the number of tasks in the scheduler.
**/
void SchedClear(sched_t *sched);

#endif /* __SCHEDULER_H_OL108_ILRD__ */

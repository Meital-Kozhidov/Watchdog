/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Keren Robbins
 * Date: August 15th, 2021
 
 * Description: Task implemantation
 *
 * Infinity Labs OL108
*******************************************************************************/

#ifndef __TASK_H_OL108_ILRD__
#define __TASK_H_OL108_ILRD__

#include <time.h> /* time_t */

#include "uid.h"


typedef struct task task_t;

/**
 * @Description: Function pointer type to a task operation function.
 * @Parameters: A void pointer to the arguments.
 * @Return: non-zero if the task should be repeated, otherwise zero.
**/
typedef int (*task_operation_func_t) (void *);


/**
 * @Description: Function pointer type to a task cleanup function (will be run
 *               when the task will be destroyed).
 * @Parameters: A void pointer to the arguments.
 * @Return: None.
**/
typedef void (*task_cleanup_func_t) (void *);

/******************************************************************************/


/**
 * @Description: Creates a new Task.
 * @Parameters: The operation function of the new task, a time to start, the
 *				length of time between two operations of the task, void pointer
 *				to the argumnets that is given to the operation function, and a 
 *				clean function that will run and clean all what the function did.
 * @Return: A pointer to the new task that was created. Returns NULL if the
 *			creation fails.
 * @Complexity: O(1).
**/
task_t *TaskCreate(task_operation_func_t operation, time_t start_time
		, time_t time_interval, void *args, task_cleanup_func_t cleanup);


/**
 * @Description: Run the cleanup function of the task and destroy it.
 * @Parameters: A pointer to a task.
 * @Return: Nothing.
 * @Complexity: O(n) (or the complexity of the cleanup function)
**/		
void TaskDestroy(task_t *task);


/**
 * @Description: Gets a task and returns its uuid.
 * @Parameters: A pointer to a task.
 * @Return: The given task's uuid.
 * @Complexity: O(1)
**/		
uuid_t TaskGetUID(const task_t *task);


/**
 * @Description: Gets a task and returns its start time.
 * @Parameters: A pointer to a task.
 * @Return: The given task's start time.
 * @Complexity: O(1)
**/		
time_t TaskGetStartTime(const task_t *task);


/**
 * @Description: Run the given task's operation function with its arguments.
 * @Parameters: A pointer to a task.
 * @Return: The operation function return.
 * @Complexity: The complexity of the operation function.
**/		
int TaskRunOperation(const task_t *task);


/**
 * @Description: Update the start time of a task to be the 
 *				 current time + interval time.
 * @Parameters: A pointer to a task.
 * @Return: The pointer to the task.
 * @Complexity: O(1).
**/	
task_t *TaskUpdateStartTime(task_t *task);

#endif /* __TASK_H_OL108_ILRD__ */

/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog
 *
 * Infinity Labs OL108
*******************************************************************************/
#ifndef __WATCHDOG_H_OL107_8_ILRD__
#define __WATCHDOG_H_OL107_8_ILRD__

#include <pthread.h> /* pthread_t */
#include <time.h> /* time_t */

typedef enum
{
	SUCCESS,
	WATCHDOG_CLOSE_FAIL,
	THREAD_CLOSE_FAIL,
	BOTH_CLOSE_FAIL
} watchdog_status_t;

typedef struct
{
	const char *watchdog_path;
	const char *process_path;
	char **argv;
	char **envp;
	time_t signal_to_wd_interval;
	time_t signal_from_wd_interval;
	int signal_to_wd_miss_limit;
	int signal_from_wd_miss_limit;
} watchdog_data_t;


/**
 * @Description: Starts a watchdog process to revive the calling process if it
 *               terminates.
 * @Parameters: wd_data - a pointer to a watchdog data struct, indicating the
 *              signal intervals & miss limits (a miss limit of 1 means the
 *              process tolarates one missed signal), argv, envp, and the path
 *              of both executable files.
 * @Return: Thread ID of the thread created to ensure the watchdog process keeps
 *          running, or -1 in case of error.
 * @Notes: SIGUSR1 and SIGUSR2 will be blocked for the calling process, behaviour 
 *		   if another process sends the calling process SIGUSR1 is undefined.
 *         Behaviour if SIGUSR1 or SIGUSR2 are sent to the watchdog process is
 *         undefined.
**/
pthread_t StartWatchDog(const watchdog_data_t *wd_data);


/**
 * @Description: Ends the watchdog process.
 * @Parameters: watchdog_thread_id - the thread ID returned by StartWatchDog.
 * @Return: Status of closing the thread & process.
 * @Notes: SIGUSR1 and SIGUSR2 will be unblocked for the calling process.
**/
watchdog_status_t EndWatchDog(pthread_t watchdog_thread_id);

#endif /* __WATCHDOG_H_OL107_8_ILRD__ */

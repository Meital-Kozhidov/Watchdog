/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog : shared api between watchdog and user
 *
 * Infinity Labs OL108
*******************************************************************************/
#include <signal.h>     /* pthread_sigmask(), sigaddset(), sigemptyset(),
                        sigtimedwait(), SIG_BLOCK, SIGUSR1 */
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* getenv(), setenv() */
#include <time.h>       /* time_t */

#include "wd_user_process.h"
#include "wd_shared_api.h"
/******************************************************************************/
int SendSignalTask(void *arg)
{
    pid_t pid = *(pid_t*)arg;

    if(stop_flag)
    {
        return 0;
    }  

    printf("pid : %d sent\n", pid);
    kill(pid, SIGUSR1);
    
    return 1;
}


void InitSched(sched_t *sched, watchdog_data_t *wd_data, pid_t *pid, time_t send_interval, time_t rec_interval, receive_sig_t ReceiveSignalTask)
{
    SchedAddTask(sched, SendSignalTask, time(NULL) + send_interval, send_interval, pid, CleanUp);
    
    SchedAddTask(sched, ReceiveSignalTask, time(NULL) +
    rec_interval, rec_interval, wd_data, CleanUp);
}


void CleanUp(void *args)
{
    UNUSED(args);
}


int SetSignalMask(sigset_t *set)
{
    if (-1 == sigemptyset(set))
    {
        return -1;
    }
    if (-1 == sigaddset(set, SIGUSR1))
    {
        return -1;
    }

    return pthread_sigmask(SIG_BLOCK, set, NULL);
}


void StopSignalHandler(int signum)
{
    UNUSED(signum);
    stop_flag = 1;
}

/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog : user process
 *
 * Infinity Labs OL108
 * 
 * instruction :
 * compile with - 
 * gd wd_user_process.c wd_user_process_test.c wd_shared_api.c -pthread -o wd_user_process.out -I include src/scheduler.c src/task.c src/uid.c src/priority_queue.c src/heap.c src/dynamic_vector.c
*******************************************************************************/
#define _POSIX_SOURCE 
#define _GNU_SOURCE

#include <signal.h>     /* pthread_sigmask(), sigaddset(), sigemptyset(),
                        sigtimedwait(), SIG_BLOCK, SIGUSR1, SIGUSR2, 
                        struct sigaction, kill() */
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* setenv() */
#include <fcntl.h>      /* O_CREAT */
#include <semaphore.h>  /* sem_t, sem_open(), sem_wait(), sem_post()*/
#include <string.h>     /* memset() */
#include <unistd.h>		/* getpid() */

#include "scheduler.h" /* timing signal sending */
#include "wd_user_process.h"
#include "wd_shared_api.h"

/******************************************************************************/
static void *ProtectWdThread(void* args);
int ReceiveOperation(void *arg);
static int SetEnvpFromWdData(const watchdog_data_t *wd_data);
static void InitSem(void);
/******************************************************************************/
sigset_t set = {0};
pid_t child_pid = 0;

int misses = 0;
/******************************************************************************/
pthread_t StartWatchDog(const watchdog_data_t *wd_data)
{
    pid_t pid = 0;
    pthread_t aux_thread = -1;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = &StopSignalHandler;
    sigaction(SIGUSR2, &sa, NULL);
    stop_flag = 0;

    pid = fork();

    if (0 > pid)
    {
        return aux_thread;
    }
    
    if (0 == pid)
    {
        SetEnvpFromWdData(wd_data);
        execvp(wd_data->watchdog_path, wd_data->argv); 

        printf("error in exec\n");
    }
    else
    {
        if (-1 == SetSignalMask(&set))
        {
            printf("signals error\n");
            return aux_thread;
        }
        child_pid = pid;

        InitSem();

        pthread_create(&aux_thread, NULL, ProtectWdThread, (watchdog_data_t *) wd_data);            
    }

    return aux_thread;
}


watchdog_status_t EndWatchDog(pthread_t watchdog_thread_id)
{	
    if (-1 == kill(getpid(), SIGUSR2))
    {
        return BOTH_CLOSE_FAIL;
    }
    if (-1 == kill(child_pid, SIGUSR2))
    {
        return WATCHDOG_CLOSE_FAIL;
    }
    if (0 != pthread_join(watchdog_thread_id, NULL))
    {
        return THREAD_CLOSE_FAIL;
    }
    
    pthread_sigmask(SIG_UNBLOCK, &set, NULL);

    return SUCCESS;
}

/******************************************************************************/
static void *ProtectWdThread(void* args)
{
    watchdog_data_t *wd = (watchdog_data_t*)args;
    sched_t *sched = NULL;

    sched = SchedCreate();
    if (NULL != sched)
    {
        InitSched(sched, wd, &child_pid, wd->signal_to_wd_interval, wd->signal_from_wd_interval, ReceiveOperation);

        SchedRun(sched);

        SchedDestroy(sched);
    }

    return NULL;
}


int ReceiveOperation(void *arg)
{
    watchdog_data_t *wd = (watchdog_data_t*)arg;
    struct timespec ts = {0,0};

    if(stop_flag)
    {
        return 0;
    }

    if (0 > sigtimedwait(&set, NULL, &ts))
    {
        ++misses;
        printf("USER missed %d\n", misses);

        if (misses == wd->signal_to_wd_miss_limit)
        {
            pid_t pid = fork();
            
            if (0 > pid)
            {
                return -1;
            }
            
            if (0 == pid)
            {
                SetEnvpFromWdData(wd);

                execvp(wd->watchdog_path, wd->argv);
            }
            else
            {
                child_pid = pid;
                misses = 0;
                
				InitSem();
            }
        }
    }

    else
    {
        printf("USER received\n");
        misses = 0;
    }

    return 1;
}


static void InitSem(void)
{
	sem_t *sem1 = sem_open("watchdog1", O_CREAT, 0666, 0);
    sem_t *sem2 = sem_open("watchdog2", O_CREAT, 0666, 0);
    
    sem_wait(sem1);
    sem_post(sem2);
        
    sem_close(sem1);
    sem_close(sem2);
    sem_unlink("watchdog1");
    sem_unlink("watchdog2");
}


static int SetEnvpFromWdData(const watchdog_data_t *wd_data)
{
    int is_set = 0;
    char buffer[20];

    is_set += setenv("watchdog_path", wd_data->watchdog_path, 1);
    is_set += setenv("process_path", wd_data->process_path, 1);
    sprintf(buffer, "%ld", wd_data->signal_to_wd_interval);
    is_set += setenv("signal_to_wd_interval", buffer, 1);
    sprintf(buffer, "%ld", wd_data->signal_from_wd_interval);
    is_set += setenv("signal_from_wd_interval", buffer, 1);
    sprintf(buffer, "%d", wd_data->signal_to_wd_miss_limit);
    is_set += setenv("signal_to_wd_miss_limit", buffer, 1);
    sprintf(buffer, "%d", wd_data->signal_from_wd_miss_limit);
    is_set += setenv("signal_from_wd_miss_limit", buffer, 1);

    return is_set;
}

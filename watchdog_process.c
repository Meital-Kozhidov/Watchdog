/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog : watchdog process
 *
 * Infinity Labs OL108
 * 
 * instruction :
 * compile with - 
 * gd watchdog_process.c wd_shared_api.c -pthread -o watchdog_process -I include src/scheduler.c src/task.c src/uid.c src/priority_queue.c src/heap.c src/dynamic_vector.c
*******************************************************************************/
#define _POSIX_C_SOURCE  199309L

#include <signal.h>  /* pthread_sigmask(), sigaddset(), sigemptyset(),
                        sigtimedwait(), SIG_BLOCK, SIGUSR1, kill() */
#include <stdio.h>   /* printf() */
#include <stdlib.h>  /* getenv() */
#include <fcntl.h>   /* O_CREAT */
#include <semaphore.h> /* sem_t, sem_open(), sem_wait(), sem_post() */
#include <unistd.h>	/* getppid() */

#include "scheduler.h" /* timing signal sending */
#include "wd_user_process.h"
#include "wd_shared_api.h"
/******************************************************************************/
static void *RunScheduler(void *args);
static void GetWDDataFromEnvp(watchdog_data_t *wd_data);
int ReciveSignalTask(void *args);

int missed = 0;
sigset_t set = {0};
pid_t ppid = 0;
/******************************************************************************/
int main(int argc, char *argv[], char *envp[])
{
    watchdog_data_t wd_data;

    printf("watchdog_process start\n");
    wd_data.argv = argv;
    wd_data.envp = envp;

    stop_flag = 0;
    
    SetSignalMask(&set);
    
    GetWDDataFromEnvp(&wd_data);
    RunScheduler(&wd_data);

    UNUSED(argc);

    return 0;
}


static void *RunScheduler(void *args)
{
    watchdog_data_t *wd = (watchdog_data_t *)args;
    sched_t *sched = SchedCreate();

    if (NULL != sched)
    {
        sem_t *sem1 = sem_open("watchdog1", O_CREAT, PERMISSION_ALL, 0);
        sem_t *sem2 = sem_open("watchdog2", O_CREAT, PERMISSION_ALL, 0);

        ppid = getppid();

        InitSched(sched, wd, &ppid, wd->signal_from_wd_interval, wd->signal_to_wd_interval, ReciveSignalTask);

        sem_post(sem1);
        sem_wait(sem2);
        
        sem_close(sem1);
        sem_close(sem2);
        sem_unlink("watchdog1");
        sem_unlink("watchdog2");

        SchedRun(sched);

        SchedDestroy(sched);
    }

    return args;
}


int ReciveSignalTask(void *args)
{
    watchdog_data_t *wd_data = (watchdog_data_t *)args;
    struct timespec ts = {0,0};

    if(stop_flag)
    {
        return 0;
    }

    if (0 > sigtimedwait(&set, NULL, &ts))
    {
        ++missed;
        printf("WD missed %d\n", missed);

        if (wd_data->signal_from_wd_miss_limit == missed)
        {
            execvp(wd_data->process_path, wd_data->argv);

            return 0;
        }
    }
    else
    {
        printf("WD received\n");
        missed = 0;
    }
    
    return 1;
}


static void GetWDDataFromEnvp(watchdog_data_t *wd_data)
{
    wd_data->watchdog_path = getenv("watchdog_path");
    wd_data->process_path = getenv("process_path");

    wd_data->signal_to_wd_interval = atol(getenv("signal_to_wd_interval"));
    wd_data->signal_from_wd_interval = atol(getenv("signal_from_wd_interval"));
    wd_data->signal_to_wd_miss_limit = atoi(getenv("signal_to_wd_miss_limit"));
    wd_data->signal_from_wd_miss_limit = atoi(getenv ("signal_from_wd_miss_limit"));
}

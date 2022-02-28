/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog : user process
 *
 * Infinity Labs OL108
*******************************************************************************/
#include <assert.h>     /* assert() */
#include <unistd.h>     /* sleep() */
#include <stdlib.h>     /* system() */
#include <stdio.h>      /* printf() */

#include "wd_user_process.h"

#define UNUSED(a) (void)a
/******************************************************************************/
static void Wait(time_t time);
static void BasicTest(char **argv, char **envp);
static void InfiniteLoopTest(char **argv, char **envp);
/******************************************************************************/
int main(int argc, char **argv, char **envp)
{
    UNUSED(argc);

    BasicTest(argv, envp);
    InfiniteLoopTest(argv, envp);

    return 0;
}


static void BasicTest(char **argv, char **envp)
{
    pthread_t thread = {0};

    watchdog_data_t wd_data = {0};
    wd_data.process_path = "/home/meital/git/ds/wd_user_process.out";
    wd_data.watchdog_path = "/home/meital/git/ds/watchdog_process";
    wd_data.argv = argv;
    wd_data.envp = envp;
    wd_data.signal_from_wd_interval = 1;
    wd_data.signal_to_wd_interval = 1;
    wd_data.signal_from_wd_miss_limit = 5;
    wd_data.signal_to_wd_miss_limit = 3;

    thread = StartWatchDog(&wd_data);
    Wait(20);
    printf("open processes:\n");
    system("ps -a");

    printf("let's stop\n");
    assert(SUCCESS == EndWatchDog(thread));
    Wait(10);
    system("ps -a");
    printf("\nSUCCESS!\n\n");
}


static void InfiniteLoopTest(char **argv, char **envp)
{
    watchdog_data_t wd_data = {0};
    wd_data.process_path = "/home/meital/git/ds/wd_user_process.out";
    wd_data.watchdog_path = "/home/meital/git/ds/watchdog_process";
    wd_data.argv = argv;
    wd_data.envp = envp;
    wd_data.signal_from_wd_interval = 1;
    wd_data.signal_to_wd_interval = 1;
    wd_data.signal_from_wd_miss_limit = 5;
    wd_data.signal_to_wd_miss_limit = 3;

    printf("\n\nThis Test is loop, look for printf that never printed\n");
    printf("Use 'killall wd_process wd_user_process' to finish the run\n\n");

    StartWatchDog(&wd_data);
    Wait(10);
    printf("open processes:\n");
    system("ps -a");

    printf("let's kill watchdog\n");
    system("killall wd_process");
    Wait(20);
    printf("Now the open processes are:\n");
    system("ps -a");
    
    printf("let's kill user's process\n");
    system("killall wd_user_process");
    Wait(20);
    printf("Won't print that\n");
    printf("Now the open processes are:\n");
    system("ps -a");
}


static void Wait(time_t time_to_wait)
{
    while(time_to_wait > 0)
    {
        time_to_wait = sleep(time_to_wait);
    }
}

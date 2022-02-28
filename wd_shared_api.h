/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog : shared api between watchdog and user
 *
 * Infinity Labs OL108
*******************************************************************************/
#ifndef __WATCHDOG_SHARED_H_OL107_8_ILRD__
#define __WATCHDOG_SHARED_H_OL107_8_ILRD__

#include "scheduler.h"
#include "wd_shared_api.h"

typedef int (*receive_sig_t) (void*);

int stop_flag;

#define UNUSED(x) (void)(x)

#define PERMISSION_ALL 0666

int SendSignalTask(void *arg);
int SetSignalMask(sigset_t *set);
void InitSched(sched_t *sched, watchdog_data_t *wd_data, pid_t *pid, 
    time_t send_interval, time_t rec_interval, receive_sig_t ReceiveSignalTask);
void CleanUp(void *args);
void StopSignalHandler(int signum);

#endif /* __WATCHDOG_SHARED_H_OL107_8_ILRD__ */

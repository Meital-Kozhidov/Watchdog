/*******************************************************************************
 * Author: Meital Kozhidov
 * Date: October 20th, 2021
 
 * Description: watchdog
 *
 * Infinity Labs OL108
*******************************************************************************/
#include <stdio.h>

#include "wd_user_process.h"

#define UNUSED(x) (void)(x)

int main(int argc, char *argv[], char *envp[])
{
    watchdog_data_t wd_data;
    wd_data.argv = argv;
    wd_data.envp = envp;
    wd_data.signal_from_wd_interval = 5;
    wd_data.signal_to_wd_interval = 5;
    wd_data.signal_to_wd_miss_limit = 4;
    wd_data.signal_from_wd_miss_limit = 4;
    wd_data.process_path = "/home/meital/git/ds/wd_user_process.out";
    wd_data.watchdog_path = "/home/meital/git/ds/watchdog_process";

    StartWatchDog(&wd_data); 
    printf("wd_user_process start\n");
    while (1)
    {
        
    }

    UNUSED(argc);
    
    return 0;
}

# Watchdog Project -  C

```diff
@@ Written By Meital Kozhidov @@
@@ OL108, Infinity Labs R&D @@
@@ Israel, August 2021 @@
```

## Overview
- Watchdog is an API for automatically 'reviving' a program when it is terminated abruptly.
- The watchdog uses multithreading for communication.
- If the watchdog is terminated - it is also revived automatically.
- Uses signals for communication between watchdog and user processes {SIGUSR1 & SIGUSR2}
- Written in C (developed on Linux, Ubuntu)

## How to compile
- The `watchdog` process: 
```sh
gcc -ansi -pedantic-errors -Wall -Wextra -g watchdog_process.c wd_shared_api.c -pthread -o 
watchdog_process -I include src/scheduler.c src/task.c src/uid.c src/priority_queue.c src/heap.c src/dynamic_vector.c
```
- The `user` process : 
```sh
gcc -ansi -pedantic-errors -Wall -Wextra -g wd_user_process.c test/wd_user_process_test.c wd_shared_api.c -pthread -o 
wd_user_process.out -I include src/scheduler.c src/task.c src/uid.c src/priority_queue.c src/heap.c src/dynamic_vector.c
```
> Note: test/wd_user_process_test.c can be replaced with wd_user_process_test2 or any other respective test.

## How to run
* Run user program (`./wd_user_process.out`)
* (On another terminal) Run Watchdog program (`./watchdog_process`)

## TODO
* makefile

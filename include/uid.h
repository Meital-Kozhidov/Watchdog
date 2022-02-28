/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Liel Sananes
 * Date: August 11th, 2021
 
 * Description: UID implementation
 *
 * Infinity Labs OL108
*******************************************************************************/

#ifndef __UID_H_OL108_ILRD__
#define __UID_H_OL108_ILRD__

#include <time.h> 		 /* time(), time_t */
#include <sys/types.h>  /* pid_t		  */
#include <unistd.h>    /* getpid()		 */

typedef struct uuid
{
	time_t time;
	size_t id;
	pid_t pid;
}uuid_t;

/* represents a 'bad' uid - where the time function failed */
extern const uuid_t bad_uuid;

/**
 * @Description: Creates a unique uid. 
 * @Parameters: void.
 * @Return: the uid (uuid_t type).
 * @Complexity: O(1).
**/
uuid_t UuidCreate(void);


/**
 * @Description: Compares two uid's. 
 * @Parameters: uuid1 - the first uid.
 				uuid2 - the second uid.
 * @Return: One if they are identical, zero if not.
 * @Complexity: O(1).
**/
int IsSameUuid(uuid_t uuid1, uuid_t uuid2);

#endif /*  __UID_H_OL108_ILRD__ */

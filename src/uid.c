/*******************************************************************************
 * Author: Meital Kozhidov
 * Reviewer: Liel Sananes
 * Date: August 11th, 2021
 
 * Description: UID implementation
 *
 * Infinity Labs OL108
*******************************************************************************/

#include "uid.h"

const uuid_t bad_uuid = {((time_t)-1), 0, 0};

uuid_t UuidCreate(void)
{
	uuid_t uid;
	static size_t counter = 1;
	
	uid.time = time(NULL);
	if (uid.time == bad_uuid.time)
	{
		return bad_uuid;
	}
	
	uid.id = counter;
	uid.pid = getpid();
	
	++counter;
	
	return uid;
}


int IsSameUuid(uuid_t uuid1, uuid_t uuid2)
{
	int res = (uuid1.time == uuid2.time);
	res &= (uuid1.id == uuid2.id);
	res &= (uuid1.pid == uuid2.pid);
	
	return res;
}

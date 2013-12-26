#include <time.h>

#include "Containers.hpp"

Containers::Date::Date()
{}
Containers::Date::Date(tm timeStruct):
timeStruct(timeStruct)
{
	
}

std::string Containers::Date::formatDate(const char* format) const
{
	char stackBuff[100], *heapBuff = stackBuff;
	size_t buffSize = 100;
	
	while(strftime(heapBuff, buffSize, format, &this->timeStruct) == 0)
	{
		if (stackBuff != heapBuff)
		{
			delete[] heapBuff;
		}
		
		buffSize += 100;
		heapBuff = new char[buffSize];
	}
	
	std::string temp(heapBuff);
	if (stackBuff != heapBuff)
	{
		delete[] heapBuff;
	}
	
	return temp;
}

unsigned int Containers::Date::getUnixTimestamp() const
{
	struct tm unixEpoch;
	unixEpoch.tm_hour = 0;
	unixEpoch.tm_min = 0;
	unixEpoch.tm_sec = 0;
	unixEpoch.tm_year = 70;
	unixEpoch.tm_mon = 0;
	unixEpoch.tm_mday = 1;
	
	return difftime(mktime((tm*) &this->timeStruct), mktime(&unixEpoch));
}

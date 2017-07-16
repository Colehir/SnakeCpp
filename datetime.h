#pragma once
#include <time.h>
#include <string>
#include "misc.h"
using namespace std;

/*
DateTime is a object oriented wrapper for the old C style time stuff.
Want to understand time.h?
http://www.tutorialspoint.com/c_standard_library/time_h.htm
*/


class DateTime
{
public:
	void setToCurrentTime()
	{
		//rawTime = time(nullptr);
		//timeStr = asctime(localtime(&rawTime));

		////Uses the safe versions of localtime and asctime (C is a pain!)
		char buffer[256];
		rawTime = time(nullptr);
		localtime_s(&timeObj, &rawTime);
		asctime_s(buffer, &timeObj);
		timeStr = buffer;

	}
	string getTimeStr()
	{
		return timeStr;
	}
	string getHour()
	{
		return timeStr.substr(11, 2);
	}
	string getMinute()
	{
		return timeStr.substr(14, 2);
	}
	string getDayOfWeek()
	{
		return timeStr.substr(0, 3);
	}



	string getDate()
	{
		return "You have to write this function!";
	}
	
	
	
	
	string getTime()
	{
		int hour = stringToInt(getHour());
		string minute = getMinute();
		string tag = " a.m.";
		if (hour == 0)
		{
			hour = 12;
		}
		else if (hour >= 12)
		{
			if (hour > 12)hour -= 12;
			tag = " p.m.";
		}
		string result = intToString(hour) + ":" + minute + tag;
		return result;
	}
	int getSec()
	{
		setToCurrentTime();
		return stringToInt(timeStr.substr(19,20));
	}

	//0123456789X123456789X12345
	//Tue Feb  2 13:21 : 33 2016

	// don't worry about this for now
	long long int operator - (const DateTime & rhs)
	{
		return rawTime - rhs.rawTime;
	}



private:
	time_t rawTime; 
	tm timeObj;
	string timeStr;
};


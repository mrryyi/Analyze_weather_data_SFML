#include "pch.h"
#include "TimeOfDay.h"


TimeOfDay::TimeOfDay(std::string time)
{
	//separateStringIntoIntegers(time, ':', 3, &h, &m, &s);

	std::istringstream ss(time);
	std::string temporary;
	std::getline(ss, temporary, ':');
	this->timeInSeconds = stoi(temporary) * 3600;
	std::getline(ss, temporary, ':');
	this->timeInSeconds += stoi(temporary) * 60;
	std::getline(ss, temporary);
	this->timeInSeconds += stoi(temporary);
}


TimeOfDay::TimeOfDay(int sec) {
	
	this->timeInSeconds = sec;

}


TimeOfDay::~TimeOfDay()
{
}



int TimeOfDay::getHours() { return this->timeInSeconds / 3600; };
int TimeOfDay::getMinutes() { return (this->timeInSeconds / 60) % 60; };
int TimeOfDay::getSeconds() { return (this->timeInSeconds % 3600) % 60; };



// hh:mm:ss. "01:00:00" instead of "1:0:0".
std::string TimeOfDay::getStr() {
	std::string str;
	int h = this->getHours();
	int m = this->getMinutes();
	int s = this->getSeconds();
	if (h < 10)
		str += '0';
	str += std::to_string(h);
	str += ':';
	if (m < 10)
		str += '0';
	str += std::to_string(m);
	str += ':';
	if (s < 10)
		str += '0';
	str += std::to_string(s);
	return str;
}


int TimeOfDay::getTotalTimeInSeconds() { return this->timeInSeconds; };

// this minus compare. difference is signed.
int TimeOfDay::difference(TimeOfDay* compare) {
	return this->timeInSeconds - compare->getTotalTimeInSeconds();
}

bool TimeOfDay::isMoreThan(TimeOfDay* compare) {


	/*
	difference > 0:
	this value is higher than compare.
		this | isMoreThan | compare.
		return true.
		differende !> 0:
	this value is equal or below compare.
		this | isNotMoreThan | compare.
		return false.
	*/
	if (difference(compare) > 0)
		return true;
	else
		return false;
}
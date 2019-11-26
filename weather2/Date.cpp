#include "pch.h"
#include "Date.h"

Date::Date(std::string date)
{
	//separateStringIntoIntegers(date, '-', 3, &this->year, &this->month, &this->day);

	// String stream based on string date. Assumed format: "...y-mm-dd"
	std::istringstream ss(date);
	std::string temporary;
	
	std::getline(ss, temporary,'-');
	this->year = stoi(temporary);

	std::getline(ss, temporary, '-');
	this->month = stoi(temporary);

	std::getline(ss, temporary);
	this->day = stoi(temporary);
}
Date::Date(int year, int month, int day) : year(year), month(month), day(day){};

Date::~Date()
{
}


bool Date::isMoreThan(Date* compare) {

	// Compares each hierarchy of value in turn
	// Could be done with this->difference().
	// Why is it not done with this->difference()?
	// Because this->difference() needs to take into
	// account a lot more information in order to find
	// the exact difference between two days.
	// We just need to know above/below, which is what we check
	// in turn.
	bool more = false;

	if (this->year > compare->getYear()) {
		more = true;
	}
	else if (this->month > compare->getMonth() && this->year == compare->getYear()) {
		more = true;
	}
	else if (this->day > compare->getDay() && this->month == compare->getMonth()) {
		more = true;
	}

	return more;
}


// This function returns number of days between two given 
// dates 
int Date::difference(Date* compare)
{
	int monthDays[12] = { 31, 28, 31, 30, 31, 30,
					   31, 31, 30, 31, 30, 31 };

	// initialize count using years and day 
	long int countThis = this->year * 365 + this->day;

	// Add days for months in given date 
	// this->month is in range 1->12, but array is in range 0->11.
	for (int i = 0; i < this->month - 1; i++)
		countThis += monthDays[i];

	// Every leap ear consists of 366 days, whereas a regular year is 365 days.
	// Therefore, we add one day per leap year.
	countThis += calculateLeapYears(this->year, this->month);

	// Count total number of days for the compared date in a similar manner. 
	long int countCompare = compare->getYear() * 365 + compare->getDay();
	for (int i = 0; i < compare->getMonth() - 1; i++)
		countCompare += monthDays[i];
	countCompare += calculateLeapYears(compare->getYear(), compare->getMonth());

	// if this is one day above the compare date, then this should return a positive one.
	return (countThis - countCompare);
}





std::string Date::getDateAsString() {

	/*
	Returns the date represented as a string.
	Returns in the format of yyyy-mm-dd
	1-1-1 becomes 0001-01-01
	932-4-5 becomes 0932-04-05
	etc.
	*/
	std::string str;

	// This if-elseif-elseif tree could surely be
	// made in a general way.
	if (this->year < 10)
		str += "0";
	else if (this->year < 100)
		str += "00";
	else if (this->year < 1000)
		str += "000";
	str += std::to_string(this->year);
	str += "-";
	if (this->month < 10)
		str += "0";
	str += std::to_string(this->month);
	str += "-";
	if (this->day < 10)
		str += "0";
	str += std::to_string(this->day);

	return str;
}
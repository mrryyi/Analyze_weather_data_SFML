#pragma once
class Date
{
	int year;
	int month;
	int day;
public:
	Date() {};

	// Initializable by string y...-m...-d...
	// And by y, m, d as three int parameters.
	// Weakness: Corrupt data with impossible dates.
	// Solution: Trust the data.
	Date(std::string date);
	Date(int year, int month, int day);
	~Date();
	
	// Simple return functions for the year, month and day.
	int getYear()	{ return this->year; };
	int getMonth()	{ return this->month; };
	int getDay()	{ return this->day; };

	// Gets the difference in days between two dates.
	// If this - compare = positive value, then this is higher.
	int difference(Date* compare);

	// Returns a string representation of the date.
	// Format: yyyy-mm-dd. yy2016 mm1 dd2 becomes 2016-01-02.
	std::string getDateAsString();

	// If this is more than compared date, then return true.
	bool isMoreThan(Date* compare);
};
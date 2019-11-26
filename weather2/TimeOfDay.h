#pragma once
class TimeOfDay
{
	int timeInSeconds;
public:
	TimeOfDay() {};
	TimeOfDay(std::string time);
	TimeOfDay(int sec);

	// String representation separated by ':'. "hh:mm:ss".
	// Fixes the str to account for values below 10, such that, for example, "1:0:0" becomes "01:00:00".
	std::string getStr();

	// Getter functions except they calculate the desired variable
	// with modulus and/or division.
	int getHours();
	int getMinutes();
	int getSeconds();
	// Getter for timeInSeconds.
	int getTotalTimeInSeconds();

	// The difference between a time of day and itself.
	// If this returns a positive value, then this is greater (or later in the day)
	// than the comparee.
	int difference(TimeOfDay* compare);


	/* Compares the difference between this and another time of day.
		If this is greater than the other time of day, return true. If equal or less, return false.
		Uses this->difference.
	*/
	bool isMoreThan(TimeOfDay* compare);

	~TimeOfDay();
};
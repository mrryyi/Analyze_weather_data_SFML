#include "pch.h"

/*
separateStringToIntegers:
	Separates a string into integers, given a separator.
Parameters:
	string str				-	Has data we want to split in to integers.
	char separator			-	What the data is separated by in str.'
	int numberOfTargets		-
	...						-	What to put the data into
Use:
	The caller gives a string, a separator, a limit and some pointers to change.
	The string is separated by the parameter separator,
	and the splat value is put into the value of a pointer as an integer.
	Stops at the limit, or when the getline fails to get anything.
	Thusly, some parameter pointers may not find a value from this function.
	It is up to the caller to find a solution if the string has no value for a parameter.
	Maybe to assume that the value is 0.
	The above assumption is okay for implementations of this function in this weather program.
		Why?
			Because if we have data in a file like this: 00:00
			And we also want seconds, then, to not get a value for seconds by this function
			is okay, because we will assume it to be zero, and thus will have no
			impact on comparison with other times. See class TimeOfDay.
			That leads to multifunctionality as we can both handle
			files with hours:minutes and hours:minutes:seconds

*/

void separateStringIntoIntegers(std::string str, char separator, int numberOfTargets, ...) {

	std::istringstream ss(str);
	std::string token;
	va_list arguments;
	va_start(arguments, numberOfTargets);
	int* target;
	int i = 0;
	while (std::getline(ss, token, separator) && i < numberOfTargets) {
		target = va_arg(arguments, int*);
		*target = std::stoi(token);
		i++;
	}
}


/*
Same as above but for strings.
*/

void separateStringIntoStrings(std::string str, char separator, int numberOfTargets, ...) {

	std::istringstream ss(str);
	std::string token;
	va_list arguments;
	va_start(arguments, numberOfTargets);
	std::string* target;
	int i = 0;
	while (std::getline(ss, token, separator) && i < numberOfTargets) {
		target = va_arg(arguments, std::string*);
		*target = token;
		i++;
	}
}



int calculateLeapYears(int y, int m) {
	if (m <= 2)
		y--;

	// An year is a leap year if it is a multiple of 4, 
	// multiple of 400 and not a multiple of 100. 
	return y / 4 - y / 100 + y / 400;
}

bool isLeapYear(int yy) {

	if (yy % 4 == 0)
	{
		if (yy % 100 == 0)
		{
			if (yy % 400 == 0) // is a leap year
				return true;
			else // is not a leap year
				return false;
		}
		else // is a leap year
			return true;
	}
	else // is a leap year
		return true;
}

// I copied this off the web. Forgive me.
inline bool isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

int getInteger(int low, int high) {
	bool correct;
	std::string attempt;
	do {
		correct = false;
		// first get an integer
		do {
			std::cin >> attempt;
			if (isInteger(attempt))
				correct = true;
		} while (not correct);
		// then compare the integer against the range,
		// as we know it is now in the format we want.
	} while ( stoi(attempt) < low || stoi(attempt) > high );


	return stoi(attempt);
}

float moldRiskLimit(float t) {
	return (float) ( -0.0015*pow(t, 3) + 0.1193*pow(t, 2) - 2.9878*t + 102.96);
}

float moldRiskDiff(float t, float h) {
	// If humidity rises above the mold risk limit, there is a mold risk index above zero.
	return h - moldRiskLimit(t);
}

bool intIsInRange(int what, int low, int high) {
	return (what >= low) && (what <= high);
}
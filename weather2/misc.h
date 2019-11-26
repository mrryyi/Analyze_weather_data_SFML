/*
Retired function:
	( not used as it wasn't effective, just comfortable and general. )

separateStringToIntegers:
	Separates a string into integers, given a separator.
Parameters:
	string str				-	Has data we want to split in to integers.
	char separator			-	What the data is separated by in str.'
	int numberOfTargets		-
	...						-	What to put the data into
*/

void separateStringIntoIntegers(std::string str, char separator, int numberOfTargets, ...);
/*
Same as above but for strings.
*/
void separateStringIntoStrings(std::string str, char separator, int numberOfTargets, ...);

/*Calculates the amount of leap years in a given year.*/
int calculateLeapYears(int y, int m);

/*Checks if the given year is a leap year.*/
bool isLeapYear(int yy);

/*Checks if string is integer*/
inline bool isInteger(const std::string & s);

/*Gets an integer from the user within a given range.*/
int getInteger(int low, int high);

/*Returns mold risk limit for a given temperature.*/
float moldRiskLimit(float t);

/*Returns mold risk difference between hum and index.*/
float moldRiskDiff(float t, float h);
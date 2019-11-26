#pragma once
class Manager
{
	// Day pointer vector to contain every day in the data.
	std::vector<Day*> days;
	// These give the indication if things we haven't calculated should be printed.
	// Also so things are calculated only once if we've already done it.
	// Just as a safeguard.
	bool daysAveraged;
	bool moldCalculated;
	
	// The two components of mergeSort.
	// mergeSort calls itself and divides into smaller and smaller
	// pieces (left and right) and merges
	// its subsections with merge when the division is complete.
	void merge(Day ** arrr, int low, int middle, int high);
	void mergeSort(Day ** arrr, int low, int high);

	// Made temporarily, so that we do not have to have two extra arguments.
	// We'd have to pass these two each merge and mergeSort call, like this:
	//
	//	void merge(Day ** arrr, int low, int middle, int high, SORTTYPE sortType, bool hilo);
	//	void mergeSort(Day ** arrr, int low, int high, SORTTYPE sortType, bool hilo);
	//
	// It's unnecessary.
	// So what we do, is store them temporarily in Manager.
	// This leads to the inability to use mergeSort or merge as global functions, as
	// they rely on these members to let the Day* sortees know how to compare themselves.
	SORTTYPE sortType;
	bool hilo;

	// Gets user input for date. Checks if it's a valid date.
	Date * getInputDate();

	// Searches for a date and returns the index in the days vector.
	int binaryDaySearch(int low, int high, Date * dateToFind);

	// Calculates the date for a meteorological season if there is such a thing, and
	// saves the information in the season related member
	void calculateAutumnOrWinter(float threshold, Date* seasonStart);
	// Divided into two functions such that calculateMeteorologicalSeason

	Date autumnDate;
	Date winterDate;

public:

	// Reads a file at constructor because every function depends on having data,
	// and reading from our beloved file is the only way to do it.
	Manager(std::string filename);
	~Manager();

	// Reads everything from a file and creates days that have slices of data.
	void getWeatherDataFromFile(std::string filename);

	//bool hasDate(Date* date);
	Day* addDay(Date* date);

	// Averages the values of the days in the day pointer vector member.
	void averageTheValuesOfDays();
	// Calculates the mold risk of the days in the day pointer vector member.
	void calculateMold();

	// Calculates the dooring of the days.
	void calculateDooring();

	// Prints every day and the information we have of them at the time.
	void printDays();
	// Prints the dooring per day.
	void printDoors();

	// User inputs what to sort as a SORTTYPE and if it's high to low or low to high.
	// Then calls mergeSort and displays it.
	void sortMenuAndDisplay();

	// Calls calculateAutumnOrWinter
	void calculateAndPrintSeason(std::string season);

	// Gets input to search for a date and calls binary search and prints the result.
	int searchDay();

	// Returns an index given the date.
	int getDayIndex(Date* date);
	// Returns a day given the index.
	Day* getDay(int index);

	//Returns the date above the parameter. If there is none, return the same date.

	Date* nextDate(Date* date) {
		for (int i = 0; i < days.size(); i++) {
			if (days[i]->getDate()->getDateAsString() == date->getDateAsString()) {
				if (i + 1 < days.size()) {
					return days[i + 1]->getDate();
				}
			}
		}
		return date;
	}

	//Returns the date below the parameter. If there is none, return the same date.

	Date* previousDate(Date* date) {
		for (int i = 0; i < days.size(); i++) {
			if (days[i]->getDate()->getDateAsString() == date->getDateAsString()) {
				if (i - 1 >= 0) {
					return days[i - 1]->getDate();
				}
			}
		}
		return date;
	}


};
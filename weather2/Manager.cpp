#include "pch.h"
#include "Manager.h"


Manager::Manager(std::string filename)
{
	// Reads from the file.
	this->getWeatherDataFromFile(filename);
	// Initializes daysAveraged and moldCalculated as false.
	this->daysAveraged = false;
	this->moldCalculated = false;
	this->autumnDate = Date(0,0,0);
}


Manager::~Manager()
{
	// Clears memory.
	for (int i = 0; i < this->days.size(); i++) {
		delete this->days[i];
	}
}


void Manager::getWeatherDataFromFile(std::string filename) {

	// Timer to record the amount of time it takes to read and save data.
	Timing timer;
	// Start the timer.
	timer.start();
	// Create a stream.
	std::ifstream weatherFile;
	// Open the file.
	weatherFile.open(filename);

	// Variable to hold information before it's divided into the (...)Line variables below.
	std::string line;

	// Will temporarily store information from the lines of the file as strings:
	std::string timeLine;			// Date and time of day
	std::string dateLine;			// Date
	std::string timeOfDayLine;		// Time of day
	std::string placeLine;			// Inside or out
	std::string temperatureLine;	// Temperature
	std::string humidityLine;		// Humidity

	// Will hold the values as they are handled in the program.
	PLACE place = INSIDE;
	float temperature;
	int humidity;
	TimeOfDay* timeOfDay;

	// The post in which to store:
	//	place
	//	temperature
	//	humidity
	//	time of day
	SliceOfData* sliceOfData;	

	// To be stored in a day.
	Date* date = new Date("0000-00-00");

	// Day. Will collect all slices of data related to the date the day represents.
	Day* day = new Day(*date);

	// Stringstream to ease dividing strings into separate variables.
	std::istringstream ss;
	if (weatherFile.good()) {

		// Get each line from the file as a string saved in line.
		while (std::getline(weatherFile, line)) {
			
			// Create a stringstream of the line.
			ss = std::istringstream(line);
			// ss fishes the data into separate strings.
			std::getline(ss, timeLine, ',');
			std::getline(ss, placeLine, ',');
			std::getline(ss, temperatureLine, ',');
			std::getline(ss, humidityLine);

			// Separates the timeLine into date and time of day.
			ss = std::istringstream(timeLine);
			std::getline(ss, dateLine, ' ');
			std::getline(ss, timeOfDayLine);
			
			// Creates a new date, sending a "...y-.m-.d" string into the constructor.
			date = new Date(dateLine);
			// Creates a new time of day, sending a ".h:.m:.s" string into the constructor.
			timeOfDay = new TimeOfDay(timeOfDayLine);
			// Assigns place according to the place data.
			if (placeLine[0] == 'I')
				place = INSIDE;
			else
				place = OUTSIDE;

			// temperature is assigned as a float, converted from a string representation of a float.
			temperature = stof(temperatureLine);
			// humidity is assigned as an int, converted from a string representation of an integer.
			humidity = stoi(humidityLine);

			// A slice of data is constructed with the information read from the file.
			sliceOfData = new SliceOfData(*timeOfDay, place, temperature, humidity);
			// Create a new day if we're reading a new date.
			if (date->isMoreThan(day->getDate())) {
				day = this->addDay(date);
			}
			// We add the slice of data to the day.
			day->addSliceOfData(sliceOfData);

		}
		// Close the file.
		weatherFile.close();
	}
	else {
		// File not good.
		std::cout << "file not good";
	}
	timer.finish();
	std::cout << std::endl << "Reading file finished in " << timer.getSeconds() << " seconds." << std::endl;

}

Day* Manager::addDay(Date* date) {

	// Creates a day object and pointer to it based on the date.
	Day* day = new Day(*date);
	// Adds the day pointer to the day vector.
	days.push_back(day);
	// Returns the day pointer so that slices of data could be added to it.
	return day;
}


void Manager::averageTheValuesOfDays() {
	// Timer to record the amount of time it takes to average the values of days.
	Timing timer;
	// Start the timer.
	timer.start();
	// Small safe-guard just in case this is called after the days have been averaged sometime before.
	if (!this->daysAveraged) {
		for (int i = 0; i < this->days.size(); i++) {
			this->days[i]->averageUnsegmented();
		}
		// Should timer.finish() be before the line below? Debateable.
		this->daysAveraged = true;
	}
	// Stop the timer.
	timer.finish();
	std::cout << "Averaging days calculated in " << timer.getSeconds() << " seconds." << std::endl;;
}

void Manager::calculateMold() {
	// Timer to record the amount of time it takes to calculate mold.
	Timing timer;
	// Start the timer.
	timer.start();
	// Small safe-guard just in case this is called after the mold has been calculated sometime before.
	if (!this->moldCalculated) {
		for (int i = 0; i < this->days.size(); i++) {
			if (this->days[i]->hasInsideData())
				this->days[i]->calulateMoldRiskTime(INSIDE);
			if (this->days[i]->hasOutsideData())
				this->days[i]->calulateMoldRiskTime(OUTSIDE);
		}
		this->moldCalculated = true;
	}
	// Stop the timer.
	timer.finish();
	std::cout << "Mold risk calculated in " << timer.getSeconds() << " seconds." << std::endl;
}

Date* Manager::getInputDate() {

	// Initialize input variables.
	int yy;
	int mm;
	int dd;

	// Days in a month.
	int monthDays[12] = { 31, 28, 31, 30, 31, 30,
					   31, 31, 30, 31, 30, 31 };

	std::cout << "Enter date. yyyymmdd" << std::endl;
	std::cout << "Enter yyyy: ";

	yy = getInteger(0, 5000);

	std::cout << "Enter mm: ";
	mm = getInteger(1, 12);

	// Max amount of days allowed for input depending om month.
	int maxDay = monthDays[mm - 1];
	// If it's february and it's a leap year then maxdays should be 29, not 28
	if (isLeapYear(yy) && mm == 2)
		maxDay++;

	std::cout << "Enter dd: ";

	dd = getInteger(1, maxDay);

	// Creates and returns a date.
	return &Date(yy, mm, dd);

}

int Manager::searchDay() {

	// Gets a date to search for.
	Date* dateToSearchFor = getInputDate();
	
	// Timer to time the time elapsed to find the date.
	Timing timer;
	// Start the timer.
	timer.start();
	// Get an index for the day pointer vector member.
	int index = binaryDaySearch(0, this->days.size() - 1, dateToSearchFor);
	// Stop the timer.
	timer.finish();

	std::cout << std::endl;

	// -1 indicates that the day could not found.
	if (index != -1) {
		// Printing avg temp and 
		std::cout << "Date found: " << this->days[index]->getDate()->getDateAsString() << std::endl;

		std::cout << "Avg temp inside: ";
		if (days[index]->hasInsideData() && this->daysAveraged)
			std::cout << this->days[index]->getAverageTempInside();
		else
			std::cout << " no data.";

		std::cout << std::endl;

		std::cout << "Avg temp outside: ";
		if (days[index]->hasOutsideData() && this->daysAveraged)
			std::cout << this->days[index]->getAverageTempOutside();
		else
			std::cout << " no data.";
	}
	else {
		std::cout << "There is no such date, sorry.";
	}
	std::cout << std::endl;
	std::cout << "Search completed in " << timer.getSeconds() << " seconds.";
	std::cout << std::endl;

	return index;
}

int Manager::getDayIndex(Date * date)
{
	return binaryDaySearch(0, this->days.size() - 1, date);
}

Day* Manager::getDay(int index) {
	if (index >= 0 && index < this->days.size())
		return this->days[index];
	return this->days[0];
}

int Manager::binaryDaySearch(int low, int high, Date* dateToFind) {

	// Search abolished if this is not the case.
	//if (high < low) {

		// Set middle as the middle between the highest impossible and the lowest impossible.
		// Then, if the date is what we seek, return it.
		//		 if the date is lower, we go to the right, toward the higher dates.
		//		 if the date is higher, we go to the left, toward the lower dates.

		int middle = low + (high - low) / 2;

		if (this->days[middle]->getDate()->getDateAsString() == dateToFind->getDateAsString())
			return middle;
		/*
		We send mid - 1 as high because we already checked mid. It is no longer relevant.
		*/
		if (this->days[middle]->getDate()->isMoreThan(dateToFind))
			return binaryDaySearch(low, middle - 1, dateToFind);

		/*
		At this point we know that daysToSearch[middle] is in a higher position than middle,
		since it was neither equal nor 

		Given that it exists in the vector at all, of course.
		*/

		return binaryDaySearch(middle + 1, high, dateToFind);

	//}
	// Indicates that the date could not be found in our day pointer vector.
	return -1;
}

void Manager::printDays()
{
	// Prints the days depending on what we have calculated and what data exists.
	std::cout << std::endl;
	for (int i = 0; i < days.size(); i++) {
		std::cout << days[i]->getDate()->getDateAsString();

		if (this->daysAveraged && (days[i]->hasInsideData() && days[i]->hasOutsideData())) {
			std::cout << " Maxdiff temp: "; printf("%.2f", days[i]->getDifferenceTemp());
		}
		else {
			std::cout <<  " Maxdiff temp: incomplete data. ";
		}

		std::cout << std::endl;
		if (days[i]->hasInsideData()) {
			if (this->daysAveraged) {
				std::cout << "tempIn : "; printf("%.2f", days[i]->getAverageTempInside());
				std::cout << " ";
				std::cout << "humIn : "; printf("%.2f", days[i]->getAverageHumidityInside());
			}
			if (this->moldCalculated) {
				std::cout << " mold risk: " << days[i]->getMoldRiskTime(INSIDE)->getStr();
				std::cout << " ";
				printf("%.2f", days[i]->getMoldRisk(INSIDE));
			}
		}
		else {
			std::cout << "No inside data.";
		}

		std::cout << std::endl;

		if (days[i]->hasOutsideData()) {
			if (daysAveraged) {
				std::cout << "tempOut: "; printf("%.2f", days[i]->getAverageTempOutside());
				std::cout << " ";
				std::cout << "humOut: "; printf("%.2f", days[i]->getAverageHumidityOutside());
			}
			if (this->moldCalculated) {
				std::cout << " mold risk: " << days[i]->getMoldRiskTime(OUTSIDE)->getStr();
				std::cout << " ";
				printf("%.2f", days[i]->getMoldRisk(OUTSIDE));
			}
		}
		else {
			std::cout << "No outside data.";
		}

		std::cout << std::endl;
		std::cout << std::endl;
	}
	
}

void Manager::merge(Day** arrr, int low, int middle, int high) {
	int leftIterator = low;
	int rightIterator = middle + 1;

	int tempArrrIterator = 0;

	Day** tempArrr = new Day*[high - low + 1];
	while (leftIterator <= middle && rightIterator <= high) {

		// Whether or not one element is considered lower or higher than the other
		// in terms of sorting value is not handled here, but in the Day class.
		// Two Days are compared and returns the result based on desired sorting type.
		if (arrr[leftIterator]->lowerInSortValue(this->sortType, arrr[rightIterator], this->hilo)) {
			tempArrr[tempArrrIterator] = arrr[leftIterator];
			leftIterator++;
			tempArrrIterator++;
		}
		else {
			tempArrr[tempArrrIterator] = arrr[rightIterator];
			rightIterator++;
			tempArrrIterator++;
		}
	}

	/*
	
	If the rightIterator reached the middle, then it means that, above,
	the elements in the left has been added to the tempArr, because 
	their sorting value said that they are where they are supposed to
	(in this merge). Therefore, the rest to the right (which is already
	sorted among themselves) are simply added as they are higher
	than those to the left and, as mentioned, already sorted among
	themselves.

	*/
	while (leftIterator <= middle) {
		tempArrr[tempArrrIterator] = arrr[leftIterator];
		tempArrrIterator++;
		leftIterator++;
	}

	/*
	
	Ditto for above, but for if the leftIterator reached its limit.

	*/

	while (rightIterator <= high) {
		tempArrr[tempArrrIterator] = arrr[rightIterator];
		tempArrrIterator++;
		rightIterator++;
	}

	// Adapt the real array to the merge.
	for (int i = low; i <= high; i++) {
		arrr[i] = tempArrr[i - low];
	}


};


void Manager::mergeSort(Day** arrr, int low, int high) {
	int middle;
	if (low < high) {
		middle = (high + low) / 2;

		this->mergeSort(arrr, low, middle);
		this->mergeSort(arrr, middle + 1, high);

		this->merge(arrr, low, middle, high);
	}
};

void Manager::calculateDooring() {
	for (int i = 0; i < days.size(); i++) {
		days[i]->calculateDoor();
	}
}

void Manager::printDoors()
{
	for (int i = 0; i < days.size(); i++) {
		std::cout << days[i]->getDate()->getDateAsString() << ": " << std::endl;
		days[i]->getDoor()->print();
	}
}


void Manager::sortMenuAndDisplay() {
	Day** dayArray;
	dayArray = new Day*[this->days.size()];

	std::cout << "[1]  [outside] temperature" << std::endl;
	std::cout << "[2]  [outside] humidity" << std::endl;
	std::cout << "[3]  [outside] mold risk" << std::endl;
	std::cout << "[4]  [inside]  temperature" << std::endl;
	std::cout << "[5]  [inside]  humidity" << std::endl;
	std::cout << "[6]  [inside]  mold risk" << std::endl;
	std::cout << "[7]  [both]    max difference in temp" << std::endl;

	std::cout << "How do you want to sort? Input: ";
	
	int choice = getInteger(1, 7) - 1;

	std::cout << std::endl << std::endl;
	std::cout << "[1] Low to high \n[2] High to low \n\nInput: ";

	
	// Continue as normal if choice is one. 
	// Add one to the choice if want high to low.
	// Then the enum value above will be chosen, and it will be sorted such
	// that higher values are at index one instead.
	int hiloChoice = getInteger(1, 2) - 1;

	// How to compare values (sorttype) and which direction (hilo (high or low));
	this->sortType = static_cast<SORTTYPE>(choice);
	// low to high = 0, high to low = 1.
	this->hilo = static_cast<bool>(hiloChoice);
	
	Timing timer;

	// Linearly copying the days into Day pointer array.
	// Debate:	Should this be moved down to after the timer
	//			has started?
	//
	//  Is this part of sorting, or just a setup
	//	based on how the sorting is intended to affect the program?
	//	The desired functionality is not to sort
	//	a pre-existing array/vector, but to 
	//	sort a temporary one and just show it.
	for (int i = 0; i < days.size(); i++) {
		dayArray[i] = this->days[i];
	}

	timer.start();
	mergeSort(dayArray, 0, this->days.size() - 1);
	timer.finish();

	// Printing each date and the data it was sorted by.
	for (int i = 0; i < this->days.size(); i++) {
		std::cout << dayArray[i]->getDate()->getDateAsString() << " ";

		switch (this->sortType) {
		case OUTSIDETEMPERATURE:
			if(dayArray[i]->hasOutsideData())
				std::cout << dayArray[i]->getAverageTempOutside();
			else
				std::cout << "No outside data for this day.";
			break;
		case OUTSIDEHUMIDITY:
			if (dayArray[i]->hasOutsideData())
				std::cout << dayArray[i]->getAverageHumidityOutside();
			else
				std::cout << "No outside data for this day.";
			break;
		case OUTSIDEMOLD:
			if (dayArray[i]->hasOutsideData()) {
				std::cout << "Mold risk time: ";
				std::cout << dayArray[i]->getMoldRiskTime(OUTSIDE)->getStr();
				std::cout << ", Mold risk: ";
				printf("%.1f", dayArray[i]->getMoldRisk(OUTSIDE));
			}
			break;
		case INSIDETEMPERATURE:
			if (dayArray[i]->hasInsideData())
				std::cout << dayArray[i]->getAverageTempInside();
			else
				std::cout << "No inside data for this day.";

			break;
		case INSIDEHUMIDITY:
			if (dayArray[i]->hasInsideData())
				std::cout << dayArray[i]->getAverageHumidityInside();
			else
				std::cout << "No inside data for this day.";
			break;
		case INSIDEMOLD:
			std::cout << "Mold risk time: ";
			std::cout << dayArray[i]->getMoldRiskTime(INSIDE)->getStr();
			std::cout << ", Mold risk: ";
			printf("%.1f", dayArray[i]->getMoldRisk(INSIDE));
			break;
		case MAXDIFFTEMP:
			if (dayArray[i]->hasInsideData() && dayArray[i]->hasOutsideData())
				std::cout << dayArray[i]->getDifferenceTemp();
			else
				std::cout << "No inside or outside data for this day.";
			break;
		}
		std::cout << std::endl;
	}
	std::cout << "Sorting finished in " << timer.getSeconds() << " seconds" << std::endl;

}


/*

The pointer parameters are actually members of Manager.

Why do we take them as parameters?

Answer:

This function is supposed to be general, such that we can use it for any of our 4
seasons. Instead of having 4 functions, we have one.

*/

void Manager::calculateAutumnOrWinter(float threshold, Date* seasonStart) {
	if (!this->daysAveraged) {
		std::cout << "Days not averaged - doing this as well." << std::endl;
		this->averageTheValuesOfDays();
	}

	// Timer to time calculation of the season.
	Timing timer;
	// Start the timer.
	timer.start();

	int validDaysInARow = 0;
	Date* lastDay = days[0]->getDate();

	int i = 0;
	bool valid = false;
	bool hasSeason = false;
	/*
	
	Will loop until we reach the last day, or until we find the date for meteorological season.
	
	*/
	while (i < days.size() && hasSeason == false) {

		valid = false;

		// If season threshold is below/above the threshold, then valid is true.
		if (days[i]->getAverageTempOutside() < threshold){
			//	We should check if the days are in a row.
			//	If it's the first valid since the last date, then it is in a row with itself.
			std::cout << "Temp: " << days[i]->getAverageTempOutside();
			if (days[i]->getDate()->difference(lastDay) == 1 || validDaysInARow == 0) {
				validDaysInARow += 1;
				lastDay = days[i]->getDate();
				std::cout << "  Date: " << days[i]->getDate()->getDateAsString() << std::endl;
			}
		}
		else {
			validDaysInARow = 0;
		}

		if (validDaysInARow == 5) {
			*seasonStart = *days[i - 4]->getDate();

			// Now that we know that there is a season start, we save it as a bool by this pointer.
			hasSeason = true;
		}

		i++;
	}

	timer.finish();
	std::cout << std::endl;
	std::cout << "Season calculated in " << timer.getSeconds() << " seconds;" << std::endl;
	std::cout << std::endl;
}

void Manager::calculateAndPrintSeason(std::string season) {

	/* Calculates a season, and then prints it.
	*/
	if (season == "autumn") {
		// Autumn calculated
		calculateAutumnOrWinter(10.f, &this->autumnDate);
		// Autumn calculated by found nothing.
		if (!(this->autumnDate.getYear() == 0))
			std::cout << "Autumn started at: " << this->autumnDate.getDateAsString();
		else
			std::cout << "Has no autumn start date.";
	}
	else if (season == "winter") {
		// Winter calculated
		calculateAutumnOrWinter(0.f, &this->winterDate);
		// Winter calculated but found nothing.
		if (!(this->winterDate.getYear() == 0))
			std::cout << "Winter started at: " << this->winterDate.getDateAsString();
		else
			std::cout << "Has no winter start date.";
	}
	std::cout << std::endl;
	std::cout << std::endl;

}
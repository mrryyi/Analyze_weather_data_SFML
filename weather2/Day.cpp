#include "pch.h"
#include "Day.h"

Day::Day(Date date) : date(date)
{

	// Such that he getters aren't returning
	// uninitialized values.
	this->averageTempInside = 0.0f;
	this->averageHumidityInside = 0.0f;

	this->averageTempOutside = 0.0f;
	this->averageHumidityOutside = 0.0f;

};

Day::~Day()
{
	// Deconstructor to delete sliceofdata.
	// These are destroyed here because a sliceofdata
	// has no relation to a day outside being contained
	// in one. They are then useless, and destroyed here,
	// with the day.
	for (int i = 0; i < this->inside.size(); i++) {
		delete inside[i];
	}
	for (int i = 0; i < this->outside.size(); i++) {
		delete outside[i];
	}
}

void Day::addSliceOfData(SliceOfData* data) {
	// Adds data pointer to one or the other vector based
	// on the PLACE variable of the data.
	switch (data->getPlace()) {
	case INSIDE:
		inside.push_back(data);
		break;
	case OUTSIDE:
		outside.push_back(data);
		break;
	default:
		throw "Data->getPlace() returned a corrupted enum.";
	}
}

bool Day::lowerInSortValue(SORTTYPE sortType, Day* compare, bool hilo)
{
	// lower in (sort) value (than that of the paramater comparee)
	bool liv = false;

	/*
	hilo == false
		Lowest at index 0.
	hilo == true:
		Highest at index 0.
	*/

	// Since the default of this function is to return this < compare,
	// then we check if this < compare by the metric indicated by sortType,
	// detailed in each switch-case.
	switch (sortType) {
		case OUTSIDETEMPERATURE:
			liv = this->averageTempOutside < compare->getAverageTempOutside();
			break;
		case OUTSIDEHUMIDITY:
			liv = this->averageHumidityOutside < compare->getAverageHumidityOutside();
			break;
		case OUTSIDEMOLD:
			liv = this->moldRiskOut < compare->getMoldRisk(OUTSIDE);
			break;
		case INSIDETEMPERATURE:
			liv = this->averageTempInside < compare->getAverageTempInside();
			break;
		case INSIDEHUMIDITY:
			liv = this->averageHumidityInside < compare->getAverageHumidityInside();
			break;
		case INSIDEMOLD:
			liv = this->moldRiskIn < compare->getMoldRisk(INSIDE);
			break;
		case MAXDIFFTEMP:
			liv = this->differenceTemp < compare->getDifferenceTemp();
			break;
	}

	// if high to low, then the sort value is reversed, as 
	if (hilo)
		return !liv;
	return liv;
}

void Day::averageUnsegmented() {

	/*
	
	Naively adds every temperature/humidity in the
	slice of data pointer vectors to long doubles.

	Then divides by the amount of slices of data.

	If there is no inside/outside data (There is
	uncertainty about if the data can/can't be trusted)
	then they will default to be saved as zeroes.
	We don't want to divide by std::vector<.>.size() == 0.

	The need for two or three bool members to indicate
	in the rest of the program if there ARE calculated averages,
	was neglected.

	Why?
		Because the rest of the program can check if 
		this has inside/outside data.
	
	*/

	long double temp = 0.0f;
	long double hum = 0.0f;

	if (this->hasInsideData()) {
		for (int i = 0; i < inside.size(); i++) {
			temp += inside[i]->getTemperature();
			hum += inside[i]->getHumidity();
		}

		temp /= inside.size();
		hum /= inside.size();

		this->averageTempInside = (float)temp;
		this->averageHumidityInside = (float)hum;
	}


	temp = 0.0f;
	hum = 0.0f;

	if (this->hasOutsideData()) {
		for (int i = 0; i < outside.size(); i++) {
			temp += outside[i]->getTemperature();
			hum += outside[i]->getHumidity();
		}

		temp /= outside.size();
		hum /= outside.size();

		this->averageTempOutside = (float)temp;
		this->averageHumidityOutside = (float)hum;
	}


	if (this->hasInsideData() && this->hasOutsideData()) {
		// The difference here is supposed to be unsigned.
		// Unlike date differences, that have significance because of order.
		this->differenceTemp = this->averageTempInside - this->averageTempOutside;
		if (differenceTemp < 0)
			differenceTemp *= -1;
	}

}

int Day::moldRisk(std::vector<SliceOfData*> listOfData) {

	int seconds = 0;

	// Disclaimer: the molding bool does not represent that molding is occuring,
	// but rather, that there is currently a mold risk present.
	// "mold risk present" meaning a humidity above the mold limit for the given temperature
	// at that slice of time.
	bool molding = false;
	int lastTime = 0;
	for (int i = 0; i < listOfData.size(); i++) {
		if (listOfData[i]->hasMoldRisk()) {

			// Record the start time for mold risk
			if (molding == false) {
				lastTime = listOfData[i]->getTimeOfDay()->getTotalTimeInSeconds();
			}
			// Set molding to true, such that if at some later iteration
			// there is no mold risk, we can stop the timer when that flag comes.
			molding = true;
		}
		else {
			// End the time for mold risk, and add it to the total.
			if (molding == true) {
				seconds += listOfData[i]->getTimeOfDay()->getTotalTimeInSeconds() - lastTime;
			}
			// Set molding to fase, such that if at some later iteration
			// there is mold risk, we can start the timer when that flag comes.
			molding = false;
		}
	}

	// If we find no end time for mold risk,
	// then it is assumed to be molding still.
	// Therefore, we add the rest between lastTime
	// and the end of the day to the seconds
	// elapsed with mold risk.
	if (molding == true) {
		seconds += 86400 - lastTime;
	}

	// The amount of seconds elapsed with a mold risk index above zero.
	return seconds;
}

void Day::calulateMoldRiskTime(PLACE inOrOut) {

	// Calculates the time per day that mold risk index > 0
	// Saves that as a member.
	// Calculates the mold risk index as ((average humidity) - F(average temp))
	// Saves that as a member.
	switch(inOrOut) {
	case INSIDE:
		this->moldRiskTimeIn = TimeOfDay(moldRisk(this->inside));
		this->moldRiskIn = moldRiskDiff(this->averageTempInside, this->averageHumidityInside);
		break;
	case OUTSIDE:
		this->moldRiskTimeOut = TimeOfDay(moldRisk(this->outside));
		this->moldRiskOut = moldRiskDiff(this->averageTempOutside, this->averageHumidityOutside);
	}

}

void Day::calculateDoor() {
	
	TimeOfDay* time;

	// The range of the time window:
	int trailingSeconds = 90;
	// How many seconds we move each time window per loop:
	int trailRate = 50;


	// The lowest index for the day that is within a given range.
	int inTrailLow = 0;
	int inTrailHigh = 0;
	// The highest index for the day that is within the given range.
	int outTrailLow = 0;
	int outTrailHigh = 0;

	// Will be used to iterate through the slices in the time window.
	int inIter;
	int outIter;

	// How many elements are within the time window
	int inSize;
	int outSize;

	// The lower bounds of the time window.
	int timeLow = 0;
	// The higher bounds of the time window.
	int timeHigh = trailingSeconds;


	// Averages of the values within the trail window.
	float avgTrailIn = 0.0f;
	float avgTrailOut = 0.0f;

	// Difference between average inside and average outside.
	float difference;
	/*
	The slope for inside. What this means is the slope between
	temperature
	*/
	float inSlope;
	// What the slope / difference will be compared against.
	float limit = 0.0001f;

	bool done = false;
	bool inValid;
	bool outValid;

	float temporary;
	float result;

	bool doorOpened = false;
	bool running = true;

	while (!done) {
		
		inValid = false;
		outValid = false;
		
		// CALCULATE THE LOWEST EXISTING TIME IN THE TIME WINDOW
		running = true;
		while (running && (inTrailLow < inside.size() - 1)) {
			if (inside[inTrailLow + 1]->secs() < timeLow)
				inTrailLow++;
			else
				running = false;
		}
		
		// CALCULATE THE HIGHEST EXISTING TIME IN THE TIME WINDOW 
		running = true;
		while (running && (inTrailHigh < inside.size() - 1)) {
			if (inside[inTrailHigh + 1 ]->secs() < timeHigh)
				inTrailHigh++;
			else
				running = false;
		}


		/*

		Conditions to check if the low and highs are within the time window.

		*/
		
		if ((inside[inTrailLow]->secs() < timeHigh) && (inside[inTrailLow]->secs() >= timeLow))
			inValid = true;

		if ((inside[inTrailHigh]->secs() <= timeHigh) && (inside[inTrailHigh]->secs() > timeLow))
			inValid = true;
		
		if (inside[inTrailLow] == inside[inTrailHigh])
			inValid = false;


		// CALCULATE THE LOWEST EXISTING TIME IN THE TIME WINDOW
		running = true;
		while (running && (outTrailLow < outside.size() - 1)) {
			if (outside[outTrailLow + 1]->secs() < timeLow)
				outTrailLow++;
			else
				running = false;
		}

		// CALCULATE THE HIGHEST EXISTING TIME IN THE TIME WINDOW 
		running = true;
		while (running && (outTrailHigh < outside.size() - 1)) {
			if (outside[outTrailHigh + 1]->secs() < timeHigh)
				outTrailHigh++;
			else
				running = false;
		}

		/*
		
		Conditions to check if the low and highs are within the time window.
		
		*/
		if ((outside[outTrailLow]->secs() < timeHigh) && (outside[outTrailLow]->secs() >= timeLow))
			outValid = true;

		if ((outside[outTrailHigh]->secs() <= timeHigh) && (outside[outTrailHigh]->secs() > timeLow))
			outValid = true;

		if (outside[outTrailLow] == outside[outTrailHigh])
			outValid = false;

		/*
		
		If both are within the time window or not the same, then continue.
		
		*/

		if (inValid && outValid) {
			// Amount of slices in the time window per in/out.
			outSize = outTrailHigh - outTrailLow + 1;
			inSize = inTrailHigh - inTrailLow + 1;
			// Iterators set to the lowest.
			inIter = inTrailLow;
			outIter = outTrailLow;
			// Averages inside the time window.
			avgTrailIn = 0.0f;
			avgTrailOut = 0.0f;
			inSlope = 0.0f;
			
			
			// CALCULATE AVERAGE INSIDE
			while (inIter <= inTrailHigh) {
				avgTrailIn += inside[inIter]->getTemperature();
				inIter++;
			}
			avgTrailIn /= inSize;

			// CALCULATE AVERAGE OUTSIDE
			while (outIter <= outTrailHigh) {
				avgTrailOut += outside[outIter]->getTemperature();
				outIter++;
			}
			avgTrailOut /= outSize;
			difference = avgTrailOut - avgTrailIn;

			// CALCULATE SLOPE BETWEEN LOWEST AND HIGHEST TIMES IN THE TIME WINDOW.
			inSlope = inside[inTrailHigh]->getTemperature();
			inSlope -= inside[inTrailLow]->getTemperature();
			temporary = inside[inTrailHigh]->secs() - inside[inTrailLow]->secs();

			// IF TEMPOORARY IS 0, THEN THE VALUES ARE ON THE SAME TIME.
			if (temporary != 0)
				inSlope /= temporary;
			else
				inSlope = 0;

			// IF THE DIFFERENCE IS ZERO, THEN LET RESULT BE 0.
			if (difference != 0)
				result = (inSlope / difference);
			else
				result = 0;

			// Open suspicion
			if (result > 0) {
				if (result > limit) {
					// Door is open for this information.
					if (doorOpened == false) {
						time = inside[inTrailLow]->getTimeOfDay();
						this->door.addTimeOfDay(time, true);
					}
					doorOpened = true;
				}
			}// Close suspicion
			else {
				if (result*-1 > limit) {
					// Door is closed for this information.
					if (doorOpened == true || !this->door.getHappened()) {
						time = inside[inTrailLow]->getTimeOfDay();
						this->door.addTimeOfDay(time, false);
					}
					doorOpened = false;
				}
			}
		}

		timeLow += trailRate;
		timeHigh = timeLow + trailingSeconds;
		done = timeHigh > 86400;
	}
}
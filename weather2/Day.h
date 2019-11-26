#pragma once
class Day
{
	// A day is defined by its date. It is what separates one day from another.
	Date date;
	std::vector<SliceOfData* > inside;
	std::vector<SliceOfData* > outside;

	// Averages as floats. Calculated by this->averageUnsegmented().
	float averageTempInside;
	float averageTempOutside;
	float averageHumidityInside;
	float averageHumidityOutside;
	// Also calculated in this->averageUnsegmented() because it's a small calculation.
	float differenceTemp;

	// This represents the amount of time in a day that has mold risk.
	// Calculated in calculateMoldRisk().
	TimeOfDay moldRiskTimeIn;
	TimeOfDay moldRiskTimeOut;
	
	// The time of day the door is closed/opened.
	// Based on temperature or humidity.
	Door door;

	// Mold risk for inside or outside.
	float moldRiskIn;
	float moldRiskOut;

	int moldRisk(std::vector<SliceOfData*> listOfData);

public:
	Day() {};
	Day(Date date);
	~Day();

	// Averages the temperature and humidity naively.
	// Does not account for overrepresentation of data
	// Maybe there are 200 measures in the morning and
	// only one in the evening. The average becomes close
	// to the morning temperatures and the affect the evening
	// had on the average is negligible.
	void averageUnsegmented();

	// Calculates the amount of time this day that has humidity > mold index
	//
	// As well as:
	//
	// Calculates the mold risk index as ((average humidity) - F(average temp))
	// F(x) = -0.0015*(x^3) + 0.1193*(x^2) - 2.9878*x + 102.96.
	// Does inside and outside risk times depending on the parameter.
	// Saves that as a member.
	void calulateMoldRiskTime(PLACE inOrOut);



	void calculateDoor();

	// Getter functions.
	float getAverageTempInside() { return averageTempInside; };
	float getAverageTempOutside() { return averageTempOutside; };
	float getAverageHumidityInside() { return averageHumidityInside; };
	float getAverageHumidityOutside() { return averageHumidityOutside; };
	float getDifferenceTemp() { return differenceTemp; };

	// Returns the date of the day
	Date* getDate() { return &date; };

	// Returns door. Which tells stuff about the door openings and closings.
	Door* getDoor() {
		return &door;
	};

	// Returns the mold risk time. Either for inside or outside.
	TimeOfDay* getMoldRiskTime(PLACE place) { return (place == INSIDE) ? &moldRiskTimeIn : &moldRiskTimeOut; };

	// Returns the index (the difference between humidity and index AT the temperature.)
	// Does not account for the closest distance between humidity and the mold index.
	/*
		What is sought here? It's the risk of mold.
		What is given:
			T temperature
			H humidity
			F formula

		F(T) gives a limit for the humidity, an index for mold. If H is above F(T), then
		there is risk of mold. ***No matter*** if we count "above" as the closest pythagorean
		distance to F, or if we count it as the difference between F(T) and H (one dimensional)...

			...we still end up with a positive value to flag for mold risk.

		However,

		The limit at the closest pythagorean distance is derived from a 
		totally different temperature than the temperatures we have recorded.

		It is assumed, in this program, that the index derived from a one dimensional
		distance to the mold limit is more relevant, as that coincides with the data
		we have.

		Let's say we would have compared using the closest distance to the curve.
		Then we'd ignore the current temperature recorded, because we'd find the 
		position on the curve that is closest, based on a temperature not necessarily
		present in the data, and base the index off of that.

		This is why mold risk is derived from the average temperature we actually have.
	*/
	float getMoldRisk(PLACE place) { return (place == INSIDE) ? this->moldRiskIn : this->moldRiskOut; };

	// These two functions returns true if there is inside/outside data.
	bool hasInsideData() { return (this->inside.size() > 0) ? true : false;  };
	bool hasOutsideData() { return (this->outside.size() > 0) ? true : false;  };

	std::vector<SliceOfData*> getInsideData() {
		return this->inside;
	}

	std::vector<SliceOfData*> getOutsideData() {
		return this->outside;
	}

	// Adds a slice of data to the inside or outside SliceOfData pointer vector member.
	void addSliceOfData(SliceOfData* data);

	// Based on the paramter sort type, values this data versus (parameter) compare
	// and decides if this has a lower sort value over the other.
	// Useful for sorting days because they can call this instead of
	// calling the details in a bunch of customized sorting functions
	// such that we have more than 10 rewritten sorting functions.
	bool lowerInSortValue(SORTTYPE sortType, Day* compare, bool hilo);

};
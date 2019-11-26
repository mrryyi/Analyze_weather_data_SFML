#pragma once
class SliceOfData
{
	// A slice of data does not know which day it's in.
	// That is handled by being in a Day's SliceOfData pointer vector member.

	// That which denotes where the slice of data is, within the day.
	TimeOfDay timeOfDay;
	// INSIDE or OUTSIDE.
	PLACE place;
	// The temperature.
	float temperature;
	// The humidity.
	int humidity;

public:

	// The constructor gets what it needs to exist.
	SliceOfData(TimeOfDay timeOfDay, PLACE place, float temperature, int humidity);
	~SliceOfData();

	// Uses the formula for mold risk to return a bool for this slice.
	// "Is the humidity above the mold index (calculated based on temperature)?"
	// "yes: true."
	// "no: false."
	bool hasMoldRisk();

	// Getter functions.
	TimeOfDay*	getTimeOfDay() { return &timeOfDay; };
	int			secs() { return this->timeOfDay.getTotalTimeInSeconds(); };
	PLACE		getPlace() { return place; };
	float		getTemperature() { return temperature; };
	int			getHumidity() { return humidity; };
};
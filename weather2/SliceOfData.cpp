#include "pch.h"
#include "SliceOfData.h"




SliceOfData::SliceOfData(TimeOfDay timeOfDay, PLACE place, float temperature, int humidity)
	: timeOfDay(timeOfDay), place(place), temperature(temperature), humidity(humidity) {};



SliceOfData::~SliceOfData()
{

}

bool SliceOfData::hasMoldRisk() {
	bool above = this->humidity > moldRiskLimit(this->temperature);
	// If humidity is above the limit, then this slice of data has mold risk.
	return above;
}
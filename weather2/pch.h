#ifndef PCH_H
#define PCH_H

enum PLACE {
	INSIDE,
	OUTSIDE
};

enum SORTTYPE {
	OUTSIDETEMPERATURE,
	OUTSIDEHUMIDITY,
	OUTSIDEMOLD,
	INSIDETEMPERATURE,
	INSIDEHUMIDITY,
	INSIDEMOLD,
	MAXDIFFTEMP
} sortType;

#include <string>		// strings
#include <iostream>		// cin, cout
#include <stdarg.h>     // va_list, va_start, va_arg, va_end
#include <vector>		// vectors.
#include <regex>		// Not sure if used. ToDo: find out.
#include <sstream>		// string stream for easy separation of string info.
#include <fstream>		// reading from a file
#include <cmath>		// pow()
#include <chrono>		// To time algorithms.
#include <SFML/Graphics.hpp>

#include "misc.h"

#include "TimeOfDay.h"		// Represents an amount of time or time of day.
#include "Date.h"			// Represents a date. y m d.
#include "SliceOfData.h"	// Represents a slice of weather data identified by PLACE and timeofday.
#include "Door.h"
#include "Day.h"			// Represents a day, having two vectors of SliceOfData (inside/outside).
#include "Timing.h"			// Used to simplify timing algorithms.
#include "Manager.h"		// The manager that contains the days.
#include "SFMLSTUFF.h"

#endif //PCH_H
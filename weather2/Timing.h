#pragma once

class Timing {

	// The time that the clock started. Done at start.
	std::chrono::high_resolution_clock::time_point startTimePoint;
	unsigned int microseconds;

	// Such that unwanted functionality is guarded from.
	bool started;
	bool finished;
public:

	// Initializes the Timer.
	Timing() {
		this->started = false;
		this->finished = false;
	}

	// Starts the timer.
	void start() {
		// Must be before to not affect the timing.
		// A timer can be started several time.
		// Will just move the start time.
		// What if one calls start, finish, start, getseconds?
		// microseconds receives an unsigned int, so difference 
		// is the same and you have to know in which direction.
		this->started = true;
		this->startTimePoint = std::chrono::high_resolution_clock::now();
	}

	// Finishes the timer.
	void finish() {
		// So that we don't operate on variables that have not been initialized.
		if (this->started) {
			auto elapsed = std::chrono::high_resolution_clock::now() - this->startTimePoint;
			this->microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

			// Must be after to not affect the timing.
			this->finished = true;
		}
	}

	// Gets time in microseconds as an unsigned long int.
	unsigned int getMicroseconds() {
		// So that we don't return uninitialized variables.
		if (this->finished)
			return this->microseconds;
		else
			return -1;
	}

	// Gets time in seconds as a long double.
	double getSeconds() {
		// Returned only if finished, such that we know
		// that this->microseconds has been calculated
		if (this->finished)
			return ((double) this->getMicroseconds()) / 1000000;
		else
			return -1.f;
	}

};
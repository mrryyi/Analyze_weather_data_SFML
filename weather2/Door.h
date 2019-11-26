#pragma once
class Door
{
public:
	Door();
	~Door();

	bool anythingHasHappenedWithTheDoor = false;

	std::vector<TimeOfDay*> opened;
	std::vector<TimeOfDay*> closed;

	void addTimeOfDay(TimeOfDay* time, bool opened) {
		this->anythingHasHappenedWithTheDoor = true;
		if (opened) {
			this->opened.push_back(time);
		}
		else {
			this->closed.push_back(time);
		}
	}

	std::vector<TimeOfDay*> getOpened() {
		return opened;
	}

	std::vector<TimeOfDay*> getClosed() {
		return closed;
	}

	int getOpenedSize() {
		return this->opened.size();
	}

	int getClosedSize() {
		return this->closed.size();
	}


	bool getHappened() { return this->anythingHasHappenedWithTheDoor; };

	void print() {
		int openedIterator = 0;
		int closedIterator = 0;
		bool running;
		
		while (openedIterator < this->opened.size() && closedIterator < this->closed.size()) {
			if (opened[openedIterator]->isMoreThan(closed[closedIterator])) {
				std::cout << "Closed: ";
				std::cout << closed[closedIterator]->getStr();
				closedIterator++;
			}
			else {
				std::cout << "Opened: ";
				std::cout << opened[openedIterator]->getStr();
				openedIterator++;
			}
			std::cout << std::endl;
		}

		while (openedIterator < this->opened.size()) {
			std::cout << "Opened: ";
			std::cout << opened[openedIterator]->getStr();
			openedIterator++;
			std::cout << std::endl;
		}
		while (closedIterator < this->closed.size()) {
			std::cout << "Closed: ";
			std::cout << closed[closedIterator]->getStr();
			closedIterator++;
			std::cout << std::endl;
		}
		std::cout << std::endl;
		
	}
};
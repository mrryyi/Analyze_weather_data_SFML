#include "pch.h"

// Explanation from pch.h
// TimeOfDay	// Represents an amount of time or time of day.
// Date			// Represents a date. y m d.
// SliceOfData	// Represents a slice of weather data identified by PLACE and timeofday.
// Day			// Represents a day, having two vectors of SliceOfData (inside/outside).
// Timing		// Used to simplify timing algorithms.
// Manager		// The manager that contains the days.

/*

Structure: 
	[Manager]
	Has:
		Seasons
		Days
	Does:
		Reads		file
		Calculates	mold and averages of days
					( Or rather, in an iterating for loop, calls the
					  functions in the days that do so. )
					seasons
					
		Sorts		days
		Searches	for a day
		Prints		days

	Seasons are:
		Just a set of 3 members of
		manager, not a class.

	[Days]
	Has:
		A date
		slices of data
			> inside
			> outside
		Average temperatures
		Average humidities
		Max difference in temperature
	Does:
		Compare itself against another day.
		Average its values.
		Calculates mold risk index and time.

	[Slices of data]
	Has:
		A time of day
		A temperature
		A humidity
		A place
	Does:
		Give if it has mold risk.

*/


int main()
{
	/*
		Setup
	*/
	int windowHeight = 1080*0.8;
	int windowWidth = 1920*0.8;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "My window");
	int graphW = windowWidth - 300;
	int graphH = windowHeight - 100;


	MasterGraph grapher(&window, graphW, graphH);

	sf::Font font;
	if (!font.loadFromFile("C:\\fonts\\bestfont2.TTF")) {
		std::cout << "error font" << std::endl;
	}

	sf::Text buttonText;
	buttonText.setFont(font);
	buttonText.setCharacterSize(20);
	buttonText.setFillColor(sf::Color(0x000000ff));

	Menu mainMenu(buttonText, graphW + 10, 4);

	Button nextGraph{ &buttonText, "Next graph" };
	Button previousGraph{ &buttonText, "Previous graph" };
	Button inputTerminal{ &buttonText, "Terminal input" };
	Button showDoorStuff{ &buttonText, "Show/hide door" };
	mainMenu.addButton(&nextGraph);
	mainMenu.addButton(&previousGraph);
	mainMenu.addButton(&showDoorStuff);
	mainMenu.addButton(&inputTerminal);
	mainMenu.arrangeGraphButtonPositions();

	sf::Text currentDateText;
	currentDateText = buttonText;
	currentDateText.setFillColor(sf::Color(0xffffffff));
	currentDateText.setPosition(sf::Vector2f(20, graphH - 40));

	sf::Text currentTimeText;
	currentTimeText = currentDateText;
	currentTimeText.setPosition(sf::Vector2f(currentDateText.getPosition().x + 190, graphH - 40));

	/*
		Program start
	*/

	std::string filename = "tempdata4.csv";


	Manager manager(filename);

	manager.averageTheValuesOfDays();
	manager.calculateMold();
	manager.calculateDooring();

	bool terminalInput = false;
	bool showDoor = false;
	int choice;

	Date* currentGraphDate = manager.getDay(0)->getDate();
	grapher.addGraphOfDay(currentGraphDate, &manager);
	int index;
	// A main menu choicing.
	bool menuing = true;
	while (menuing) {
		if (window.isOpen())
		{
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();

				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed:
					if (nextGraph.click(sf::Mouse::getPosition(window))) {
						grapher.removeCurrentGraph();

						currentGraphDate = manager.nextDate(currentGraphDate);
						grapher.removeCurrentGraph();
						grapher.addGraphOfDay(currentGraphDate, &manager);
					}
					if (previousGraph.click(sf::Mouse::getPosition(window))) {
						grapher.removeCurrentGraph();
						currentGraphDate = manager.previousDate(currentGraphDate);
						grapher.removeCurrentGraph();
						grapher.addGraphOfDay(currentGraphDate, &manager);
					}
					if (inputTerminal.click(sf::Mouse::getPosition(window))) {
						terminalInput = true;
					}
					if (showDoorStuff.click(sf::Mouse::getPosition(window))) {
						showDoor = !showDoor;
					}
					break;

				}

				// clear the window with black color
				window.clear(sf::Color::Black);

				// draw everything here...


				mainMenu.draw(&window);

				// window.draw(...);

				grapher.draw(sf::Mouse::getPosition(window), showDoor);
				currentDateText.setString(currentGraphDate->getDateAsString());
				currentTimeText.setString(grapher.getStrTimeByMouse(sf::Mouse::getPosition(window)));

				window.draw(currentDateText);
				window.draw(currentTimeText);

				// end the current frame
				window.display();

			}
		}


		if (terminalInput) {

			// Gives instruction of what you can do.
			std::cout << "[1]  Sort days and show a list." << std::endl;
			std::cout << "[2]  Search day and get avg temp." << std::endl;
			std::cout << "[3]  Find season start date." << std::endl;
			std::cout << "[4]  Just print the days." << std::endl;
			std::cout << "[5]  Get door opening shenanigans " << std::endl;
			std::cout << "[6]  Exit." << std::endl;


			std::cout << std::endl;
			std::cout << "Input: ";
			choice = getInteger(1, 6);
			std::cout << std::endl;

			switch (choice) {
			case 1:
				manager.sortMenuAndDisplay();
				break;
			case 2:
				index = manager.searchDay();
				if (index != -1) {
					currentGraphDate = manager.getDay(index)->getDate();
					grapher.removeCurrentGraph();
					grapher.addGraphOfDay(currentGraphDate, &manager);
				}
				break;
			case 3:
				manager.calculateAndPrintSeason("autumn");
				manager.calculateAndPrintSeason("winter");
				break;
			case 4:
				manager.printDays();
				break;
			case 5:
				manager.printDoors();
				break;
			case 6:
				menuing = false;
				break;
			default:
				menuing = false;
				break;
			}
		}
		terminalInput = false;
	}


	
	manager.~Manager();
}
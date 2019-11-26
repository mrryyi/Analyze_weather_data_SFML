#include "pch.h"
#include "SFMLSTUFF.h"

Menu::~Menu()
{
}


void MasterGraph::newTemperatureGraph(std::vector<SliceOfData*> points, sf::Color color, float size) {
	Graph* newGraph = new Graph;
	GraphPoint* point;
	sf::Vector2f pointPos;

	if (lowTemp < 0)
		lowTemp *= -1;
	for (int i = 0; i < points.size(); i++) {
		pointPos.y = (int)(points[i]->getTemperature() * height / (lowTemp + highTemp));
		pointPos.x = (int)((float)points[i]->getTimeOfDay()->getTotalTimeInSeconds() / 86400 * width);

		// Offset by the position of the graph.
		pointPos.x += background.getPosition().x;
		pointPos.y += background.getPosition().y;

		point = new GraphPoint(pointPos, color, size);
		newGraph->addPoint(point);
	}
	this->graphs.push_back(newGraph);
}

void MasterGraph::newHumidityGraph(std::vector<SliceOfData*> points, sf::Color color, float size) {
	Graph* newGraph = new Graph;
	GraphPoint* point;
	sf::Vector2f pointPos;

	for (int i = 0; i < points.size(); i++) {
		pointPos.y = points[i]->getHumidity() * height / 100;
		pointPos.x = (int)((float)points[i]->getTimeOfDay()->getTotalTimeInSeconds() / 86400 * width);

		// Offset by position of the graph.
		pointPos.x += background.getPosition().x;
		pointPos.y += background.getPosition().y;

		point = new GraphPoint(pointPos, color, size);
		newGraph->addPoint(point);
	}
	this->graphs.push_back(newGraph);
}

void MasterGraph::draw(const sf::Vector2i& mouse_pos, bool showDoorStuff) {

	window->draw(background);

	for (int i = 0; i < graphs.size(); i++) {
		graphs[i]->draw(window);
	}

	if (this->door->getHappened() && showDoorStuff) {
		sf::RectangleShape rectangle;
		std::vector<TimeOfDay*> times = this->door->getOpened();
		int x;
		for (int i = 0; i < this->door->getOpenedSize(); i++) {
			x = (int)((float)times[i]->getTotalTimeInSeconds() / 86400 * this->width);
			rectangle = sf::RectangleShape(sf::Vector2f(1, this->height));
			rectangle.setPosition(sf::Vector2f(x, background.getPosition().y));
			rectangle.setFillColor(sf::Color(0x00ff00f0));
			window->draw(rectangle);
		}
		times = this->door->getClosed();
		for (int i = 0; i < this->door->getClosedSize(); i++) {
			x = (int)((float)times[i]->getTotalTimeInSeconds() / 86400 * this->width);
			rectangle = sf::RectangleShape(sf::Vector2f(1, this->height));
			rectangle.setPosition(sf::Vector2f(x, background.getPosition().y));
			rectangle.setFillColor(sf::Color(0xff0000f0));
			window->draw(rectangle);
		}
	}
	
	if (background.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
	{
		//Horizontal line
		sf::RectangleShape horizontalLine(sf::Vector2f(this->width, 1));
		horizontalLine.setPosition(sf::Vector2f(background.getPosition().x, mouse_pos.y));
		//Vertical line
		sf::RectangleShape verticalLine(sf::Vector2f(1, this->height));
		verticalLine.setPosition(sf::Vector2f(mouse_pos.x, background.getPosition().y));	
		

		window->draw(horizontalLine);
		window->draw(verticalLine);
	}



}
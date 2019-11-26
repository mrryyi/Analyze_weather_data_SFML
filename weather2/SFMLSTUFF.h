#pragma once

class ClickableShape : public sf::RectangleShape
{
public:
	ClickableShape() {};
	~ClickableShape() {};

	bool click(const sf::Vector2i& mouse_pos) {
		return getGlobalBounds().contains(mouse_pos.x, mouse_pos.y);
	}
};

class Button : public ClickableShape
{
	sf::Text text;
	float margin;
	sf::Color mouseOverColor = sf::Color(0x00ffffff);
	sf::Color color;

public:

	Button(sf::Text* text, std::string str, float margin = 4.0f, sf::Color color = sf::Color(0xffffffff)) : text(*text), margin(margin)
	{
		this->color = color;
		this->setFillColor(color);
		this->text.setString(str);
		int charSize = this->text.getCharacterSize();
		float charSpacing = this->text.getLetterSpacing();
		float totalWidth = str.size() * charSize * charSpacing;
		this->setSize(sf::Vector2f(totalWidth + margin, charSize + margin * 2));
	}

	void setPositionOfText() {
		this->text.setPosition(this->getPosition().x + margin, this->getPosition().y + margin);
	}

	void draw(sf::RenderWindow* window) {
		window->draw(*this);
		window->draw(this->text);
	}

	void updateBackground(const sf::Vector2i& mouse_pos) {
		if (getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
			this->setFillColor(this->mouseOverColor);
		}
		else {
			this->setFillColor(this->color);
		}
	}

	std::string getText() { return this->text.getString(); };
};


class Menu {
	std::vector<Button*> menuButtons;
	int x;
	int y;
	float defaultMargin = 4.0f;
	sf::Color defaultColor = sf::Color(0xffffffff);
	sf::Text textStats;
public:
	Menu(sf::Text buttonStats, int x = 5, int y = 5) : textStats(buttonStats), x(x), y(y) {};
	~Menu();

	void arrangeGraphButtonPositions() {
		float yPos = this->y;
		float previousHeight = 0.0f;
		for (int i = 0; i < menuButtons.size(); i++) {
			menuButtons[i]->setPosition(this->x, yPos);
			std::cout << "i: " << i << ", prev: " << previousHeight << ", yPos: " << yPos << std::endl;
			menuButtons[i]->setPositionOfText();
			previousHeight = menuButtons[i]->getSize().y;
			yPos += defaultMargin + previousHeight;
		}
	}

	void addButton(Button* butt) {
		menuButtons.push_back(butt);
	}

	void draw(sf::RenderWindow* window) {
		for (int i = 0; i < menuButtons.size(); i++) {
			menuButtons[i]->draw(window);
		}
	}

};

class GraphPoint : sf::RectangleShape
{
public:
	GraphPoint(sf::Vector2f pos, sf::Color color = sf::Color(0x00ff00ff), float size = 2.0f) {
		this->setSize(sf::Vector2f(size, size));
		this->setFillColor(color);
		this->setPosition(pos);
	};

	void DRAW(sf::RenderWindow* window) {

		window->draw(*this);
	}

	~GraphPoint() {};
};


class Graph
{
	std::vector<GraphPoint*> points;

public:
	Graph() {};

	void addPoint(GraphPoint* point) {
		this->points.push_back(point);
	}

	~Graph() {
		for (int i = 0; i < points.size(); i++) {
			delete points[i];
		}
	}

	void draw(sf::RenderWindow * window) {
		for (int i = 0; i < this->points.size(); i++) {
			this->points[i]->DRAW(window);
		}
	}

};

class MasterGraph : sf::RectangleShape
{

	std::vector<Graph*> graphs;

	sf::RectangleShape background;

	int width;
	int height;

	int interval;

	int lowTemp;
	int highTemp;

	sf::RenderWindow* window;

	Door* door;


public:
	MasterGraph(sf::RenderWindow* w, int width = 600, int height = 200, int low = -50, int high = 50, int interval = 5) : window(w), lowTemp(low), highTemp(high), interval(interval)
	{
		this->background = sf::RectangleShape(sf::Vector2f(width, height));
		this->background.setPosition(sf::Vector2f(5, 5));
		this->background.setFillColor(sf::Color(0x000000ff));
		this->background.setOutlineColor(sf::Color(0xffffffff));
		this->background.setOutlineThickness(2.0f);
		this->width = width;
		this->height = height;
	};

	void newTemperatureGraph(std::vector<SliceOfData*> points, sf::Color color, float size);
	void newHumidityGraph(std::vector<SliceOfData*> points, sf::Color color, float size);
	void draw(const sf::Vector2i & mouse_pos, bool showDoorStuff);

	void setNewPointsBySlice(std::vector<SliceOfData* > points, std::string humtem, std::string inOrOut) {


		//int graphColors[4] = { 0x00ffffff, 0x0042ffff, 0xff0000ff, 0xff5d00ff };

		int graphColors[4] = {
			0x0000ffff,
			0x00ff00ff,
			0x0000ffff,
			0x00ff00ff
		};

		if (humtem == "temperature" || humtem == "both") {
			if (inOrOut == "inside")
				newTemperatureGraph(points, sf::Color(graphColors[0]), 3.0f);
			if (inOrOut == "outside")
				newTemperatureGraph(points, sf::Color(graphColors[2]), 1.0f);
		}

		if (humtem == "humidity" || humtem == "both") {
			if (inOrOut == "inside")
				newHumidityGraph(points, sf::Color(graphColors[1]), 3.0f);
			if (inOrOut == "outside")
				newHumidityGraph(points, sf::Color(graphColors[3]), 1.0f);
		}
	}

	std::string getStrTimeByMouse(const sf::Vector2i& mouse_pos) {

		if (background.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {

			float margin_x = this->background.getPosition().x;

			float seconds = ((mouse_pos.x - margin_x) / this->width) * 86400;

			TimeOfDay time((int)seconds);

			return time.getStr();

		}

		return "00:00:00";

	}

	~MasterGraph() {
		for (int i = 0; i < graphs.size(); i++) {
			delete graphs[i];
		}
	}

	void addGraphOfDay(Date* date, Manager* manager, std::string humortemp = "both", std::string inOrOut = "both") {
		int index = manager->getDayIndex(date);
		Day* day;
		if (index != -1) {
			day = manager->getDay(index);
			this->door = day->getDoor();
			if (inOrOut == "both" || inOrOut == "inside")
				setNewPointsBySlice(day->getInsideData(),humortemp, "inside");
			if (inOrOut == "both" || inOrOut == "outside")
				setNewPointsBySlice(day->getOutsideData(),humortemp, "outside");
		}
	}

	void removeCurrentGraph() {
		while (graphs.size() > 0) {
			graphs.pop_back();
		}
	}

};
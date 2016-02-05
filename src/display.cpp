#include "display.h"

#include "game.h"

Display::Display(Game* _game)
  : game(_game), window(sf::VideoMode(CFG->readInt("WindowWidth"), CFG->readInt("WindowHeight")), CFG->readString("WindowTitle")),
	STATUS_BAR_WIDTH(CFG->readInt("StatusBarWidth")), WORLD_SIZE(CFG->readInt("WorldSize")), VIEW_MOVE_DELTA(CFG->readInt("ViewMoveDelta")),
	windowWidth(CFG->readInt("WindowWidth")), windowHeight(CFG->readInt("WindowHeight")), hasFocus(true) {
	window.setVerticalSyncEnabled(true);
	
	// circle defining the animal
	int animalRadius = CFG->readInt("AnimalRadius");
	animalShape.setRadius(animalRadius);
	animalShape.setOrigin(animalRadius, animalRadius);
	animalShape.setOutlineThickness(-2);
	animalShape.setFillColor(sf::Color(0, 0, 0));
	// animal's head, represented by a triangle at the front of the animal
	animalHeadShape.setPointCount(3);
	animalHeadShape.setPoint(0, sf::Vector2f(animalRadius, -animalRadius/2));
	animalHeadShape.setPoint(1, sf::Vector2f(animalRadius + sqrt(3 * animalRadius*animalRadius / 4), 0));
	animalHeadShape.setPoint(2, sf::Vector2f(animalRadius, animalRadius/2));
	animalHeadShape.setFillColor(sf::Color(0, 0, 0));
	
	int fruitRadius = CFG->readInt("FruitRadius");
	fruitShape.setRadius(fruitRadius);
	fruitShape.setOrigin(fruitRadius, fruitRadius);
	fruitShape.setFillColor(sf::Color::Green);
	
	lineShape.setSize(sf::Vector2f(15, 1));
	
	font.loadFromFile("files/DroidSans.ttf");
	text.setFont(font);
	text.setCharacterSize(16);
	text.setStyle(sf::Text::Bold);
	
	// options des views
	mainView.setCenter(WORLD_SIZE/2, WORLD_SIZE/2);
	mainView.setSize(windowWidth, windowHeight);
}

void Display::update(const EntityManager &manager, const float &_interpolation) {
	interpolation = _interpolation;
	
	// evenements liés au déplacement de la caméra
	cameraEvents();
	
	// display game elements
	window.clear();
	displayGame(manager, mainView);
	displayUI(manager);
	window.display();
}

void Display::events() {
	sf::Event event;
	
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed :
				game->quit();
				break;
				
			case sf::Event::KeyReleased :
				switch (event.key.code) {
					case sf::Keyboard::Escape :
						game->quit();
						break;
						
					default :
						break;
				}
				break;
			
			case sf::Event::KeyPressed :
				switch (event.key.code) {
					// Accelerer/ralentir le temps
					case sf::Keyboard::Right :
						game->increaseGameSpeed();
						break;
					case sf::Keyboard::Left :
						game->decreaseGameSpeed();
						break;
						
					// Zoom/dezoom de la view
					case sf::Keyboard::Up :
						mainView.zoom(0.8);
						break;
					case sf::Keyboard::Down :
						mainView.zoom(1.2);
						break;
						
					default :
						break;
				}
				break;
			
			case sf::Event::GainedFocus :
				hasFocus = true;
				break;
			
			case sf::Event::LostFocus :
				hasFocus = false;
				break;
				
			default :
				break;
		}
	}
}

void Display::displayGame(const EntityManager &manager, const sf::View &view) {
	// Vue des éléments du jeu
	window.setView(view);
	
	drawFruits(manager.getFruits(), view);
	
	std::vector<Species> species = manager.getSpecies();
	for (int i = 0; i < (int) species.size(); i++) {
		speciesColor(i);
		drawAnimals(species[i].tab, view);
	}
	
	drawGameBorders(view);
}

void Display::displayUI(const EntityManager &manager) {
	// Vue des éléments de l'UI
	window.setView(window.getDefaultView());
	
	// Texte info en haut à gauchean
	std::stringstream ss;
	ss << "Generation #" << game->getGeneration() << std::endl;
	ss << "Iterations : " << (int) game->getIterations() << "/" << game->getIterationsPerGeneration() << std::endl;
	ss << "GameSpeed : " << game->getGameSpeedRatio() << std::endl;
	ss << "FPS : " << game->getFps() << std::endl << "UPS : " << game->getUps() << std::endl;
	
	text.setString(ss.str());
	text.setPosition(10, 10);
	window.draw(text);
}

void Display::cameraEvents() {
	// Evenements de déplacement de la caméra
	if (hasFocus) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			mainView.move(0, -VIEW_MOVE_DELTA);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			mainView.move(-VIEW_MOVE_DELTA, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			mainView.move(0, VIEW_MOVE_DELTA);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			mainView.move(VIEW_MOVE_DELTA, 0);
	}
}

void Display::drawFruits(const std::vector<Fruit*> &fruits, const sf::View &view) {
	for (unsigned int i = 0; i < fruits.size(); i++) {
		if (!isInsideView(fruits[i]->getPos(), view))
			continue;
		
		fruitShape.setPosition(fruits[i]->getPos().x, fruits[i]->getPos().y);
		window.draw(fruitShape);
	}
}

void Display::drawAnimals(const std::vector<Animal*> &animals, const sf::View &view) {
	for (unsigned int i = 0; i < animals.size(); i++) {
		Vect2i animalDisplayPos = animals[i]->getDisplayPos(interpolation);
		if (!animals[i]->isAlive() || !isInsideView(animalDisplayPos, view))
			continue;
		
		animalShape.setPosition(animalDisplayPos.x, animalDisplayPos.y);
		animalShape.setRotation(animals[i]->getAngle());
		window.draw(animalShape);
		animalHeadShape.setPosition(animalDisplayPos.x, animalDisplayPos.y);
		animalHeadShape.setRotation(animals[i]->getAngle());
		window.draw(animalHeadShape);
		
		// vecteur vers le plus proche fruit
		if (animals[i]->getClosestFruitAngle() != 0)
			drawVector(animalDisplayPos, animals[i]->getClosestFruitAngle(), sf::Color::Green, sf::Vector2f(15, 1));
		// vecteur vers le plus proche ennemi
		if (animals[i]->getClosestEnemyAngle() != 0)
			drawVector(animalDisplayPos, animals[i]->getClosestEnemyAngle(), sf::Color::Red, sf::Vector2f(15, 1));
		// vecteur vers le plus proche ennemi
		if (animals[i]->getClosestAllyAngle() != 0)
			drawVector(animalDisplayPos, animals[i]->getClosestAllyAngle(), sf::Color::Blue, sf::Vector2f(15, 1));
		
		// dessin de l'angle relatif de l'ennemi
		/*
		Vect2i barPosition;
		barPosition.x = animalDisplayPos.x - STATUS_BAR_WIDTH/2;
		barPosition.y = animalDisplayPos.y + animalShape.getLocalBounds().height * 3 / 2;
		drawVector(barPosition, 0, sf::Color(100, 0, 0), sf::Vector2f(animals[i]->getEnemyRelativeAngle() * STATUS_BAR_WIDTH / 360, 2));
		// dessin de la barre de defense
		barPosition.y += 4;
		drawVector(barPosition, 0, sf::Color(0, 0, 100), sf::Vector2f(animals[i]->getAllyRelativeAngle() * STATUS_BAR_WIDTH / 360, 2));
		//*/
		// score
		std::stringstream ss;
		ss << animals[i]->getScore();
		text.setString(ss.str());
		text.setPosition(animalDisplayPos.x - text.getLocalBounds().width / 2,
						 animalDisplayPos.y - animalShape.getLocalBounds().height * 3 / 2 - text.getLocalBounds().height);
		window.draw(text);
	}
}

void Display::drawGameBorders(const sf::View &view) {
	if (mainView.getCenter().x - mainView.getSize().x / 2 < 0)
		drawVector(Vect2i(0, 0), 90, sf::Color::White, sf::Vector2f(WORLD_SIZE, 2));
	
	if (mainView.getCenter().y - mainView.getSize().y / 2 < 0)
		drawVector(Vect2i(0, 0), 0, sf::Color::White, sf::Vector2f(WORLD_SIZE, 2));
	
	if (mainView.getCenter().x + mainView.getSize().x / 2 >= WORLD_SIZE)
		drawVector(Vect2i(WORLD_SIZE, 0), 90, sf::Color::White, sf::Vector2f(WORLD_SIZE, 2));
	
	if (mainView.getCenter().y + mainView.getSize().y / 2 >= WORLD_SIZE)
		drawVector(Vect2i(0, WORLD_SIZE), 0, sf::Color::White, sf::Vector2f(WORLD_SIZE, 2));
}

void Display::speciesColor(int index) {
	switch(index) {
		case FOX :
			animalShape.setOutlineColor(sf::Color(245, 150, 0));
			animalHeadShape.setFillColor(sf::Color(245, 150, 0));
			break;
		case SNAKE :
			animalShape.setOutlineColor(sf::Color(12, 216, 49));
			animalHeadShape.setFillColor(sf::Color(12, 216, 49));
			break;
		case CHICKEN :
			animalShape.setOutlineColor(sf::Color(150, 150, 150));
			animalHeadShape.setFillColor(sf::Color(150, 150, 150));
			break;
		case LYNX :
			animalShape.setOutlineColor(sf::Color(255, 245, 169));
			animalHeadShape.setFillColor(sf::Color(255, 245, 169));
			break;
		case MONKEY :
			animalShape.setOutlineColor(sf::Color(160, 90, 69));
			animalHeadShape.setFillColor(sf::Color(160, 90, 69));
			break;
		case FISH :
			animalShape.setOutlineColor(sf::Color(177, 175, 249));
			animalHeadShape.setFillColor(sf::Color(177, 175, 249));
			break;
		default :
			animalShape.setOutlineColor(sf::Color(150, 150, 150));
			animalHeadShape.setFillColor(sf::Color(150, 150, 150));
			break;
	}
}

void Display::drawVector(const Vect2i &pos, const float &angle, const sf::Color &color, const sf::Vector2f &size) {
	lineShape.setFillColor(color);
	lineShape.setPosition(pos.x, pos.y);
	lineShape.setRotation(angle);
	lineShape.setSize(size);
	window.draw(lineShape);
}

bool Display::isInsideView(const Vect2i &pos, const sf::View &view) {
	return ((pos.x >= view.getCenter().x - view.getSize().x / 2) &&
			(pos.x < view.getCenter().x + view.getSize().x / 2) &&
			(pos.y >= view.getCenter().y - view.getSize().y / 2) &&
			(pos.y < view.getCenter().y + view.getSize().y / 2));
}

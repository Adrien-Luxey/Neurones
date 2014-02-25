#include "display.h"

#include "game.h"

Display::Display(Game* _game)
  : game(_game), window(sf::VideoMode(CFG->readInt("WindowWidth"), CFG->readInt("WindowHeight")), CFG->readString("WindowTitle")),
	statusBarWidth(CFG->readInt("StatusBarWidth")), worldSize(CFG->readInt("WorldSize")), viewMoveDelta(CFG->readInt("ViewMoveDelta")),
	windowWidth(CFG->readInt("WindowWidth")), windowHeight(CFG->readInt("WindowHeight")), hasFocus(true) {
	window.setVerticalSyncEnabled(true);
	
	animalShape.setSize(sf::Vector2f(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight")));
	animalShape.setOrigin(CFG->readInt("AnimalWidth")/2, CFG->readInt("AnimalHeight")/2);
	animalShape.setOutlineThickness(-2);
	animalShape.setFillColor(sf::Color(0, 0, 0));
	
	fruitShape.setRadius(CFG->readInt("FruitRadius"));
	fruitShape.setOrigin(CFG->readInt("FruitRadius"), CFG->readInt("FruitRadius"));
	fruitShape.setFillColor(sf::Color::Green);
	
	lineShape.setSize(sf::Vector2f(15, 1));
	
	font.loadFromFile("files/DroidSans.ttf");
	text.setFont(font);
	text.setCharacterSize(16);
	text.setStyle(sf::Text::Bold);
	
	// options des views
	mainView.setCenter(worldSize/2, worldSize/2);
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
						
					case sf::Keyboard::Space :
						game->togglePause();
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
	
	// Texte info en haut à gauche
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
			mainView.move(0, -viewMoveDelta);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			mainView.move(-viewMoveDelta, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			mainView.move(0, viewMoveDelta);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			mainView.move(viewMoveDelta, 0);
	}
}

void Display::drawFruits(const std::vector<Entity*> &fruits, const sf::View &view) {
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
		
		// set la bonne position à l'animalShape
		animalShape.setPosition(animalDisplayPos.x, animalDisplayPos.y);
		animalShape.setRotation(animals[i]->getAngle());
		
		// dessin de l'animalShape
		window.draw(animalShape);
		
		// vecteur vers le plus proche fruit
		if (animals[i]->getClosestFruitAngle() != 0)
			drawVector(animalDisplayPos, animals[i]->getClosestFruitAngle(), sf::Color::Green, sf::Vector2f(15, 1));
		// vecteur vers le plus proche ennemi
		if (animals[i]->getClosestEnemyAngle() != 0)
			drawVector(animalDisplayPos, animals[i]->getClosestEnemyAngle(), sf::Color::Red, sf::Vector2f(15, 1));
		// vecteur vers le plus proche ennemi
		if (animals[i]->getClosestAllyAngle() != 0)
			drawVector(animalDisplayPos, animals[i]->getClosestAllyAngle(), sf::Color::Blue, sf::Vector2f(15, 1));
		
		// dessin de la barre d'attaque
		Vect2i barPosition;
		barPosition.x = animalDisplayPos.x - statusBarWidth/2;
		barPosition.y = animalDisplayPos.y + animalShape.getLocalBounds().height * 3 / 2;
		drawVector(barPosition, 0, sf::Color(100, 0, 0), sf::Vector2f(animals[i]->getAttackRate() * statusBarWidth, 2));
		// dessin de la barre de defense
		//barPosition.y += 4;
		drawVector(barPosition, 0, sf::Color(0, 0, 100), sf::Vector2f(animals[i]->getDefenseRate() * statusBarWidth, 2));
		
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
		drawVector(Vect2i(0, 0), 90, sf::Color::White, sf::Vector2f(worldSize, 2));
	
	if (mainView.getCenter().y - mainView.getSize().y / 2 < 0)
		drawVector(Vect2i(0, 0), 0, sf::Color::White, sf::Vector2f(worldSize, 2));
	
	if (mainView.getCenter().x + mainView.getSize().x / 2 >= worldSize)
		drawVector(Vect2i(worldSize, 0), 90, sf::Color::White, sf::Vector2f(worldSize, 2));
	
	if (mainView.getCenter().y + mainView.getSize().y / 2 >= worldSize)
		drawVector(Vect2i(0, worldSize), 0, sf::Color::White, sf::Vector2f(worldSize, 2));
}

void Display::speciesColor(int index) {
	switch(index) {
		case FOX :
			animalShape.setOutlineColor(sf::Color(245, 150, 0));
			break;
		case SNAKE :
			animalShape.setOutlineColor(sf::Color(12, 216, 49));
			break;
		case CHICKEN :
			animalShape.setOutlineColor(sf::Color(150, 150, 150));
			break;
		case LYNX :
			animalShape.setOutlineColor(sf::Color(255, 245, 169));
			break;
		case MONKEY :
			animalShape.setOutlineColor(sf::Color(160, 90, 69));
			break;
		case FISH :
			animalShape.setOutlineColor(sf::Color(177, 175, 249));
			break;
		default :
			animalShape.setOutlineColor(sf::Color(150, 150, 150));
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

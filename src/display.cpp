#include "display.h"

#include "game.h"

Display::Display(Game* _game)
  : game(_game), window(sf::VideoMode(CFG->readInt("WindowWidth"), CFG->readInt("WindowHeight")), CFG->readString("WindowTitle")),
	statusBarWidth(CFG->readInt("StatusBarWidth")) {
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
	
	clock.restart();
}

void Display::update(EntityManager &manager) {
	// window clear
	window.clear();
	
	drawFruits(manager.getFruits());
	
	std::vector<Species> species = manager.getSpecies();
	for (int i = 0; i < (int) species.size(); i++) {
		speciesColor(i);
		drawAnimals(species[i].tab);
	}
	
	std::stringstream ss;
	ss << "Generation #" << game->getGeneration() << std::endl;
	ss << "Timer : " << (int) game->getElapsedTime() << "/" << game->getEpocDuration() << std::endl;
	ss << "GameSpeed : " << game->getGameSpeed() << std::endl;
	ss << "FPS : " << game->getFps() / game->getGameSpeed() << std::endl;
	text.setString(ss.str());
	text.setPosition(10, 10);
	window.draw(text);
	
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
					case sf::Keyboard::Up :
						game->increaseGameSpeed();
						break;
					
					case sf::Keyboard::Down :
						game->decreaseGameSpeed();
						break;
					
					default :
						break;
				}
				break;
			
			default :
				break;
		}
	}
}

float Display::getElapsedTime() {
	return clock.restart().asSeconds();
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

void Display::drawFruits(const std::vector<Entity*> &fruits) {
	for (unsigned int i = 0; i < fruits.size(); i++) {
		fruitShape.setPosition(fruits[i]->getPos().x, fruits[i]->getPos().y);
		window.draw(fruitShape);
	}
}

void Display::drawAnimals(const std::vector<Animal*> &animals) {
	for (unsigned int i = 0; i < animals.size(); i++) {
		if (!animals[i]->isAlive())
			continue;
		
		// set la bonne position à l'animalShape
		animalShape.setPosition(animals[i]->getPos().x, animals[i]->getPos().y);
		animalShape.setRotation(animals[i]->getAngle());
		
		//animalShape.setFillColor(sf::Color(255, 0, 0, 255 * animal->getAttackRate()));
		
		// dessin de l'animalShape
		window.draw(animalShape);
		
		// vecteur vers le plus proche fruit
		drawVector(animals[i]->getPos(), animals[i]->getClosestFruitAngle(), sf::Color::Green, sf::Vector2f(15, 1));
		// vecteur vers le plus proche ennemi
		drawVector(animals[i]->getPos(), animals[i]->getClosestEnemyAngle(), sf::Color::Red, sf::Vector2f(15, 1));
		
		// dessin de la barre d'attaque
		Vect2i barPosition;
		barPosition.x = animals[i]->getPos().x - statusBarWidth/2;
		barPosition.y = animals[i]->getPos().y + animalShape.getLocalBounds().height * 3 / 2;
		drawVector(barPosition, 360.f, sf::Color(100, 0, 0), sf::Vector2f(animals[i]->getAttackRate() * statusBarWidth, 2));
		// dessin de la barre de defense
		barPosition.y += 4;
		drawVector(barPosition, 360.f, sf::Color(0, 0, 100), sf::Vector2f(animals[i]->getDefenseRate() * statusBarWidth, 2));
		
		// score
		std::stringstream ss;
		ss << animals[i]->getScore();
		text.setString(ss.str());
		text.setPosition(animals[i]->getPos().x - text.getLocalBounds().width / 2,
						 animals[i]->getPos().y - animalShape.getLocalBounds().height * 3 / 2 - text.getLocalBounds().height);
		window.draw(text);
	}
}

void Display::drawVector(const Vect2i &pos, const float &angle, const sf::Color &color, const sf::Vector2f &size) {
	if (angle != 0.f) {
		lineShape.setFillColor(color);
		lineShape.setPosition(pos.x, pos.y);
		lineShape.setRotation(angle);
		lineShape.setSize(size);
		window.draw(lineShape);
	}
}

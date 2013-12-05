#include "display.h"

#include "game.h"

Display::Display(Game* _game)
: game(_game), window(sf::VideoMode(CFG->readInt("WindowWidth"), CFG->readInt("WindowHeight")), CFG->readString("WindowTitle")) {
	window.setVerticalSyncEnabled(true);
	
	animalShape.setSize(sf::Vector2f(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight")));
	animalShape.setOrigin(CFG->readInt("AnimalWidth")/2, CFG->readInt("AnimalHeight")/2);
	animalShape.setOutlineThickness(-2);
	
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

void Display::update(const EntityManager &manager) {
	// window clear
	window.clear();
	
	for (int i = 0; i < (int) manager.getEntities().size(); i++) {
		
		if (i == manager.getFruitsIndex()) {
			drawFruits(manager.getEntities()[i].tab);
			
		} else if (i >= manager.getAnimalsIndex() &&
		  i < manager.getAnimalsIndex() + manager.getSpeciesNumber()) {
			speciesColor(i - manager.getAnimalsIndex());
			drawAnimals(manager.getEntities()[i].tab);
		}
	}
	
	std::stringstream ss;
	ss << "Generation #" << game->getGeneration() << std::endl;
	ss << "Timer : " << (int) game->getElapsedTime() << "/" << game->getEpocDuration() << std::endl;
	ss << "FPS : " << game->getFps();
	text.setString(ss.str());
	text.setPosition(10, 10);
	window.draw(text);
	
	window.display();
}

void Display::events() {
	sf::Event event;
	
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed ||
			(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
			game->quit();
		if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space))
			game->togglePause();
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

void Display::drawAnimals(const std::vector<Entity*> &animals) {
	Animal *animal;
	
	for (unsigned int i = 0; i < animals.size(); i++) {
		animal = (Animal*) animals[i];
		
		if (!animal->isAlive())
			continue;
		
		// set la bonne position à l'animalShape
		animalShape.setPosition(animal->getPos().x, animal->getPos().y);
		animalShape.setRotation(animal->getAngle());
		
		if (animal->isAttacking())
			animalShape.setFillColor(sf::Color(100, 100, 100));
		else
			animalShape.setFillColor(sf::Color::Black);
		
		// dessin de l'animalShape
		window.draw(animalShape);
		
		// vecteurs vers le plus proche ennemi et la bouffe
		lineShape.setFillColor(sf::Color::Green);
		lineShape.setPosition(animal->getPos().x, animal->getPos().y);
		lineShape.setRotation(animal->getClosestPrayAngle());
		window.draw(lineShape);
		lineShape.setFillColor(sf::Color::Red);
		lineShape.setPosition(animal->getPos().x, animal->getPos().y);
		lineShape.setRotation(animal->getClosestPredatorAngle());
		window.draw(lineShape);
		
		// score
		std::stringstream ss;
		ss << animal->getScore();
		text.setString(ss.str());
		text.setPosition(animal->getPos().x, animal->getPos().y - 4 * animal->getSize().x / 3);
		window.draw(text);
	}
}

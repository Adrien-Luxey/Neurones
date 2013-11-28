#include "display.h"

#include "game.h"

Display::Display(Game* _game)
: game(_game), window(sf::VideoMode(CFG->readInt("WindowWidth"), CFG->readInt("WindowHeight")), CFG->readString("WindowTitle")) {
	sf::IntRect rect;
	
	window.setVerticalSyncEnabled(false);
	
	animal.setSize(sf::Vector2f(CFG->readInt("AnimalWidth"), CFG->readInt("AnimalHeight")));
	animal.setOrigin(CFG->readInt("AnimalWidth")/2, CFG->readInt("AnimalHeight")/2);
	animal.setFillColor(sf::Color::Black);
	animal.setOutlineThickness(-2);
	
	line.setSize(sf::Vector2f(15, 1));
	line.setFillColor(sf::Color(150, 150, 150));
	
	font.loadFromFile("files/DroidSans.ttf");
	text.setFont(font);
	text.setCharacterSize(16);
	text.setStyle(sf::Text::Bold);
	
	clock.restart();
}

void Display::update(std::vector<Entity*> entities) {
	// window clear
	window.clear();
	
	// Pour chaque eneité vivante
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (((Animal*) entities[i])->isDead())
			continue;
			
		// set la bonne position à l'animal
		animal.setPosition(entities[i]->getPos().x, entities[i]->getPos().y);
		animal.setRotation(entities[i]->getAngle());
		
		// couleur
		switch(entities[i]->type()) {
			case FOX :
				animal.setOutlineColor(sf::Color(245, 150, 0));
				break;
			case SNAKE :
				animal.setOutlineColor(sf::Color(12, 216, 49));
				break;
			case CHICKEN :
				animal.setOutlineColor(sf::Color(150, 150, 150));
				break;
			case LYNX :
				animal.setOutlineColor(sf::Color(255, 245, 169));
				break;
			case MONKEY :
				animal.setOutlineColor(sf::Color(160, 90, 69));
				break;
		}
		
		// dessin de l'animal
		window.draw(animal);
		
		// vecteurs vers le plus proche ennemi et la bouffe
		line.setFillColor(sf::Color::Green);
		line.setPosition(entities[i]->getPos().x, entities[i]->getPos().y);
		line.setRotation(((Animal*) entities[i])->getClosestFoodAngle());
		window.draw(line);
		line.setFillColor(sf::Color::Red);
		line.setPosition(entities[i]->getPos().x, entities[i]->getPos().y);
		line.setRotation(((Animal*) entities[i])->getClosestEnemyAngle());
		window.draw(line);
		
		// score
		std::stringstream ss;
		ss << ((Animal*) entities[i])->getScore();
		text.setString(ss.str());
		text.setPosition(entities[i]->getPos().x, entities[i]->getPos().y - 4 * CFG->readInt("TileSize") / 3);
		window.draw(text);
	}
	
	std::stringstream ss;
	ss << "Generation #" << game->getGeneration() << std::endl << Stats::highScore(entities) << std::endl <<  Stats::averageScore(entities) << std::endl << Stats::totalScore(entities) << std::endl << Stats::printDetailledScore(entities) << std::endl << "Timer :\t" << (int) game->getElapsedTime() << "/" << CFG->readInt("EpocDuration") << std::endl << "FPS :\t\t" << game->getFps();
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

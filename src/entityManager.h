#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

#include "utils.h"

class EntityManager {
	public :
		EntityManager();
		~EntityManager();

		void update(const float dt);
		
		const std::vector<std::vector<Entity*>> getEntities() { return entities; }
		
	private :
		std::vector<std::vector<Entity*>> entities;
                int fruitsIndex, animalsIndex, speciesNumber;
                
                void update(const Animal *animal, const int index, const float dt);
};

#endif // ENTITY_MANAGER_H

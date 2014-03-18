#ifndef FRUIT_H
#define FRUIT_H

#include <stdlib.h>

#include "utils.h"
#include "entity.h"

/// \brief Structure representig a bush : point and size where fruits have high probabylity to appear
typedef struct Bush {
	Vect2i pos; ///< Center of the bush
	int size;	///< size of the bush (to be used in a gaussian probability function)
} Bush;

/**
 * \brief class representing a fruit : static element that can be eaten by an animal
 */
class Fruit : public Entity {
	public :
		Fruit();
		virtual ~Fruit() {}

		virtual void init();
		/**
		 * \brief Initialise a fruit somewhere near the bush in parameter
         * @param bush	The bush the fruit belongs to
         */
		void init(const Bush &bush);
};

#endif // FRUIT_H

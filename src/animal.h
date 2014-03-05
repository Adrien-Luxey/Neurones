#ifndef ANIMAL_H
#define ANIMAL_H

#include <vector>
#include <cmath>

#include "entity.h"
#include "neural_network.h"


/**
 * @brief Class representing an animal, containing a brain (NeuralNetwork), that changes the animal's state given game inputs
 */
class Animal : public Entity {
  public :
	Animal();
	virtual ~Animal() {}

	virtual void init();
	/**
	 * @brief Give new random position to the animal and set it's neural network's new DNA
     * @param DNA	The new DNA to give to the NeuralNetwork
     */
	void init(const std::vector<float> &DNA);

	/**
	 * @brief Call the brain, giving it game inputs, to update the animal's state
     * @param inputs	The game inputs to give to the NN
     */
	void update(const std::vector<float> inputs);
	
	void incrementScore();
	
	void die();

	// getters
	bool isAlive() const { return alive; }
	int getScore() const { return score; }
	int getClosestEnemyAngle() const { return closestEnemyAngle; }
	int getClosestFruitAngle() const { return closestFruitAngle; }
	int getClosestAllyAngle() const { return closestAllyAngle; }
	float getBattleOutput() const { return battleOutput; }
	/**
     * @return If battleOutput > 0, attackValue = battleOutput
     */
	float getAttackValue() const;
	/**
     * @return If battleOutput < 0, defenseFalue = -battleOutput
     */
	float getDefenseValue() const;
	/**
	 * @brief Return a intermediate position between old and new pos given the interpolation rate
     * @param interpolation	The interpolation rate
     * @return				A 2D position between old and new pos
     */
	Vect2i getDisplayPos(const float &interpolation) const;
	
	
	/**
	 * @brief Give the current network's DNA
     * @return	The current animal's NN DNA
     */
	std::vector<float> getDNA();

  protected :
	/**
	 * @brief Update the animal's position given 2 network outputs
     * @param da	[-1; 1] value representing the angular difference between old and new pos
     * @param dp	[-1; 1] value representing the linear difference between old and new pos
     */
	void updatePosition(float da, float dp);
	
	Vect2i lastPos, speed;
	
	int score;
	int closestEnemyAngle, closestFruitAngle, closestAllyAngle;
	
	// NN output representing the att/def value of the animal
	float battleOutput;
	
	bool alive;
	
	const int ANIMAL_LINEAR_SPEED, ANIMAL_ANGULAR_SPEED;

	NeuralNetwork network;
};

#endif // ANIMAL_H

/*
=================
cEnemy.h
- Header file for class definition - SPECIFICATION
- Header file for the Enemy class which is a child of cSprite class
=================
*/
#ifndef _CENEMY_H
#define _CENEMY_H
#include "cSprite.h"

class cEnemy : public cSprite
{
private:
	SDL_Point enemyVelocity;

public:
	cEnemy();

	void update(double deltaTime);		// Player update method
	void setEnemyVelocity(SDL_Point enemyVel);   // Sets the velocity for the enemy
	SDL_Point getEnemyVelocity();				 // Gets the enemy velocity
};
#endif
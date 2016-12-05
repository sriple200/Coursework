/*
=================
cEnemy.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemy.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cEnemy::cEnemy() : cSprite()
{
	this->enemyVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cEnemy::update(double deltaTime)
{

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	if (this->getSpritePos().x < 1024 && this->getSpritePos().x > 1 && this->getSpritePos().y < 768 && this->getSpritePos().y > 1)
	{
	cout << "Enemy position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	}
	this->setBoundingRect(this->getSpritePos());

	//if (this->getSpritePos().y == 384 && this->getSpritePos().x == 512)
	//{

	//}
}
/*
=================================================================
  Sets the velocity for the enemy
=================================================================
*/
void cEnemy::setEnemyVelocity(SDL_Point enemyVel)
{
	enemyVelocity = enemyVel;
}
/*
=================================================================
  Gets the enemy velocity
=================================================================
*/
SDL_Point cEnemy::getEnemyVelocity()
{
	return enemyVelocity;
}

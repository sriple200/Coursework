/*
=================
cPlayer.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"
/*
=================================================================
Defualt Constructor
=================================================================
*/
cPlayer::cPlayer() : cSprite()
{
	this->playerVelocity = { 0.0f, 0.0f };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPlayer::update(double deltaTime)
{

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->playerVelocity.x = this->playerVelocity.x + direction.X;
	this->playerVelocity.y = this->playerVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->playerVelocity.x * deltaTime;
	currentSpritePos.y += this->playerVelocity.y * deltaTime;

	this->playerVelocity.x *= 0.95;
	this->playerVelocity.y *= 0.95;
	
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the Player
=================================================================
*/
void cPlayer::setPlayerVelocity(SDL_Point playerVel)
{
	playerVelocity = playerVel;
}
/*
=================================================================
Gets the Player velocity
=================================================================
*/
SDL_Point cPlayer::getPlayerVelocity()
{
	return playerVelocity;
}


/*
=================================================================
Update the sprite position
=================================================================
*/


//
//	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
//	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
//	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));
//
//	spriteVelocityAdd *= spriteTranslation;
//
//	rocketVelocity += spriteVelocityAdd;
//
//	spritePos2D += rocketVelocity * deltaTime;
//
//	rocketVelocity *= 0.95;
//
//	/*
//	==============================================================
//	| Check for collisions
//	==============================================================
//	*/
//	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
//	{
//		(*bulletIterartor)->update(deltaTime);
//		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
//		{
//			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and asteroid to false
//				(*asteroidIterator)->setActive(false);
//				(*bulletIterartor)->setActive(false);
//			}
//		}
//	}
//
//	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
//	while (bulletIterartor != theRocketBullets.end())
//	{
//		if ((*bulletIterartor)->isActive() == false)
//		{
//			bulletIterartor = theRocketBullets.erase(bulletIterartor);
//		}
//		else
//		{
//			//(*bulletIterartor)->update(deltaTime);
//			(*bulletIterartor)->render();
//			(*bulletIterartor)->renderCollisionBox();
//			++bulletIterartor;
//		}
//	}
//}
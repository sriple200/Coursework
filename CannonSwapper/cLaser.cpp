/*
=================
cLaser.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cLaser.h"

/*
=================================================================
Default Constructor
=================================================================
*/
cLaser::cLaser() : cSprite()
{
	this->laserVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cLaser::update(double deltaTime)
{

	FPoint direction = { 0.0, 0.0 };
	direction.X = (sin((this->getSpriteRotAngle())*PI / 180));
	direction.Y = -(cos((this->getSpriteRotAngle())*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->laserVelocity.x = this->laserVelocity.x + direction.X;
	this->laserVelocity.y = this->laserVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->laserVelocity.x * deltaTime;
	currentSpritePos.y += this->laserVelocity.y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });

	/*if (this->getSpritePos().x < 1024 && this->getSpritePos().x > 1 && this->getSpritePos().y <768 && this->getSpritePos().y > 1)
	{
		cout << "Laser position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	}*/
	this->setBoundingRect(this->getSpritePos());


}
/*
=================================================================
  Sets the velocity for the Laser
=================================================================
*/
void cLaser::setLaserVelocity(SDL_Point laserVel)
{
	this->laserVelocity = laserVel;
}
/*
=================================================================
  Gets the Laser velocity
=================================================================
*/
SDL_Point cLaser::getLaserVelocity()
{
	return this->laserVelocity;
}

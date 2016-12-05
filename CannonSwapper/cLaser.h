/*
=================
cBullet.h
- Header file for class definition - SPECIFICATION
- Header file for the Laser class which is a child of cSprite class
=================
*/
#ifndef _CLASER_H
#define _CLASER_H
#include "cSprite.h"


class cLaser : public cSprite
{
private:
	SDL_Point laserVelocity;

public:
	cLaser();
	void update(double deltaTime);		// Laser update method
	void setLaserVelocity (SDL_Point laserVel);   // Sets the velocity for the Laser
	SDL_Point getLaserVelocity();				 // Gets the Laser velocity
};
#endif
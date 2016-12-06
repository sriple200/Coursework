/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	score = 0;

	// Store the textures
	textureName = { "BlueEnemy", "RedEnemy", "YellowEnemy" ,"GreenEnemy" ,"BlueLaser", "RedLaser", "YellowLaser", "GreenLaser" ,"BlueCannon", "theBackground", "MenuBackground", "GameBackground" };
	texturesToUse = { "Images\\BlueEnemy.png", "Images\\RedEnemy.png", "Images\\YellowEnemy.png", "Images\\GreenEnemy.png", "Images\\BlueLaser.png", "Images\\RedLaser.png", "Images\\YellowLaser.png","Images\\GreenLaser.png", "Images\\BlueCannon.png", "Images\\Background.png", "Images\\MenuBackground.png", "Images\\GameBackground.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the textures for the buttons
	btnNameList = { "exit_btn", "menu_btn", "play_btn"};
	btnTexturesToUse = { "Images\\ExitButton.png", "Images\\MenuButton.png", "Images\\StartButton.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 200, 300 }};

	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}

	theGameState = MENU;
	theBtnType = EXIT;

	// Font for text
	fontList = { "agency" };
	fontsToUse = { "Fonts/agency.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}

	// Create text Textures
	gameTextNames = {"Laser", "Score" ,"rLaserFire", "bLaserFire", "yLaserFire", "gLaserFire", "TitleTxt", "MovementTxt", "LaserTxt", "GameOverTxt"};
	gameTextList = {"Laser", "Score: ", "1","2","3","4","CANNON SWAPPER!", "Use Arrows Keys To Rotate Cannon", "Use Keys 1-4 To Shoot Enemies" , "Thanks for Playing!"};
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("agency")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
	}

	// Load game sounds
	soundList = { "gameTheme" , "laserFire", "enemyDeath", "playerDeath"};
	soundTypes = { MUSIC,  SFX, SFX, SFX};
	soundsToUse = { "Sound/MenuMusic.wav","Sound/Laser_fire.wav", "Sound/Enemy_death.wav", "Sound/Lose_life.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	// Play menu music
	theSoundMgr->getSnd("gameTheme")->play(-1);

	//Apply game background texture
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	//Apply menu background texture
	spriteMenuBkgd.setSpritePos({ 0, 0 });
	spriteMenuBkgd.setTexture(theTextureMgr->getTexture("MenuBackground"));
	spriteMenuBkgd.setSpriteDimensions(theTextureMgr->getTexture("MenuBackground")->getTWidth(), theTextureMgr->getTexture("MenuBackground")->getTHeight());

	//Apply menu background texture
	spriteGameBkgd.setSpritePos({ 0, 0 });
	spriteGameBkgd.setTexture(theTextureMgr->getTexture("GameBackground"));
	spriteGameBkgd.setSpriteDimensions(theTextureMgr->getTexture("GameBackground")->getTWidth(), theTextureMgr->getTexture("GameBackground")->getTHeight());


	// Array of enemies

	//Blue enemies

	for (int blue_enemy = 0; blue_enemy < 20; blue_enemy++)
	{
		bEnemies.push_back(new cEnemy);
		bEnemies[blue_enemy]->setSpritePos({ 500, -50 - (300*blue_enemy) });
		bEnemies[blue_enemy]->setSpriteTranslation({ 0, 95 });
		bEnemies[blue_enemy]->setTexture(theTextureMgr->getTexture("BlueEnemy"));
		bEnemies[blue_enemy]->setSpriteDimensions(theTextureMgr->getTexture("BlueEnemy")->getTWidth(), theTextureMgr->getTexture("BlueEnemy")->getTHeight());
		bEnemies[blue_enemy]->setEnemyVelocity({ 3.0f, 3.0f });
		bEnemies[blue_enemy]->setActive(true);
	}

	// Red Enemies
	for (int red_enemy = 0; red_enemy < 20; red_enemy++)
	{
		rEnemies.push_back(new cEnemy);
		rEnemies[red_enemy]->setSpritePos({ 500, 800 + (300 * red_enemy) });
		rEnemies[red_enemy]->setSpriteTranslation({ 0, -60});
		rEnemies[red_enemy]->setTexture(theTextureMgr->getTexture("RedEnemy"));
		rEnemies[red_enemy]->setSpriteDimensions(theTextureMgr->getTexture("RedEnemy")->getTWidth(), theTextureMgr->getTexture("RedEnemy")->getTHeight());
		rEnemies[red_enemy]->setEnemyVelocity({ 3.0f, 3.0f });
		rEnemies[red_enemy]->setActive(true);

	}
	// Yellow Enemies
		for (int yellow_enemy = 0; yellow_enemy < 20; yellow_enemy++)
	{
		yEnemies.push_back(new cEnemy);
		yEnemies[yellow_enemy]->setSpritePos({ -50 -(300 * yellow_enemy), 360 });
		yEnemies[yellow_enemy]->setSpriteTranslation({ 75, 0 });
		yEnemies[yellow_enemy]->setTexture(theTextureMgr->getTexture("YellowEnemy"));
		yEnemies[yellow_enemy]->setSpriteDimensions(theTextureMgr->getTexture("YellowEnemy")->getTWidth(), theTextureMgr->getTexture("YellowEnemy")->getTHeight());
		yEnemies[yellow_enemy]->setEnemyVelocity({ 3.0f, 3.0f });
		yEnemies[yellow_enemy]->setActive(true);
	}
	// Green Enemies
		for (int green_enemy = 0; green_enemy < 20; green_enemy++)
	{
		gEnemies.push_back(new cEnemy);
		gEnemies[green_enemy]->setSpritePos({ 1050 + (300 * green_enemy), 360 });
		gEnemies[green_enemy]->setSpriteTranslation({ -90, 0 });
		gEnemies[green_enemy]->setTexture(theTextureMgr->getTexture("GreenEnemy"));
		gEnemies[green_enemy]->setSpriteDimensions(theTextureMgr->getTexture("GreenEnemy")->getTWidth(), theTextureMgr->getTexture("GreenEnemy")->getTHeight());
		gEnemies[green_enemy]->setEnemyVelocity({ 3.0f, 3.0f });
		gEnemies[green_enemy]->setActive(true);
	}
		playerCannon.push_back(new cPlayer);
		playerCannon[0]->setSpritePos({ 496, 336 });
		playerCannon[0]->setTexture(theTextureMgr->getTexture("BlueCannon"));
		playerCannon[0]->setSpriteDimensions(theTextureMgr->getTexture("BlueCannon")->getTWidth(), theTextureMgr->getTexture("BlueCannon")->getTHeight());
		playerCannon[0]->setPlayerVelocity({ 0, 0 });
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		spriteMenuBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 265, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("MovementTxt");
		pos = { 200, 500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("LaserTxt");
		pos = { 210, 600, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 300 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	
		// Rend the laser header
		tempTextTexture = theTextureMgr->getTexture("Laser");
		pos = { 850, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Rend the red colour menu header
		tempTextTexture = theTextureMgr->getTexture("rLaserFire");
		pos = { 830, 170, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Rend the blue colour menu header
		tempTextTexture = theTextureMgr->getTexture("bLaserFire");
		pos = { 830, 270, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Rend the yellow colour menu header
		tempTextTexture = theTextureMgr->getTexture("yLaserFire");
		pos = { 830, 370, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Rend the green colour menu header
		tempTextTexture = theTextureMgr->getTexture("gLaserFire");
		pos = { 830, 470, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

	}
	break;
	case PLAYING:
	{
		spriteGameBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render each blue enemy in the vector array
		for (int draw = 0; draw < bEnemies.size(); draw++)
		{
			bEnemies[draw]->render(theRenderer, &bEnemies[draw]->getSpriteDimensions(), &bEnemies[draw]->getSpritePos(), bEnemies[draw]->getSpriteRotAngle(), &bEnemies[draw]->getSpriteCentre(), bEnemies[draw]->getSpriteScale());
		}

		// Render each red enemy in the vector array
		for (int draw = 0; draw < rEnemies.size(); draw++)
		{
			rEnemies[draw]->render(theRenderer, &rEnemies[draw]->getSpriteDimensions(), &rEnemies[draw]->getSpritePos(), rEnemies[draw]->getSpriteRotAngle(), &rEnemies[draw]->getSpriteCentre(), rEnemies[draw]->getSpriteScale());
		}
		// Render each yellow enemy in the vector array
		for (int draw = 0; draw < yEnemies.size(); draw++)
		{
			yEnemies[draw]->render(theRenderer, &yEnemies[draw]->getSpriteDimensions(), &yEnemies[draw]->getSpritePos(), yEnemies[draw]->getSpriteRotAngle(), &yEnemies[draw]->getSpriteCentre(), yEnemies[draw]->getSpriteScale());
		}
		// Render each green enemy in the vector array
		for (int draw = 0; draw < gEnemies.size(); draw++)
		{
			gEnemies[draw]->render(theRenderer, &gEnemies[draw]->getSpriteDimensions(), &gEnemies[draw]->getSpritePos(), gEnemies[draw]->getSpriteRotAngle(), &gEnemies[draw]->getSpriteCentre(), gEnemies[draw]->getSpriteScale());
		}
		// Render the blue laser in the vector array
		for (int draw = 0; draw < bLasers.size(); draw++)
		{
			bLasers[draw]->render(theRenderer, &bLasers[draw]->getSpriteDimensions(), &bLasers[draw]->getSpritePos(), bLasers[draw]->getSpriteRotAngle(), &bLasers[draw]->getSpriteCentre(), bLasers[draw]->getSpriteScale());
		}
		// Render the red laser in the vector array
		for (int draw = 0; draw < rLasers.size(); draw++)
		{
			rLasers[draw]->render(theRenderer, &rLasers[draw]->getSpriteDimensions(), &rLasers[draw]->getSpritePos(), rLasers[draw]->getSpriteRotAngle(), &rLasers[draw]->getSpriteCentre(), rLasers[draw]->getSpriteScale());
		}
		// Render the yellow laser in the vector array
		for (int draw = 0; draw < yLasers.size(); draw++)
		{
			yLasers[draw]->render(theRenderer, &yLasers[draw]->getSpriteDimensions(), &yLasers[draw]->getSpritePos(), yLasers[draw]->getSpriteRotAngle(), &yLasers[draw]->getSpriteCentre(), yLasers[draw]->getSpriteScale());
		}
		// Render the green laser in the vector array
		for (int draw = 0; draw < gLasers.size(); draw++)
		{
			gLasers[draw]->render(theRenderer, &gLasers[draw]->getSpriteDimensions(), &gLasers[draw]->getSpritePos(), gLasers[draw]->getSpriteRotAngle(), &gLasers[draw]->getSpriteCentre(), gLasers[draw]->getSpriteScale());
		}

		// Render the Score
		//cTexture* tempTextTexture = theTextureMgr->getTexture("Score")
		theTextureMgr->addTexture("Score", theFontMgr->getFont("agency")->createTextTexture(theRenderer, strScore.c_str(), SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// render the player
		playerCannon[0]->render(theRenderer, &playerCannon[0]->getSpriteDimensions(), &playerCannon[0]->getSpritePos(), playerCannon[0]->getSpriteRotAngle(), &playerCannon[0]->getSpriteCentre(), playerCannon[0]->getSpriteScale());
		SDL_RenderPresent(theRenderer);
	}
	break;
	case END:
	{
		// Render end scene background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render title on end scene
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 365, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render game end message
		tempTextTexture = theTextureMgr->getTexture("GameOverTxt");
		pos = { 365, 250, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Render final score
		theTextureMgr->addTexture("Score", theFontMgr->getFont("agency")->createTextTexture(theRenderer, strScore.c_str(), SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 405, 300, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render menu and exit buttons
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 280, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}


void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	switch (theGameState)
	{
	case MENU:
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	}
	break;

		case PLAYING:
		{
			// Update the player position
			thePlayer.update(deltaTime);

			// Update the visibility and position of each blue enemy
			vector<cEnemy*>::iterator bEnemyIterator = bEnemies.begin();
			while (bEnemyIterator != bEnemies.end())
			{
				if ((*bEnemyIterator)->isActive() == false)
				{
					bEnemyIterator = bEnemies.erase(bEnemyIterator);
				}
				else
				{
					(*bEnemyIterator)->update(deltaTime);
					++bEnemyIterator;
				}
			}

			// Update the visibility and position of each red enemy
			vector<cEnemy*>::iterator rEnemyIterator = rEnemies.begin();
			while (rEnemyIterator != rEnemies.end())
			{
				if ((*rEnemyIterator)->isActive() == false)
				{
					rEnemyIterator = rEnemies.erase(rEnemyIterator);
				}
				else
				{
					(*rEnemyIterator)->update(deltaTime);
					++rEnemyIterator;
				}
			}
			// Update the visibility and position of each yellow enemy
			vector<cEnemy*>::iterator yEnemyIterator = yEnemies.begin();
			while (yEnemyIterator != yEnemies.end())
			{
				if ((*yEnemyIterator)->isActive() == false)
				{
					yEnemyIterator = yEnemies.erase(yEnemyIterator);
				}
				else
				{
					(*yEnemyIterator)->update(deltaTime);
					++yEnemyIterator;
				}
			}

			// Update the visibility and position of each green enemy
			vector<cEnemy*>::iterator gEnemyIterator = gEnemies.begin();
			while (gEnemyIterator != gEnemies.end())
			{
				if ((*gEnemyIterator)->isActive() == false)
				{
					gEnemyIterator = gEnemies.erase(gEnemyIterator);
				}
				else
				{
					(*gEnemyIterator)->update(deltaTime);
					++gEnemyIterator;
				}
			}

			// Update the visibility and position of the blue laser
			vector<cLaser*>::iterator blaserIterator = bLasers.begin();
			while (blaserIterator != bLasers.end())
			{
				if ((*blaserIterator)->isActive() == false)
				{
					blaserIterator = bLasers.erase(blaserIterator);
				}
				else
				{
					(*blaserIterator)->update(deltaTime);
					++blaserIterator;
				}
			}
			// Update the visibility and position of the red laser
			vector<cLaser*>::iterator rlaserIterator = rLasers.begin();
			while (rlaserIterator != rLasers.end())
			{
				if ((*rlaserIterator)->isActive() == false)
				{
					rlaserIterator = rLasers.erase(rlaserIterator);
				}
				else
				{
					(*rlaserIterator)->update(deltaTime);
					++rlaserIterator;
				}
			}

			// Update the visibility and position of the yellow laser
			vector<cLaser*>::iterator ylaserIterator = yLasers.begin();
			while (ylaserIterator != yLasers.end())
			{
				if ((*ylaserIterator)->isActive() == false)
				{
					ylaserIterator = yLasers.erase(ylaserIterator);
				}
				else
				{
					(*ylaserIterator)->update(deltaTime);
					++ylaserIterator;
				}
			}
			// Update the visibility and position of the green laser
			vector<cLaser*>::iterator glaserIterator = gLasers.begin();
			while (glaserIterator != gLasers.end())
			{
				if ((*glaserIterator)->isActive() == false)
				{
					glaserIterator = gLasers.erase(glaserIterator);
				}
				else
				{
					(*glaserIterator)->update(deltaTime);
					++glaserIterator;
				}
			}
			// Update the visibility and position of the player
			vector<cPlayer*>::iterator playerIterator = playerCannon.begin();
			while (playerIterator != playerCannon.end())
			{
				if ((*playerIterator)->isActive() == false)
				{
					playerIterator = playerCannon.erase(playerIterator);
				}
				else
				{
					(*playerIterator)->update(deltaTime);
					++playerIterator;
				}
			}

			/*
			// Collision Detection with enemies
			*/
			//Blue Enemy collision with laser
			for (vector<cLaser*>::iterator blaserIterator = bLasers.begin(); blaserIterator != bLasers.end(); ++blaserIterator)
			{
				//(*laserIterator)->update(deltaTime);
				for (vector<cEnemy*>::iterator bEnemyIterator = bEnemies.begin(); bEnemyIterator != bEnemies.end(); ++bEnemyIterator)
				{
					if ((*bEnemyIterator)->collidedWith(&(*bEnemyIterator)->getBoundingRect(), &(*blaserIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*bEnemyIterator)->setActive(false);
						(*bEnemyIterator)->setActive(false);
						(*blaserIterator)->setActive(false);
						theSoundMgr->getSnd("enemyDeath")->play(0);
						score += 100; //increment score by 100
						strScore = gameTextList[1] + to_string(score); //update score variable
						theTextureMgr->deleteTexture("Score"); //update score texture
					}
				}
			}

			//Red enemy with laser
			for (vector<cLaser*>::iterator rlaserIterator = rLasers.begin(); rlaserIterator != rLasers.end(); ++rlaserIterator)
			{
				//(*laserIterator)->update(deltaTime);
				for (vector<cEnemy*>::iterator rEnemyIterator = rEnemies.begin(); rEnemyIterator != rEnemies.end(); ++rEnemyIterator)
				{
					if ((*rEnemyIterator)->collidedWith(&(*rEnemyIterator)->getBoundingRect(), &(*rlaserIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*rEnemyIterator)->setActive(false);
						(*rEnemyIterator)->setActive(false);
						(*rlaserIterator)->setActive(false);
						theSoundMgr->getSnd("enemyDeath")->play(0);
						score += 100; //increment score by 100
						strScore = gameTextList[1] + to_string(score); //update score variable
						theTextureMgr->deleteTexture("Score"); //update score texture

					}
				}
			}

			//Yellow enemy with laser
			for (vector<cLaser*>::iterator ylaserIterator = yLasers.begin(); ylaserIterator != yLasers.end(); ++ylaserIterator)
			{
				//(*laserIterator)->update(deltaTime);
				for (vector<cEnemy*>::iterator yEnemyIterator = yEnemies.begin(); yEnemyIterator != yEnemies.end(); ++yEnemyIterator)
				{
					if ((*yEnemyIterator)->collidedWith(&(*yEnemyIterator)->getBoundingRect(), &(*ylaserIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*yEnemyIterator)->setActive(false);
						(*yEnemyIterator)->setActive(false);
						(*ylaserIterator)->setActive(false);
						theSoundMgr->getSnd("enemyDeath")->play(0);
						score += 100; //increment score by 100
						strScore = gameTextList[1] + to_string(score); //update score variable
						theTextureMgr->deleteTexture("Score"); //update score texture
					}
				}
			}

			//Green enemy with laser
			for (vector<cLaser*>::iterator glaserIterator = gLasers.begin(); glaserIterator != gLasers.end(); ++glaserIterator)
			{
				//(*laserIterator)->update(deltaTime);
				for (vector<cEnemy*>::iterator gEnemyIterator = gEnemies.begin(); gEnemyIterator != gEnemies.end(); ++gEnemyIterator)
				{
					if ((*gEnemyIterator)->collidedWith(&(*gEnemyIterator)->getBoundingRect(), &(*glaserIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*gEnemyIterator)->setActive(false);
						(*glaserIterator)->setActive(false);
						theSoundMgr->getSnd("enemyDeath")->play(0);
						score += 100; //increment score by 100
						strScore = gameTextList[1] + to_string(score); //update score variable
						theTextureMgr->deleteTexture("Score"); //update score texture
					}
				}
			}
			//Green enemy collision with player
			for (vector<cEnemy*>::iterator gEnemyIterator = gEnemies.begin(); gEnemyIterator != gEnemies.end(); ++gEnemyIterator)
			{
				for (vector<cPlayer*>::iterator playerIterator = playerCannon.begin(); playerIterator != playerCannon.end(); ++playerIterator)
				{

					if ((*gEnemyIterator)->collidedWith(&(*gEnemyIterator)->getBoundingRect(), &(*playerIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*gEnemyIterator)->setActive(false);
						theSoundMgr->getSnd("playerDeath")->play(0);
						theGameState = END;
					}
				}
			}
			//Yellow enemy collision with player
			for (vector<cEnemy*>::iterator yEnemyIterator = yEnemies.begin(); yEnemyIterator != yEnemies.end(); ++yEnemyIterator)
			{
				for (vector<cPlayer*>::iterator playerIterator = playerCannon.begin(); playerIterator != playerCannon.end(); ++playerIterator)
				{

					if ((*yEnemyIterator)->collidedWith(&(*yEnemyIterator)->getBoundingRect(), &(*playerIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*yEnemyIterator)->setActive(false);
						theSoundMgr->getSnd("playerDeath")->play(0);
						theGameState = END;
					}
				}
			}

			//Red enemy collision with player
			for (vector<cEnemy*>::iterator rEnemyIterator = rEnemies.begin(); rEnemyIterator != rEnemies.end(); ++rEnemyIterator)
			{
				for (vector<cPlayer*>::iterator playerIterator = playerCannon.begin(); playerIterator != playerCannon.end(); ++playerIterator)
				{

					if ((*rEnemyIterator)->collidedWith(&(*rEnemyIterator)->getBoundingRect(), &(*playerIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*rEnemyIterator)->setActive(false);
						theSoundMgr->getSnd("playerDeath")->play(0);
						theGameState = END;
					}
				}
			}

			//Enemy collision with player
			for (vector<cEnemy*>::iterator bEnemyIterator = bEnemies.begin(); bEnemyIterator != bEnemies.end(); ++bEnemyIterator)
			{
				for (vector<cPlayer*>::iterator playerIterator = playerCannon.begin(); playerIterator != playerCannon.end(); ++playerIterator)
				{

					if ((*bEnemyIterator)->collidedWith(&(*bEnemyIterator)->getBoundingRect(), &(*playerIterator)->getBoundingRect()))
					{
						// if a collision set the laser and enemy to false
						(*bEnemyIterator)->setActive(false);
						theSoundMgr->getSnd("playerDeath")->play(0);
						theGameState = END;
					}
				}
			}
		}
		break;
		case END:
		{
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		}
		break;
		case QUIT:
		{
			loop = false;
		}
		break;
		default:
			break;
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
				}
				break;
				case SDL_BUTTON_RIGHT:

					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{

				}
				break;

				case SDLK_UP:
				{


				}
				break;
				case SDLK_RIGHT:
				{
					playerCannon[0]->setSpriteRotAngle(playerCannon[0]->getSpriteRotAngle() + 90);
				}
				break;

				case SDLK_LEFT:
				{
					playerCannon[0]->setSpriteRotAngle(playerCannon[0]->getSpriteRotAngle() - 90);
				}
				break;

				case SDLK_1:
				{
					bLasers.push_back(new cLaser);
					int bNumLasers = bLasers.size() - 1;
					bLasers[bNumLasers]->setSpritePos({ playerCannon[0]->getBoundingRect().x + playerCannon[0]->getSpriteCentre().x, playerCannon[0]->getBoundingRect().y + playerCannon[0]->getSpriteCentre().y });
					bLasers[bNumLasers]->setSpriteTranslation({ 10.0f, 10.0f });
					bLasers[bNumLasers]->setTexture(theTextureMgr->getTexture("BlueLaser"));
					bLasers[bNumLasers]->setSpriteDimensions(theTextureMgr->getTexture("BlueLaser")->getTWidth(), theTextureMgr->getTexture("BlueLaser")->getTHeight());
					bLasers[bNumLasers]->setLaserVelocity({ 2.0f, 2.0f });
					bLasers[bNumLasers]->setSpriteRotAngle(playerCannon[0]->getSpriteRotAngle());
					bLasers[bNumLasers]->setActive(true);

					cout << "Blue laser added to Vector at position - x: " << playerCannon[0]->getBoundingRect().x << " y: " << playerCannon[0]->getBoundingRect().y << endl;

				}

				theSoundMgr->getSnd("laserFire")->play(0);

				break;
				case SDLK_2:
				{
					rLasers.push_back(new cLaser);
					int rNumLasers = rLasers.size() - 1;
					rLasers[rNumLasers]->setSpritePos({ playerCannon[0]->getBoundingRect().x + playerCannon[0]->getSpriteCentre().x, playerCannon[0]->getBoundingRect().y + playerCannon[0]->getSpriteCentre().y });
					rLasers[rNumLasers]->setSpriteTranslation({ 10.0f, 10.0f });
					rLasers[rNumLasers]->setTexture(theTextureMgr->getTexture("RedLaser"));
					rLasers[rNumLasers]->setSpriteDimensions(theTextureMgr->getTexture("RedLaser")->getTWidth(), theTextureMgr->getTexture("RedLaser")->getTHeight());
					rLasers[rNumLasers]->setLaserVelocity({ 2.0f, 2.0f });
					rLasers[rNumLasers]->setSpriteRotAngle(playerCannon[0]->getSpriteRotAngle());
					rLasers[rNumLasers]->setActive(true);
					cout << "Red laser added to Vector at position - x: " << playerCannon[0]->getBoundingRect().x << " y: " << playerCannon[0]->getBoundingRect().y << endl;
				}


				theSoundMgr->getSnd("laserFire")->play(0);

				break;
				case SDLK_3:
				{
					yLasers.push_back(new cLaser);
					int yNumLasers = yLasers.size() - 1;
					yLasers[yNumLasers]->setSpritePos({ playerCannon[0]->getBoundingRect().x + playerCannon[0]->getSpriteCentre().x, playerCannon[0]->getBoundingRect().y + playerCannon[0]->getSpriteCentre().y });
					yLasers[yNumLasers]->setSpriteTranslation({ 10.0f, 10.0f });
					yLasers[yNumLasers]->setTexture(theTextureMgr->getTexture("YellowLaser"));
					yLasers[yNumLasers]->setSpriteDimensions(theTextureMgr->getTexture("YellowLaser")->getTWidth(), theTextureMgr->getTexture("YellowLaser")->getTHeight());
					yLasers[yNumLasers]->setLaserVelocity({ 2.0f, 2.0f });
					yLasers[yNumLasers]->setSpriteRotAngle(playerCannon[0]->getSpriteRotAngle());
					yLasers[yNumLasers]->setActive(true);

					cout << "Yellow laser added to Vector at position - x: " << playerCannon[0]->getBoundingRect().x << " y: " << playerCannon[0]->getBoundingRect().y << endl;

				}

				theSoundMgr->getSnd("laserFire")->play(0);

				break;

				case SDLK_4:
				{
					gLasers.push_back(new cLaser);
					int gNumLasers = gLasers.size() - 1;
					gLasers[gNumLasers]->setSpritePos({ playerCannon[0]->getBoundingRect().x + playerCannon[0]->getSpriteCentre().x, playerCannon[0]->getBoundingRect().y + playerCannon[0]->getSpriteCentre().y });
					gLasers[gNumLasers]->setSpriteTranslation({ 10.0f, 10.0f });
					gLasers[gNumLasers]->setTexture(theTextureMgr->getTexture("GreenLaser"));
					gLasers[gNumLasers]->setSpriteDimensions(theTextureMgr->getTexture("GreenLaser")->getTWidth(), theTextureMgr->getTexture("GreenLaser")->getTHeight());
					gLasers[gNumLasers]->setLaserVelocity({ 2.0f, 2.0f });
					gLasers[gNumLasers]->setSpriteRotAngle(playerCannon[0]->getSpriteRotAngle());
					gLasers[gNumLasers]->setActive(true);

					cout << "Green laser added to Vector at position - x: " << playerCannon[0]->getBoundingRect().x << " y: " << playerCannon[0]->getBoundingRect().y << endl;

				}

				theSoundMgr->getSnd("laserFire")->play(0);

				break;
				case SDLK_SPACE:
				{

				}
				break;
				default:
				break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}


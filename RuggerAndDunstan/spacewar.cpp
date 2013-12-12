// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.cpp v1.1
// This class is the core of the game

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{
    menuOn = true;
    countDownOn = false;
    roundOver = false;
    initialized = false;
	srand(time(0));
	gameTime = 0;
	menuActive = false;
	atMainMenu = true;
	currentRoom = 0;
	maxRoomNum = 2; // 3 rooms
	died = false;
	highScore = 0;
	score = 0;
	upDepressedLastFrame = false;
	bulletDist = 0;
	gameOver = false;
	invincible = false;
	timeDone = 0;
	inCutscene = -1;
}

//=============================================================================
// Destructor
//=============================================================================
Spacewar::~Spacewar()
{
	for(int i=walls.size(); i>0; i--)
	{
		delete walls.back();
		walls.pop_back();
	}
	walls.clear();
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
	rugger.rotate(ruggerNS::NONE);

    // initialize DirectX fonts
    fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
    fontBig.setFontColor(spacewarNS::FONT_COLOR);
    fontScore.initialize(graphics, spacewarNS::FONT_SCORE_SIZE, false, false, spacewarNS::FONT);
    debugText.initialize(graphics, 24, true, false, "Segoe UI");


	//_________________________
	// ----------------------
	// initialize achievements
	achievements.push_back(new Achievement(&rugger.graving, true, "pictures\\testAchievement.png"));


	//_________________________
	// ----------------------
	// initialize textures

	//achievements
	for (int i = 0; i < achievements.size(); i++) {
		if (!achievements[i]->getTexture()->initialize(graphics, achievements[i]->getImageName()))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing achievement "));
	}

    // rugger texture
    if (!ruggerTexture.initialize(graphics,RUGGER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing rugger texture"));
  
	// enemy1 texture
    if (!enemy1Texture.initialize(graphics,ENEMY1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing rugger texture"));

    // background texture
    if (!backgroundTexture.initialize(graphics,BACKGROUND_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));
	
    // background texture
    if (!background2Texture.initialize(graphics,BACKGROUND2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background2 texture"));
	
    // background texture
    if (!background3Texture.initialize(graphics,BACKGROUND3_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background3 texture"));

	// cutscene texture
    if (!cutscene1Texture.initialize(graphics,CUTSCENE1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cutscene1 texture"));

	// cutscene texture
    if (!cutscene2Texture.initialize(graphics,CUTSCENE2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cutscene2 texture"));

	// cutscene texture
    if (!cutscene3Texture.initialize(graphics,CUTSCENE3_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cutscene3 texture"));
	
	if(!gameOverTM.initialize(graphics, GAMEOVER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init gameover texture"));

	//bullet texture
	if(!bulletTM.initialize(graphics, BULLET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet texture"));
	
    // vision textures
    if (!visionTexture.initialize(graphics,VISION_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing vision textures"));

	//gravity ball texture
	if(!gravTM.initialize(graphics, GRAV_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing gravball texture"));
	
	//portal texture
	if (!portTexture.initialize(graphics, PORTAL_IMAGE))
		throw (GameError(gameErrorNS::FATAL_ERROR, "Error initializing portal texture"));

	//trap door texture
	if(!trapDoorTexture.initialize(graphics, TRAPDOOR_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing trap door texture"));

	//menu texture
	if(!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	
	//deadmenu texture
	if(!deadMenuTexture.initialize(graphics, MENU_DEAD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing deadmenu texture"));

	//wall texture
	if(!wallLongTM.initialize(graphics, WALL_LONG_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall long texture"));
	if(!wallLongHTM.initialize(graphics, WALL_LONG_H_IMG))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall long h texture"));
	if(!wallShortTM.initialize(graphics, WALL_SHORT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall short texture"));
	if(!wallShortHTM.initialize(graphics, WALL_SHORT_H_IMG))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall short h texture"));

	if(!casketTM.initialize(graphics, CASKET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init casket texture"));
	//_________________________
	// ----------------------
	// initialize objects

    // background image
    if (!background.initialize(graphics,0,0,0,&backgroundTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));
	
    // background2 image
    if (!background2.initialize(graphics,0,0,0,&background2Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background2"));
	
    // background3 image
    if (!background3.initialize(graphics,0,0,0,&background3Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background3"));

	// cutscene
    if (!cutscene1.initialize(this, 1500, 720, 0, &cutscene1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cutscene1"));
	cutscene1.setActive(false);

		// cutscene
    if (!cutscene2.initialize(this, 1500, 720, 0, &cutscene2Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cutscene2"));
	cutscene2.setActive(false);

		// cutscene
    if (!cutscene3.initialize(this, 1500, 720, 0, &cutscene3Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cutscene3"));
	cutscene3.setActive(false);

	 // menu iamge
    if (!menu.initialize(graphics, 1280, 720 ,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));
	menu.setVisible(true);

	 // deadmenu iamge
    if (!deadMenu.initialize(graphics, 1280, 720 ,0,&deadMenuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing deadmenu"));
	deadMenu.setVisible(true);
	if(!gameOverBackground.initialize(graphics, 1280, 720, 0, &gameOverTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error inintializing gameover screen"));
    // rugger
    if (!rugger.initialize(this, ruggerNS::WIDTH, ruggerNS::HEIGHT, ruggerNS::TEXTURE_COLS, &ruggerTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing rugger"));

	 // trapDoor
    if (!trapDoor.initialize(this, 64, 64, 0, &trapDoorTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing trapdoor"));

	// portal
	if(!portal.initialize(this, 35, 35, 6, &portTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing portal"));
	portal.setActiveAndVisible(false);
	

	//Gravity ball
	if(!gravBall.initialize(this, 128, 128, 0, &gravTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing gravball"));
	gravBall.setX(150);
	gravBall.setY(150);
	gravBall.setActiveAndVisible(false);
	gravBall.setScale(.25);
	gravBall.setMass(planetNS::MASS);
	//gravBall.activate();
	
    if (!vision1.initialize(this, visionNS::WIDTH, visionNS::HEIGHT, visionNS::TEXTURE_COLS, &visionTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing vision1"));
	
	if(!bullet.initialize(this, bulletNS::WIDTH, bulletNS::HEIGHT, bulletNS::TEXTURE_COLS, &bulletTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));
	bullet.setFrames(bulletNS::BULLET_START_FRAME, bulletNS::BULLET_END_FRAME);
	bullet.setCurrentFrame(bulletNS::BULLET_START_FRAME);
	bullet.setActive(false);
	bullet.setVisible(false);

	 // enemy1
    if (!enemy1_1.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy1"));
	enemy1_1.setX(GAME_WIDTH - 100);
	enemy1_1.setY(60);
	enemy1_1.setActiveAndVisible(false);
	
    if (!enemy1_2.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy1"));
	enemy1_2.setX(560);
	enemy1_2.setY(400);
	enemy1_2.setActiveAndVisible(false);

	if (!enemy1_3.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy1"));
	enemy1_3.setX(454);
	enemy1_3.setY(235);
	enemy1_3.setActiveAndVisible(false);

	if (!enemy1_4.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy1"));
	enemy1_4.setX(860);
	enemy1_4.setY(430);
	enemy1_4.setActiveAndVisible(false);

    if (!enemy2_1.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy2"));
	enemy2_1.setX(GAME_WIDTH - 100);
	enemy2_1.setY(200);
	enemy2_1.setActiveAndVisible(false);

    if (!enemy2_2.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy3"));
	enemy2_2.setX(GAME_WIDTH - 100);
	enemy2_2.setY(GAME_HEIGHT - 200);
	enemy2_2.setActiveAndVisible(false);

	if(!enemy2_3.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy4"));
	enemy2_3.setX(GAME_WIDTH - 500);
	enemy2_3.setY(GAME_HEIGHT - 300);
	enemy2_3.setActiveAndVisible(false);

	if(!enemy2_4.initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy4"));
	enemy2_4.setX(100);
	enemy2_4.setY(100);
	enemy2_4.setActiveAndVisible(false);

	for(int i=0; i<spacewarNS::ROOM3EN_NUM; i++)
	{
		if(!room3En[i].initialize(this, enemy1NS::WIDTH, enemy1NS::HEIGHT, enemy1NS::TEXTURE_COLS, bullet, vision1, &enemy1Texture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing room 3 enemies"));
		room3En[i].setActiveAndVisible(false);
	}

	for (int i = 0; i < spacewarNS::ROOM3PORT_NUM; i++) {
		if(!room3Port[i].initialize(this, portalNS::WIDTH, portalNS::HEIGHT, portalNS::TEXTURE_COLS, &portTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing room 3 portals"));
		room3Port[i].setActiveAndVisible(false);
	}

	for (int i = 0; i < achievements.size(); i++) {
		if (!achievements[i]->initialize(this, achievementNS::WIDTH, achievementNS::HEIGHT, achievementNS::TEXTURE_COLS, achievements[i]->getTexture()))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing achievements"));
	}

	for(int i=0; i<ruggerNS::NUM_BULLETS; i++)
	{
		if(!pBullets[i].initialize(this, 32, 32, 0, &bulletTM))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet i"));
		pBullets[i].setActiveAndVisible(false);
		pBullets[i].setX(10 + i*5);
		pBullets[i].setY(10);
		pBullets[i].setScale(.45);
	}
	
	if(!casket.initialize(this, 128, 64, 0, &casketTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing casket"));
	casket.setActiveAndVisible(false);
	casket.setCollisionType(entityNS::BOX);
	casket.setEdge(CASKETRECT);
	casket.setScale(0.75);
	
	audio->stopCue(BACKGROUND_MUSIC);
	audio->playCue(BACKGROUND_MUSIC);

	for (int i = 0; i < achievements.size(); i++)
		achievements[i]->setVisibleAndActive(true); //This doesn't do anything :)

	startRoom(currentRoom);
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
{
	if (input->isKeyDown(VK_BACK))
		audio->stopCue(BACKGROUND_MUSIC);

    if (menuOn)
    {
		if (!died)
		{
			if(gameOver)if(input->isKeyDown(VK_SPACE))PostQuitMessage(0);
			if (input->anyKeyPressed() || input->getMouseLButton() || input->getMouseMButton() || input->getMouseRButton())
			{
				menuOn = false;
				input->clearAll();
				roundStart();
			}
		}
		else
		{
			if (input->isKeyDown(VK_SPACE))
			{
				//menuOn = false;
				died = false;
				menuOn = false;
				input->clearAll();
				resetPos();
				gravBall.setVisibleAndActive(false);
			}
		}
    } 
    else if(countDownOn)
    {
        countDownTimer -= frameTime;
        if(countDownTimer <= 0)
            countDownOn = false;
    } 
	else if (inCutscene >= 0)
	{

		inCutscene+= frameTime;
		if (inCutscene > 5)
		{
			inCutscene = -1;
			cutscene1.setActiveAndVisible(false);
			cutscene2.setActiveAndVisible(false);
			cutscene3.setActiveAndVisible(false);
		}
		
		Entity * cutscene;
		switch(currentRoom)
		{
		case 0: cutscene = &cutscene1; break;
		case 1: cutscene = &cutscene2; break;
		case 2: cutscene = &cutscene3; break;
		default: break;
		}

		cutscene->setX(cutscene->getX() + cutscene->getVelocity().x * frameTime);
		cutscene->setY(cutscene->getY() + cutscene->getVelocity().y * frameTime);
		
		if(input->isKeyDown(VK_SPACE))
			stopCutscene();
	}
    else if (!menuOn)
    {
		int directionX = 0;
		int directionY = 0;

		if(input->isKeyDown(RUGGER_RIGHT_KEY) && !input->isKeyDown(RUGGER_UP_KEY) && !input->isKeyDown(RUGGER_DOWN_KEY)) {// d
			directionX++;
			rugger.rotate(ruggerNS::RIGHT);
		}if(input->isKeyDown(RUGGER_LEFT_KEY) && !input->isKeyDown(RUGGER_UP_KEY) && !input->isKeyDown(RUGGER_DOWN_KEY)){ // a
			directionX--;
			rugger.rotate(ruggerNS::LEFT);
		}if(input->isKeyDown(RUGGER_UP_KEY) && !input->isKeyDown(RUGGER_LEFT_KEY) && !input->isKeyDown(RUGGER_RIGHT_KEY)){ // w
			directionY--;
			rugger.rotate(ruggerNS::UP);
		}if(input->isKeyDown(RUGGER_DOWN_KEY)&&!input->isKeyDown(RUGGER_LEFT_KEY) && !input->isKeyDown(RUGGER_RIGHT_KEY)){ // s
			directionY++;
			rugger.rotate(ruggerNS::DOWN);
		}if (input->isKeyDown(RUGGER_RIGHT_KEY) && input->isKeyDown(RUGGER_UP_KEY)) {
			directionX++;
			directionY--;
			rugger.rotate(ruggerNS::RIGHTUP);
		}if (input->isKeyDown(RUGGER_RIGHT_KEY) && input->isKeyDown(RUGGER_DOWN_KEY)) {
			directionX++;
			directionY++;
			rugger.rotate(ruggerNS::RIGHTDOWN);
		}if (input->isKeyDown(RUGGER_LEFT_KEY) && input->isKeyDown(RUGGER_UP_KEY)) {
			directionX--;
			directionY--;
			rugger.rotate(ruggerNS::LEFTUP);
		}if (input->isKeyDown(RUGGER_LEFT_KEY) && input->isKeyDown(RUGGER_DOWN_KEY)) {
			directionX--;
			directionY++;
			rugger.rotate(ruggerNS::LEFTDOWN);
		}

		//if no direction keys are down
		if (!input->isKeyDown(RUGGER_DOWN_KEY) && !input->isKeyDown(RUGGER_LEFT_KEY) && !input->isKeyDown(RUGGER_RIGHT_KEY) && !input->isKeyDown(RUGGER_UP_KEY))
			rugger.rotate(ruggerNS::NONE);

		if (directionX < -1)
			directionX = -1;
		if (directionX > 1)
			directionX = 1;
		if (directionY < -1)
			directionY = -1;
		if (directionY > 1)
			directionY = 1;

		if (input->isKeyDown(VK_UP))
			upDepressedLastFrame = true;
		else
		{
			if (upDepressedLastFrame && currentRoom < maxRoomNum)
			{
				if (!invincible)
					invincible = true;
				else if (invincible)
					invincible = false;
				//currentRoom++;
				//startRoom(currentRoom);
			}

				upDepressedLastFrame = false;
		}

		if (areAllEnemiesDead())
			trapDoor.setVisibleAndActive(true);
			
			

		if(input->getMouseLButton())	//SHOOT BULLETS
		{
			rugger.setFiring(true);
			rugger.fire(input->getMouseX(), input->getMouseY(), pBullets);
			if(rugger.getTimeShot() == 0)
			{
				bulletDist = 0;
				audio->playCue(PLINK);
			}
		}

		if(input->getMouseRButton())	//place the gravity ball at mouse click
		{
			rugger.setGraving(true);
			gravBall.activate();
			gravBall.setVisible(true);
			gravBall.setX(input->getMouseX() - planetNS::WIDTH*gravBall.getScale()/2.0);
			gravBall.setY(input->getMouseY() - planetNS::HEIGHT*gravBall.getScale()/2.0);
		}

		if (!input->getMouseLButton() && !input->getMouseRButton()) {
			rugger.setFiring(false);
			rugger.setGraving(false);
		}

		if (directionX != 0 && directionY != 0)
		{
			if (abs(rugger.getVelocity().x) < ruggerNS::SPEED * .80)
				rugger.setVelocity(VECTOR2(rugger.getVelocity().x * ruggerNS::ACCEL + 2 * directionX, rugger.getVelocity().y));
			if (abs(rugger.getVelocity().y) < ruggerNS::SPEED * .80)
				rugger.setVelocity(VECTOR2(rugger.getVelocity().x, rugger.getVelocity().y * ruggerNS::ACCEL + 2 * directionY));

			if (abs(rugger.getVelocity().x) >= ruggerNS::SPEED * .80)
				rugger.setVelocity(VECTOR2(ruggerNS::SPEED * .80 * directionX, rugger.getVelocity().y));
			if (abs(rugger.getVelocity().y) >= ruggerNS::SPEED * .80)
				rugger.setVelocity(VECTOR2(rugger.getVelocity().x, ruggerNS::SPEED * .80 * directionY));
		}
		else 
		{
			if (directionX != 0)
			{
				if (abs(rugger.getVelocity().x) < ruggerNS::SPEED)
					rugger.setVelocity(VECTOR2(rugger.getVelocity().x * ruggerNS::ACCEL + 2 * directionX, rugger.getVelocity().y));

				if (abs(rugger.getVelocity().x) >= ruggerNS::SPEED)
					rugger.setVelocity(VECTOR2(ruggerNS::SPEED * directionX, rugger.getVelocity().y));
			}
			else // key not pressed, decel
			{
					rugger.setVelocity(VECTOR2(rugger.getVelocity().x * ruggerNS::DECEL, rugger.getVelocity().y));

				if (abs(rugger.getVelocity().x) <= .2)
					rugger.setVelocity(VECTOR2(0, rugger.getVelocity().y));
			}
			if (directionY != 0)
			{
			
				if (abs(rugger.getVelocity().y) < ruggerNS::SPEED)
					rugger.setVelocity(VECTOR2(rugger.getVelocity().x, rugger.getVelocity().y * ruggerNS::ACCEL + 2 * directionY));
				if (abs(rugger.getVelocity().y) >= ruggerNS::SPEED)
					rugger.setVelocity(VECTOR2(rugger.getVelocity().x, ruggerNS::SPEED * directionY));
			}
			else // key not pressed, decel
			{
				rugger.setVelocity(VECTOR2(rugger.getVelocity().x, rugger.getVelocity().y * ruggerNS::DECEL));
					
				if (abs(rugger.getVelocity().y) <= .2)
					rugger.setVelocity(VECTOR2(rugger.getVelocity().x, 0));
			}
		}

		if (input->getMouseMButton())
		{
			rugger.setX(input->getMouseX());
			rugger.setY(input->getMouseY());
		}


		if (pBullets[0].getActive())
		{
			bulletDist++;
		}
		//else
		//	bulletDist = 0;

	}

    // Update the entities
    
	if (!menuOn && inCutscene == -1)
	{
		if (!died)
			gameTime += frameTime;

		int oldx = rugger.getX();
		int oldy = rugger.getY();
		rugger.update(frameTime);

		VECTOR2 v;
		for(int i=0; i<walls.size(); i++)
		{
			if (rugger.collidesWith(*walls[i], v))
			{
				rugger.setX(oldx);
				rugger.setY(oldy);
				break;
			}
		}

		gravBall.update(frameTime);
		for(int i=0; i<ruggerNS::NUM_BULLETS; i++)
		{
			VECTOR2 dist = VECTOR2(gravBall.getX() - pBullets[i].getX(), gravBall.getY() - pBullets[i].getY());
			if(gravBall.getActive() && (D3DXVec2Length(&dist) < 500)) pBullets[i].gravityForce(&gravBall, frameTime);
			pBullets[i].update(frameTime);
		}
		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->update(frameTime);
		}

		for (int i = 0; i < portals.size(); i++)
			if (portals[i]->getVisible()) 
				portals[i]->update(frameTime);

		for(int i=0; i<walls.size(); i++) walls[i]->update(frameTime);
		
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->getVisible())
				enemies[i]->update(frameTime);
		
			for(int j = 0; j < enemies[i]->bullets.size(); j++)
			{
				if(enemies[i]->bullets[j].getActive())
					enemies[i]->bullets[j].update(frameTime);
				else
				{
					enemies[i]->bullets[j].setX(enemies[i]->getX());
					enemies[i]->bullets[j].setY(enemies[i]->getY());
				}
			}
		}
	}
}

//=============================================================================
// Start a new round of play
//=============================================================================
void Spacewar::roundStart()
{
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Spacewar::ai()
{
	if (!menuOn)
	{
		shootingAI();
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->getActive())
				enemies[i]->ai(frameTime);
		}
	}
}

//=============================================================================
// Handle collisions
//=============================================================================
void Spacewar::collisions()
{
	VECTOR2 collisionVector;
	rugger.setCanBeSeen(false);
	
	if (trapDoor.getActive() && rugger.collidesWith(trapDoor, collisionVector))
	{
		audio->playCue(WOH);
		currentRoom++;
		startRoom(currentRoom);
	}
	
	//-------------------------------------------------------------------------------
	//	COLLISION WITH PLAYER BULLETS

	for (int i = 0; i < portals.size(); i++) {
		
		if (portals[i]->getActive() && rugger.collidesWith(*portals[i], collisionVector)) {
			portals[i]->teleportTheThing();
			rugger.setX(portals[i]->getPortX());
			rugger.setY(portals[i]->getPortY());
		}

		for (int j = 0; j < ruggerNS::NUM_BULLETS; j++) {
			if (portals[i]->collidesWith(pBullets[j], collisionVector))
			{
				portals[i]->teleportTheThing();
				pBullets[j].setX(portals[i]->getPortX());
				pBullets[j].setY(portals[i]->getPortY());
			}
		}
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = 0; j < ruggerNS::NUM_BULLETS; j++)
		{
			if(enemies[i]->collidesWith(pBullets[j], collisionVector))
			{
				enemies[i]->setActiveAndVisible(false);
				pBullets[j].setActiveAndVisible(false);
				score += bulletDist;
				audio->playCue(HITSMACK);
			}
		}

	}

	for(int i=0; i<walls.size(); i++)
	{
		for(int j=0; j<ruggerNS::NUM_BULLETS; j++)
		{
			if(pBullets[j].collidesWith(*walls[i], collisionVector)) 
			{
				pBullets[j].setActiveAndVisible(false);
			}
			if (pBullets[j].collidesWith(gravBall, collisionVector))
			{
				if (collisionVector.x < gravBall.getWidth() * gravBall.getScale() / 2 &&
				    collisionVector.y < gravBall.getHeight() * gravBall.getScale() / 2)
				pBullets[j].setActiveAndVisible(false);
			}
		}
	}

	//-------------------------------------------------------------------------------
	//	COLLISION WITH ENEMY BULLETS
	VECTOR2 NOCOLLIDE(-1000, -1000);
	VECTOR2 pt = NOCOLLIDE;
	VECTOR2 pt2 = NOCOLLIDE;
	float minDist;
	float ruggerDist;

	
	for (int e = 0; e < enemies.size(); e++)
	{
		for(int j=0; j < enemies[e]->bullets.size(); j++)
		{
			minDist = 100000;

			VECTOR2 ruggerPt1(rugger.getX(), rugger.getY());
			VECTOR2 ruggerPt2(rugger.getX() + rugger.getWidth() * rugger.getScale(), rugger.getY() + rugger.getHeight() * rugger.getScale());
			VECTOR2 ruggerPt1Alt(rugger.getX() + rugger.getWidth() * rugger.getScale(), rugger.getY());
			VECTOR2 ruggerPt2Alt(rugger.getX(), rugger.getY() + rugger.getHeight() * rugger.getScale());
			VECTOR2 bulletStart(enemies[e]->getCenterX(), enemies[e]->getCenterY());
			VECTOR2 bulletEnd = *enemies[e]->bullets[j].getCenter();

			for(int i=0; i<walls.size(); i++)
			{
				VECTOR2 wallPt1(walls[i]->getX(), walls[i]->getY()); // top left point of the wall
				VECTOR2 wallPt2(walls[i]->getX() + walls[i]->getWidth() * walls[i]->getScale(), walls[i]->getY() + walls[i]->getHeight() * walls[i]->getScale()); // bottom right point of the wall 
				VECTOR2 wallPt1Alt(walls[i]->getX(), walls[i]->getY() + walls[i]->getHeight() * walls[i]->getScale());
				VECTOR2 wallPt2Alt(walls[i]->getX() + walls[i]->getWidth() * walls[i]->getScale(), walls[i]->getY());

				pt = intersection(bulletStart, bulletEnd, wallPt1, wallPt2);
				if (		  dist(pt, *enemies[e]->getCenter()) < minDist)
					minDist = dist(pt, *enemies[e]->getCenter());

				pt = intersection(bulletStart, bulletEnd, wallPt1Alt, wallPt2Alt);
				if (		  dist(pt, *enemies[e]->getCenter()) < minDist)
					minDist = dist(pt, *enemies[e]->getCenter());
			}

			pt = intersection(bulletStart, bulletEnd, ruggerPt1, ruggerPt2);
			pt2 = intersection(bulletStart, bulletEnd, ruggerPt1Alt, ruggerPt2Alt);

			if (pt != NOCOLLIDE && pt2 != NOCOLLIDE)
				ruggerDist = min(dist(pt, *enemies[e]->getCenter()), dist(pt2, *enemies[e]->getCenter()));
			
			else if (pt == NOCOLLIDE && pt2 != NOCOLLIDE)
				ruggerDist = dist(pt, *enemies[e]->getCenter());
				
			else if (pt != NOCOLLIDE && pt2 == NOCOLLIDE)
				ruggerDist = dist(pt2, *enemies[e]->getCenter());
			
			else if (pt == NOCOLLIDE && pt2 == NOCOLLIDE)
				ruggerDist = -1;

			if (ruggerDist != -1 && ruggerDist < minDist && !invincible)
			{
				// rugger seen
				if (!died)
					audio->playCue(ORCHHIT2);
				died = true;
				enemies[e]->bullets[j].setActiveAndVisible(false);
			}
		}
	}

	//---------------------------------------

	if(rugger.collidesWith(casket, collisionVector))
	{ 
		rugger.setX(-1000);
		timeDone = gameTime;
		gameOver = true;
	}

}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	
	for (int i = 0; i < achievements.size(); i++) {
		achievements[i]->testHappened();
	}

	switch(currentRoom)
	{
	case 0: background.draw(); break;
	case 1: background2.draw(); break;
	case 2: background3.draw(); break;
	default: background.draw(); break;
	}
	if(trapDoor.getActive())
		trapDoor.draw();

	for(int i=0; i<walls.size(); i++)
		walls[i]->draw();

	rugger.draw();
	casket.draw();
	for(int i=0; i<ruggerNS::NUM_BULLETS; i++)
	{
		if(pBullets[i].getActive()) pBullets[i].draw();
	}
	for (int i = 0; i < portals.size(); i++)
		if (portals[i]->getActive()) 
			portals[i]->draw();
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->getActive())
			enemies[i]->draw();
		for (int j = 0; j < enemies[i]->bullets.size(); j++)
			enemies[i]->bullets[j].draw();
	}

	if(gravBall.getActive()) gravBall.draw();
	
	stringstream ss;
	string s1, s2, s3, s4, s5, s6, s7, s8;
	ss << score << ' ' << currentRoom << ' ' << input->getMouseX() << ' ' << input->getMouseY() << ' ' << static_cast<int>(gameTime/3) << ' ';
	ss >> s1 >> s2 >> s3 >> s4 >> s5;
	
	debugText.print("Time: " + s5, 10, 10); 
	debugText.print("Score: " + s1, 10, 30);
	if (invincible)
		debugText.print("Invincibility active (press up arrow to toggle)", 10, 100);
	//debugText.print("up arrow to go to next room", 10, GAME_HEIGHT - 50);
	debugText.print("mouse: (" + s3 + ", " + s4 + ")", 300, 10);
	//if (died)
	//	debugText.print("You have been spotted!", 10, GAME_HEIGHT / 2);
	//if (areAllEnemiesDead())
	//	debugText.print("All the enemies are gone!", 10, GAME_HEIGHT / 2 + 20);

	
	if (bulletDist > 0)
	{
		ss.clear();
		ss << bulletDist;
		ss >> s1;
		debugText.print("Bullet distance: " + s1, 10, 50);
	}
	
	if (cutscene1.getActive())
		cutscene1.draw();
	if (cutscene2.getActive())
		cutscene2.draw();
	if (cutscene3.getActive())
		cutscene3.draw();

	if (died || gameOver)
		menuOn = true;

    if(menuOn)
	{
		if (!died)
		{
			if(gameOver) {

				int timebonus = 500 - (gameTime * 5);
				if (timebonus < 0)
					timebonus = 0;

				gameOverBackground.draw();
				ss.clear();
				string s1, s2, s3;
				ss << score << ' ' << timebonus << ' ' << score + timebonus;
				ss >> s1    >> s2        >> s3;
				fontScore.print("Base score: " + s1, GAME_WIDTH / 2 - 100, GAME_HEIGHT / 2 - 60);
				fontScore.print("Time bonus: " + s2, GAME_WIDTH / 2 - 100, GAME_HEIGHT / 2 - 30);
				fontScore.print("Total score: " + s3, GAME_WIDTH / 2 - 100, GAME_HEIGHT / 2);
			}
			else menu.draw();
		}
		else
			deadMenu.draw();
	}


	for (int i = 0; i < achievements.size(); i++) {
		if (achievements[i]->hasHappened())
			if (!achievements[i]->alreadyDid())
				achievements[i]->doAchievement();
	}



    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// process console commands
//=============================================================================
void Spacewar::consoleCommand()
{
    command = console->getCommand();    // get command from console
    if(command == "")                   // if no command
        return;

    if (command == "help")              // if "help" command
    {
        console->print("Console Commands:");
        console->print("fps - toggle display of frames per second");
        return;
    }
    if (command == "fps")
    {
        fpsOn = !fpsOn;                 // toggle display of fps
        if(fpsOn)
            console->print("fps On");
        else
            console->print("fps Off");
    }
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Spacewar::releaseAll()
{
    menuTexture.onLostDevice();
	deadMenuTexture.onLostDevice();
	backgroundTexture.onLostDevice();
    gameTextures.onLostDevice();
	ruggerTexture.onLostDevice();
	enemy1Texture.onLostDevice();
	visionTexture.onLostDevice();
	portTexture.onLostDevice();
    fontScore.onLostDevice();
    fontBig.onLostDevice();
	debugText.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Spacewar::resetAll()
{
    fontBig.onResetDevice();
    fontScore.onResetDevice();
    gameTextures.onResetDevice();
	backgroundTexture.onResetDevice();
	visionTexture.onLostDevice();
	ruggerTexture.onResetDevice();
	enemy1Texture.onResetDevice();
    menuTexture.onResetDevice();
	deadMenuTexture.onLostDevice();
	debugText.onResetDevice();

    Game::resetAll();
    return;
}


void Spacewar::startRoom(int roomNum)
{
	string fileName;
	switch(roomNum)
	{
	case 0:
		fileName = "level0.txt";
		break;
	case 1:
		fileName = "level1.txt";
		break;
	case 2:
		fileName = "level2.txt";
		break;
	}
	fin.open(fileName);
	//fout.open(fileName);
	trapDoor.setVisibleAndActive(false);
	died = false;
	bulletDist = 0;
	upDepressedLastFrame = false;
	playCutscene(roomNum);

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setActive(false);
		enemies[i]->setVisible(false);
		for (int j = 0; j < enemies[i]->bullets.size(); j++)
		{
			enemies[i]->bullets[j].setActive(false);
			enemies[i]->bullets[j].setVisible(false);
		}
	}

	for (int i = 0; i < portals.size(); i++){
		portals[i]->setActive(false);
		portals[i]->setVisible(false);
	}

	for (int i = 0; i < ruggerNS::NUM_BULLETS; i++)
	{
		pBullets[i].setActive(false);
		pBullets[i].setVisible(false);
	}

	gravBall.setVisibleAndActive(false);

	enemies.clear();
	portals.clear();
	rugger.setX(20);
	rugger.setY(GAME_HEIGHT / 2 - (ruggerNS::HEIGHT / 2));

	if (roomNum == 0) // starting room
	{
		trapDoor.setX(spacewarNS::ROOM1_TRAPDOOR_X);
		trapDoor.setY(spacewarNS::ROOM1_TRAPDOOR_Y);
		rugger.setX(spacewarNS::ROOM1_STARTING_X);
		rugger.setY(spacewarNS::ROOM1_STARTING_Y);
		rugger.setVisible(true);
		enemies.push_back(&enemy1_1);
		enemies[0]->setDegrees(-90);
		enemies[0]->patternSteps[0].setAction(LEFT, 5);
		enemies[0]->patternSteps[1].setAction(ROTATECCW, 90);
		enemies[0]->patternSteps[2].setAction(DOWN, 5);
		enemies[0]->patternSteps[3].setAction(ROTATECCW, 90);
		enemies[0]->patternSteps[4].setAction(RIGHT, 5);
		enemies[0]->patternSteps[5].setAction(ROTATECCW, 90);
		enemies[0]->patternSteps[6].setAction(UP, 5);
		enemies[0]->patternSteps[7].setAction(ROTATECCW, 90);
		
		
		enemies.push_back(&enemy1_2);
		enemies[1]->setDegrees(270);
		enemies[1]->setX(enemies[1]->getX()-100);

		enemies.push_back(&enemy1_3);
		enemies[2]->setDegrees(90);
		enemies[2]->patternSteps[0].setAction(ROTATECW, 30);
		enemies[2]->patternSteps[1].setAction(PAUSE, 3);
		enemies[2]->patternSteps[2].setAction(ROTATECCW, 30);
		enemies[2]->patternSteps[3].setAction(PAUSE, 3);

		enemies.push_back(&enemy1_4);
		enemies[3]->setDegrees(0);
		enemies[3]->patternSteps[0].setAction(PAUSE, 3);
		getWallPositions();

	}
	if (roomNum == 1) // 2nd room
	{
		trapDoor.setX(spacewarNS::ROOM2_TRAPDOOR_X);
		trapDoor.setY(spacewarNS::ROOM2_TRAPDOOR_Y);
		rugger.setX(spacewarNS::ROOM2_STARTING_X);
		rugger.setY(spacewarNS::ROOM2_STARTING_Y);

		enemies.push_back(&enemy2_1);
		enemies.push_back(&enemy2_2);
		enemies[0]->setDegrees(-50);
		enemies[1]->setDegrees(-45);

		enemies[0]->patternSteps[1].setAction(PAUSE, 2);
		enemies[1]->patternSteps[0].setAction(ROTATECW, 45);
		enemies[1]->patternSteps[1].setAction(PAUSE, 2);
		enemies[1]->patternSteps[2].setAction(ROTATECCW, 45);
		enemies[1]->patternSteps[3].setAction(PAUSE, 2);

		enemies.push_back(&enemy2_3);
		enemies[2]->setDegrees(180);
		enemies[2]->patternSteps[0].setAction(PAUSE, 5);

		enemies.push_back(&enemy2_4);
		enemies[3]->setDegrees(90);
		enemies[3]->patternSteps[0].setAction(RIGHT, 8);
		enemies[3]->patternSteps[5].setAction(ROTATECCW, 180);
		enemies[3]->patternSteps[6].setAction(LEFT, 8);
		enemies[3]->patternSteps[7].setAction(ROTATECCW, 180);

		getWallPositions();
	}
	if(roomNum == 2)
	{
		for (int i = 0; i < spacewarNS::ROOM3PORT_NUM; i++)
			portals.push_back(&room3Port[i]);
		
		portals[0]->setX(458);
		portals[0]->setY(275);
		portals[0]->setPortGoX(352);
		portals[0]->setPortGoY(436);
		
		portals[1]->setX(615);
		portals[1]->setY(27);
		portals[1]->setPortGoX(1192);
		portals[1]->setPortGoY(65);
		
		portals[2]->setX(369);
		portals[2]->setY(629);
		portals[2]->setPortGoX(451);
		portals[2]->setPortGoY(83);

		casket.setActiveAndVisible(true);
		casket.setX(72);
		casket.setY(52);
		rugger.setX(spacewarNS::ROOM3_STARTING_X);
		rugger.setY(spacewarNS::ROOM3_STARTING_Y);
		trapDoor.setX(spacewarNS::ROOM3_TRAPDOOR_X);
		trapDoor.setY(spacewarNS::ROOM3_TRAPDOOR_Y);

		for (int i=0; i < spacewarNS::ROOM3EN_NUM; i++) 
			enemies.push_back(&room3En[i]);
		

		enemies[0]->setX(675);
		enemies[0]->setY(430);
		enemies[0]->setRadians(3*PI/2);
		enemies[1]->setX(1180);
		enemies[1]->setY(255);
		enemies[1]->setRadians(0);
		enemies[2]->setX(583);
		enemies[2]->setY(615);
		enemies[2]->setRadians(PI/2.0);
		enemies[2]->patternSteps[0].setAction(ROTATECCW, 10);
		enemies[2]->patternSteps[1].setAction(PAUSE, 5);
		enemies[2]->patternSteps[2].setAction(ROTATECW, 10);
		enemies[2]->patternSteps[3].setAction(PAUSE, 5);
		enemies[2]->patternSteps[4].setAction(ROTATECW, 10);
		enemies[2]->patternSteps[5].setAction(PAUSE, 5);
		enemies[2]->patternSteps[6].setAction(ROTATECCW, 10);
		enemies[2]->patternSteps[7].setAction(PAUSE, 5);

		getWallPositions();
	}

	for (int i = 0; i < portals.size(); i++) 
		portals[i]->setActiveAndVisible(true);
	
	
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setActiveAndVisible(true);
		for (int j = 0; j < MAX_PATTERN_STEPS; j++)
		{
			enemies[i]->patternSteps[j].initialize(enemies[i]);
			enemies[i]->patternSteps[j].setActive();
		}
	}
	fin.close();
}

bool Spacewar::areAllEnemiesDead()
{
	for (int i = 0; i < enemies.size(); i++)
		if (enemies[i]->getActive())
			return false;

	return true;

}

void Spacewar::shootingAI()
{
	for(int a = 0; a < enemies.size(); a++)
	{
		if (enemies[a]->getTimeSinceShot() > enemy1NS::SHOT_DELAY && enemies[a]->vision.isEntityInVision(rugger) && enemies[a]->getActive())
		{
			enemies[a]->setTimeSinceShot();
			VECTOR2 temp((rugger.getX() /*+ rugger.getWidth() * rugger.getScale() / 2*/ - enemies[a]->getX()), (rugger.getY() +/* rugger.getHeight() * rugger.getScale() / 2*/ - enemies[a]->getY()));	
			temp /= D3DXVec2Length(&temp);

			for (int i = 0; i < enemies[a]->bullets.size(); i++)
			{
				
				if (!(enemies[a]->bullets[i].getActive()))
				{
					enemies[a]->bullets[i].setActive(true);
					//enemies[a]->bullets[i].setVisible(true); // uncomment this out to see camera hit detection
					enemies[a]->bullets[i].setX(enemies[a]->getX());
					enemies[a]->bullets[i].setY(enemies[a]->getY()+enemy1NS::HEIGHT/2.5);
					enemies[a]->bullets[i].setVelocity(VECTOR2(temp.x * 5000, temp.y * 5000));

					break;
				}

			}

		}
	}
}

void Spacewar::restartGame()
{
	startRoom(0);

	currentRoom = 0;
	score = 0;
	menuOn = true;
	died = false;
	gameTime = 0.0;
	Sleep(200);
}

void Spacewar::resetPos()
{
	for (int i = 0;i<ruggerNS::NUM_BULLETS; i++)
	{
		pBullets[i].setActiveAndVisible(false);
	}
	bulletDist = 0;

	switch(currentRoom)
	{
	case 0: 
		rugger.setX(spacewarNS::ROOM1_STARTING_X);
		rugger.setY(spacewarNS::ROOM1_STARTING_Y); 
		break;
	case 1: 
		rugger.setX(spacewarNS::ROOM2_STARTING_X);
		rugger.setY(spacewarNS::ROOM2_STARTING_Y);
		break;
	case 2: 
		rugger.setX(spacewarNS::ROOM3_STARTING_X);
		rugger.setY(spacewarNS::ROOM3_STARTING_Y);
		break;
	default:
		rugger.setX(spacewarNS::DEFAULT_STARTING_X);
		rugger.setY(spacewarNS::DEFAULT_STARTING_Y);
		break;
	}
}

void Spacewar::playCutscene(int num)
{
	Entity * cutscene;
	inCutscene = 0;

	switch(num)
	{
	case 0: cutscene = &cutscene1; break;
	case 1: cutscene = &cutscene2; break;
	case 2: cutscene = &cutscene3; break;
	default: break;
	}

	cutscene->setVisibleAndActive(true);
	cutscene->setX(0);
	cutscene->setY(0);
	cutscene->setVelocity(VECTOR2(-10, 0));
	

}

void Spacewar::stopCutscene()
{
	inCutscene = -1;
	cutscene1.setActive(false);
	cutscene2.setActive(false);
	cutscene3.setActive(false);
}

void Spacewar::getWallPositions()
{
	int type, rotation, x, y;
	
	for(int i=walls.size(); i>0; i--)
	{
		delete walls.back();
		walls.pop_back();
	}
	walls.clear();
	while(fin >> type)
	{
		if(type == 0)	//if the wall is a long wall
		{
			walls.push_back(new Entity);
			fin >> rotation >> x >> y;
			if(rotation == 0)
			{
				walls.back()->initialize(this, 32, 300,  0, &wallLongTM);
				walls.back()->setCollisionType(entityNS::BOX);
				walls.back()->setEdge(LONGRECT);
			}
			else
			{
				walls.back()->initialize(this, 300, 32,  0, &wallLongHTM);
				walls.back()->setCollisionType(entityNS::BOX);
				walls.back()->setEdge(LONGHRECT);
			}
			walls.back()->setX(x);
			walls.back()->setY(y);
		}
		else           //if the wall is a short wall
		{
			walls.push_back(new Entity);
			fin >> rotation >> x >> y;
			if(rotation == 0)
			{
				walls.back()->initialize(this, 32, 128,  0, &wallShortTM);
				walls.back()->setCollisionType(entityNS::BOX);
				walls.back()->setEdge(SHORTRECT);
			}
			else
			{
				walls.back()->initialize(this, 128, 32,  0, &wallShortHTM);
				walls.back()->setCollisionType(entityNS::BOX);
				walls.back()->setEdge(SHORTHRECT);
			}
			walls.back()->setX(x);
			walls.back()->setY(y);
		}
	}
	for(int i=0; i<walls.size(); i++)
	{
		walls[i]->setActiveAndVisible(true);
	}
}

VECTOR2 Spacewar::intersection(VECTOR2 p1, VECTOR2 p2, VECTOR2 p3, VECTOR2 p4)
{
	// Store the values for fast access and easy
	// equations-to-code conversion
	float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
 
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return VECTOR2(-1000,-1000);
 
	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
 
	// Check if the x and y coordinates are within both lines
	if ( x < min(x1, x2) || x > max(x1, x2) ||	x < min(x3, x4) || x > max(x3, x4) ) 
		return VECTOR2(-1000,-1000);
	if ( y < min(y1, y2) || y > max(y1, y2) ||	y < min(y3, y4) || y > max(y3, y4) ) 
		return VECTOR2(-1000,-1000);
 
	// Return the point of intersection
	VECTOR2 ret(x,y);
	return ret;
}

float Spacewar::dist(VECTOR2 p1, VECTOR2 p2)
{
	float x = abs(p2.x - p1.x);
	float y = abs(p2.y - p1.y);

	return (sqrt(x * x + y * y));
}
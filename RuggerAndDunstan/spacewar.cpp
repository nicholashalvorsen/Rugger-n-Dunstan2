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
	if(CloseHandle(hFile) == 0) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to close file before exiting."));
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
	// initialize textures

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

	for(int i=0; i<ruggerNS::NUM_BULLETS; i++)
	{
		if(!pBullets[i].initialize(this, 32, 32, 0, &bulletTM))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet i"));
		pBullets[i].setActiveAndVisible(false);
		pBullets[i].setX(10 + i*5);
		pBullets[i].setY(10);
		pBullets[i].setScale(.45);
	}

	for(int i=0; i<spacewarNS::NUM_WALLS; i++)
	{
		if(!wallLong[i].initialize(this, 32, 300, 0, &wallLongTM))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall long"));
		wallLong[i].setActiveAndVisible(false);
		wallLong[i].setCollisionType(entityNS::BOX);
		wallLong[i].setEdge(LONGRECT);
		if(!wallLongH[i].initialize(this, 300, 32, 0, &wallLongHTM))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall long h"));
		wallLongH[i].setActiveAndVisible(false);
		wallLongH[i].setCollisionType(entityNS::BOX);
		wallLongH[i].setEdge(LONGHRECT);
		if(!wallShort[i].initialize(this, 32, 150, 0, &wallShortTM))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall short"));
		wallShort[i].setActiveAndVisible(false);
		wallShort[i].setCollisionType(entityNS::BOX);
		wallShort[i].setEdge(SHORTRECT);
		if(!wallShortH[i].initialize(this, 150, 32, 0, &wallShortHTM))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing wall short h"));
		wallShortH[i].setActiveAndVisible(false);
		wallShortH[i].setCollisionType(entityNS::BOX);
		wallShortH[i].setEdge(SHORTHRECT);
		
	}
	
	if(!casket.initialize(this, 128, 64, 0, &casketTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing casket"));
	casket.setActiveAndVisible(false);
	casket.setCollisionType(entityNS::BOX);
	casket.setEdge(CASKETRECT);
	casket.setScale(0.75);
	
	audio->stopCue(BACKGROUND_MUSIC);
	audio->playCue(BACKGROUND_MUSIC);

	/*********************************************************************/
	dump.clear();
	fileName = "input.txt";
	hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(hFile == INVALID_HANDLE_VALUE) throw(GameError(gameErrorNS::FATAL_ERROR, "Error opening file for reading."));
	
	DWORD c;
	ReadFile(hFile, &dump, 64, &c, nullptr);
	/*********************************************************************/

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


		/*if (directionX != 0 && directionY != 0)
			rugger.setVelocity(VECTOR2(ruggerNS::SPEED * .80 * directionX, ruggerNS::SPEED * .80 * directionY));
		else
			rugger.setVelocity(VECTOR2(ruggerNS::SPEED * directionX, ruggerNS::SPEED * directionY));*/
		
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
		for(int i=0; i<spacewarNS::NUM_WALLS; i++)
		{
			if (rugger.collidesWith(wallLong[i], v) || rugger.collidesWith(wallLongH[i], v) || rugger.collidesWith(wallShort[i], v) || rugger.collidesWith(wallShortH[i], v))
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
		for(int i=0; i<spacewarNS::NUM_WALLS; i++)
		{
			wallLong[i].update(frameTime);
			wallLongH[i].update(frameTime);
			wallShort[i].update(frameTime);
			wallShortH[i].update(frameTime);
		}
		
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->getVisible())
				enemies[i]->update(frameTime);
		
			for(int j = 0; j < enemies[i]->bullets.size(); j++)
				if(enemies[i]->bullets[j].getActive())
					enemies[i]->bullets[j].update(frameTime);
		}
	}
}

//=============================================================================
// Start a new round of play
//=============================================================================
void Spacewar::roundStart()
{
    // Start ships on opposite sides of planet in stable clockwise orbit
    /*ship1.setX(GAME_WIDTH/4 - shipNS::WIDTH);
    ship2.setX(GAME_WIDTH - GAME_WIDTH/4);
    ship1.setY(GAME_HEIGHT/2 - shipNS::HEIGHT);
    ship2.setY(GAME_HEIGHT/2);
    ship1.setVelocity(VECTOR2(0,-shipNS::SPEED));
    ship2.setVelocity(VECTOR2(0,shipNS::SPEED));

    ship1.setDegrees(0);
    ship2.setDegrees(180);
    ship1.repair();
    ship2.repair();
    countDownTimer = spacewarNS::COUNT_DOWN;
    countDownOn = true;
    roundOver = false;
    ship1Scored = false;
    ship2Scored = false;*/
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

	for(int i=0; i<spacewarNS::NUM_WALLS; i++)
	{
		for(int j=0; j<ruggerNS::NUM_BULLETS; j++)
		{
			if(pBullets[j].collidesWith(wallLong[i], collisionVector) || pBullets[j].collidesWith(wallLongH[i], collisionVector) || pBullets[j].collidesWith(wallShort[i], collisionVector) || pBullets[j].collidesWith(wallShortH[i], collisionVector)) 
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

	for(int i=0; i<spacewarNS::NUM_WALLS; i++)
	{
		for (int e = 0; e < enemies.size(); e++)
		{
			for(int j=0; j < enemies[e]->bullets.size(); j++)
			{
				if(enemies[e]->bullets[j].collidesWith(wallLong[i], collisionVector) || enemies[e]->bullets[j].collidesWith(wallLongH[i], collisionVector) || enemies[e]->bullets[j].collidesWith(wallShort[i], collisionVector) || enemies[e]->bullets[j].collidesWith(wallShortH[i], collisionVector)) 
				{
					enemies[e]->bullets[j].setActiveAndVisible(false);
				}

				if (enemies[e]->bullets[j].collidesWith(rugger, collisionVector) && !invincible)
				{
					// rugger seen
					died = true;
					audio->playCue(ORCHHIT2);
					enemies[e]->bullets[j].setActiveAndVisible(false);
				}
			}
		}
	}

	if(rugger.collidesWith(casket, collisionVector))
	{ 
		rugger.setX(-1000);
		timeDone = gameTime;
		//score = static_cast<int>(score + timebonus);
		gameOver = true;
	}


}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	
	switch(currentRoom)
	{
	case 0: background.draw(); break;
	case 1: background2.draw(); break;
	case 2: background3.draw(); break;
	default: background.draw(); break;
	}
	if(trapDoor.getActive())
		trapDoor.draw();
	for(int i=0; i<spacewarNS::NUM_WALLS; i++)
	{
		wallLong[i].draw();
		wallLongH[i].draw();
		wallShort[i].draw();
		wallShortH[i].draw();
	}
	rugger.draw();
	casket.draw();
	for(int i=0; i<ruggerNS::NUM_BULLETS; i++)
	{
		if(pBullets[i].getActive()) pBullets[i].draw();
	}
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
	//debugText.print("mouse: (" + s3 + ", " + s4 + ")", 300, 10);
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

	/*if (vision.size() > 0)
	{
		ss.clear();
		ss << rugger.getCenter()->x << ' ' << rugger.getCenter()->y << ' ';
		ss << vision[0]->c1.x << ' ' << vision[0]->c1.y << ' ';
		ss << vision[0]->c2.x << ' ' << vision[0]->c2.y << ' ';
		ss << vision[0]->c3.x << ' ' << vision[0]->c3.y << ' ';
		ss >> s1 >> s2 >> s3 >> s4 >> s5 >> s6 >> s7 >> s8;

		debugText.print("testing if point (" + s1 + ", " + s2 + ") is within this triangle: \n" + 
							  "(" + s3 + ", " + s4 + ")\n" + 
							  "(" + s5 + ", " + s6 + ")\n" + 
							  "(" + s7 + ", " + s8 + ")\n", 300, 30);

	}*/


    // display health bars
    //healthBar.setX((float)spacewarNS::SHIP1_HEALTHBAR_X);
    ///healthBar.set(ship1.getHealth());
   // healthBar.draw(spacewarNS::SHIP1_COLOR);
    //healthBar.setX((float)spacewarNS::SHIP2_HEALTHBAR_X);
    //healthBar.set(ship2.getHealth());
    //healthBar.draw(spacewarNS::SHIP2_COLOR);

    //ship1.draw();                           // draw the spaceships
    //ship2.draw();

    //torpedo1.draw(graphicsNS::FILTER);      // draw the torpedoes using colorFilter
   // torpedo2.draw(graphicsNS::FILTER);
	
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

				int timebonus = 500 - (gameTime * 10);
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

	debugText.print(dump, 100, 200);
    //if(countDownOn)
    //{
   //     _snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
   //     fontBig.print(buffer,spacewarNS::COUNT_DOWN_X,spacewarNS::COUNT_DOWN_Y);
   // }

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
        /*console->print("gravity off - turns off planet gravity");
        console->print("gravity on - turns on planet gravity");
        console->print("planet off - disables planet");
        console->print("planet on - enables planet");*/
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

    /*if (command == "gravity off")
    {
        planet.setMass(0);
        console->print("Gravity Off");
    }else if (command == "gravity on")
    {
        planet.setMass(planetNS::MASS);
        console->print("Gravity On");
    }else if (command == "planet off")
    {
        planet.disable();
        console->print("Planet Off");
    }else if (command == "planet on")
    {
        planet.enable();
        console->print("Planet On");
    }*/
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
	//trapDoor.setVisibleAndActive(false);
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

	for (int i = 0; i < ruggerNS::NUM_BULLETS; i++)
	{
		pBullets[i].setActive(false);
		pBullets[i].setVisible(false);
	}

	gravBall.setVisibleAndActive(false);

	enemies.clear();

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
		enemies[1]->patternSteps[0].setAction(PAUSE, 10);

		enemies.push_back(&enemy1_3);
		enemies[2]->setDegrees(90);
		enemies[2]->patternSteps[0].setAction(ROTATECW, 30);
		enemies[2]->patternSteps[1].setAction(PAUSE, 3);
		enemies[2]->patternSteps[2].setAction(ROTATECCW, 30);
		enemies[2]->patternSteps[3].setAction(PAUSE, 3);

		enemies.push_back(&enemy1_4);
		enemies[3]->setDegrees(0);
		enemies[3]->patternSteps[0].setAction(PAUSE, 3);
		//enemies[3]->patternSteps[1].setAction(UP, 6);
		//enemies[3]->patternSteps[2].setAction(ROTATECCW, 90);
		//enemies[3]->patternSteps[3].setAction(PAUSE, 3);
		//enemies[3]->patternSteps[4].setAction(ROTATECCW, 90);
		//enemies[3]->patternSteps[5].setAction(DOWN, 6);
		//enemies[3]->patternSteps[6].setAction(ROTATECCW, 180);

		wallLong[0].setActiveAndVisible(true);
		wallLong[0].setX(300);
		wallLong[0].setY(0);
		wallLongH[0].setActiveAndVisible(true);
		wallLongH[0].setX(240);
		wallLongH[0].setY(wallLong[0].getHeight());
		//wallShort[0].setActiveAndVisible(true);
		wallShort[0].setX(450);
		wallShort[0].setY(0);
		//wallShort[1].setActiveAndVisible(true);
		wallShort[1].setX(wallShort[0].getX() + wallLongH[1].getWidth() - wallShort[1].getWidth());
		wallShort[1].setY(0);
		wallLongH[1].setActiveAndVisible(true);
		wallLongH[1].setX(wallShort[0].getX());
		wallLongH[1].setY(wallShort[0].getHeight());
		wallLongH[2].setActiveAndVisible(true);
		wallLongH[2].setX(GAME_WIDTH - wallLongH[2].getWidth());
		wallLongH[2].setY(wallLongH[0].getY() + 2*enemies[0]->getHeight());
		wallLong[1].setActiveAndVisible(true);
		wallLong[1].setX(wallLongH[2].getX());
		wallLong[1].setY(wallLongH[2].getY()+wallLongH[2].getHeight());
		wallLong[2].setActiveAndVisible(true);
		wallLong[2].setX(wallLong[1].getX());
		wallLong[2].setY(wallLong[1].getY()+wallLong[1].getHeight());
		wallLong[3].setActiveAndVisible(true);
		wallLong[3].setX(wallShort[1].getX());
		wallLong[3].setY(wallShort[2].getY()+wallShort[2].getHeight() + wallShortH[0].getHeight());
		wallLongH[3].setActiveAndVisible(true);
		wallLongH[3].setX(-40);
		wallLongH[3].setY(wallLongH[0].getY() + 6*enemies[0]->getHeight());
		wallLongH[4].setActiveAndVisible(true);
		wallLongH[4].setX(wallLongH[3].getX()+wallLongH[3].getWidth());
		wallLongH[4].setY(wallLongH[0].getY() + 6*enemies[0]->getHeight());
		wallLongH[5].setActiveAndVisible(true);
		wallLongH[5].setX(wallLongH[4].getX()+wallLongH[4].getWidth());
		wallLongH[5].setY(wallLongH[0].getY() + 6*enemies[0]->getHeight());
		wallShortH[0].setActiveAndVisible(true);
		wallShortH[0].setX(wallLongH[5].getX()+wallLongH[5].getWidth());
		wallShortH[0].setY(wallLongH[0].getY() + 6*enemies[0]->getHeight());

		
	}
	if (roomNum == 1) // 2nd room
	{
		trapDoor.setX(spacewarNS::ROOM2_TRAPDOOR_X);
		trapDoor.setY(spacewarNS::ROOM2_TRAPDOOR_Y);
		rugger.setX(spacewarNS::ROOM2_STARTING_X);
		rugger.setY(spacewarNS::ROOM2_STARTING_Y);

		for(int i=0; i<spacewarNS::NUM_WALLS; i++)
		{
			wallLong[i].setActiveAndVisible(false);
			wallLongH[i].setActiveAndVisible(false);
			wallShort[i].setActiveAndVisible(false);
			wallShortH[i].setActiveAndVisible(false);
		}
		enemies.push_back(&enemy2_1);
		enemies.push_back(&enemy2_2);
		enemies[0]->setDegrees(-50);
		enemies[1]->setDegrees(-45);
		
		//enemies[0]->patternSteps[0].setAction(ROTATECW, 90);
		enemies[0]->patternSteps[1].setAction(PAUSE, 2);
		//enemies[0]->patternSteps[2].setAction(ROTATECCW, 90);
		//enemies[0]->patternSteps[3].setAction(PAUSE, 2);

		enemies[1]->patternSteps[0].setAction(ROTATECW, 45);
		enemies[1]->patternSteps[1].setAction(PAUSE, 2);
		enemies[1]->patternSteps[2].setAction(ROTATECCW, 45);
		enemies[1]->patternSteps[3].setAction(PAUSE, 2);

		enemies.push_back(&enemy2_3);
		enemies[2]->setDegrees(180);
		//enemies[2]->patternSteps[0].setAction(LEFT, 10);
		//enemies[2]->patternSteps[1].setAction(ROTATECCW, 90);
		//enemies[2]->patternSteps[2].setAction(DOWN, 5);
		//enemies[2]->patternSteps[3].setAction(ROTATECW, 180);
		//enemies[2]->patternSteps[4].setAction(UP, 5);
		//enemies[2]->patternSteps[5].setAction(ROTATECW, 180);
		//enemies[2]->patternSteps[6].setAction(RIGHT, 10);
		//enemies[2]->patternSteps[7].setAction(ROTATECW, 180);
		enemies[2]->patternSteps[0].setAction(PAUSE, 5);

		enemies.push_back(&enemy2_4);
		enemies[3]->setDegrees(90);
		enemies[3]->patternSteps[0].setAction(RIGHT, 8);
		//enemies[2]->patternSteps[1].setAction(ROTATECCW, 90);
		//enemies[2]->patternSteps[2].setAction(DOWN, 5);
		//enemies[2]->patternSteps[3].setAction(ROTATECW, 180);
		//enemies[2]->patternSteps[4].setAction(UP, 5);
		enemies[3]->patternSteps[5].setAction(ROTATECCW, 180);
		enemies[3]->patternSteps[6].setAction(LEFT, 8);
		enemies[3]->patternSteps[7].setAction(ROTATECCW, 180);

		//WALL POSITIONING YAAAY
		wallLongH[0].setActiveAndVisible(true);
		wallLongH[0].setX(-150);
		wallLongH[0].setY(GAME_HEIGHT - 150);
		wallLongH[1].setActiveAndVisible(true);
		wallLongH[1].setX(wallLongH[0].getX() + wallLongH[0].getWidth());
		wallLongH[1].setY(GAME_HEIGHT - 150);
		wallLongH[2].setActiveAndVisible(true);
		wallLongH[2].setX(wallLongH[1].getX() + wallLongH[1].getWidth());
		wallLongH[2].setY(GAME_HEIGHT - 150);
		wallLongH[3].setActiveAndVisible(true);
		wallLongH[3].setX(0);
		wallLongH[3].setY(GAME_HEIGHT - 450);
		wallLongH[4].setActiveAndVisible(true);
		wallLongH[4].setX(wallLongH[3].getX() + wallLongH[3].getWidth());
		wallLongH[4].setY(GAME_HEIGHT - 450);

		wallLong[0].setActiveAndVisible(true);
		wallLong[0].setX(3*wallLongH[0].getWidth());
		wallLong[0].setY(GAME_HEIGHT - 2*wallLong[0].getHeight());
		wallLong[1].setActiveAndVisible(true);
		wallLong[1].setX(3*wallLongH[0].getWidth());
		wallLong[1].setY(wallLong[0].getY() + wallLong[0].getHeight());
		
	}
	if(roomNum == 2)
	{
		casket.setActiveAndVisible(true);
		casket.setX(GAME_WIDTH - 120);
		casket.setY(15);
		rugger.setX(spacewarNS::ROOM3_STARTING_X);
		rugger.setY(spacewarNS::ROOM3_STARTING_Y);
		trapDoor.setX(spacewarNS::ROOM3_TRAPDOOR_X);
		trapDoor.setY(spacewarNS::ROOM3_TRAPDOOR_Y);
		for(int i=0; i<spacewarNS::NUM_WALLS; i++)
		{
			wallLong[i].setActiveAndVisible(false);
			wallLongH[i].setActiveAndVisible(false);
			wallShort[i].setActiveAndVisible(false);
			wallShortH[i].setActiveAndVisible(false);
		}
		for(int i=0; i<spacewarNS::ROOM3EN_NUM; i++)  
		{
			enemies.push_back(&room3En[i]);	
			int iter = 1;
			int switcher = 1;
			for(int j=0; j<i; j++) iter *= -1;
			if(iter == -1)	//bottom of screen
			{
				enemies[i]->setX(300*ceil(i/2.0) + 50);
				enemies[i]->setY(GAME_HEIGHT - 25);
				enemies[i]->patternSteps[0].setAction(ROTATECW, 45);
				enemies[i]->patternSteps[1].setAction(PAUSE, 1);
				enemies[i]->patternSteps[2].setAction(ROTATECCW, 90);
				enemies[i]->patternSteps[3].setAction(PAUSE, 1);
				enemies[i]->patternSteps[4].setAction(ROTATECW, 45);
			}
			else //top of screen
			{
				enemies[i]->setX(300*ceil(i/2.0) + 200);
				enemies[i]->setY(GAME_HEIGHT - 100);
				enemies[i]->setRadians(0);
				enemies[i]->patternSteps[0].setAction(UP, 8);
				enemies[i]->patternSteps[1].setAction(PAUSE, rand()%6);
				enemies[i]->patternSteps[2].setAction(ROTATECW, 180);
				enemies[i]->patternSteps[3].setAction(DOWN, 8);
				enemies[i]->patternSteps[4].setAction(ROTATECW, 180);
				if(i == 6)
				{
					enemies[i]->setRadians(PI);
					enemies[i]->setY(150);
					enemies[i]->patternSteps[0].setAction(DOWN, 9);
					enemies[i]->patternSteps[1].setAction(ROTATECW, 180);
					enemies[i]->patternSteps[2].setAction(UP, 9);
					enemies[i]->patternSteps[3].setAction(ROTATECW, 180);
					enemies[i]->patternSteps[4].setAction(PAUSE, 1);
				}
			}
		}
		enemies[3]->setY(25);
		enemies[3]->setRadians(PI);


		wallLong[0].setActiveAndVisible(true);
		wallLong[0].setX(75);
		wallLong[0].setY(125);
		wallLong[1].setActiveAndVisible(true);
		wallLong[1].setX(wallLong[0].getX());
		wallLong[1].setY(wallLong[0].getY()+wallLong[0].getHeight());
		wallLong[2].setActiveAndVisible(true);
		wallLong[2].setX(350);
		wallLong[2].setY(0);
		wallLong[3].setActiveAndVisible(true);
		wallLong[3].setX(wallLong[2].getX());
		wallLong[3].setY(wallLong[2].getY()+wallLong[2].getHeight());
		wallLong[4].setActiveAndVisible(true);
		wallLong[4].setX(650);
		wallLong[4].setY(GAME_HEIGHT - wallLong[4].getHeight()*2);
		wallLong[5].setActiveAndVisible(true);
		wallLong[5].setX(wallLong[4].getX());
		wallLong[5].setY(wallLong[4].getY() + wallLong[4].getHeight());
		wallLong[6].setActiveAndVisible(true);
		wallLong[6].setX(950);
		wallLong[6].setY(0);
		wallLong[7].setActiveAndVisible(true);
		wallLong[7].setX(wallLong[6].getX());
		wallLong[7].setY(wallLong[6].getY() + wallLong[6].getHeight());
		wallLong[8].setActiveAndVisible(true);
		wallLong[8].setX(1250);
		wallLong[8].setY(GAME_HEIGHT - 2*wallLong[8].getHeight());
		wallLong[9].setActiveAndVisible(true);
		wallLong[9].setX(wallLong[8].getX());
		wallLong[9].setY(wallLong[8].getY() + wallLong[8].getHeight());
		wallShortH[0].setActiveAndVisible(true);
		wallShortH[0].setX(GAME_WIDTH - 150);
		wallShortH[0].setY(wallLong[8].getY() - wallShortH[0].getHeight());
	}

	
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setActiveAndVisible(true);
		for (int j = 0; j < MAX_PATTERN_STEPS; j++)
		{
			enemies[i]->patternSteps[j].initialize(enemies[i]);
			enemies[i]->patternSteps[j].setActive();
		}
	}


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
					enemies[a]->bullets[i].setVelocity(VECTOR2(temp.x * 3000, temp.y * 3000));

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
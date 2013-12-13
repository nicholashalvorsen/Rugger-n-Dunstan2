// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// rugger.cpp v1.0

#include "RuggerHalvorsenMartRoberts.h"

//=============================================================================
// default constructor
//=============================================================================
Rugger::Rugger() : Entity()
{
	firing = false;
	direction = ruggerNS::UP;
    spriteData.width = ruggerNS::WIDTH;           // size of Rugger1
    spriteData.height = ruggerNS::HEIGHT;
    spriteData.x = ruggerNS::X;                   // location on screen
    spriteData.y = ruggerNS::Y;
    spriteData.rect.bottom = ruggerNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = ruggerNS::WIDTH;
    oldX = ruggerNS::X;
    oldY = ruggerNS::Y;
    oldAngle = 0.0f;
    rotation = 0.0f;
    velocity.x = 0;
    velocity.y = 0;
    frameDelay = ruggerNS::SHIP_ANIMATION_DELAY;
    startFrame = ruggerNS::SHIP1_START_FRAME;     // first frame of rugger animation
    endFrame     = ruggerNS::SHIP1_END_FRAME;     // last frame of rugger animation
    currentFrame = startFrame;
    radius = ruggerNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
    direction = ruggerNS::NONE;                   // direction of rotation thruster
    engineOn = false;
    shieldOn = false;
    explosionOn = false;
    mass = ruggerNS::MASS;
	timeSinceLastShot = 0.0;
	canBeSeen = false;
	vandalized = false;
	spottedAchievement = false;
	spots = 0;
	messingAround = false;
}

//=============================================================================
// Initialize the Rugger.
// Post: returns true if successful, false if failed
//=============================================================================
bool Rugger::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
	setFrames(RUGGER_STILL_START,RUGGER_STILL_END);
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the rugger
//=============================================================================
void Rugger::draw()
{
	Image::draw();              // draw rugger
} // Rugger is now drawn


//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Rugger::update(float frameTime)
{
	Entity::update(frameTime);

	if (direction == ruggerNS::LEFT) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(1.5f*static_cast<float>(PI));
	} else if (direction == ruggerNS::RIGHT) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(0.5f*static_cast<float>(PI));
	} else if (direction == ruggerNS::UP) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(0.0f);
	} else if (direction == ruggerNS::DOWN) {
		setFrames(RUGGER_WALK_START, RUGGER_WALK_END);
		setRadians(static_cast<float>(PI));
	} else if (direction == ruggerNS::LEFTUP) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(0.75f*static_cast<float>(PI));
	} else if (direction == ruggerNS::RIGHTUP) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(0.25f*static_cast<float>(PI));
	} else if (direction == ruggerNS::LEFTDOWN) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(1.25f*static_cast<float>(PI));
	} else if (direction == ruggerNS::RIGHTDOWN) {
		setFrames(RUGGER_WALK_START,RUGGER_WALK_END);
		setRadians(0.75f*static_cast<float>(PI));
	} else if (direction == ruggerNS::NONE) {
		setFrames(RUGGER_STILL_START,RUGGER_STILL_END);
	}

	if (firing || graving) 
		setRadians(radiansToMouse());

	if (graving)
		setFrames(RUGGER_GRAV_START, RUGGER_GRAV_END);
	if (firing)
		setFrames(RUGGER_GUN_START, RUGGER_GUN_END);

    spriteData.x += frameTime * velocity.x;
    spriteData.y += frameTime * velocity.y;

	if (spriteData.x < 0)
		spriteData.x = 0;
	if (spriteData.x > GAME_WIDTH - ruggerNS::WIDTH * getScale())
		spriteData.x = GAME_WIDTH - ruggerNS::WIDTH * getScale();
	if (spriteData.y < 0)
		spriteData.y = 0;
	if (spriteData.y > GAME_HEIGHT - ruggerNS::HEIGHT * getScale())
		spriteData.y = GAME_HEIGHT - ruggerNS::HEIGHT * getScale();
	
   	timeSinceLastShot += frameTime;
}

//=============================================================================
// explode
//=============================================================================
void Rugger::explode()
{
    audio->playCue(EXPLODE);
    active = false;
    health = 0;
    explosionOn = true;
    engineOn = false;
    shieldOn = false;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
}

//=============================================================================
// repair
//=============================================================================
void Rugger::repair()
{
    active = true;
    health = FULL_HEALTH;
    explosionOn = false;
    engineOn = false;
    shieldOn = false;
    rotation = 0.0f;
    direction = ruggerNS::NONE;           // direction of rotation thruster
    visible = true;
}

void Rugger::fire(int mouseX, int mouseY, Bullet b[])
{
	if(timeSinceLastShot < ruggerNS::SHOT_DELAY) return;
	for(int i=0; i<ruggerNS::NUM_BULLETS; i++)
	{
		if(!b[i].getActive())
		{
			timeSinceLastShot = 0;
			VECTOR2 fireVec(mouseX - getCenterX(), mouseY - getCenterY());
			VECTOR2* normalFireVec = D3DXVec2Normalize(&fireVec, &fireVec);
			b[i].setX(getCenterX());
			b[i].setY(getCenterY());
			b[i].setVelocity(fireVec * bulletNS::SPEED);
			b[i].setActiveAndVisible(true);
			return;
		}
	}
}
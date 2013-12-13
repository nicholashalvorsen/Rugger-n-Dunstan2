// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 ship.cpp v1.0

#include "enemyHalvorsenRoberts.h"
#include "bulletHalvorsen.h"

//=============================================================================
// default constructor
//=============================================================================
Enemy1::Enemy1() : Entity()
{
	patternSteps.resize(MAX_PATTERN_STEPS);
    spriteData.width = enemy1NS::WIDTH;           // size of Ship1
    spriteData.height = enemy1NS::HEIGHT;
    spriteData.x = enemy1NS::X;                   // location on screen
    spriteData.y = enemy1NS::Y;
    spriteData.rect.bottom = enemy1NS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = enemy1NS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    frameDelay = enemy1NS::SHIP_ANIMATION_DELAY;
    startFrame = enemy1NS::SHIP1_START_FRAME;     // first frame of ship animation
    endFrame     = enemy1NS::SHIP1_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = enemy1NS::WIDTH/2.0;
    mass = enemy1NS::MASS;
    collisionType = entityNS::CIRCLE;
	timeSinceShot = 0;
	health = enemy1NS::STARTING_HEALTH;
	shotType = enemy1NS::DEFAULT_SHOT_TYPE;
	patternStepIndex = 0;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Enemy1::initialize(Game *gamePtr, int width, int height, int ncols, Bullet bulletType, Vision v,
    TextureManager *textureM)
{
	for (int i = 0; i < enemy1NS::NUM_BULLETS; i++)
		bullets.push_back(bulletType);

	vision = v;

    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the ship
//=============================================================================
void Enemy1::draw()
{
	vision.draw();
    Image::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Enemy1::update(float frameTime)
{
	/*if (input->isKeyDown(VK_RIGHT))
		setRadians(getRadians() + .005);
	if (input->isKeyDown(VK_LEFT))
		setRadians(getRadians() - .005);*/

    Entity::update(frameTime);
    //spriteData.angle += frameTime * enemy1NS::ROTATION_RATE;  // rotate the ship
    spriteData.x += frameTime * velocity.x;         // move ship along X 
    spriteData.y += frameTime * velocity.y;         // move ship along Y

    // Bounce off walls
    if (spriteData.x > GAME_WIDTH-enemy1NS::WIDTH)    // if hit right screen edge
    {
		spriteData.x = GAME_WIDTH-enemy1NS::WIDTH;
		
        //spriteData.x = GAME_WIDTH-enemy1NS::WIDTH;    // position at right screen edge
        //velocity.x = -velocity.x;                   // reverse X direction
    } else if (spriteData.x < 0)					// else if hit left screen edge
    {
		spriteData.x = 0;
    }
    if (spriteData.y > GAME_HEIGHT-enemy1NS::HEIGHT)  // if hit bottom screen edge
    {
        spriteData.y = GAME_HEIGHT-enemy1NS::HEIGHT;  // position at bottom screen edge
    } else if (spriteData.y < 0)                    // else if hit top screen edge
    {
        spriteData.y = 0;                           // position at top screen edge
    }

	velocity = VECTOR2(0, 0);

	timeSinceShot += frameTime;

	vision.stickToEntity(*this);
}

//=============================================================================
// damage
//=============================================================================
void Enemy1::ai(float time)
{
	if (patternStepIndex == MAX_PATTERN_STEPS)
	{
		for (int j = 0; j < MAX_PATTERN_STEPS; j++)
			patternSteps[j].setActive();

		patternStepIndex = 0;
	}

	if (patternSteps[patternStepIndex].isFinished())
		patternStepIndex++;

	if (patternStepIndex == MAX_PATTERN_STEPS)
	{
		for (int j = 0; j < MAX_PATTERN_STEPS; j++)
			patternSteps[j].setActive();

		patternStepIndex = 0;
	}

	patternSteps[patternStepIndex].update(time);

	return;
}
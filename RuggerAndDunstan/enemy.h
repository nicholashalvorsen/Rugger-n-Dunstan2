// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly

#ifndef _ENEMY_H                 // Prevent multiple definitions if this 
#define _ENEMY_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "bullet.h"
#include "vision.h"
class Enemy1;
#include "patternStep.h"
#include <vector>
using std::vector;


namespace enemy1NS
{
    const int WIDTH = 32;                   // image width
    const int HEIGHT = 32;                 // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    //const float ROTATION_RATE = (float)PI/4; // radians per second
	const float ROTATION_RATE = 0.2f;
    const float SPEED = 40.0f;                // 100 pixels per second
    const float MASS = 300.0f;              // mass
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   SHIP1_START_FRAME = 0;      // ship1 starts at frame 0
    const int   SHIP1_END_FRAME = 0;        // ship1 animation frames 0,1,2,3
    const float SHIP_ANIMATION_DELAY = 0.2f;    // time between frames
    //const int   SHIELD_START_FRAME = 24;    // shield start frame
    //const int   SHIELD_END_FRAME = 27;      // shield end frame
    const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
	const float SHOT_DELAY = 0.25f;			// delay between firing bullets
	const int	NUM_BULLETS = 3;			// max number of bullets out at once
	const int	STARTING_HEALTH = 8;		// health that the unit starts with
	const int	SHOT_SPEED = 500;
	const int	DEFAULT_SHOT_TYPE = 1;
}

namespace bossNS
{
    const int WIDTH = 32;                   // image width
    const int HEIGHT = 32;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float ROTATION_RATE = (float)PI/4; // radians per second
    const float SPEED = 300;                // 100 pixels per second
    const float MASS = 300.0f;              // mass
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   SHIP1_START_FRAME = 0;      // ship1 starts at frame 0
    const int   SHIP1_END_FRAME = 0;        // ship1 animation frames 0,1,2,3
    const float SHIP_ANIMATION_DELAY = 0.2f;    // time between frames
    //const int   SHIELD_START_FRAME = 24;    // shield start frame
    //const int   SHIELD_END_FRAME = 27;      // shield end frame
    const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
	const float SHOT_DELAY = .05;			// delay between firing bullets
	const int	NUM_BULLETS = 6;			// max number of bullets out at once
	static int	STARTING_HEALTH = 15;		// health that the unit starts with
	const int	SHOT_SPEED = 1000;
	const int	DEFAULT_SHOT_TYPE = 3;
}


// inherits from Entity class
class Enemy1 : public Entity
{
private:
	float timeSinceShot;
	int shotType;
		// shotType:
	   //  1 = straight line
	  //   2 = 3 round burst
	 //    3 = tighter 3 round burst 
	

public:
    Enemy1();

	vector<Bullet> bullets;
	Vision vision;

	float getTimeSinceShot(){return timeSinceShot;}
	void setTimeSinceShot(){timeSinceShot = 0;}

    // inherited member functions
    virtual void draw();
    
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols, Bullet bulletType, Vision v,
                            TextureManager *textureM);
    void update(float frameTime);
	void reduceHealth(int dmg);
	void setShotType(int st){shotType = st;}
	int getShotType(){return shotType;}
	
	vector<PatternStep> patternSteps;
	int patternStepIndex;
	void ai(float time);
};
#endif